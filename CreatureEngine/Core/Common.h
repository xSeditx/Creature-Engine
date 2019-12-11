#include<cstdint>
#include<string>
#include<iostream>

//$(VCInstallDir)Auxiliary\VS\UnitTest\include
//$(Solution)..\Bin\glad\include


//$(Solution)..\Bin

#include "glad/include/glad/glad.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"


/* Will determine if the Given class or structure is exportable */
#define CREATURE_API

/* Calling convention of the API */
#define CREATURE_CALL 

/* For turning off exceptions on optimization */
#define NOEXCEPT noexcept

// ===================================================================================================================================================
// TYPES FOR OUR ENGINE
// ===================================================================================================================================================
///  Naming Convention. [sign][precision][type][modifier]
///  ex: iVec2 Integer of Vec type with 2 members
///  ex: Uint8 Unsigned Integer 8 bytes




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
#define Print(x) std::cout << x << "\n"

#define NO_COPY_OR_ASSIGNMENT(Class_X)	void operator=(const Class_X&) = delete;\
Class_X(const Class_X&) = delete


#define TODO(x) do{\
static bool _seenAlready = false;\
if(!_seenAlready){\
_seenAlready = true;\
std::cout << "File: " << __FILE__ << "\n"; \
std::cout << "Line: " << __LINE__ << "\n"; \
std::cout << "Message: " << #x << "\n"; \
__pragma(message(#x))\
}\
}while(0)


#define for_loop(itr, count)     for(unsigned int itr{0}; itr < ((count)); ++itr)

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

/* Descriptor Defines to make code readability easier. Static tells in a CPP module if a function is defined as static 
pure_Virtual tells if a function is pure virtual in plain english*/
#define _static
#define pure_virtual  0

#define NO_WARNING

#ifdef NO_WARNING
#define OPTIMIZATION_OFF() 
#define OPTIMIZATION_ON() 
#else
#define OPTIMIZATION_OFF()  __pragma(optimize("",off))\
__pragma(message("WARNING: Global Optimization Disabled"))

#define OPTIMIZATION_ON()  __pragma(optimize("", on))\
__pragma(message("WARNING: Global Optimization Enabled"))
#endif


/*
NOTES: 

CppCon 2016: Jason Jurecka “Game engine using STD C++ 11"
https://www.youtube.com/watch?v=8AjRD6mU96s

*/