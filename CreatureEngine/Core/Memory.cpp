#include"Memory.h"


bool ConsoleMessages{ true };
bool TerminateOnError{ true };

static void AssertDisplayOn()  { ConsoleMessages = true; }
static void AssertDisplayOff() { ConsoleMessages = false; }


/* Intentional Clusterfuck to mess up Alignments and Sizes */
struct TestClass
{
    TestClass() = default;
    float F1{ 0 };
    size_t Sz1{ 0 };
    bool B1{ 0 } , B2{ 0 }, B3{ 0 };
    size_t Sz2{ 0 };;
    uint32_t Int1{ 0 };;
    float F2{ 0 };;
    char C1{ 0 };;
    float F3{ 0 };;
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
        Print("Range For_Loop: " << T.Value);
        ++ElementTest;
    }
    TEST_ASSERT(      ElementTest == TestClass::Pool.chunkCount(),"Improper Element Count being Created "     ," Correct Element Could on Pool Creation");
    TEST_ASSERT(sizeof(TestClass) == TestClass::Pool.chunkSize() ,"Object size and Block size are not Correct", "sizeof(_Ty) and the Block size for the Pool are Correct");


    //==============================================================================
    //   TEST  get_Data(byte Index) :
    //   to see if it properly returns a single char location
    //==============================================================================

    size_t Index = 10;    uint32_t Val = 354;
    size_t BlockSize = sizeof(TestClass);

    TestClass::Pool[Index].Value = Val;
    DEBUGPrint(CON_Yellow, "Element " << Index << " = " << TestClass::Pool[Index].Value);
    TEST_ASSERT(TestClass::Pool[Index].Value _EQUALS_ Val, "Failed Assigning via overloaded Array Operator", "Assignment using Array operator worked");
    
    DEBUGPrint(CON_Yellow, "Chunk Size = " << TestClass::Pool.chunkSize());
    TEST_ASSERT(TestClass::Pool.chunkSize() _EQUALS_ BlockSize, "Block Size is Incorrect", "Block Size is Correct ");


    TestClass::Pool[Index].Value = Val;
    char *CharData = TestClass::Pool.get_Data(Index * BlockSize);
    TestClass NewValue;
    memcpy(&NewValue, CharData, sizeof(TestClass));

    TEST_ASSERT(NewValue.Value _EQUALS_ Val,"get_Data() Returned wrong Memory Address","get_Data() properly fetching byte pointer");


    //==============================================================================
    //   TEST size()
    //==============================================================================

    DEBUGPrint(CON_Blue, "Pool Size = " << TestClass::Pool.size());
    TEST_ASSERT(TestClass::Pool.size() _EQUALS_ sizeof(TestClass) * TestClass::Pool.chunkCount(),"Pools Byte Count is incorrect Should be sizeof(_Ty) * number of Elements"," Pools Byte count is correct");

 
    //==============================================================================
    // TEST CLEAR() Note: This Test is Fucking Annoying and Takes to long for me. 
    //==============================================================================
    TestClass::Pool.clear();
    for (int i{ 0 }; i < TestClass::Pool.size(); ++i)
    {
        TEST_ASSERT((int)*TestClass::Pool.get_Data(i) _EQUALS_ 0, " Pool Clear did not properly zero out all the memory for the Pool", " Pool::clear() properly zeroed out all the pools memory");
    }


    //==============================================================================
    //   TEST IS_EMPTY() && IS_FULL()
    //==============================================================================
    for (int i = 0; i < 99; ++i)
    {
        Test1[i] = new TestClass(i);
        TEST_ASSERT(Test1[i]->Value _EQUALS_ i,"Constructor is not properly assigning data", " Constructor correctly applied the params to the Class Data " );
    }

    // Test if the Pool Filled up after Allocating enough objects to leave a single Block Free
    Print("Is It Full : " << PBool(TestClass::Pool.is_Full()));
    TEST_ASSERT(TestClass::Pool.is_Full() _EQUALS_ false, "Pool should not be saying its full", "Pool should not be saying its full");

    // Allocate the Last one and Test to see if it is now Filed
    Test1[99] = new TestClass(99);
    Print("Allocate One More is It Full now: " << PBool(TestClass::Pool.is_Full()));
    TEST_ASSERT(TestClass::Pool.is_Full() _EQUALS_ true,"Pool Should be full but is not", "Pool is properly Filled up now");

    // Now Delete one and Test again to see if it is full.
    delete(Test1[1]);
    Print("Testing Delete to Free up Space... Is it Still Full : " << PBool(TestClass::Pool.is_Full()));
    TEST_ASSERT(TestClass::Pool.is_Full() _EQUALS_ false,"If Pool is Full it either is reporting wrong or deallocating wrong","Properly Freed object on Delete");

    // Is Not Full returns True when the Container Still have space to grow
    delete(Test1[1]);// Not sure if I want to keep it honestly
    Print("Testing is not full. Tells if Space is Left" << PBool(TestClass::Pool.is_Not_Full()));
    TEST_ASSERT(TestClass::Pool.is_Not_Full() _EQUALS_ true,"Pool Should not be reporting itself as full","Pool is properly not Full");


    // Check if is_Empty works as Pool should be Not be Empty
    Print("Is It Empty : " << PBool(TestClass::Pool.is_Empty()));
    TEST_ASSERT(TestClass::Pool.is_Empty() _EQUALS_ false,"is_Empty() Should not be Empty or reporting that its empty","is_Empty() is reporting none Empty Pool");

    // Allocate more Spaces then Check if again the Pool is Full 
    Test1[1] = new TestClass(200);
    Test1[2] = new TestClass(300);
    Print("Allocating Again Is it Full Again: " << PBool(TestClass::Pool.is_Full()));
    TEST_ASSERT(TestClass::Pool.is_Full() _EQUALS_ true," Pool is not full and it should be"," Pool is properly filled although I question this test a bit");

    //==============================================================================
    // Return Successful 
    //==============================================================================

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
    DEBUGPrint(CON_Yellow, "TESTING: Ability to Add Elements to Buffer and Asserting its Size Equals the amount of Elements we Pushed into it");
    for (uint32_t i = 0; i < Size; i++)
    {
        TestRingBuffer.push(i);
    }
    TEST_ASSERT(TestRingBuffer.size() _EQUALS_ Size, "Buffer Size is not Equal to the Size we created it with", "Buffer Size is Correct " );


    DEBUGPrint(CON_Yellow, "TESTING: Ensuring each Element has the Proper Value that we placed in it \n\n");
    for (uint32_t i = 0; i < Size; i++)
    {
        TEST_ASSERT(TestRingBuffer.get_Index(TestRingBuffer.get_Ptr(i)) _EQUALS_ i , "Elements being in proper location", "Elements were assigned the correct Values");
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
        TEST_ASSERT((void*)TestRingBuffer.get_Ptr(Index) _EQUALS_ (void*)TestRingBuffer.get_Ptr(Index + TestRingBuffer.element_Count()), "Pointer and Index Test", "Pointer and Index Test" );
    }
    /* ============================================================================== */

    return TEST_PASSED;
}