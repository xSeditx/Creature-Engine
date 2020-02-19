#pragma once


#include "../OpenGL.h"
#include "../Camera/Camera2D.h"
#include "../Shader/Shader.h"
#include "../../Materials/Image/Texture.h"
#include "../Renderer/Transform.h"

#include "Mesh.h"

namespace OpenGL
{

	class Renderer
	{
	protected:
		 
		///Shader* mainProgram; Comment to avoid Minor confusion until its working
	public:
		Renderer() = default;
		static void drawArray(uint32_t _vbo, uint32_t _elementCount)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glDrawArrays(GL_TRIANGLES, 0, _elementCount);
			DEBUG_CODE(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}	
		static void drawArrayInstanced(uint32_t _vbo, uint32_t _elementCount)
		{
			DEBUG_CODE(CheckGLERROR());
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (int)(10));//mode/first/count/instancecount
			DEBUG_CODE(glBindBuffer(GL_ARRAY_BUFFER, 0));
			DEBUG_CODE(CheckGLERROR());
		}
		static void drawIndices(uint32_t _ibo, uint32_t _elementCount)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glDrawElements (GL_TRIANGLES, _elementCount, GL_FLOAT, (char*)0);
			DEBUG_CODE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
	};
}
