#pragma once

#include"Common.h"
#include<stack>
#include<atomic>
#pragma warning(disable: 4267) //Warning size_t to uint32_t 


/* =======================================================================================
/*      Pool Of Memory for Objects which are all the same size and are numerous. 
/*  The Type of data we wish to place in Arrays yet regularly add and Delete 
/* =======================================================================================*/
template<typename _Ty, size_t _SZ>
struct Memory_Pool
{/* Currently Requires an Object be default Constructable but might change in near future*/
    using data_type = _Ty;

    using value_type = _Ty;
    using pointer = _Ty * ;
    using const_pointer = const _Ty*;

    using reference = _Ty & ;
    using constant_reference = const _Ty&;

    using size_type = size_t;
    using difference_type = size_t;

    using Lock_t = std::unique_lock<std::mutex>;




    /* Ensure Pool is not Copies or Copy Assigned */
    Memory_Pool(Memory_Pool&) = delete;
    Memory_Pool& operator=(const Memory_Pool&) = delete;

    /*W Control Pool with _size number of free Blocks*/
    Memory_Pool() noexcept
        :
        BlockCount(_SZ)
    {
        if (!Data)
        {
            Alignment = alignof(data_type);
            size_t Extra = sizeof(data_type) % Alignment;
            BlockSize = sizeof(data_type) + ( Extra);
            Size = BlockSize * BlockCount;
          //  Data = static_cast<_Ty*>(malloc(Size));  
        }
        // No Need for Critical section as c_Tor is Naturally Thread Safe
        for (uint32_t i = 0; i < _SZ; ++i)
        {// Push indices in reverse Order to the Stack 
            freeIDs.push(_SZ - i - 1);
        }
    }
    ~Memory_Pool()
    {
        Destroy_Memory_Pool();
    }
    void Destroy_Memory_Pool()
    {
    }

    /*RW Return pointer to free block of Memory */
    void *Allocate()// noexcept
    {
        {/* ~ CRITICAL SECTION ~*/
//            Lock_t Lock(PoolMtx);

            void *Address = &Data[freeIDs.top()];   //+ (BlockSize * freeIDs.top());// [freeIDs.top()];
            freeIDs.pop();
            return static_cast<void*> (Address);   
        }
    }

    /*RW Returns block of Memory Index to the Pool */
    void Deallocate(void *_item) // noexcept
    {
        // Needing to lock on every deallocation feels like 
        // it might get expensive however idk anyway around it

        {/* ~ CRITICAL SECTION ~*/
            //Lock_t Lock(PoolMtx);

            freeIDs.push(get_id(_item));
        }
    }


    /*W Wipe all Elements clear to Zero */
    void clear() noexcept
    {
        {/* ~ CRITICAL SECTION ~*/
            Lock_t Lock(PoolMtx);

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
    }

    /*W Currently just calls clear,
    /* NOTE: very likely more functionality will be added soon */
    void reset() noexcept                                                 { clear();     }
                                                                          
    /*R Start of Raw Data in the pool*/                                   
    data_type *begin() noexcept                                           { return Data; }
    /*R Last Block in the pool */                                         
    data_type *end() noexcept                                             { return &Data[BlockCount];           }


    /*R Returns Size of the Raw Data in all the Blocks */
    size_t size() const noexcept                                          { return BlockCount * BlockSize;      }

    /*R Determines if the Memory Pool is Empty */
    bool is_Empty() const noexcept                                        { return freeIDs.size() >= BlockSize; }
    /*R Determines if the Memory Pool is Full */
    bool is_Full() const noexcept                                         { return freeIDs.empty();             }
    /*R Returns if there is still room to Allocate  */                                            
    bool is_Not_Full() const noexcept                                     { return !freeIDs.empty();            }

    /*R Returns Pointer to the Start of the raw Data */
    char *get_Data(size_t _at) noexcept                                   { return reinterpret_cast<char*>(Data) + _at; }
    /*R Calculates the Index of a Memory block via pointer*/              
    uint32_t get_id(void *_item) noexcept                                 { return static_cast<uint32_t>(((char*)_item - (char*)Data) / sizeof(data_type));    }


    /* Size of each Element allocated by this class */
    size_t chunkSize() const noexcept                                     { return BlockSize;    }
    /* Max number of Objects Allocator is able to create */                                    
    size_t chunkCount() const noexcept                                    { return BlockCount;   }
                                                                          
    /*R Returns the Object at the Specified _Index */                     
    data_type& operator[](size_t _index) const noexcept                   { return const_cast<_Ty&>(Data[_index]); }
//
//   /*W Locks the Pool while we read Item at the Specified _Index */
//   data_type& operator[](size_t _index) noexcept                         {/*~CRITICAL SECTIONS~*/ 
//                                                                         { Lock_t Lock(PoolMtx);  return Data[_index]; } }


    /*======================================================================================================================================================================
    /*  POSSIBLE FIXES FOR MEMORY CORRUPTION TAKING PLACE WHENEVER THE PROGRAM USES NEW OR DELETE ANYWHERE ELSE AFTER THE TEST RUN FOR THIS CLASS
    /*======================================================================================================================================================================
    /*
    /*  REPLACE MALLOC WITH THIS ONCE EVERYTHING WORKS:
    /*  _aligned_malloc(size, alignof(_Ty));
    /*
    /*  You delete pointers which you have allocated with operator new. You have a static array of pointers there with automatic storage. – DeiDei Sep 10 '16 at 18:50
    /*===================================================================================================================================================================== */
    void *operator new(size_t _size)
    {
        Lock_t Lock;
        std::cout << " Memory Pool :: new() " << std::endl;
        return malloc(_size);
    }
    void operator delete(void *_item)
    {
        Lock_t Lock(PoolMtx);
        std::cout << " Memory Pool :: delete()" << std::endl;
        if (_item)
        {
            free(_item);
        }
    }
    // Overloading Global new[] operator
    void* operator new[](size_t sz)
    {
        Lock_t Lock(PoolMtx);
        std::cout << " Memory Pool  :: new []()" << std::endl;
        __debugbreak();
      //  void* m = malloc(sz);
        
        return malloc(sz);
    }
    // Overloading Global delete[] operator
    void operator delete[](void* m)
    {
        std::cout << " Memory Pool :: delete[]()" << std::endl;
        __debugbreak();
        free(m);
    }

private:
    uint32_t BlockCount{ 0 };// Number of Items Pools is Capable of Storing
    uint32_t BlockSize{ sizeof(data_type)};// Size of each block in the Pool
    size_t Size{ 0 };
    size_t Alignment{ 0 };

    data_type Data[_SZ];

    std::mutex PoolMtx;

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
        /*  RANGE FOR Returns an Iterator for Begin and End. Inorder to properly wrap the Ring Buffer that Iterator needs to wrap
            auto __begin = begin_expr;
            auto __end = end_expr;
        /* =======================================================================================================================*/
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
         //   DEBUG_CODE(Print("Error: Ring Buffer Full attempting to Forward a value:");)
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
           // DEBUG_CODE(Print("Error: Ring Buffer Full and you are Attempting to Add more Elements to it:" << _element);)
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
             //   DEBUG_CODE(Print("Error: Ring Buffer Empty. You are Attempting to Pop Elements from empty buffer"); );
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

   RANGE FOR LOOP Implementation
 ---------------------------------
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



     OPERATOR NEW Implementation
 -----------------------------------
 --> new_scalar.cpp

  _CRT_SECURITYCRITICAL_ATTRIBUTE
  void* __CRTDECL operator new(size_t const size)
  {
      for (;;)
      {
          if (void* const block = malloc(size))
          {
              return block;
          }
  
          if (_callnewh(size) == 0)
          {
              if (size == SIZE_MAX)
              {
                  __scrt_throw_std_bad_array_new_length();
              }
              else
              {
                  __scrt_throw_std_bad_alloc();
              }
          }
  
          // The new handler was successful; try to allocate again...
      }
  }

  AFTER SETTING ALL THE LOCKS AND CRITICAL SECTIONS UP
  -->delete_scalar.cpp
// +--+--------------+  +------+-------------+
// |delete_array_size|  |delete_array_nothrow|
// +-----------------+  +--------------------+

_CRT_SECURITYCRITICAL_ATTRIBUTE
void __CRTDECL operator delete(void* const block) noexcept
{
    #ifdef _DEBUG
    _free_dbg(block, _UNKNOWN_BLOCK);
    #else
    free(block);
    #endif
}


/*=========================================================================================*/


 