#pragma once

#include"Common.h"
#include<stack>
// 2467 Warning size_t to uint32_t 

#pragma warning(disable: 4267)
/* =======================================================================================
/*      Pool Of Memory for Objects which are all the same size and are numerous. 
/*  The Type of data we wish to place in Arrays yet regularly add and Delete 
/* =======================================================================================*/
template<typename _Ty>
struct Memory_Pool
{/* Currently Requires an Object be default Constructable but might change in near future*/
    
        /* Ensure Pool is not Copies or Copy Assigned */
    Memory_Pool(Memory_Pool&) = delete;
    Memory_Pool& operator=(const Memory_Pool&) = delete;

    using data_type = _Ty;

    /* Control Pool with _size number of free Blocks*/
    Memory_Pool(size_t _size)
        :
        Data(new _Ty[_size]),
        BlockCount(_size)
    {
        for (uint32_t i = 0; i < _size; ++i)
        {// Push indices in reverse Order to the Stack 
            freeIDs.push(_size - i );
        }
    }

    /* Return pointer to free block of Memory */
    inline void *Allocate()
    {
        void *Address = &Data[freeIDs.top()];
        freeIDs.pop();
        return static_cast<void*> (Address);
    }
    /* Returns block of Memory Index to the Pool */
    inline void Deallocate(void *_item)
    {
        freeIDs.push(((char *)_item - (char*)Data) / sizeof(data_type));
    }

    /* Start of Raw Data in the pool*/
    data_type *begin() 
    {
        return Data;
    }
    /* End of the Raw Data in the pool. Points to the Very last Block at the start of it */
    data_type *end()
    {
        Print("Data size" << sizeof(Data));
        return &Data[BlockCount];//Data + (BlockSize * BlockCount) - BlockSize; // Data[BlockCount] <- Same thing? 
    }
    /* Returns Size of the Raw Data in all the Blocks */
    size_t size() { return BlockCount * BlockSize; }


    /* Wipe all Elements clear to Zero */
    void clear()
    {
        memset(Data, 0, size());
        while (!freeIDs.empty())
        { // Make sure we are not putting IDs onto a stack that has elements already
            freeIDs.pop(); 
        }
        for (uint32_t i = 0; i < chunkCount(); ++i)
        {// Push indices in reverse Order to the Stack 
            freeIDs.push(chunkCount() - i);
        }
    }
    /* Currently just calls clear, very likely more functionality will be added soon */
    void reset() 
    {
        clear();
    }


    /* Determines if the Memory Pool is Empty */
    bool is_Empty() { return freeIDs.size() >= BlockSize; }
    /* Determines if the Memory Pool is Full */
    bool is_Full() 
    {
        bool result = freeIDs.empty(); 
        return result;
    }
    /* Returns if there is still room to Allocate  */
    bool is_Not_Full()
    {
        return !freeIDs.empty();
    }

    /* Returns Pointer to the Start of the raw Data */
    char *get_Data(size_t _at) { return (char*)(Data)+_at; }

    /* Size of each Element allocated by this class */
    size_t chunkSize()  { return BlockSize;  }
    /* Max number of Objects this Allocator is able to create */
    size_t chunkCount() { return BlockCount; }

private:
    uint32_t BlockCount{ 0 };
    uint32_t BlockSize{ sizeof(data_type) };

    data_type *Data{ nullptr };
    std::stack<uint32_t> freeIDs;
};
/* Assertive Test of the Memory Pool Class */
bool TEST_Memory_Pool_Class();







#include<array>

template<typename _Ty, size_t _SZ>
class ring_buffer
{
public:
    using value_type = _Ty;
    using reference = _Ty & ;
    using pointer = const _Ty*;

    ring_buffer() = default;

    /* Adds a new Element to the Ring Buffer*/
    bool push(const reference _element)
    {// Adds new Element to Queue
        auto OldWritePosition = WritePosition.load();
        auto NewWritePosition = NextElement(OldWritePosition);

        if (NewWritePosition == ReadPosition.load())
        {/* If Read position and Write position are the same Buffer is Full */
            DEBUG_CODE
            (
                Print("Error: Ring Buffer Full and you are Attempting to Add more Elements to it:" << _element); ;// assert(false);
            );
            return false;
        }
        Data[OldWritePosition].store(_element);
        WritePosition.store(NewWritePosition);
        return true;
    }

    /* Pops an Element from the back of the Buffer and returns it as a parameter */
    bool pop(reference _returnElement)
    {/* Returns True if more elements in Queue*/
        while (true)
        {/* READER Multiple in Existance */
            auto OldWritePosition = WritePosition.load();
            auto OldReadPosition = ReadPosition.load();

            if (OldWritePosition == OldReadPosition)
            {// If attempting to read the same position again or buffer is full return false;
                return false;
            }

            _returnElement = Data[OldReadPosition].load();

            if (ReadPosition.compare_exchange_strong(OldReadPosition, NextElement(OldReadPosition)))
            {
                return true;
            }
        }
    }/// WE MIGHT BE ABLE TO PACK THE READER AND WRITER INTO THE SAME ATOMIC INTEGER WHICH WILL REDUCE THE OVERHEAD

    /* Get the Start of the Ring Buffer */
// auto front()
// {
//     return &Data[ReadPosition];
// }/// Possibly wrong currently a place holder to make the experimental ringbuffer compatible with older style of queue

     /* Remove the First Element in the Ring Buffer */
// void pop_front()
// {
//     WritePosition = NextElement(WritePosition);
// }/// Possibly wrong currently a place holder to make the experimental ringbuffer compatible with older style of queue

    /* Test if container is Empty
       NOTE: Change this to is_Empty() because the STL naming convention is retarded */
    bool empty()
    {
        return (ReadPosition == WritePosition);
    }/// Possibly wrong currently a place holder to make the experimental ringbuffer compatible with older style of queue

    /* Overload to properly wrap the _offset in the Ring Buffer */
    reference operator[](const int _offset)
    {
        return Data[_offset % BufferSize]; //[ReadPosition % BufferSize];
    }

    /* Deletes the Ring Buffers Data */
    bool destroy()
    {
        delete[](Data);
    }

    /* Current size of the Ring Buffer from Start to End */
    size_t size()
    {
        return WritePosition - ReadPosition; //return BufferSize; b<0- This is not Correct, We need WritePosition - ReadPosition;
    }

private:

    std::array<std::atomic<_Ty>, _SZ + 1> Data;

    /* Retrieved the next Element in the RB taking care to wrap when needed */
    size_t NextElement(size_t _pos)
    {// Note: This can be accomplished Via Bit operations extremely fast if the Size is proper
        return ++_pos == BufferSize ? 0 : _pos;
    }

    std::atomic<size_t>
        ReadPosition{ 0 },
        WritePosition{ 0 };

    size_t BufferSize = _SZ + 1;
};
/* Assertive Test of Ring Buffer Class */
bool TEST_Ring_Buffer_Class();




/*

bool push(const _Ty& newElement);
bool push(_Ty&& newElement);
popAll(Output *itr);
pushRange(InputIterator *first, InputIterator *last);
pushElements(InputIterator *first, size_t numElements);
popElements(outputIterator* itr, size_t numElements);




//memory.h(167) :
//error C2664 : 'bool std::_Atomic_ullong::compare_exchange_strong(unsigned __int64 &,unsigned __int64,std::memory_order) noexcept' :
//cannot convert argument 1 from 'unsigned __int64' to 'unsigned __int64 &' _returnElement = Data[OldReadPosition.load()].load();







/*

                DEBUG_CODE
                (
                    Print("Error: Ring Buffer Full and you are Attempting to Add more Elements to it"); assert(false);
                );




*/







/* Note:
RANGE_FOR_LOOP()
{
  auto && __range = range_expression ;
  auto __begin = begin_expr;
  auto __end = end_expr;
  for (;__begin != __end; ++__begin) {
    range_declaration = *__begin;
    loop_statement
  }
}
*/






// bool push_back(reference _element)
// {/// Just deferring this function to make it compatible with older Queue system for Threadpool
//     return push(_element);
// }