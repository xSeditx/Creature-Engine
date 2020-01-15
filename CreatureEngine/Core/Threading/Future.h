#if 0
#pragma  once
#include<chrono>
#include<mutex>
#include<condition_variable>
	namespace Experimental
		{
namespace Core
{
	namespace Threading
	{
	
			template<typename _Ty>
			struct _associated_state
			{
			public:
				enum class future_status
				{
					ready,
					timeout,
					deferred
				};

				//using _State_type = _Ty;
				////using _Mydel = _Deleter_base<_Ty>;
				//
				//_associated_state(_Mydel* _Dp = nullptr)
				//	: _Refs(1),	// non-atomic initialization
				//	_Retrieved(false),
				//	_Ready(false),
				//	_Has_stored_result(false),
				//	_Running(false),
				//{	// construct
				//// TODO: _Associated_state ctor assumes _Ty is default constructible
				//}

				virtual ~_associated_state() noexcept;
				void _Retain();
				void _Release();

			private:
				uint64_t _Refs;

			public:
				virtual void _Wait();

				struct _Test_ready
				{	// wraps _Associated_state
					_Test_ready(const _associated_state* _St)
						: _State(_St) {}

					bool operator()() const {}
					const _associated_state* _State;
				};

				template<class _Rep, class _Per> future_status _Wait_for(const std::chrono::duration<_Rep, _Per>& _Rel_time);
				template<class _Clock, class _Dur> future_status _Wait_until(const std::chrono::time_point<_Clock, _Dur>& _Abs_time);

				virtual _Ty& _Get_value(bool _Get_only_once);
				void _Set_value(const _Ty& _Val, bool _At_thread_exit);
				void _Set_value_raw(const _Ty& _Val, std::unique_lock<std::mutex>* _Lock, bool _At_thread_exit);
				void _Set_value(_Ty&& _Val, bool _At_thread_exit);
				void _Set_value_raw(_Ty&& _Val, std::unique_lock<std::mutex>* _Lock, bool _At_thread_exit);
				void _Set_value(bool _At_thread_exit);
				void _Set_value_raw(std::unique_lock<std::mutex>* _Lock, bool _At_thread_exit);

				bool _Is_ready() const;
				bool _Is_ready_at_thread_exit() const;
				bool _Already_has_stored_result() const;
				bool _Already_retrieved() const;
				void _Abandon();
			protected:
				void _Make_ready_at_thread_exit();
				void _Maybe_run_deferred_function(std::unique_lock<std::mutex>& _Lock);

			public:
				_Ty _Result;

				std::mutex _Mtx;
				std::condition_variable _Cond;
				bool _Retrieved;
				int _Ready;
				bool _Ready_at_thread_exit;
				bool _Has_stored_result;
				bool _Running;

			private:
				virtual bool _Has_deferred_function() const noexcept;
				virtual void _Run_deferred_function(std::unique_lock<std::mutex>&);
				virtual void _Do_notify(std::unique_lock<std::mutex>* _Lock, bool _At_thread_exit);

				void _Delete_this();
	 		public:
				_associated_state(const _associated_state&) = delete;
				_associated_state& operator=(const _associated_state&) = delete;
			};

			template<typename _Ty>
			struct _state_manager
			{
			public:
				_state_manager();
				_state_manager(_associated_state<_Ty>* _New_state, bool _Get_once);
				_state_manager(const _state_manager& _Other, bool _Get_once = false);
				_state_manager(_state_manager&& _Other, bool _Get_once = false);

				~_state_manager() noexcept;
				_state_manager& operator=(const _state_manager& _Other);
				_state_manager& operator=(_state_manager&& _Other);
				[[nodiscard]] bool valid() const noexcept;
				void wait() const;

				//template<class _Rep, class _Per> future_status wait_for(const std::chrono::duration<_Rep, _Per>& _Rel_time) const;
				//template<class _Clock, class _Dur> future_status wait_until(const std::chrono::time_point<_Clock, _Dur>& _Abs_time) const;
				_Ty& _Get_value() const;
				void _Set_value(const _Ty& _Val, bool _Defer);
				void _Set_value(_Ty&& _Val, bool _Defer);

				void _Abandon();
				void _Swap(_state_manager& _Other);
				_associated_state<_Ty>* _Ptr() const;
				void _Copy_from(const _state_manager& _Other);
				void _Move_from(_state_manager& _Other);
				bool _Is_ready() const;
				bool _Is_ready_at_thread_exit() const
			private:
				_associated_state<_Ty>* _Assoc_state;
				bool _Get_Once;
			};

			template<typename _Ty>
			struct Shared_State
			{
			public:
				enum class future_status
				{
					ready,
					timeout,
					deferred
				};

				Shared_State() {}

				void Set_Value(_Ty&& _val) {
					std::unique_lock<std::mutex> Lock(Mtx);
					Value = std::forward(_val);
				}

				void Wait()
				{
					std::unique_lock<std::mutex> Lock(Mtx); //_Maybe_run_deferred_function(_Lock);
					while (!_Ready)
					{
						CV.wait(Lock);
					}
				}
				struct Test {
					Test(Shared_State* _state) : State(_state){}
					bool operator()() const	{ return (State->_Ready != 0);	}
					Shared_State *State;
				}


				template<class _Rep, class _Per>
				future_status wait_for(	const std::chrono::duration<_Rep, _Per>& _Rel_time)
				{	// wait for duration
					std::unique_lock<std::mutex> Lock(Mtx);
					//if (_Has_deferred_function())return (future_status::deferred);
					if (CV.wait_for(Lock, _Rel_time, Test(this)))
					{
						return (future_status::ready);
					}
					return (future_status::timeout);
				}

				bool is_Ready() { return _Ready }
				std::mutex Mtx;
				std::condition_variable CV;
				_Ty& Value;
				uint64_t RefCounter{ 0 };
				bool _Retrieved{ false };
				bool _Ready{false};
				bool _Ready_at_thread_exit{ false };
				bool _Has_stored_result{ false };
				bool _Running{ false };


			private:
  
			};

			template<typename _Ty>
			struct promise
			{
				//Member functions
				
				///constructs/* the promise object*/
				promise();
				promise(promise&& _other) noexcept
					:
					_MyPromise(std::move(_other))
				{}

				promise(const promise& other) = delete;

				/* promise(_associated_state<_Ty>* _State_ptr):_State(_State_ptr, false),_Future_retrieved(false){} */
 
				promise(Shared_State<_Ty>* _State_ptr)
					:
					_MyPromise(_State_ptr, false),
					_Retrieved(false)
				{	// construct from associated asynchronous state object
				}


 

				Shared_State<_Ty>

				//(destructor)/*destructs the promise object*/
				promise() = default;

				promise& operator=(promise&& other) noexcept;/*assigns the shared state*/
				promise& operator=(const promise& rhs) = delete;

				void swap(promise& other) noexcept;/*swaps two promise objects*/


			///Getting the result
				[[nodiscard]] future<_Ty> get_future();/*returns a future associated with the promised result
													   return (future<_Ty>(_MyPromise._Get_state_for_future(), _Nil()));*/

													   ///Setting the result
				void set_value(const _Ty& _Val)
				{/*sets the result to specific value
											   _MyPromise._Get_state_for_set()._Set_value(_Val, false);*/
				}

				/*sets the result to specific value while delivering the notification only at thread exit*/
				void set_value_at_thread_exit(const _Ty& value);
				void set_value_at_thread_exit(_Ty&& value);

 			private:
				Shared_State<_Ty> _MyPromise;
				bool _Retrieved;
			};


			template<typename _Ty>
			struct future
				: public _state_manager<_Ty>
			{
				/// Types
				using _myBase = _state_manager<_Ty>;
				using type = _Ty;

				///Member functions
				/// C TOR
				future() noexcept;/*Default constructor.Constructs a std::future with no shared state.After construction, valid() == false.*/
				future(future&& other) noexcept;/* Move constructor.Constructs a std::future with the shared state of other using move semantics.After construction, other.valid() == false.*/
				future(const future& other) = delete;/*future is not CopyConstructible.*/
				/// PARAMS
				/*other	-	another std::future to acquire shared state from*/

				///------------------------
				~future() = default;/*Releases any shared state.This means*/
				/* if the return object or provider holds the last reference to its shared state,
				the shared state is destroyed; and the return object or provider gives up its reference to
				its shared state;
				these actions will not block for the shared state to become ready,
				except that it may block if all of the following are true:
				the shared state was created by a call to std::async, the shared state is not yet ready, and this was the last reference to the shared state*/


				future& operator=(future&& other) noexcept
				{/* other	-	a std::future that will transfer state to *this*/
					*this = std::forward<future>(_other);
					return this;
				};
				future& operator=(const future& other) = delete;

				/// Getting the Results
				/*returns the result*/                                                                      /*Return value*/
				type get();       //    (member only of generic future template)               // The value v stored in the shared state, as std::move(v).
				type& get();      //    (member only of future<T&> template specialization)    // The reference stored as value in the shared state.
				void get();       //    (member only of future<void> template specialization)  // Nothing. 

				///State
				bool valid() const noexcept { return true; }/*checks if the future has a shared state*/ /*RETURN: true if *this refers to a shared state, otherwise false.*/
				void wait() const {}/*waits for the result to become available Block*/

				template< class Rep, class Period >
				void wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const {} /* waits for the result, returns if it is not available for the specified timeout duration*/


				template< class Clock, class Duration >
				future_status wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) const
				{}
				/*waits for the result, returns if it is not available until specified time point has been reached
											It blocks until specified timeout_time has been reached or the result becomes available, whichever comes first. */
			};
		}
	}
}

//return (new _Task_async_state<_Ret>(_STD forward<_Fty>(_Fnarg)));
// v

#endif 





/*
==========================================================================================================================================================================
														   NOTES:
==========================================================================================================================================================================

https://en.cppreference.com/w/cpp/thread/yield

 Promise implementation
 https://github.com/arlettedata/promise/blob/master/promise.h

  Future Implementation
 https://github.com/mongodb/mongo/blob/master/src/mongo/util/future.h

  C++ Threading Primatives from Scratch
 https://www.youtube.com/watch?v=9TVvpgHJElU&feature=emb_logo

*/







/*
,___,
|   |
|___|

MAIN()
,_____________________________,
|                             |                                                 
|                             |                                                 
|                             |          ,_____________________________,        ,_____________________________,                               
|                             |          |                             |        |                             |                                
|                             |          |                             |        |                             |                                                                                         
|_____________________________|          |_____________________________|        |_____________________________|                                                                                        
                                                                                                                                  
,_____________________________,          ,_____________________________,        ,_____________________________,                               
|                             |          |                             |        |                             |                               
|      MergeSort(Left)1       |-------===|        MergeSort(Left)1     |===---->|        MergeSort(Right)2    |                               
|      MergeSort(Left)2       |--------->|           (Suspend)         |        |                             |                               
|                             |          | MergeSort(Left)3            |        |                             |                               
|                             |          | MergeSort(Left)4            |        |                             |                               
|_____________________________|          |_____________________________|        |_____________________________|                               
                                                    							           
,_____________________________,          ,_____________________________,        ,_____________________________,                               
|                             |          |                             |        |                             |                               
|                             |          |                             |        |                             |                               
|                             |          |                             |        |                             |                               
|                             |          |                             |        |                             |                               
|                             |          |                             |        |                             |                               
|_____________________________|          |_____________________________|        |_____________________________|                               

,_____________________________,          ,_____________________________,        ,_____________________________,
|                             |          |                             |        |                             |                               
|                             |          |                             |        |                             |                               
|                             |          |                             |        |                             |                               
|                             |          |                             |        |                             |                               
|                             |          |                             |        |                             |                               
|_____________________________|          |_____________________________|        |_____________________________|                               
																													   \











*/