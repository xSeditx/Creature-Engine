#pragma once



#include"Core\Common.h"
#include"Bitmap.h"
#include<vector>

/*
Texture Base Class
Discussion: Should Derivation from the Asset class really be a thing or should there be an Asset Manager object in each Asset composing instead of inheriting.
This has already caused a good amount of issues during destruction when deep copies are made as blocks are being invalidated and the Asset class is attempting to be destroyed twice.
Asset derivation needs to cease. It is a pain in the ass and needs to go. It is attempting to double delete objects for whatever reasons and by far if a Manager is needed
an Object of static Asset<T> type should be created in its place instead.
*/
namespace Graphics
{

	///struct Shader {};

	class CREATURE_API Texture
	{
	public:
		NO_COPY_OR_ASSIGNMENT(Texture);

		Texture() = default;

		/* Load Texture from Image file*/
		Texture(std::string file) noexcept;

		/* Create a Texture from a Bitmap*/
		Texture(Bitmap &image) noexcept;

		/* Create a Texture other */
		Texture(Texture&& _other) noexcept
			:
			Picture(std::move(_other.Picture)),
			GL_Handle(std::move(_other.GL_Handle)),
			Target(std::move(_other.Target)),
			Type(std::move(_other.Type)),
			Format(std::move(_other.Format)),
			WrapMode(std::move(_other.WrapMode)),
			Filtering(std::move(_other.Filtering)),
			InternalFormat(std::move(_other.InternalFormat)),
			Handle(std::move(_other.Handle))
		{
			DEBUGPrint(CON_Red, " I believe I messed this up but do not really have time to think about this right now");
		}

		/* Assign this Texture from other*/
		Texture& operator =(Texture&& _other) noexcept
		{
			Picture = std::move(_other.Picture);
			GL_Handle = std::move(_other.GL_Handle);
			Target = std::move(_other.Target);
			Type = std::move(_other.Type);
			Format = std::move(_other.Format);
			WrapMode = std::move(_other.WrapMode);
			Filtering = std::move(_other.Filtering);
			InternalFormat = std::move(_other.InternalFormat);
			Handle = std::move(_other.Handle);
			DEBUGPrint(CON_Red, " I believe I messed this up but do not really have time to think about this right now");
			return *this;
		}

		void Update(uint8_t *_memory)
		{
			/* Depending on if we are using Bindldess TYextures or not determines how we update this */
REFACTOR("Change this for Bindless Textures later on. Odds are we should instead generate two sets of Texture code for the Engine\
 such that a simple preprocessor directive can determine which functiosns we are using. If runtime is needed than we will\
 need to load up the texture functions as pointers so their functionality can be assigned inside of an Init function somewhere,\
 ALSO: Likely should make it update Bitmap then Texture but currently Bitmap contains original");

DEBUG_CODE(memcpy(Picture->Data() , _memory, Picture->size()));
			Bind();
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Picture->Width(), Picture->Height(), GL_RGBA, GL_UNSIGNED_BYTE, _memory);
			Unbind();
  		}

		void SetTarget(unsigned int param);

		void SetFiltering(unsigned int param);
		void SetMagFiltering(unsigned int param);
		void SetMinFiltering(unsigned int param);

		void SetWrap(unsigned int param);
		void SetWrapX(unsigned int param);
		void SetWrapY(unsigned int param);

		//	OpenGL has a particular syntax for writing its color format enumerants.It looks like this: GL_[components?][size?][type?]

		inline void Bind()
		{
			////UniformLocation = glGetUniformLocation(Shader::getHandle(), "Texture1");
			//glUniform1i(UniformLocation, 0);
			//glActiveTexture(GL_TEXTURE0 + 0);

			glBindTexture(Target, GL_Handle);
		}
		inline void Unbind()
		{
			glBindTexture(Target, 0);
		}


		inline void MipmapOn();
		inline void MipmapOff();

		void Render(int _x, int _y, int _w, int _h);
	private:

		Bitmap *Picture{ nullptr };
		uint32_t GL_Handle{ 0 };
		uint32_t Target{ GL_TEXTURE_2D };

		uint32_t
			Type{ GL_DEPTH_COMPONENT32F }, //GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32 and GL_DEPTH_COMPONENT32F.
			Format{ GL_RGB },
			WrapMode{ GL_REPEAT },
			Filtering{ GL_LINEAR },
			InternalFormat{ GL_RGB };

		GPUptr Handle{ NULL };


		bool MipmapComplete{ false };
		bool ImageFormatComplete{ false };
	};



	/*
	TextureBufferObject
    	Accept a flat formated block of memory for sequential writing in the Shader
    	Discussion:
    	Should this be in the GLBuffers.h header instead of here as it functions more like a Buffer Object then a Texture Object
	*/

	template<typename T>
	struct CREATURE_API TextureBufferObject
	{
		TextureBufferObject() = default;
		TextureBufferObject(std::string _name, std::vector<T> _data);
		void Bind(GLuint _slot);

		GLuint TextureID;
		GLuint BufferID;
		GLuint UniformLocation;
		size_t Size;
		std::string Name;
		std::vector<T> Data;
	};

	template<typename _Ty>
	TextureBufferObject<_Ty>::TextureBufferObject(std::string _name, std::vector<_Ty> _data)
		:
		Name(_name),
		Data(_data)
	{
		glGenBuffers(1, &BufferID);
		glBindBuffer(GL_TEXTURE_BUFFER, BufferID);
		glBufferData(GL_TEXTURE_BUFFER, sizeof(_Ty) * _data.size(), _data.data(), GL_STATIC_DRAW);
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_BUFFER, TextureID);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, BufferID);
	}

	template<typename _Ty>
	void TextureBufferObject<_Ty>::Bind(GLuint _slot)
	{
		UniformLocation = glGetUniformLocation(Shader::GetActiveShader()->GetName(), Name.c_str());
		glUniform1i(UniformLocation, _slot);
		glActiveTexture(GL_TEXTURE0 + _slot);
		glBindTexture(GL_TEXTURE_BUFFER, TextureID);
	}



}// NS Graphics


//http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html

	// Simple move assignment operator
 //VertexArrayObject& operator=(VertexArrayObject&& other)
 //{
 //   // *this = 
 //    return *std::move(other);//
 //}
/*
#ifdef BINDLESS_ATTRIBUTES
void Bind() {}
void Unbind() {}

template<typename T>
void Attach(BufferTypes bufferT, VertexBufferObject<T>* buffer)
{
	if (GL_Handle == NULL)
	{
		glGenVertexArrays(1, &GL_Handle);
	}
	Bind();
	GLint Amount = sizeof(T) / sizeof(float);
	Buffers.push_back(buffer);

	switch (bufferT)
	{
	case INDICE:
		buffer->AttributeType = INDICE;
		return;
		break;
	case VERTEX:
		buffer->AttributeType = VERTEX;
		break;
	case COLOR:
		buffer->AttributeType = COLOR;
		break;
	case NORMAL:
		buffer->AttributeType = NORMAL;
		break;
	case UVCOORD:
		buffer->AttributeType = UVCOORD;
		break;
	case TANGENT:
		buffer->AttributeType = TANGENT;
		break;
	}
}
#else
//#endif

 */