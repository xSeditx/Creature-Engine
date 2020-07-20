
#include<vector>
#include<atomic>

struct Async_Dynamic_Buffer
{
   
};

/* ==============================================================================================================
       LIKELY NOTHING BUT JUST MULLING OVER THE IDEA OF USING PROPERTY GETTERS AND SETTERS

	   C++ Properties:    https://www.codeproject.com/Articles/118921/C-Properties
	   Public Fields or Properties: http://www.cplusplus.com/forum/beginner/95862/
   ==============================================================================================================

#define PROPERTY(t,n)  __declspec( property (put = property__set_##n, get = property__get_##n)) t n; \
	typedef t property__tmp_type_##n

#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n

#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n;\
	typedef t property__tmp_type_##n

#define GET(n) property__tmp_type_##n property__get_##n()
#define SET(n) void property__set_##n(const property__tmp_type_##n& value)

#ifndef _PROPERTIES_H
#define _PROPERTIES_H

#define PROPERTY(t,n)  __declspec( property (put = property__set_##n, get = property__get_##n)) t n; \
	typedef t property__tmp_type_##n

#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n

#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n;\
	typedef t property__tmp_type_##n

#define GET(n) property__tmp_type_##n property__get_##n()
#define SET(n) void property__set_##n(const property__tmp_type_##n& value)

#endif  _PROPERTIES_H

//#define Property(_Data_Type, __get, __set)  __declspec(property (__set = setFunction, __get = getFunction)) _Data_Type property - name;


#define Property(__PUT, __GET)  __declspec(property(put = __PUT, get = __GET))

//__DATA_N_TYPE

struct PropTest{
private:
	int _x;
	float SecondProperty{ 0 };
public:


	// SecondProperty Variable
	Property(set_SecondProp, get_SecondProp) float Second;
	float get_SecondProp()                                  { return SecondProperty; }
	void  set_SecondProp(float _val)                        { SecondProperty = _val; }

	// _x Variable
	Property(property__set_x , property__get_x) int x;
	int  property__get_x()                 { return _x;  }
	void property__set_x(const int& value) { _x = value; }
};*/
// ===========================================================================================================================================================
// ===========================================================================================================================================================



//
//template<typename T, typename N, typename O>
//T power(T x, N n, O op)
//{
//	if (n == 0) return op;//identity_element(op);
//
//	while ((n & 1) == 0) {
//		n > 1;
//		x = op(x, x);
//	}
//	T result = x;
//	n >>= 1;
//	while (n != 0)
//	{
//		x = op(x, x);
//		if ((n & 1) != 0) result = op(result, x);
//		n >> 1;
//	}
//	return result'
//}

/*    Apply tuple to a function: https://cpppatterns.com/patterns/apply-tuple-to-function.html    */
//template<typename _FUNC, typename _TUPLE, size_t ..._SZ >
//auto apply_tuple_impl(_FUNC&& _function, _TUPLE&& _arguments, std::index_sequence<_SZ...>)
//->decltype((std::forward<_FUNC>(_function)(std::get<_SZ>(std::forward<_TUPLE>(_arguments))...)))/// Return was Decltype(auto) just auto and trailing return added to create zero/low cost abstraction
//{
//	return std::forward<_FUNC>(_function)(std::get<_SZ>(std::forward<_TUPLE>(_arguments))...);
//}
//
//template<typename _FUNC, typename _TUPLE>
//decltype(auto) apply_from_tuple(_FUNC&& _function, _TUPLE&& _tupleArguments)
//{
//	std::size_t constexpr tSize
//	{// Initialize tSize instead of using =
//		 std::tuple_size<typename std::remove_reference<_TUPLE>::type>::value
//	};
//
//	return
//		apply_tuple_impl(std::forward<_FUNC>(_function),
//			std::forward<_TUPLE>(_tupleArguments),
//			std::make_index_sequence<tSize>());
//}



/*
struct do_something_helper
{
	do_something_helper()
	{
		// do something first (e.g. take a lock)
	}
	~do_something_helper()
	{
		// do something after (e.g. release a lock)
	}
};

template<typename T>
T Foo(T(*Func)())
{
	do_something_helper _dummy_helper; // constructor called here

	return Func();
	// destructor called here
}

template< typename Pre, typename Post >
struct scope_guard
{
	scope_guard(Pre&& pre, Post&& post)
		: _post(std::forward< Post >(post))
	{
		pre();
	}
	~scope_guard()
	{
		_post();
	}

	Post _post;
};

template< typename Pre, typename Post >
scope_guard< Pre, Post > make_scope_guard(Pre&& pre, Post&& post)
{
	return scope_guard< Pre, Post >(std::forward< Pre >(pre), std::forward< Post >(post));
}
 */
//template<typename T>
//T Foo(T(*Func)())
//{
//	auto do_something_helper =
//		make_scope_guard(
//			[]() { /* do something first (e.g. take a lock) */ },
//			[]() { /* do something after (e.g. release a lock) */ }
//	);
//
//	return Func();
//}