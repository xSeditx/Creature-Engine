
#include"Texture.h"


_static std::vector<Texture *> Texture::Texture_Resources;



Texture::Texture(std::string _file) noexcept
    :
    Picture(nullptr),
    ImageFormatComplete(false),
    imageOwned{ false }
{
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

    Texture_Resources.push_back(this);
}

Texture::Texture(Bitmap& image) noexcept
    :
    Picture(&image),
    GL_Handle(0),
    MipmapComplete(0)
{

    GL_Handle = OpenGL::new_TextureHandle();
    Bind();
    SetFiltering(GL_NEAREST);
    SetWrap(GL_CLAMP_TO_EDGE);

    if (image.Channels() == 3)
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


    Texture_Resources.push_back(this);
}
Texture::Texture(Vec2 _size, int32_t _dataFormat, int32_t _internalFormat, uint32_t _wrap, uint32_t _filtering, uint32_t _type) noexcept
    :
    Target(GL_TEXTURE_2D)
{
    GL_Handle = OpenGL::new_TextureHandle();
    Bind();
    Picture = new Bitmap(nullptr, _size);
    SetFiltering(_filtering);
    SetWrap(_wrap);
    OpenGL::update_Texture(nullptr, _size, _internalFormat, _dataFormat, _type);

    Texture_Resources.push_back(this);
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

void * Texture::get_Pixels()
{// Returns a Memory Address of the Pixels in the Texture */

    return get_Pixels({ 0,0,Width(), Height() });
}
void * Texture::get_Pixels(Vec4 _sourceRect)
{// Returns a Memory Address of the Pixels in the Texture of the given area */

    Bind();
    {
        glGetTexImage(GL_TEXTURE_2D, 0, Format, GL_UNSIGNED_BYTE, Picture->Data());
    }
    Unbind();
    CheckGLERROR();
    return (void*)Picture->Data();
}



Texture::~Texture()
{
    DEBUGPrint(CON_Red, "Deleting Texture: " << GL_Handle);
    OpenGL::delete_Texture(GL_Handle);

    int n = 0;
    for (auto& T : Texture_Resources)
    {
        if (T == this)
        {
            Texture_Resources.erase(Texture_Resources.begin() + n);
        }
        ++n;
    }
    delete(Picture);
}




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










