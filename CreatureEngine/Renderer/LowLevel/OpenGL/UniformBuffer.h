#pragma once


#include"OpenGL.h"

namespace OpenGL
{

	class Shader;


	template<typename _Ty>
	struct CREATURE_API ResidentUniformBufferObject final
	{
		ResidentUniformBufferObject() NOEXCEPT = default;
		ResidentUniformBufferObject(Shader *_program, const char *_name, unsigned int _slot) NOEXCEPT;

		void Create() NOEXCEPT;
		void BindBase(GLuint _slot) NOEXCEPT;
		void SetBinding(GLuint _slot) NOEXCEPT;
		void Update() NOEXCEPT;

		void SetData(_Ty* _data) NOEXCEPT;
		void SetSubData(size_t _offset, size_t _range, void* _data) NOEXCEPT;
		void Bind() NOEXCEPT;
		void Unbind() NOEXCEPT;

		void LockBuffer() NOEXCEPT;
		void WaitBuffer() NOEXCEPT;

		int fetchIndex(const char *_name) NOEXCEPT;

		inline GLuint g_Index() NOEXCEPT
		{
			return Index;
		}

		_Ty Data;
	private:
		// http://docs.gl/gl4/glBufferSubData
		unsigned int BlockSize;

		const char *Name;

		GLsync gSync;
		GLuint Slot;
		GLuint Index;
		GLuint GL_Handle;
		GLuint Program;
	};

}// End OpenGL NS