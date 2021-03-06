#pragma once

#ifndef COMMON_H
#define COMMON_H

//#define SCREEN_X 1920
//#define SCREEN_Y 1200
const int SCREEN_X = 1920;
const int SCREEN_Y = 1200;


#include<cstdint>
#include<string>
#include<iostream>

// PATHS for includes
//$(VCInstallDir)Auxiliary\VS\UnitTest\include
//$(Solution)..\Bin\glad\include
//$(Solution)..\Bin

/* OpenGL Function Loader */
#include "../../Bin/glad/include/glad/glad.h"

/* Math Library GLM for vectors */
#include "../../Bin/glm/glm/glm.hpp"
#include "../../Bin/glm/glm/gtc/type_ptr.hpp"
#include "../../Bin/glm/glm/gtc/matrix_transform.hpp"


#if !_HAS_CXX17
#    error Program needs C++ 17 standard to proceed. Please enable C++ 17 to continue.  ProjectProperties-> C/C++->Languages-> C++ Language Standard ISO C++ 17 Standard
#    error Please enable C++ 17 to continue.  ProjectProperties-> C/C++->Languages-> C++ Language Standard ISO C++ 17 Standard
#    error ProjectProperties-> C/C++->Languages-> C++ Language Standard ISO C++ 17 Standard
#endif

#include"Defines.h"
#include <Unknwnbase.h>


#if _MSVC_LANG
#    define CacheLineFlush(Address) _mm_clflush(Address)
#endif




/* ===================================================================================================================================================
 *  STATE CONTROLLER FOR THE COMPILE TIME BEHAVIOR OF THE CREATURE GAME ENGINE 
 *  TRUE TURNS FEATURE ON
 *  FALSE SHUTS IT OFF
 *  ===================================================================================================================================================
 */

/* Turns optimizations on or off for a given section when TRUE */
#define OPTIMIZATION_CONTROLER  FALSE
/* Prevent the output of pandentic messages and TODOs */
#define NO_WARNINGS             TRUE

/* Defines the level of Messages the user wishes to see */
#define MESSAGE_LEVEL       0//USER_WARNING_MESSAGES// OBJECT_CREATION_MESSAGE//  ERROR_MESSAGE_DISPLAY // TODO_MESSAGES//FUNCTION_TRACE_MESSAGES//OBJECT_CREATION_MESSAGE//REFACTOR_MESSAGES// 

/* Test block of code for testing Threadspools Speeds */
///#define  _TEST_THREADPOOL_SPEED  TRUE // FALSE

/* List all the warnings we wish to disable or enable */
#define DISABLED_WARNINGS 4018 
//  Arithmetic overflow: Using operator Just pain in the ass warning about using Integer inside of [] brackets and mostly shit like that.
//  Check it every now and then but mostly keep this off
#pragma warning( disable : 26451 ) 
#pragma warning( disable : DISABLED_WARNINGS ) 

// ===================================================================================================================================================
//  Declarations for Library Import and Exportation: [incomplete]
// ===================================================================================================================================================

#ifdef DLL_CONFIG
#    ifdef MY_EXPORTS
#    define CREATURE_API __declspec(dllexport)
#else
#    define CREATURE_API __declspec(dllimport)
#endif
#    else
#    define CREATURE_API // What goes here?
#endif

#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)


/* Will determine if the Given class or structure is exportable */


/* Calling convention of the API */
#define CREATURE_CALL   __stdcall
#define Creature_Call   __stdcall 
/* For turning off exceptions on optimization */
#define NOEXCEPT noexcept

#define NO_VTABLE __declspec(novtable) 


// ===================================================================================================================================================
// TYPES FOR OUR ENGINE
// ===================================================================================================================================================
//  Naming Convention. [sign][precision][type][modifier]
//  ex: iVec2 Integer of Vec type with 2 members
//  ex: Uint8 Unsigned Integer 8 bytes

// Float Vectors
typedef glm::vec2   Vec2;
typedef glm::vec3   Vec3;
typedef glm::vec4   Vec4;

// Integer Vectors
typedef glm::ivec2  iVec2;
typedef glm::ivec3  iVec3;
typedef glm::ivec4  iVec4;

// Matrices
typedef glm::mat3   Mat3;
typedef glm::mat4   Mat4;

// Colors		    
typedef glm::vec3   fRGB;
typedef glm::vec4   fRGBA;

// Packed Integers
typedef glm::uint8  Uint8;
typedef glm::uint16 Uint16;
typedef glm::uint32 Uint32;
typedef glm::uint64 Uint64;


// Object Tags
typedef uint32_t     idTag;
typedef std::string  nameTag;

// 64 bit pointer to resident GPU objects
using GPUptr = uint64_t;

// ===================================================================================================================================================
/* Outputs Info to the Console. Replacing later with more proper Logging Function */
#define Print(x) std::cout << x << "\n"


/* Denotes that Object Can not be Copied or Assigned or moved */

#define NO_COPY_OR_ASSIGNMENT(Class_X) Class_X(const Class_X&) = delete;\
									   Class_X& operator=(const Class_X&) = delete

#define NO_COPY_ASSIGNMENT_OR_MOVE(Class_X)  NO_COPY_OR_ASSIGNMENT(Class_X);\
                                                 Class_X(Class_X&&) = delete;\
									             Class_X& operator=(Class_X&&) = delete;
                                         

// with line number
#define STRING2(x) #x
#define STRING(x) STRING2(x)

/*      TODO(_msg) gives us a Console Runtime Message as well as a Compile time Message reminding the user of their 
    Intent to fix or implement something */

#if MESSAGE_LEVEL >= TODO_MESSAGES
#    define TODO(_msg) do{                                   \
     static bool _seenAlready = false;                       \
     if(!_seenAlready){                                      \
     _seenAlready = true;                                    \
     std::cout << "File: " << __FILE__ << "\n";              \
     std::cout << "Line: " << __LINE__ << "\n";              \
     std::cout << "Message: " << #_msg << "\n";              \
     __pragma(message ("                    "))              \
     __pragma(message ("TODO: " #_msg ))                     \
     __pragma(message(_msg) )                                \
	 __pragma(message ("Line: [ " STRING(__LINE__) " ]"))    \
     __pragma(message ("File: -" __FILE__ " "))              \
     __pragma(message ("                    "))              \
	 }                                                       \
     }while(0)
#else
#    define TODO(_msg)
#endif/* TODO MESSAGE */


/* Message Reminding user to fix a given section of code or Find a Better way */
#if MESSAGE_LEVEL >= REFACTOR_MESSAGES
#    define REFACTOR(_msg) do{                               \
     static bool _seenAlready = false;                       \
     if(!_seenAlready){                                      \
     _seenAlready = true;                                    \
     std::cout << "NOTE: " << #_msg << "\n";                 \
     std::cout << "Refactor code at ";                       \
     std::cout << "Line: " << __LINE__ << "\n";              \
     std::cout << "File: " << __FILE__ << "\n";              \
     __pragma(message("                    "))               \
     __pragma(message("Note : " #_msg))                      \
     __pragma(message("Refactor Code at "))                  \
     __pragma(message("Line: [ " STRING(__LINE__) " ]"))     \
     __pragma(message("File: -" __FILE__ " "))               \
     __pragma(message("                    "))               \
     }                                                       \
     }while(0)
#else
#    define REFACTOR(_msg)
#endif/* REFACTOR MESSAGE */


/* Message warning user of possible size effects, defective code left in, Test blocks in use and any other possible use case for a compiler/console Warning */
#if MESSAGE_LEVEL >= USER_WARNING_MESSAGES
#    define WARN_ME(_msg) do{                                \
     static bool _seenAlready = false;                       \
     if(!_seenAlready){                                      \
     _seenAlready = true;                                    \
     std::cout << "~*!WARNING!*~: " << #_msg << " \n In";    \
     std::cout << "File: " << __FILE__ << "\n";              \
     std::cout << "Line: " << __LINE__ << "\n";              \
     __pragma(message("                    "))               \
     __pragma(message("~*!WARNING!*~: " #_msg))              \
     __pragma(message("User Generated Warning at "))         \
     __pragma(message("Line: [ " STRING(__LINE__) " ]"))     \
     __pragma(message("File: -" __FILE__ " "))               \
     __pragma(message("                    "))               \
     }                                                       \
     }while(0)
#else
#    define WARN_ME(_msg)                                
#endif/* WARNING MESSAGE */

#define TRACE_DEPTH 100000
/* Message warning user of possible size effects, defective code left in, Test blocks in use and any other possible use case for a compiler/console Warning */
#if MESSAGE_LEVEL >=FUNCTION_TRACE_MESSAGES
#    define _Trace(_msg, _level) do{                         \
     if( _level < TRACE_DEPTH){                              \
     static bool _seenAlready = false;                       \
     if(!_seenAlready){                                      \
     _seenAlready = true;                                    \
     std::cout << "~*!WARNING!*~: " << #_msg << " \n In";    \
     __pragma(message("                    "))               \
     __pragma(message("~*!WARNING!*~: " _msg))               \
     __pragma(message("Function " __FUNCTION__))             \
     __pragma(message("Line: [ " STRING(__LINE__) " ]"))     \
     __pragma(message("File: -" __FILE__ " "))               \
     __pragma(message("                    "))               \
     }}                                                      \
     }while(0)                                               
#else
#    define _Trace(_msg, _level)  
#endif/* TRACE THE IN AND OUTS OF FUNCTIONS */


//     std::cout << "File: " << __FILE__ << "\n";              \
//   __pragma(message("~*!WARNING!*~: " #_msg))              \
//   if(this){ std::cout << typeid(*this).name() << "\n" ;}  \
//   std::cout << "Line: " << __LINE__ << "\n";              \
//

/* Iterative For-Next loop Ends when for(uint32_t Itr < Count) */
#define for_loop(itr, count)     for(uint32_t itr{0}; itr < ((count)); ++itr)


/* Descriptor Defines to make code readability easier. Static tells in a CPP module if a function is defined as static 
pure_Virtual tells if a function is pure virtual in plain english*/
#define _static
#define pure_virtual  0


/* Control Block for Optimization Controller */
#if OPTIMIZATION_CONTROLER
#    define OPTIMIZATION_OFF() 
#    define OPTIMIZATION_ON() 
#else
#    if NO_WARNINGS
#        define OPTIMIZATION_OFF()  __pragma(optimize("",off))
#        define OPTIMIZATION_ON()   __pragma(optimize("", on))
#    else
#        define OPTIMIZATION_OFF()  __pragma(optimize("",off))\
             __pragma(message("WARNING: Global Optimization Disabled"))
#        define OPTIMIZATION_ON()   __pragma(optimize("", on))\
             __pragma(message("WARNING: Global Optimization Enabled"))
#    endif
#endif


#define Concat(x,y) x##y
#define paster( n ) printf_s( "token" #n " = %d", token##n )

//======================================================================================
//========================= DEBUG INFOMATION ======================================================
#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers

#include <windows.h>


#define CON_DarkBlue 1
#define CON_DarkGreen 2
#define CON_Darkteal 3
#define CON_DarkRed 4
#define CON_DarkPurple 5
#define CON_DarkYellow 6
#define CON_Grey 7
#define CON_DarkGrey 8
#define CON_Blue 9
#define CON_Green 10
#define CON_Teal 11
#define CON_Red 12
#define CON_Purple 13
#define CON_Yellow 14
#define CON_White 15

extern HANDLE hConsole;
#define SetColor(x) SetConsoleTextAttribute(hConsole, x);

#ifdef _DEBUG
#    define DEBUGPrint(col, x) {DEBUGMutex.lock();\
                                SetColor(col);\
                                std::thread::id CurrentThread = std::this_thread::get_id();\
                                std::cout <<  x << " : Thread[" << CurrentThread << "]" << std::endl;\
                                SetColor(7);\
                                DEBUGMutex.unlock();}
#else
#    define DEBUGPrint(col,x)
#endif
#include<mutex>
extern std::mutex DEBUGMutex;

//======================================================================================

#define TEST_PASSED   true
#define TEST_FAILED   false

/*============ Comparison =========== */
#define _EQUALS_                == 
#define _IS_EQUAL_TO_           == 
#define _NOT_EQUALS_            != 
#define _NOT_EQUAL_TO_          !=
#define _IS_GREATER_THAN_       >
#define _IS_LESS_THAN_          <
#define _EQUAL_OR_GREATER_      >= 
#define _EQUAL_OR_LESS_THAN_    <=

/*======== Logical Operators =========*/
#define _AND_                   &&
#define _OR_                    ||
#define _NOT_                   !

#define pure_virtual      0
#define _static 


#define TEST_UNIT(x)   assert((x) == true);\
DEBUGPrint(CON_Green, "Test " << #x << " Passed")

#if _DEBUG
/*Basic Macro does as appears and only allows code to appear on Debug Builds */
#    define DEBUG_CODE(_code)   _code
#else
/* Code Currently turned off */
#    define DEBUG_CODE(_code)   
#endif




/* For new versions of C++ that allows Attributes */
#define   UNLIKELY   //[[unlikely]]

/* May used this for code that is only present in specific versions
/* of OpenGL */
#define  _GL(_code)    _code
#define OpenGL4_6(_code)
#define OpenGL3_1(_code) _code

inline double RADIANS(double _x) { return ((_x) * 0.01745329251); }
inline double DEGREES(double _x) { return ((_x) * 57.2957795131); }
//#define RADIANS(x)            ((x) * 0.01745329251)
//#define DEGREES(x)            ((x) * 57.2957795131)

/* Creates a 32bit word from RGB Values */
#define _RGB(r,g,b)     (((b << 16) | (g << 8)) | (r))
#define _RGBA(r,g,b,a)  ((a << 24)  | _RGB(r,g,b))

/* Prints a bool as a True/False string */
#define PBool(x)  ((x == true) ? " True" : "False")


std::ostream& operator <<(std::ostream& _stream, const Vec2& _vector);
std::ostream& operator <<(std::ostream& _stream, const Vec3& _vector);
std::ostream& operator <<(std::ostream& _stream, const Vec4& _vector);
std::ostream& operator <<(std::ostream& _stream, const Mat4& _matrix);



#define CheckGLERROR() {uint32_t ERR = 0;\
if((ERR = OpenGL::glCheckError_(__FILE__, __LINE__)))\
{\
	Print("Error" << ERR);\
	__debugbreak();\
}\
}

/* I REALLY DON"T LIKE THIS BEING HERE, IT NEEDS A HOME OTHER THAN IN COMMON AS IT IS POLLUTING EVERYTHING */
#include<atomic>
template<typename _Ty>
struct CREATURE_API Protected_Value
{
	void Store(_Ty _value)
	{
		while (Flag.test_and_set(std::memory_order_acquire))
		{}// maybe an atomic test wait?
		
		Value = _value;
		Flag.clear(std::memory_order_release);
	}
	_Ty Load()
	{
		return Value;
	}
private:
	std::atomic_flag Flag{ ATOMIC_FLAG_INIT };
	_Ty Value{ 0 };
};


#define Squared(x)   ((x) * (x))


#ifndef NDEBUG
#   define              TEST_ASSERT(condition, FAIL_MSG, PASS_MSG) do{if (!(condition)){\
/* Colored Debug Console Logger */ DEBUGPrint(CON_Red,                                  \
                        "\n ========================================= "                 \
/* Expression to Evaluate       */     << "\n Assertion: " << #condition                \
/* FILE NAME                    */     << "\n File: "      << __FILE__                  \
/* LINE NUMBER                  */     << "\n Line: "      << __LINE__                  \
/* ERROR MESSAGE                */     << "\n FAILED: "    << FAIL_MSG  <<              \
                        "\n ========================================= ");               \
/* Terminate if Flag is Set     */     if (TerminateOnError){ std::terminate(); }       \
/* Expression PASSED:           */ }else{                                               \
/* Prevent same test triggering */ static bool          _seenAlready = false;           \
/* Multiple Logs                */ if( !_seenAlready ){ _seenAlready = true;            \
/* is Console Display On        */ if(ConsoleMessages){                                 \
/*                              */ DEBUGPrint(CON_Green,                                \
                        "\n ========================================= "                 \
/* Expression to Evaluate       */     << "\n Condition: " << #condition                \
/* FILE NAME                    */     << "\n File: "      << __FILE__                  \
/* LINE NUMBER                  */     << "\n Line: "      << __LINE__                  \
/* SUCCESS MESSAGE              */     << "\n PASSED: "    << PASS_MSG <<               \
                        "\n ========================================= ");               \
                                  }}}} while (false)
#else
#   define TEST_ASSERT(condition, message) do { } while (false)
#endif

extern bool ConsoleMessages;
extern bool TerminateOnError;




/// FIX THIS LATER
#define GetRandom( min, max )     ((rand() % (int)(((max) + 1) - (min))) + (min))  
#define RANDOM(x)                 ((float)((rand() * (1.0 / (1.0 + RAND_MAX))) * x))
#define RANDOM_RANGE(x)           (RANDOM(x * 2) - (x))












/* Defines for accessing the upper and lower byte of an integer. */
#define  LOW_BYTE(x)    (x & 0x00FF)
#define  HIGH_BYTE(x)     ((x & 0xFF00) >> 8)

#define  BIT(x)         (1 << x) // GET THE STATE OF A SINGLE BIT AT X POSITION
#define  MAX_INTEGER    0xFFFFFFFF

#define  BIT_32         0x80000000         //      DEC 2147483648   BIN:  1000 0000 0000 0000 0000 0000 0000 0000
#define  BIT_31         0x40000000         //      DEC 1073741824	BIN:  0100 0000 0000 0000 0000 0000 0000 0000
#define  BIT_30         0x20000000         //      DEC 536870912	BIN:  0010 0000 0000 0000 0000 0000 0000 0000
#define  BIT_29         0x10000000         //      DEC 268435456	BIN:  0001 0000 0000 0000 0000 0000 0000 0000
#define  BIT_28         0x8000000          //      DEC 134217728	BIN:  0000 1000 0000 0000 0000 0000 0000 0000
#define  BIT_27         0x4000000          //      DEC 67108864	    BIN:  0000 0100 0000 0000 0000 0000 0000 0000
#define  BIT_26         0x2000000          //      DEC 33554432	    BIN:  0000 0010 0000 0000 0000 0000 0000 0000
#define  BIT_25         0x1000000          //      DEC 16777216	    BIN:  0000 0001 0000 0000 0000 0000 0000 0000
#define  BIT_24         0x800000           //      DEC 8388608		BIN:  0000 0000 1000 0000 0000 0000 0000 0000
#define  BIT_23         0x400000           //      DEC 4194304		BIN:  0000 0000 0100 0000 0000 0000 0000 0000
#define  BIT_22         0x200000           //      DEC 2097152		BIN:  0000 0000 0010 0000 0000 0000 0000 0000
#define  BIT_21         0x100000           //      DEC 1048576		BIN:  0000 0000 0001 0000 0000 0000 0000 0000
#define  BIT_20         0x80000            //      DEC 524288		BIN:  0000 0000 0000 1000 0000 0000 0000 0000
#define  BIT_19         0x40000            //      DEC 262144		BIN:  0000 0000 0000 0100 0000 0000 0000 0000
#define  BIT_18         0x20000            //      DEC 131072		BIN:  0000 0000 0000 0010 0000 0000 0000 0000
#define  BIT_17         0x10000	           //      DEC 65536		BIN:  0000 0000 0000 0001 0000 0000 0000 0000
#define  BIT_16         0x8000             //      DEC 32768        BIN:  0000 0000 0000 0000 1000 0000 0000 0000
#define  BIT_15         0x4000             //      DEC 16384        BIN:  0000 0000 0000 0000 0100 0000 0000 0000
#define  BIT_14         0x2000             //      DEC 8192         BIN:  0000 0000 0000 0000 0010 0000 0000 0000
#define  BIT_13         0x1000             //      DEC 4096         BIN:  0000 0000 0000 0000 0001 0000 0000 0000
#define  BIT_12         0x800              //      DEC 2048         BIN:  0000 0000 0000 0000 0000 1000 0000 0000
#define  BIT_11         0x400              //      DEC 1024         BIN:  0000 0000 0000 0000 0000 0100 0000 0000
#define  BIT_10         0x200              //      DEC 512          BIN:  0000 0000 0000 0000 0000 0010 0000 0000
#define  BIT_9          0x100              //      DEC 256          BIN:  0000 0000 0000 0000 0000 0001 0000 0000
#define  BIT_8          0x80               //      DEC 128          BIN:  0000 0000 0000 0000 0000 0000 1000 0000
#define  BIT_7          0x40               //      DEC 64           BIN:  0000 0000 0000 0000 0000 0000 0100 0000
#define  BIT_6          0x20               //      DEC 32           BIN:  0000 0000 0000 0000 0000 0000 0010 0000
#define  BIT_5          0x10               //      DEC 16           BIN:  0000 0000 0000 0000 0000 0000 0001 0000
#define  BIT_4          0x8                //      DEC 8            BIN:  0000 0000 0000 0000 0000 0000 0000 1000
#define  BIT_3          0x4                //      DEC 4            BIN:  0000 0000 0000 0000 0000 0000 0000 0100
#define  BIT_2          0x2                //      DEC 2            BIN:  0000 0000 0000 0000 0000 0000 0000 0010
#define  BIT_1          0x1                //      DEC 1            BIN:  0000 0000 0000 0000 0000 0000 0000 0001

#define BITS_32_25 ( BIT_32 | BIT_31 | BIT_30 | BIT_29 | BIT_28 | BIT_27 | BIT_26 | BIT_25 )
#define BITS_24_17 ( BIT_24 | BIT_23 | BIT_22 | BIT_21 | BIT_20 | BIT_19 | BIT_18 | BIT_17 )
#define BITS_16_9  ( BIT_16 | BIT_15 | BIT_14 | BIT_13 | BIT_12 | BIT_11 | BIT_10 | BIT_9  )
#define BITS_8_1   ( BIT_8  | BIT_7  | BIT_6  | BIT_5  | BIT_4  | BIT_3  | BIT_2  | BIT_1  )
#define BITS_32_17 ( BITS_32_25 |  BITS_24_17 )
#define BITS_16_1  ( BITS_16_9  |  BITS_8_1   )

/// THIS ALL NEEDS TO BE FIXED, NO REASON FOR TEMPLATES WHEN THE INPUT TYPE IS ALMOST ALWAYS THE SAME
template<typename T>
T ConcatenateBitsets(T _highbits, T _lowbits)
{// Combines Two values bits to generate a new value
    return ((int_fast64_t)_highbits << 32) | ((int_fast64_t)_lowbits & MAX_INTEGER);
}


// USE CASE: New_Value = My_Value & Mask(X)
template<typename T>
T BitMask(T _value)
{ // Creates a Bitmask from Value
    return  BIT(_value) - 1;
}

// USE CASE: int32_t New_Value = SetBit(8, 0xFFFFFFFF);                           [ RESULTS: FFFFFF7F ]
template<typename T>
T SetBit(T _value, int bitIndex, bool _state)
{// Sets the Value of a Specific Bit for a given Value.
    if (_state == true)
    {
        return _value | BIT(bitIndex);
    }
    return _value & ~BIT(bitIndex);
}

// USE CASE: int32_t New_Value = ExtractLowBits<int32_t>(16, int32_t 0xFFFFFFFF); [ RESULTS:0x0000FFFF ]
template<typename T>
T ExtractBitRange(T _value, unsigned char from, unsigned char to)
{ // Extracts the result of the bits to-from in T _value and returns results
    unsigned mask = (BIT((to - from + 1)) - 1) << from;
    return (_value & mask) >> from;
}

// USE CASE: int32_t New_Value = ExtractLowBitRange(16, 0xFFFFFFFF);              [ RESULTS: 0xFFFF ]
template<typename T>
T ExtractLowBitRange(char bitcount, T _value)
{ // Extracts the Bottom bits from T Value for Bitcount number of Bits and returns the results 
    return _value & (BIT(bitcount) - 1);
}

// USE CASE: int32_t New_Value = ExtractHigh16Bits(0xFFF7FFFF)                    [RESULTS: FFF7]
template<typename T>
T ExtractHigh16Bits(T _value)
{// Extracts the Top 16 bits from Likely a 32bit word.
    return  (_value & (0x1FFFF << (32 - 17))) >> (32 - 17);
}




#define MIN(v1, v2)   ( ((v1) < (v2)) ? (v1) : (v2) )
#define MAX(v1, v2)   ( ((v1) > (v2)) ? (v1) : (v2) )
#define CLAMP(_x1, _val)  (fmod((_x1), (_val)))

// _x1 = 

// THESE NEED A HOME DEFINED IN UTILITY.CPP But I want utilities to have roughly global scope
std::string get_FileName(const  std::string& s);






#define _STACK_TRACE_

/* Trace calls in and our of specified functions

This will likely become so much more in which ALL of our function calls will be wrapped in a method that traces their creation and destruction so when desired
we will have more functionality for tracking the stack and functions while easily being capable of turning it off at will
*/
// std::cout << "IN: "<< x << typeid(*this).name() << "\n"
// std::cout <<"OUT: "<< x << typeid(*this).name() << "\n"

#pragma warning( disable :4003) // not enough arguments for function-like macro invocation 'Return'
#define ConCat1(_a, _b)  _a##_b
#define ConCat(_a, _b)  ConCat1(_a, _b)



#ifdef _STACK_TRACE_
#    define PROFILE_BEGIN_SESSION(_name, _filepath) Instrumentor::get().BeginSession(_name, _filepath)
#    define PROFILE_END_SESSION()                   Instrumentor::get().EndSession()
#    define PROFILE_SCOPE(_name)                   InstrumentationTimer timer##__LINE__(_name);
#    define PROFILE_FUNCTION() PROFILE_SCOPE( __FUNCSIG__ ) //void __cdecl Stop()

#    define trace_IN(x)   InstrumentationTimer timer##__LINE__ = InstrumentationTimer( __FUNCSIG__ );   //PROFILE_SCOPE(__FUNCSIG__) // PROFILE_FUNCTION()
#    define trace_OUT(x) 
#    define trace(x) { InstrumentationTimer ConCat(timer,__LINE__) = InstrumentationTimer( __FUNCSIG__ );
#    define Return(x)       return x; }

// trace_OUT(0);

#    define trace_scope(_name) PROFILE_SCOPE(_name)
//trace_IN(x);

#else
#    define trace_IN(x)   
#    define trace_OUT(x)  
#    define trace(x)
#    define Return(x)       return x;

#    define PROFILE_BEGIN_SESSION(_name, _filepath) 
#    define PROFILE_END_SESSION()   
#    define PROFILE_SCOPE(_name) 
#    define PROFILE_FUNCTION() 
#endif



#endif// COMMON_H

















/*
==========================================================================================================================================================================
                                                           NOTES:
 ==========================================================================================================================================================================

 Preprocessor Directives (push/pop
 https://docs.microsoft.com/en-us/cpp/preprocessor/pop-macro?view=vs-2019
 Push/Pop Macros
 #pragma push_macro("Y") / #pragma pop_macro("X")

 vtordisp(On/Off)
 https://docs.microsoft.com/en-us/cpp/preprocessor/vtordisp?view=vs-2019



 Github Markdown Cheat
 https://guides.github.com/pdfs/markdown-cheatsheet-online.pdf

 CppCon 2016: Jason Jurecka �Game engine using STD C++ 11"
 https://www.youtube.com/watch?v=8AjRD6mU96s

*/











// OLD POSSIBLE TRASH


//#define Property (_getter, _setter) __declspec(property(get = _getter, put = _setter))


///================================================================================================================
///================= SIMPLE ERROR HANDLING ========================================================================
///================================================================================================================



/* Later on we will use this Error flag as a global that is checked as we will be turning off Exceptions. Should make
   my Error handling module before moving forward with this project

      struct ErrorMessage
      {
      	ErrorMessage(uint32_t _error, uint32_t _level)
      		:
      		Level(_level),
      		ErrorNumber(_error)
      	{
      		GetSystemTime(Time);
      	}
      	uint32_t Level;
      	uint32_t ErrorNumber;
      	LPSYSTEMTIME Time;
      
      	static bool isError() { return !Errors.empty(); }
      	static void RaiseException(uint32_t _error, uint32_t _level)
      	{
      		Errors.push({ _error, _level });
      	}
      	static bool GetError(ErrorMessage& _msg)
      	{
      		if (isError())
      		{
      			_msg = Errors.top();
      			Errors.pop();
      		}
      		return Errors.empty();
      	}
      	static std::stack<ErrorMessage> Errors;
      	friend std::ostream& operator <<(std::ostream& os, ErrorMessage& _msg);
      };

      std::ostream& operator <<(std::ostream& os, ErrorMessage& _msg);
      std::ostream& operator <<(std::ostream& os, ErrorMessage& _msg)
      {
      	os << "Error: " << _msg.ErrorNumber << "\n At: " << _msg.Time << "\n ";
      	return os;
      }

*/

/* 
CREATURE_ENGINE PROJECT
========================
COMPILER :
/ JMC / permissive - / MP / GS / Qpar / W3 / Zc:wchar_t / ZI / Gm - / Od / sdl / Fd"x64\Debug\CreatureEngine.pdb" / fp : precise / D "_MBCS" / errorReport : prompt / WX - / Zc : forScope / RTC1 / Gd / Oi / MTd / std : c++17 / FC / Fa"x64\Debug\" /EHsc /nologo /Fo"x64\Debug\" /Fp"x64\Debug\CreatureEngine.pch" /diagnostics:classic 
LINKER:
/ OUT:"C:\Users\curti\Source\Repos\xSeditx\Creature-Engine\x64\Debug\CreatureEngine.lib" "SOIL.lib" / MACHINE : X64 / NOLOGO



SANDBOX
========
COMPILER:
/ JMC / permissive - / GS / W3 / Zc:wchar_t / ZI / Gm - / Od / sdl / Fd"x64\Debug\vc141.pdb" / Zc : inline / fp : precise / D "_MBCS" / errorReport : prompt / WX - / Zc : forScope / RTC1 / Gd / MTd / std : c++17 / FC / Fa"x64\Debug\" /EHsc /nologo /Fo"x64\Debug\" /Fp"x64\Debug\Sandbox.pch" /diagnostics:classic 

LINKER:
/ OUT:"C:\Users\curti\Source\Repos\xSeditx\Creature-Engine\x64\Debug\Sandbox.exe" / MANIFEST / NXCOMPAT / PDB : "C:\Users\curti\Source\Repos\xSeditx\Creature-Engine\x64\Debug\Sandbox.pdb" / DYNAMICBASE "SOIL.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" / DEBUG : FASTLINK / MACHINE : X64 / INCREMENTAL / PGD : "C:\Users\curti\Source\Repos\xSeditx\Creature-Engine\x64\Debug\Sandbox.pgd" / SUBSYSTEM : CONSOLE / MANIFESTUAC : "level='asInvoker' uiAccess='false'" / ManifestFile : "x64\Debug\Sandbox.exe.intermediate.manifest" / ERRORREPORT : PROMPT / NOLOGO / TLBID : 1



SOIL
====
COMPILER:
/ GS / W3 / Zc:wchar_t / Zi / Gm / Od / Fd"x64\Debug\SOIL.pdb" / Zc : inline / fp : precise / D "WIN32" / D "_DEBUG" / D "_LIB" / D "_UNICODE" / D "UNICODE" / errorReport : prompt / WX - / Zc : forScope / RTC1 / Gd / MTd / FC / Fa"x64\Debug\" /EHsc /nologo /Fo"x64\Debug\" /Fp"x64\Debug\SOIL.pch" /diagnostics:classic 

LINKER:
/ OUT:"C:\Users\curti\Source\Repos\xSeditx\Creature-Engine\Bin\Soil\Debug\SOIL.lib" / MACHINE : X64 / NOLOGO


*/




/*
===================================================================================================================================================================================================
                                           BIT PATTERN MEANING
===================================================================================================================================================================================================
0xcccccccc
  bit pattern it means that you are reading memory that is on the current thread stack that has not been initialised.

0xC0000005 
  Access Violation error.

0xbaadf00d
  The 0xbaadf00d bit pattern is the bit pattern for memory allocated with HeapAlloc(), LocalAlloc(LMEM_FIXED), GlobalAlloc(GMEM_FIXED).
  If you are seeing the 0xbaadf00d bit pattern it means that you are reading memory that has been allocated by HeapAlloc() (or reallocated by HeapReAlloc()) and which has not been initialised by the caller of HeapAlloc(or HeapReAlloc, LocalAlloc, GlobalAlloc).

0xdeadbeef
  The 0xdeadbeef bit pattern is the bit pattern for memory deallocated using HeapFree(), LocalFree(), GlobalFree().
  If you are seeing the 0xdeadbeef bit pattern it means that you are reading memory that has been deallocated by HeapFree(), LocalFree() or GlobalFree().

0xabababab
  The 0xabababab bit pattern is the bit pattern for the guard block after memory allocated using HeapAlloc(), LocalAlloc(LMEM_FIXED), GlobalAlloc(GMEM_FIXED) or CoTaskMemAlloc().
  If you are seeing the 0xabababab bit pattern it means that you are reading memory after a memory block that has been allocated by HeapAlloc(), LocalAlloc(LMEM_FIXED), GlobalAlloc(GMEM_FIXED) or CoTaskMemAlloc().

0xbdbdbdbd
  The 0xbdbdbdbd bit pattern is the guard pattern around memory allocations allocated with the "aligned" allocators.
  Memory allocated with malloc(), realloc(), new and new[] are provided with a guard block before and after the memory allocation.When this happens with an aligned memory allocator, the bit pattern used in the guard block is 0xbdbdbdbd.
  If you are seeing the 0xbdbdbdbd bit pattern it means that you are reading memory before the start of a memory block created by an aligned allocation.

0xfdfdfdfd
  The 0xfdfdfdfd bit pattern is the guard pattern around memory allocations allocated with the "non-aligned" (default) allocators.
  Memory allocated with malloc(), realloc(), new and new[] are provided with a guard block before and after the memory allocation.When this happens with an non - aligned(default) memory allocator, the bit pattern used in the guard block is 0xfdfdfdfd.
  If you are seeing the 0xfdfdfdfd bit pattern it means that you are reading memory either before the start of a memory block or past the end of a memory block.In either case the memory has been allocated by malloc(), realloc() or new.

0xcdcdcdcd
  The 0xcdcdcdcd bit pattern indicates that this memory has been initialised by the memory allocator(malloc() or new) but has not been initialised by your software(object constructor or local code).
  If you are seeing the 0xcdcdcdcd bit pattern it means that you are reading memory that has been allocated by malloc(), realloc() or new, but which has not been initialised.

0xdddddddd
  The 0xdddddddd bit pattern indicates that this memory is part of a deallocated memory allocation(free() or delete).
  If you are seeing the 0xdddddddd bit pattern it means that you are reading memory that has been deallocated by free() or delete.

0xfeeefeee
  The 0xfeeefeee bit pattern indicates that this memory is part of a deallocated memory allocation(free() or delete).
  If you are seeing the 0xfeeefeee bit pattern it means that you are reading memory that has been deallocated by free() or delete.
===================================================================================================================================================================================================

*/