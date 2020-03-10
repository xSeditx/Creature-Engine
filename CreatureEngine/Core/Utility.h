
#include<vector>
#include<atomic>



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