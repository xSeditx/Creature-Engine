#include<cstdint>
#include<string>
//$(VCInstallDir)Auxiliary\VS\UnitTest\include

#include"glad\glad.h"
#include"GLFW\glfw3.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


// Will determine if the Given class or structure is exportable 
#define UNKNOWN_API


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


typedef int8_t byte;
typedef uint8_t ubyte;

// Object Tags
typedef uint32_t idTag;
typedef std::string  nameTag;

// 64 bit pointer to resident GPU objects
using GPUptr = uint64_t;

// ===================================================================================================================================================


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



/* Descriptor Defines to make code readability easier. Static tells in a CPP module if a function is defined as static 
pure_Virtual tells if a function is pure virtual in plain english*/
#define _static
#define pure_virtual  0