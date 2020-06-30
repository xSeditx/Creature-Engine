
#include"Texture.h"

namespace Graphics
{
    Texture::~Texture()
    {
        if (imageOwned)
        {
            delete(Picture);
        }
    }

    Texture::Texture(std::string _file) noexcept 
		:
		Type(0),
		Picture(nullptr),
		ImageFormatComplete(false),
        imageOwned{ false }
	{
		WARN_ME("SOIL Load on the Texture constructor is Deactivated right now");
        Picture = new Bitmap(_file);
        if (!Picture->Data())
        {
            static Bitmap *default_Texture = new Bitmap("../Resources/Background.bmp");
            DEBUGPrint(CON_Yellow, "Using Default Texture");
            Picture = default_Texture;
        }
        GL_Handle = OpenGL::new_TextureHandle();

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

        OpenGL::update_Texture(Picture->Data(), Picture->Dimensions(), InternalFormat, Format);
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
    Texture::Texture(Vec2 _size, int32_t _dataFormat, int32_t _internalFormat, uint32_t _wrap, uint32_t _filtering, uint32_t _type) noexcept
        :
        Target(GL_TEXTURE_2D)
    {
        GL_Handle = OpenGL::new_TextureHandle();
        Bind();

        SetFiltering(_filtering);
        SetWrap(_wrap);
        OpenGL::update_Texture(nullptr, _size, _internalFormat, _dataFormat, _type);
    } 

	void Texture::SetTarget(unsigned int param) noexcept
	{
		Target = param;
	}

	void Texture::SetWrap(unsigned int _param) noexcept
	{
        OpenGL::set_Texture_WrapX(Target, _param);
        OpenGL::set_Texture_WrapY(Target, _param);
	}
	void Texture::SetFiltering(unsigned int _param) noexcept
	{
        OpenGL::set_Texture_Magnification(Target, _param);
        OpenGL::set_Texture_Minification(Target, _param);
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