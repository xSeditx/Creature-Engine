#pragma once


#include "../OpenGL.h"
#include "../Camera/Camera2D.h"
#include "../Shader/Shader.h"
#include "../../Materials/Image/Texture.h"
#include "../Renderer/Transform.h"

#include "Mesh.h"

/* Sort or Bucket Visible Objects
foreach( render target )    // framebuffer
foreach( pass )             // depth, blending, etc... states
foreach( material )         // shaders
foreach( material instance) // textures, normals, diffuse,
foreach( vertex format )    // vertex buffers
foreach( object )           //
{
 WriteUniformData( object );
 glDrawElementBaseVertex(
     GL_TRIANGLES,
	 object->indexCount,
	 GL_UNSIGNED_SHORT,
	 object->indexDataOffset,
	 object->baseVertex );
}

*/
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

        static void drawArrayLines(uint32_t _vbo, uint32_t _elementCount)
        {
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glDrawArrays(GL_LINES, 0, _elementCount);
            DEBUG_CODE(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

		static void drawArrayInstanced( uint32_t _indicesCount, uint32_t _instanceCount)
		{
			glDrawArraysInstanced(GL_TRIANGLES, 0, _indicesCount, _instanceCount);//mode/first/count/instancecount
		}
		static void drawIndices(uint32_t _ibo, uint32_t _elementCount)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glDrawElements (GL_TRIANGLES, _elementCount, GL_FLOAT, (char*)0);
			DEBUG_CODE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
	};
}
