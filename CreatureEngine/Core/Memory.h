#pragma once

#include"Common.h"
#include<stack>

#pragma warning(disable: 4267)//Warning size_t to uint32_t 


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
    Memory_Pool(size_t _size) noexcept
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
    inline void *Allocate() noexcept
    {
        void *Address = &Data[freeIDs.top()];
        freeIDs.pop();
        return static_cast<void*> (Address);
    }
    /* Returns block of Memory Index to the Pool */
    inline void Deallocate(void *_item) noexcept
    {
        freeIDs.push(get_id(_item));
    }


    /* Wipe all Elements clear to Zero */
    void clear() noexcept
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

    /* Currently just calls clear,
    /* NOTE: very likely more functionality will be added soon */
    void reset() noexcept                                           { clear();     }

    /* Start of Raw Data in the pool*/
    data_type *begin() noexcept                                     { return Data; }
    /* Last Block in the pool */
    data_type *end() noexcept                                       { return &Data[BlockCount];           }
    /* Returns Size of the Raw Data in all the Blocks */
    size_t size() noexcept                                          { return BlockCount * BlockSize;      }

    /* Determines if the Memory Pool is Empty */
    bool is_Empty() noexcept                                        { return freeIDs.size() >= BlockSize; }
    /* Determines if the Memory Pool is Full */
    bool is_Full() noexcept                                         { return freeIDs.empty();             }
    /* Returns if there is still room to Allocate  */                                            
    bool is_Not_Full() noexcept                                     { return !freeIDs.empty();            }

    /* Returns Pointer to the Start of the raw Data */
    char *get_Data(size_t _at) noexcept                             { return reinterpret_cast<char*>(Data) + _at; }
    /* Calculates the Index of a Memory block via pointer*/
    uint32_t get_id(void *_item) noexcept                           { return static_cast<uint32_t>(((char*)_item - (char*)Data) / sizeof(data_type));    }


    /* Size of each Element allocated by this class */
    size_t chunkSize() noexcept                                     { return BlockSize;    }
    /* Max number of Objects Allocator is able to create */                         
    size_t chunkCount() noexcept                                    { return BlockCount;   }

    /* Returns the Object at the Specified _Index */
    data_type& operator[](size_t _index) noexcept                   { return Data[_index]; }

private:
    uint32_t BlockCount{ 0 };
    uint32_t BlockSize{ sizeof(data_type) };

    data_type *Data{ nullptr };
    std::stack<uint32_t> freeIDs;
};

/* Assertive Test of the Memory Pool Class */
bool TEST_Memory_Pool_Class();
/* =======================================================================================*/

        



#include<array>


/* =======================================================================================
/*      Ring Buffer class. Static Array of Data which cycles around when it reaches the 
/*  end The below Implementation is Thread Safe and Lock Free
/*  TODO: Pack the Reader and Writer into a Single Atomic Int to save on overhead
/* =======================================================================================*/
template<typename _Ty, size_t _SZ>
class ring_buffer
{
public:

    class iterator
    {
    public:
        using self_type  = iterator;
        using value_type = std::atomic<_Ty>;
        using reference  = value_type&;
        using pointer    = value_type*;
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = int;
        
        size_t Difference{ 0 };
        iterator(pointer ptr, ring_buffer<_Ty, _SZ>* _rb)
            :
            ptr_(ptr),
            Parent(_rb)
        {
            Difference = ((ptr - _rb->front()) / sizeof(std::atomic<_Ty>));
        }

        self_type operator++()                { self_type i = *this; get_Next(ptr_); return i; }
        self_type operator++(int junk)        { get_Next(ptr_); return *this;                  }  //{ ptr_++; return *this;                  }
        reference operator* ()                { return *ptr_;                          }  // { return *ptr_;                          }
        pointer   operator->()                { return ptr_;                           }
        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_;               }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_;               }
    private:
        void get_Next(pointer _location)
        {
            ++_location >= Parent->back() ? _location = Parent->front() : _location;
        }

        pointer ptr_;
        ring_buffer<_Ty, _SZ> *Parent;
    };

    /* begin() to allow for Range For Loops */
    iterator begin()
    {
        CurrentPosition.store(ReadPosition.load());
        return iterator(&Data[ReadPosition], this);
    }
    /* end() to allow for Range For Loops  */
    iterator end()
    {       // iterator Itr = iterator(&Data[WritePosition], this);//Address;
        return iterator(&Data[WritePosition], this);
    }

    ring_buffer() = default;

    using value_type = _Ty;
    using reference = _Ty&;
    using pointer = const _Ty*;

    using atomic_value_type = std::atomic<_Ty>;
    using atomic_ptr = std::atomic<_Ty>*;

    /* Returns Index from a pointer by Subtracting the Starting Address from provided Address */
    size_t get_Index(void *_ptr) 
    {
        size_t result = (static_cast<atomic_ptr>(_ptr) - front());
        return result;
    }
    /* Gets a Pointer to _element in the Ring Buffer */
    void* get_Ptr(size_t _element)
    {
        atomic_ptr result = &Data[_element % BufferSize];
        return result;
    }

    /* Return the number of Elements in the Ring Buffer before it Rolls Over */
    size_t element_Count()
    {
        return BufferSize ;
    }

   /* pushRange   (InputIterator* first, InputIterator* last); */
   /* pushElements(InputIterator* first, size_t numElements);  */
   /* popElements (outputIterator*  itr, size_t numElements);  */
   /* popAll      (Output* itr);                               */


    /* Forwards an Element to the Ring Buffer */
    bool push(_Ty&& _element)
    {
        auto OldWritePosition = WritePosition.load();
        auto NewWritePosition = nextElement(OldWritePosition);

        if (NewWritePosition == ReadPosition.load())
        {/* If Read position and Write position are the same Buffer is Full */
            DEBUG_CODE(Print("Error: Ring Buffer Full attempting to Forward a value:");)
            return false;
        }
        Data[OldWritePosition].store(std::forward<value_type>(_element));
        WritePosition.store(NewWritePosition);
        return true;
    }

    /* Adds a new Element to the Ring Buffer*/
    bool push(const reference _element)
    {// Adds new Element to Queue
        auto OldWritePosition = WritePosition.load();
        auto NewWritePosition = nextElement(OldWritePosition);

        if (NewWritePosition == ReadPosition.load())
        {/* If Read position and Write position are the same Buffer is Full */
            DEBUG_CODE(Print("Error: Ring Buffer Full and you are Attempting to Add more Elements to it:" << _element);)
            return false;
        }
        Data[OldWritePosition].store(_element);
        WritePosition.store(NewWritePosition);
        return true;
    }

    /* Pops an Element from the back of the Buffer and returns it as a parameter  */
    bool pop(reference _returnElement)
    {/* Returns True if more elements in Queue*/
        while (true)
        {/* READER: Multiple in Existance */
            auto OldWritePosition = WritePosition.load();
            auto OldReadPosition = ReadPosition.load();

            if (OldWritePosition == OldReadPosition)
            {// If attempting to read the same position again or buffer is full return false;
                DEBUG_CODE(Print("Error: Ring Buffer Empty. You are Attempting to Pop Elements from empty buffer"); );
                return false;
            }

            _returnElement = Data[OldReadPosition].load();

            if (ReadPosition.compare_exchange_strong(OldReadPosition, nextElement(OldReadPosition)))
            {
                return true;
            }
        }
    }/// WE MIGHT BE ABLE TO PACK THE READER AND WRITER INTO THE SAME ATOMIC INTEGER WHICH WILL REDUCE THE OVERHEAD

    /* Get the Start of the Ring Buffer */
    auto front()    { return &Data[0];    } 
    /* Get the Start of the Ring Buffer */
    auto back()    { return &Data[BufferSize - 1];    }

    /* Remove the First Element in the Ring Buffer */
    void pop_front()    { ReadPosition = nextElement(ReadPosition);    }

    /* Test if container is Empty
       NOTE: Change this to is_Empty() because the STL naming convention is retarded */
    bool is_Empty()    { return (ReadPosition == WritePosition);    }

    /* Deletes the Ring Buffers Data */
    bool destroy()    { delete[](Data);    }//return BufferSize; b<0- This is not Correct, We need WritePosition - ReadPosition;

    /* Current size of the Ring Buffer from Start to End */
    size_t size()    { return WritePosition - ReadPosition;     }
 //[ReadPosition % BufferSize];
    /* Overload to properly wrap the _offset in the Ring Buffer */
    reference operator[](const int _offset)    { return Data[_offset % BufferSize];    }

private:

    /* Retrieved the next Element in the RB taking care to wrap when needed */
    size_t nextElement(size_t _pos)
    {// Note: This can be accomplished Via Bit operations extremely fast if the Size is proper
        return ++_pos == BufferSize ? 0 : _pos;
    }
    /* Retrieved the next Element in the RB taking care to wrap when needed */
    pointer incMemory(size_t _pos)
    {// Note: This can be accomplished Via Bit operations extremely fast if the Size is proper
        size_t Index =  ++_pos == BufferSize ? 0 : _pos;

    }


    std::atomic<size_t>
        ReadPosition{ 0 },
        WritePosition{ 0 },
        CurrentPosition{ 0 };

    size_t BufferSize = _SZ + 1;
    std::array<std::atomic<_Ty>, _SZ + 1> Data;
};

/* Assertive Test of Ring Buffer Class */
bool TEST_Ring_Buffer_Class();
/* =======================================================================================*/







/*=======================================================================================
         NOTES:
/*=========================================================================================

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
/*=========================================================================================*/