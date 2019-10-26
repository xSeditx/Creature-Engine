#include"UniformBuffer.h"

namespace OpenGL
{

	template<typename _Ty>
	ResidentUniformBufferObject<_Ty>::
		ResidentUniformBufferObject(Shader *_program, const char *_name, unsigned int _slot) NOEXCEPT
		:
		BlockSize(sizeof(_Ty)),
		Name(_name),
		Slot(_slot),
		Program(_program->GL_Handle)
	{
		Index = fetchIndex(Name);
		SetBinding(Slot);

		Create();
		Bind();
		SetData(NULL);

		BindBase(_slot);
		SetSubData(0, BlockSize, NULL);
		Unbind();
	}


	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::Create() NOEXCEPT
	{
		glCreateBuffers(1, &GL_Handle);
	}
	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::BindBase(GLuint _slot) NOEXCEPT
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, _slot, GL_Handle);
	}
	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::SetBinding(GLuint _slot) NOEXCEPT
	{
		glUniformBlockBinding(Program, Index, _slot);
	}
	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::Update() NOEXCEPT
	{
		Bind();
		SetSubData(0, BlockSize, &Data);
		Unbind();
	}

	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::SetData(_Ty* _data) NOEXCEPT
	{
		glBufferData(GL_UNIFORM_BUFFER, BlockSize, _data, GL_DYNAMIC_DRAW);
	}
	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::SetSubData(size_t _offset, size_t _range, void* _data) NOEXCEPT
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, _range, _data);
	}
	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::Bind() NOEXCEPT
	{
		glBindBuffer(GL_UNIFORM_BUFFER, GL_Handle);
	}
	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::Unbind() NOEXCEPT
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::LockBuffer() NOEXCEPT
	{
		if (gSync)
		{
			glDeleteSync(gSync);
		}
		gSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}
	template<typename _Ty>  void ResidentUniformBufferObject<_Ty>::WaitBuffer() NOEXCEPT
	{
		if (gSync)
		{
			while (true)
			{
				GLenum waitReturn = glClientWaitSync(gSync, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
				if (waitReturn == GL_ALREADY_SIGNALED || waitReturn == GL_CONDITION_SATISFIED)
				{
					return;
				}
			}
		}
	}

	template<typename _Ty>   int ResidentUniformBufferObject<_Ty>::fetchIndex(const char *_name) NOEXCEPT
	{
		return glGetUniformBlockIndex(Program, _name);
	}

}// End OpenGL NS