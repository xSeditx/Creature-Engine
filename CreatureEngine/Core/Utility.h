
#include<vector>



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
template<typename _FUNC, typename _TUPLE, size_t ..._SZ >
auto apply_tuple_impl(_FUNC&& _function, _TUPLE&& _arguments, std::index_sequence<_SZ...>)
->decltype((std::forward<_FUNC>(_function)(std::get<_SZ>(std::forward<_TUPLE>(_arguments))...)))/// Return was Decltype(auto) just auto and trailing return added to create zero/low cost abstraction
{
	return std::forward<_FUNC>(_function)(std::get<_SZ>(std::forward<_TUPLE>(_arguments))...);
}

template<typename _FUNC, typename _TUPLE>
decltype(auto) apply_from_tuple(_FUNC&& _function, _TUPLE&& _tupleArguments)
{
	std::size_t constexpr tSize
	{// Initialize tSize instead of using =
		 std::tuple_size<typename std::remove_reference<_TUPLE>::type>::value
	};

	return
		apply_tuple_impl(std::forward<_FUNC>(_function),
			std::forward<_TUPLE>(_tupleArguments),
			std::make_index_sequence<tSize>());
}




template<typename _Ty, size_t _SZ>
class ring_buffer
{
public:
	using value_type = _Ty;
	using reference = _Ty&;
	using pointer = const _Ty*;

	ring_buffer() = default;

	bool push_back(reference _element)
	{/// Just deferring this function to make it compatible with older Queue system for Threadpool
		return push(_element);
	}
	bool push(reference _element)
	{// Adds new Element to Queue
		std::atomic<size_t> OldWritePosition;/// = WritePosition.load();
		std::atomic<size_t> NewWritePosition = NextElement(OldWritePosition);
		if (NewWritePosition)/// == ReadPosition.load())
		{/* If Read position and Write position are the same Buffer is Full */
			return false;
		}
		///		Data[OldWritePosition] = _element;
		WritePosition = NewWritePosition;
	}

	/// WE MIGHT BE ABLE TO PACK THE READER AND WRITER INTO THE SAME ATOMIC INTEGER WHICH WILL REDUCE THE OVERHEAD
	bool pop(reference _returnElement)
	{/* Returns True if more elements in Queue*/
		while (true)
		{/* READER Multiple in Existance */
			std::atomic<size_t> OldReadPosition = ReadPosition.load();

			if (WritePosition.load() == OldReadPosition.load())
			{// If attempting to read the same position again or buffer is full return false;
				return false;
			}

			_returnElement = Data[OldReadPosition];// Perhaps std::move() would be better for performance
			if (ReadPosition.compare_exchange_strong(OldReadPosition, NextElement(OldReadPosition)))
			{
				return true;
			}
		}
	}

	auto front()
	{/// Possibly wrong currently a place holder to make the experimental ringbuffer compatible with older style of queue
		return &Data[ReadPosition];
	}
	void pop_front()
	{/// Possibly wrong currently a place holder to make the experimental ringbuffer compatible with older style of queue
		WritePosition = NextElement(WritePosition);
	}
	bool empty()
	{/// Possibly wrong currently a place holder to make the experimental ringbuffer compatible with older style of queue
		return (ReadPosition == WritePosition);
	}

	reference operator[](const int _offset)
	{
		return Data[_offset]; //[ReadPosition % BufferSize];
	}

	bool destroy()
	{
		delete[](Data);
	}

	size_t size()
	{
		return BufferSize;
	}

private:

	std::array<std::atomic<_Ty*>, _SZ + 1> Data;

	size_t NextElement(size_t _pos)
	{
		return ++_pos == BufferSize ? 0 : _pos;
	}

	size_t
		ReadPosition{ 0 },
		WritePosition{ 0 };
	size_t Length, Size, Start, End, Elements;
	size_t BufferSize = _SZ + 1;
};
