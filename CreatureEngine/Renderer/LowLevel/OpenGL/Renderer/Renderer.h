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
///TODO: Why the fuck will it not let me extend the OpenGL namespace here, it does then it goes on stopping me from using check_FBO_Status besides bind_FBO being found directly above it and it has no problem with that. 
///        IT makes not fucking sense''
	class Renderer
	{
	protected:
		 
		///Shader* mainProgram; Comment to avoid Minor confusion until its working
	public:
		Renderer() = default;

        /* Binds a Specific VBO and Renders it */
        static void drawArray(uint32_t _vbo, uint32_t _elementCount)
		{
            OpenGL::bind_VBO(_vbo);
            glDrawArrays(GL_TRIANGLES, 0, _elementCount);
			DEBUG_CODE(OpenGL::unbind_VBO());
		}	
        /* Renders _elementCount number of Vertices from the Currently bound Vertex Buffer Object */
        static void drawArray(uint32_t _elementCount)
        {
            glDrawArrays(GL_TRIANGLES, 0, _elementCount);
        }


        static void drawArrayLines(uint32_t _vbo, uint32_t _elementCount)
        { 
            OpenGL::bind_VBO(_vbo);
            glDrawArrays(GL_LINES, 0, _elementCount);
            DEBUG_CODE(OpenGL::unbind_VBO());
            DEBUG_CODE(CheckGLERROR());
        }
        static void drawArrayLines(uint32_t _elementCount)
        {
            glDrawArrays(GL_LINES, 0, _elementCount);
            DEBUG_CODE(OpenGL::unbind_VBO());
            DEBUG_CODE(CheckGLERROR());
        }

        static void drawElementsInstanced(void *_indices, uint32_t _indicesCount, uint32_t _instanceCount)
        {
            glDrawElementsInstanced(GL_TRIANGLES, 0, _indicesCount, _indices, _instanceCount);//mode/count/type, *indices, instancecount
        }
        static void drawArrayInstanced(uint32_t _indicesCount, uint32_t _instanceCount)
		{
			glDrawArraysInstanced(GL_TRIANGLES, 0, _indicesCount, _instanceCount);//mode/first/count/instancecount
		}
		static void drawIndices(uint32_t _ibo, uint32_t _elementCount)
		{
            OpenGL::bind_IBO(_ibo);
			glDrawElements(GL_TRIANGLES, _elementCount, GL_FLOAT, (char*)0);
			DEBUG_CODE(OpenGL::unbind_IBO());
		}

        /* Returns a vec4 of Normalized Colors for OpenGL Accepts 0-255*/
        static Vec4 normalize_RGBA_Color(int _r, int _g, int _b, int _a)
        {
            constexpr float coef = 1.0f / 255.0f;
            Vec4 result;
            result.r = _r * coef;
            result.g = _g * coef;
            result.b = _b * coef;
            result.a = _a * coef;
            return result;
        }

        /* Returns a vec3 of Normalized Colors for OpenGL Accepts 0-255*/
        static Vec3 normalize_RGB_Color(int _r, int _g, int _b)
        {
            constexpr float coef = 1.0f / 255.0f;
            Vec3 result;
            result.r = _r * coef;
            result.g = _g * coef;
            result.b = _b * coef;
            return result;
        }

        /* Returns a single Normalzed Color value for OpenGL Accepts 0-255*/
        static float normalize_Color(int _col)
        {
            constexpr float coef = 1.0f / 255.0f;
            return _col * coef;;
        }
	};
}
