#pragma once


#include"Bitmap.h"
#include"Core\Common.h"

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

	struct Shader {};


	class Texture
	{
	public:
		NO_COPY_OR_ASSIGNMENT(Texture);

		Texture() = default;

		/* Load Texture from Image file*/
		Texture(std::string file);

		/* Create a Texture from a Bitmap*/
		Texture(Bitmap &image);

		Bitmap *Picture;
		uint32_t GL_Handle{ 0 };
		uint32_t Target{ GL_TEXTURE_2D };

		uint32_t
			Type, // GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32 and GL_DEPTH_COMPONENT32F.
			Format{ GL_RGB },
			WrapMode,
			Filtering,
			InternalFormat{ GL_RGB };

		GPUptr Handle{ NULL };

		void SetTarget(unsigned int param);

		void SetFiltering(unsigned int param);
		void SetMagFiltering(unsigned int param);
		void SetMinFiltering(unsigned int param);

		void SetWrap(unsigned int param);
		void SetWrapX(unsigned int param);
		void SetWrapY(unsigned int param);

		//	OpenGL has a particular syntax for writing its color format enumerants.It looks like this: GL_[components?][size?][type?]
		inline void Bind();
		inline void Unbind();
		inline void MipmapOn();
		inline void MipmapOff();

		void Render(int _x, int _y, int _w, int _h);
	private:
		bool MipmapComplete;
		bool ImageFormatComplete;
	};



	/*
	TextureBufferObject
    	Accept a flat formated block of memory for sequential writing in the Shader
    	Discussion:
    	Should this be in the GLBuffers.h header instead of here as it functions more like a Buffer Object then a Texture Object
	*/

	template<typename T>
	struct TextureBufferObject
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
}


//http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html
