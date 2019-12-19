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



#include"Defines.h"

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
#define MESSAGE_LEVEL           OBJECT_CREATION_MESSAGE

/* Test block of code for testing Threadspools Speeds */
#define  _TEST_THREADPOOL_SPEED  TRUE // FALSE






// ===================================================================================================================================================
//  Declarations for Library Import and Exportation: [incomplete]
// ===================================================================================================================================================

/* Will determine if the Given class or structure is exportable */
#define CREATURE_API

/* Calling convention of the API */
#define CREATURE_CALL 

/* For turning off exceptions on optimization */
#define NOEXCEPT noexcept

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


/* Denotes that Object Can not be Copied or Assigned */
#define NO_COPY_OR_ASSIGNMENT(Class_X)	void operator=(const Class_X&) = delete;\
Class_X(const Class_X&) = delete


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
	 __pragma(message ("Line: [ " STRING(__LINE__) " ]"))    \
     __pragma(message ("File: -" __FILE__ " "))              \
     __pragma(message ("                    "))              \
	 }                                                       \
     }while(0)
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
     __pragma(message(#_msg))                                \
     }                                                       \
     }while(0)
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
     __pragma(message(#_msg))                                \
     }                                                       \
     }while(0)
#endif/* WARNING MESSAGE */



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
#        define OPTIMIZATION_ON()  __pragma(optimize("", on))
#    else
#        define OPTIMIZATION_OFF()  __pragma(optimize("",off))\
             __pragma(message("WARNING: Global Optimization Disabled"))
#        define OPTIMIZATION_ON()  __pragma(optimize("", on))\
             __pragma(message("WARNING: Global Optimization Enabled"))
#    endif
#endif


/*
NOTES: 

CppCon 2016: Jason Jurecka “Game engine using STD C++ 11"
https://www.youtube.com/watch?v=8AjRD6mU96s

*/