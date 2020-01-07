#pragma once



///await-expression
///e.await_resume().
//Print("Suspending Thread: " << ThreadID);
//_func(args...);
//jmp_buf Context;
//setjmp(Context);
//auto _function = new Suspend<_FUNC, ARGS... >(&Context, std::move(_func), std::forward<ARGS>(args)...);  // Create our task which binds the functions parameters
// 
//auto result = _function->get_future();                                                           // Get the future of our async task for later use
// 
//int Attempts = 5;
//for (unsigned int n{ 0 }; n != ThreadCount * Attempts; ++n)                                      // K is Tunable for better work distribution
//{// Cycle over all Queues K times and attempt to push our function to one of them
//
//	if (ThreadQueue[static_cast<size_t>((i + n) % ThreadCount)].try_push_front(static_cast<Executor*>(_function)))
//	{// If succeeded return our functions Future
//		//_func(args...);
//		//result.get();
//		return result;
//	}
//}
//
//// In the rare instance that all attempts at adding work fail just push it to the Owned Queue for this thread
//ThreadQueue[i % ThreadCount].push_front(static_cast<Executor*>(_function));
////_func(args...);
//result.get();
//return result;






/*

#include<future>
// STL Async Declaration 
template<class _Fty, class... _ArgTypes>
[[nodiscard]] inline std::future<std::_Invoke_result_t<std::decay_t<_Fty>, std::decay_t<_ArgTypes>...>>
async(_Fty&& _Fnarg, _ArgTypes&&... _Args){}



template<class _Ty>
struct __declspec(novtable) _Deleter_base
{	// abstract base class for managing deletion of state objects
	virtual void _Delete(_Associated_state<_Ty> *) = 0;
	virtual ~_Deleter_base() noexcept
	{	// destroy the object
	}
};


template<class _Ty>
class _Associated_state
{
	using _State_type = _Ty;
	using _Mydel = _Deleter_base<_Ty>;
	_Mydel *_Deleter;

	virtual ~_Associated_state() noexcept
	{	// destroy
		if (_Has_stored_result && !_Ready)
		{	// registered for release at thread exit
			_Cond._Unregister(_Mtx);
		}
	}

	volatile long _Refs;

	_Ty _Result;
	std::mutex _Mtx;
	std::condition_variable _Cond;
	bool _Retrieved;
	int _Ready;
	bool _Ready_at_thread_exit;
	bool _Has_stored_result;
	bool _Running;
};

template<class _Ty>
class _State_manager
{
	_Associated_state<_Ty> *_Assoc_state;
	bool _Get_only_once;
};

template<typename T>
struct _Promise
{

	_State_manager<T> _State;
	bool _Future_retrieved;
};

template<typename T>
struct Promise
{
	_Promise<T> MyPromise;
};








template<class _Ty,
	class _Alloc> inline
	_Associated_state<_Ty> *_Make_associated_state(const _Alloc& _Al) {}


*/


