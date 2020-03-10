#pragma once

#include"Core/Common.h"
#include"Bitmap.h"
#include<vector>

#include "../../OpenGL/Camera/Camera2D.h"
#include "../../OpenGL/Shader/Shader.h"

/*
Texture Base Class
Discussion: Should Derivation from the Asset class really be a thing or should there be an Asset Manager object in each Asset composing instead of inheriting.
This has already caused a good amount of issues during destruction when deep copies are made as blocks are being invalidated and the Asset class is attempting to be destroyed twice.
Asset derivation needs to cease. It is a pain in the ass and needs to go. It is attempting to double delete objects for whatever reasons and by far if a Manager is needed
an Object of static Asset<T> type should be created in its place instead.
*/
namespace Graphics
{
    /* ==============================================================
    /* TEXTURE CLASS
    /* Image Uploaded to the GPU for Reading/Writing
    /* - Texture(std::string file) noexcept;
    /* - Texture(Bitmap &image) noexcept;
    /* ============================================================== */
	class CREATURE_API Texture
	{
	public:
		NO_COPY_OR_ASSIGNMENT(Texture);

		Texture() = default;
        ~Texture();
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
            memset(&_other, 0, sizeof(_other));
			DEBUGPrint(CON_Red, " I believe I messed this up but do not really have time to think about this right now");
             
			return *this;
		}

        /* Create an Empty Texture from the Given Size _size and Format parameters */
        Texture(Vec2 _size, int32_t _dataFormat, int32_t _internalFormat = GL_RGBA, uint32_t _wrap = GL_CLAMP_TO_EDGE, uint32_t _filtering = GL_NEAREST, uint32_t _type = GL_FLOAT) noexcept;

        /* Updates the Texture with the Memory Pointed to in _memory. 
         NOTE: User is responsible for ensuring memory is proper size */
		void Update(uint8_t *_memory) noexcept
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

        /* Sets the Target for the Texture. Default is GL_TEXTURE2D. 
         NOTE: Further work might be needed before it takes effect */
		void SetTarget(unsigned int param) noexcept;

        /* Sets the Min Mag filering for Texture */
		void SetFiltering(unsigned int param) noexcept;
        /* Sets the Magnification filering for Texture */
        void SetMagFiltering(unsigned int param) noexcept;
        /* Sets the Minification filering for Texture */
        void SetMinFiltering(unsigned int param) noexcept;

        /* Set the X and Y Wrap params for the Texture */
		void SetWrap(unsigned int param) noexcept;
        /* Set the X Wrap params for the Texture */
        void SetWrapX(unsigned int param) noexcept;
        /* Set the Y Wrap params for the Texture */
        void SetWrapY(unsigned int param) noexcept;

		//	OpenGL has a particular syntax for writing its color format enumerants.It looks like this: GL_[components?][size?][type?]

        /* Bind Texture Handle to OpenGL State */
		inline void Bind() noexcept
		{
			glBindTexture(Target, GL_Handle);
		}
        /* Bind Texture Handle to OpenGL State as well as the Slot in the Active Shader */
        inline void Bind(uint32_t _slot) noexcept
        {
            glActiveTexture(GL_TEXTURE0 + _slot);
            glBindTexture(Target, GL_Handle);
        }
        /* Disable Texture from bound slot*/
		inline void Unbind() noexcept
		{
			glBindTexture(Target, 0);
		}

        /* Generates Mipmaps for a Texture */
        void CreateMipmap() noexcept;
        /* Turns ON Mipmapping for a Texture that already has Mipmaps Generated ~ User Must Bind Texture First ~ */
        inline void MipmapOn() noexcept;
        /* Turns OFF  Mipmapping for a Texture that already has Mipmaps Generated ~ User Must Bind Texture First ~  */
        inline void MipmapOff() noexcept;

        /* Debug Render which Sends a Rect with the given Texture to the Screen */
		void Render(int _x, int _y, int _w, int _h) noexcept;

		/* Gets the OpenGL Handle of the Texture*/
		const uint32_t g_Handle() const noexcept
		{
			return GL_Handle;
		}

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

        uint32_t Border{ 0 };

        /* Bindless Pointer for AZDO */
		GPUptr Handle{ NULL };

        bool imageOwned{ false };
		bool MipmapComplete{ false };
		bool ImageFormatComplete{ false };
    };


    

    
    /*==================================================================================================================================
	/*                    TEXTURE BUFFER OBJECT 
    /*	Accept a flat formated block of memory for sequential writing in the Shader
    /*	Discussion:
    /*	Should this be in the GLBuffers.h header instead of here as it functions more like a Buffer Object then a Texture Object
	/*==================================================================================================================================*/

	template<typename T>
	struct CREATURE_API TextureBufferObject
	{
		TextureBufferObject() = default;
		TextureBufferObject(std::string _name, std::vector<T> _data);

        void Bind();
		void Bind(GLuint _slot);
        void Unbind();

        GLuint GL_Handle{ 0 };
        GLuint BufferID{ 0 };
        GLuint UniformLocation{0};
        size_t Size{ 0 };
        std::string Name{};
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
		glGenTextures(1, &GL_Handle);
		glBindTexture(GL_TEXTURE_BUFFER, GL_Handle);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, BufferID);
	}
    template<typename _Ty>
    void TextureBufferObject<_Ty>::Bind()
    {
        glBindTexture(GL_TEXTURE_BUFFER, GL_Handle);
    }

	template<typename _Ty>
	void TextureBufferObject<_Ty>::Bind(GLuint _slot)
	{
		UniformLocation = glGetUniformLocation(Shader::GetActiveShader()->GetName(), Name.c_str());
		glActiveTexture(GL_TEXTURE0 + _slot);
		glUniform1i(UniformLocation, _slot);
		glBindTexture(GL_TEXTURE_BUFFER, GL_Handle);
	}
}// NS Graphics


//http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html
