#pragma once


#include"Core\Common.h"

#include<GL/gl.h>
#include<GL/glu.h>
#define _USING_OPENGL_API /* Are we using OpenGL or something else like DX*/
#pragma comment(lib, "OpenGL32.lib") 

namespace OpenGL
{
	CREATURE_API void InitOpenGL();
}