#include"Memory.h"


struct TestClass
{
    TestClass() = default;
    float F1;
    size_t Sz1;
    bool B1, B2, B3;
    size_t Sz2;
    uint32_t Int1;
    float F2;
    char C1;
    float F3;
    static Memory_Pool<TestClass> Pool;


    TestClass(int _value)
        :
        Value(_value)
    {}

    uint32_t  Value{ 0 };

    void *operator new(size_t _size)
    {
        return Pool.Allocate();;
    }
    void operator delete(void *_item)
    {
        Pool.Deallocate(_item);
    }
};
Memory_Pool<TestClass> TestClass::Pool(100);

#define PBool(x)  ((x == true) ? " True" : "False")
bool TEST_Memory_Pool_Class()
{

    // Test Array to fill with TestClass objects. These will be contiguous in Memory
    // Allocator returns an Index to a preallocated Block of Memory, delete frees 
    // That memory up for use by additional Objects
    TestClass *Test1[100];

    //==============================================================================
    //   TEST Allocate via TestClass Overloaded New operator
    //==============================================================================
    for (int i = 0; i < 100; ++i)
    {
        Test1[i] = new TestClass(i);
    }

    //==============================================================================
    //   TEST Deallocate via TestClass Overloaded New operator
    //==============================================================================
    for (int i = 0; i < 50; ++i)
    {
        delete(Test1[i]);
    }

    //==============================================================================
    //   TEST if it is returning proper previous freed blocks of memory
    //==============================================================================
    for (int i = 0; i < 50; ++i)
    {
        Test1[i] = new TestClass(i);
    }
    for (int i = 0; i < 25; ++i)
    {
        delete(Test1[i]);
    }
    for (int i = 0; i < 25; ++i)
    {
        Test1[i] = new TestClass(i);
    }

    //==============================================================================
    //   TEST Proper Amount of Elements and Size and Count are reporting correctly
    //==============================================================================

    size_t ElementTest{ 0 };
    for (auto& T : TestClass::Pool)
    {// Cycle over ever Block in the Pool
        Print("Range ForLoop: " << T.Value);
        ++ElementTest;
    }
    assert(ElementTest == TestClass::Pool.chunkCount());
    assert(sizeof(TestClass) == TestClass::Pool.chunkSize());


    //==============================================================================
    //   TEST  get_Data(byte Index) :
    //   to see if it properly returns a single char location
    //==============================================================================

    for (int i{ 0 }; i < TestClass::Pool.size(); ++i)
    {// Cycle over every Byte in the Raw Data
        Print("Data[" << i << "] = " << (int)*TestClass::Pool.get_Data(i));
    }

    //==============================================================================
    //   TEST size()
    //==============================================================================

    Print("Pool Size" << TestClass::Pool.size());
    assert(TestClass::Pool.size() == sizeof(TestClass) * TestClass::Pool.chunkCount());


    //==============================================================================
    // TEST CLEAR() Note: This Test is Fucking Annoying and Takes to long for me. 
    //==============================================================================
    TestClass::Pool.clear();
    for (int i{ 0 }; i < TestClass::Pool.size(); ++i)
    {
        //Print("Data[" << i << "] = " << (int)*TestClass::Pool.get_Data(i));
    }


    //==============================================================================

    //==============================================================================
    //   TEST IS_EMPTY() && IS_FULL()
    //==============================================================================
    for (int i = 0; i < 99; ++i)
    {
        Test1[i] = new TestClass(i);
    }
    // Test if the Pool Filled up after Allocating enough objects to leave a single Block Free
    Print("Is It Full : " << PBool(TestClass::Pool.is_Full()));
    assert(TestClass::Pool.is_Full() == false);

    // Allocate the Last one and Test to see if it is now Filed
    Test1[99] = new TestClass(99);
    Print("Allocate One More is It Full now: " << PBool(TestClass::Pool.is_Full()));
    assert(TestClass::Pool.is_Full() == true);

    // Now Delete one and Test again to see if it is full.
    delete(Test1[1]);
    Print("Testing Delete to Free up Space... Is it Still Full : " << PBool(TestClass::Pool.is_Full()));
    assert(TestClass::Pool.is_Full() == false);

    // Is Not Full returns True when the Container Still have space to grow
    delete(Test1[1]);// Not sure if I want to keep it honestly
    Print("Testing is not full. Tells if Space is Left" << PBool(TestClass::Pool.is_Not_Full()));
    assert(TestClass::Pool.is_Not_Full() == true);


    // Check if is_Empty works as Pool should be Not be Empty
    Print("Is It Empty : " << PBool(TestClass::Pool.is_Empty()));
    assert(TestClass::Pool.is_Empty() == false);

    // Allocate more Spaces then Check if again the Pool is Full 
    Test1[1] = new TestClass(200);
    Test1[2] = new TestClass(300);
    Print("Allocating Again Is it Full Again: " << PBool(TestClass::Pool.is_Full()));
    assert(TestClass::Pool.is_Full() == true);

    //==============================================================================

    // Return Successful mock up
    return TEST_PASSED;
}

bool TEST_Ring_Buffer_Class()
{// Returns true if all tests of the Ring Buffer Pass
    const size_t Size{ 100 };
    ring_buffer<uint32_t, Size> TestRingBuffer;
    ring_buffer<uint32_t, Size> TestRingBuffer2;



    /* ==============================================================================
    /*   TEST:  Push ability to add Items to end of Buffer                  
    /* ============================================================================== */
    DEBUGPrint(CON_Yellow, "TESTING: Ability to Add Elements to Buffer and Asserting its Size Equals the amount of Elements we Pushed into it \n\n ");
    for (uint32_t i = 0; i < Size; i++)
    {
        TestRingBuffer.push(i);
    }
    CREATURE_ASSERT(TestRingBuffer.size() _NOT_EQUALS_ Size, "FAILING: Buffer Size is not Equal to the Size we created it with");


    DEBUGPrint(CON_Yellow, "TESTING: Ensuring each Element has the Proper Value that we placed in it \n\n");
    for (uint32_t i = 0; i < Size; i++)
    {
        CREATURE_ASSERT(TestRingBuffer.get_Index(TestRingBuffer.get_Ptr(i)) _EQUALS_ i , "/n/n FAILING: Elements being in proper location");
    }

    /* ==============================================================================
    /*   TEST:  Push ability to add Items to end of Buffer
    /* ============================================================================== */
    DEBUGPrint(CON_Yellow, "");

    uint32_t val{ 0 };
    for (uint32_t j = 0; j < 10; j++)
    {
        for (uint32_t i = 0; i < 10; i++)
        {
            TestRingBuffer.push(i);
        }
        for (uint32_t i = 0; i < 10; i++)
        {
            TestRingBuffer.pop(val);
            TestRingBuffer2.push(val);
        }
    }

    /* ==============================================================================
    /*   TEST:     Ability to Pop value off then move it to second RingBuffer                                                           
    /* ============================================================================== */

    for (uint32_t i = 0; i < Size * 2; i++)
    {
        uint32_t Item{ 0 };
        TestRingBuffer.push(i);
        TestRingBuffer.pop(Item);
        TestRingBuffer2.push(Item);
    }

    uint32_t Val;
    for (uint32_t i = 0; i < Size; i++)
    {// Empty Ring Buffer
        TestRingBuffer.pop(Val);
    }
    for (uint32_t i = 0; i < Size; i++)
    {// Refill it
        TestRingBuffer.push(i);
    }
    /* ==============================================================================
    /*   TEST:  RangeBased For_Loop                                                                      
    /* ============================================================================== */
   //for (auto& RB : TestRingBuffer)
   //{
   //    Print("Range For: " << RB);
   //}

    uint32_t Element = 13;
    void* MemoryAddress = (void*)TestRingBuffer.get_Ptr(Element);
    uint32_t Ind = TestRingBuffer.get_Index(static_cast<std::atomic<uint32_t>*>(MemoryAddress));
    //assert(*TestRingBuffer.get_Ptr(Ind) == TestRingBuffer.get_Index(MemoryAddress));


    /* ============================================================================== */
     DEBUGPrint(CON_Yellow, 
         "Test to see if the Memory Address get_Ptr(index) returns is the same Address \n\
that get_Ptr() returns when you feed it that Index;\n\
Wrapping is also adjusted for in this Test which means if it passes it properly wraps when feeding it an Index and returns the proper Memory Address\n\
and Index information"
);

    for (int i = 0; i < Size; ++i)
    {
        int Index = rand() % (TestRingBuffer.element_Count() * 10);
        Print("Testing get_Ptr() wrapping using Index " << Index << " at address " << (void*)TestRingBuffer.get_Ptr(Index));
        CREATURE_ASSERT((void*)TestRingBuffer.get_Ptr(Index) _EQUALS_ (void*)TestRingBuffer.get_Ptr(Index + TestRingBuffer.element_Count()), "FAILING: Pointer and Index Test");
    }
    /* ============================================================================== */


    return TEST_PASSED;
}








  //  assert(*TestRingBuffer.get_Ptr(12) == *TestRingBuffer.get_Ptr(113));









