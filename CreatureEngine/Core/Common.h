#pragma once
#ifndef COMMON_H
#define COMMON_H

#define SCREEN_X 1280
#define SCREEN_Y 720
/*


*/
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
#define MESSAGE_LEVEL           TODO_MESSAGES//FUNCTION_TRACE_MESSAGES//OBJECT_CREATION_MESSAGE//USER_WARNING_MESSAGES//

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
typedef uint32_t idTag;
typedef std::string  nameTag;

// 64 bit pointer to resident GPU objects
using GPUptr = uint64_t;

// ===================================================================================================================================================
/* Outputs Info to the Console. Replacing later with more proper Logging Function */
#define Print(x) std::cout << x << "\n"


/* Denotes that Object Can not be Copied or Assigned or moved */

#define NO_COPY_OR_ASSIGNMENT(Class_X) Class_X(const Class_X&) = delete;\
									   Class_X& operator=(const Class_X&) = delete

#define  NO_COPY_ASSIGNMENT_OR_MOVE(Class_X)  NO_COPY_OR_ASSIGNMENT(Class_X);\
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
/*
NOTES: 

CppCon 2016: Jason Jurecka “Game engine using STD C++ 11"
https://www.youtube.com/watch?v=8AjRD6mU96s

*/


//======================================================================================
//========================= DEBUG INFOMATION ======================================================
#include<Windows.h>


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
                                std::cout <<  x << " : " << CurrentThread<< std::endl;\
                                SetColor(7);\
                                DEBUGMutex.unlock();}
#else
#    define DEBUGPrint(col,x)
#endif
#include<mutex>
extern std::mutex DEBUGMutex;

//======================================================================================


#define _EQUALS_          ==
#define _NOT_EQUAL_TO_    !=
#define _OR_              ||
#define _AND_             &&
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






//#define _STACK_TRACE_

/* Trace calls in and our of specified functions

This will likely become so much more in which ALL of our function calls will be wrapped in a method that traces their creation and destruction so when desired
we will have more functionality for tracking the stack and functions while easily being capable of turning it off at will
*/
//#define _STACK_TRACE_
#ifdef _STACK_TRACE_
#    define trace_IN(x)   std::cout << "IN: "<< x << typeid(*this).name() << "\n"
#    define trace_OUT(x)  std::cout <<"OUT: "<< x << typeid(*this).name() << "\n"
#    define trace(x)      { trace_IN(x);
#    define Return(x)       return x; } trace_OUT(0);
#else
#    define trace_IN(x)   
#    define trace_OUT(x)  
#    define trace(x)
#    define Return(x)       return x;
#endif



/* For new versions of C++ that allows Attributes */
#define   UNLIKELY   //[[unlikely]]



/* May used this for code that is only present in specific versions
/* of OpenGL */
#define  _GL(_code)    _code

#define OpenGL4_6(_code)
#define OpenGL3_1(_code) _code



#define RADIANS(x)            ((x) * 0.01745329251)
#define DEGREES(x)            ((x) * 57.2957795131)

/* Creates a 32bit word from RGB Values */
#define _RGB(r,g,b) (((b << 16)| (g << 8)) | (r))

 /*
Idk how to handle this yet in a platform independent way so we will cast a void * to the Native Application Instance for now, 
In this case a Windows HINSTANCE
*/
#define  GetInstance() (HINSTANCE)Application::get().OSInstance 




///================================================================================================================
///================= SIMPLE ERROR HANDLING ========================================================================
///================================================================================================================
/* Later on we will use this Error flag as a global that is checked as we will be turning off Exceptions. Should make
my Error handling module before moving forward with this project */
//struct ErrorMessage
//{
//	ErrorMessage(uint32_t _error, uint32_t _level)
//		:
//		Level(_level),
//		ErrorNumber(_error)
//	{
//		GetSystemTime(Time);
//	}
//	uint32_t Level;
//	uint32_t ErrorNumber;
//	LPSYSTEMTIME Time;
//
//	static bool isError() { return !Errors.empty(); }
//	static void RaiseException(uint32_t _error, uint32_t _level)
//	{
//		Errors.push({ _error, _level });
//	}
//	static bool GetError(ErrorMessage& _msg)
//	{
//		if (isError())
//		{
//			_msg = Errors.top();
//			Errors.pop();
//		}
//		return Errors.empty();
//	}
//	static std::stack<ErrorMessage> Errors;
//	friend std::ostream& operator <<(std::ostream& os, ErrorMessage& _msg);
//};
//std::ostream& operator <<(std::ostream& os, ErrorMessage& _msg);
//
//std::ostream& operator <<(std::ostream& os, ErrorMessage& _msg)
//{
//	os << "Error: " << _msg.ErrorNumber << "\n At: " << _msg.Time << "\n ";
//	return os;
//}



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




*/

#define CheckGLERROR() {uint32_t ERR = 0;\
if((ERR = OpenGL::glCheckError_(__FILE__, __LINE__)))\
{\
	Print("Error" << ERR);\
	__debugbreak();\
}\
}






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

#define Property (_getter, _setter) __declspec(property(get = _getter, put = _setter))


#endif// COMMON_H
