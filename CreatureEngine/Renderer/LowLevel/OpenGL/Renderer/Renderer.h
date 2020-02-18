
#include"../OpenGL.h"
#include"Mesh.h"
namespace OpenGL
{
	class Renderer
	{
	
	public:


        void Submit(Mesh& _mesh)
        {
        //    _mesh.VAO->Buffers[0]->data_ptr();
        }


        static void drawArray(uint32_t _vbo, uint32_t _elementCount)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glDrawArrays(GL_TRIANGLES, 0, _elementCount);
			DEBUG_CODE(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}
		static void drawIndices(uint32_t _ibo, uint32_t _elementCount)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glDrawElements (GL_TRIANGLES, _elementCount, GL_FLOAT, (char*)0);
			DEBUG_CODE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
	};
}