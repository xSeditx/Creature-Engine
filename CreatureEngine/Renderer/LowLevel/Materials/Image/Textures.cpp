
#include"Texture.h"

namespace Graphics
{

    Texture::Texture(std::string _file) noexcept 
		:
		Type(0),
		Picture(nullptr),
		ImageFormatComplete(false)
	{
		WARN_ME("SOIL Load on the Texture constructor is Deactivated right now");
        Picture = new Bitmap(_file);

        Bind();

        SetFiltering(GL_NEAREST);
        SetWrap(GL_CLAMP_TO_EDGE);

        if (Picture->Channels() == 3)
        {
            Format = GL_RGB;
            InternalFormat = GL_RGBA;

            OpenGL::PixelAlignmentUnpack(1);
            OpenGL::PixelAlignmentPack(1);
        }
        else
        {
            InternalFormat = GL_RGBA;
            Format = GL_RGBA;

            OpenGL::PixelAlignmentUnpack(4);
            OpenGL::PixelAlignmentPack(4);
        }

        glTexImage2D(Target, 0, InternalFormat, (Picture->Width()), Picture->Height(), 0, Format, GL_UNSIGNED_BYTE, Picture->Data()); // 	_GL(glTexImage2D(Target, 0, GL_RGB, Picture.Size.x, Picture.Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, Picture.Data));
	}

	Texture::Texture(Bitmap& image) noexcept
		:
		Picture(&image),
		Type(0),
		GL_Handle(0),
		MipmapComplete(0)
	{

        GL_Handle = OpenGL::new_TextureHandle();
		Bind();
		SetFiltering(GL_NEAREST); 
		SetWrap( GL_CLAMP_TO_EDGE);

        if(image.Channels() == 3)
        { 
            Format = GL_RGB;
            InternalFormat = GL_RGBA;

            OpenGL::PixelAlignmentUnpack(1);
            OpenGL::PixelAlignmentPack(1);
        }
        else
        {
            InternalFormat = GL_RGBA;
            Format = GL_RGBA; 

            OpenGL::PixelAlignmentUnpack(4);
            OpenGL::PixelAlignmentPack(4);

        }

		glTexImage2D(Target, 0, InternalFormat, (Picture->Width()), Picture->Height(), 0, Format, GL_UNSIGNED_BYTE, Picture->Data()); // 	_GL(glTexImage2D(Target, 0, GL_RGB, Picture.Size.x, Picture.Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, Picture.Data));
	}
    Texture::Texture(Vec2 _size, int32_t _dataFormat, int32_t _internalFormat, uint32_t _wrap, uint32_t _filtering, uint32_t _type)
        :
        Target(GL_TEXTURE_2D)
    {
        glGenTextures(1, &GL_Handle);
        glBindTexture(Target, GL_Handle);

        SetFiltering(_filtering);
        SetWrap(_wrap);
        glTexImage2D(Target, 0, _internalFormat, (size_t)_size.x, (size_t)_size.y, Border, _dataFormat, _type, (void*)NULL);
    } 

	void Texture::SetTarget(unsigned int param)
	{
		Target = param;
	}

	void Texture::SetWrap(unsigned int param)
	{
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, param);
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, param);
	}
	void Texture::SetWrapX(unsigned int param)
	{
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, param);

	}
	void Texture::SetWrapY(unsigned int param)
	{
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, param);
	}

	void Texture::SetFiltering(unsigned int param)
	{
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, param);
		glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, param);
	}
	void Texture::SetMagFiltering(unsigned int param)
	{
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, param);
	}
	void Texture::SetMinFiltering(unsigned int param)
	{
		glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, param);
	}
	
    void Texture::CreateMipmap()
    {// Generates Mipmaps for our Texture
        Bind();
        glGenerateMipmap(GL_TEXTURE_2D);
        MipmapComplete = true;
    }
    void Texture::MipmapOn()
	{// Turns on Mipmap for this texture
		glTexParameteri(Target, GL_GENERATE_MIPMAP, GL_TRUE);
	}
	void Texture::MipmapOff()
	{// Turns off Mipmaps for this Texture
		glTexParameteri(Target, GL_GENERATE_MIPMAP, GL_FALSE);
	}

}//End NS Graphics


//  ALTERNATIVE.... Figure out what to do here
//
//texture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture 
//(
//    "face.png",
//    SOIL_LOAD_AUTO,
//    SOIL_CREATE_NEW_ID,
//    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
//);'
///TextureID = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);