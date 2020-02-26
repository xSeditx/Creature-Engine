


#include"Texture.h"
//#include"Shader.h"
//#include"Camera.h"
#include"Core\Common.h"
//#include"Primitive.h"

//assimpmt.lib
//glfw3.lib
//libSOIL.lib
namespace Graphics
{

    _static uint32_t Texture::debugVAO;
    _static uint32_t Texture::debugVBO;
    _static Shader* Texture::debugShader;
    _static Camera2D Texture::debugCamera;
    _static Vec2 QuadData[6] =
    {
        Vec2(0, 0),  Vec2(1, 0),  Vec2(0, 1),
        Vec2(1, 1),  Vec2(0, 1),  Vec2(1, 0)
    };
    _static void Texture::InitDebug()
    {
        debugCamera = Camera2D(1280, 740);
        debugShader = new Shader(VdebugRenderer, FdebugRenderer);

        debugVAO = OpenGL::create_VAO();
        debugVBO = OpenGL::create_VBO();

        debugShader->Bind();
        {
            OpenGL::bind_VAO(debugVAO);
            OpenGL::bind_VBO(debugVBO);
            OpenGL::set_Attribute(2, "aPos");
        }
        debugShader->Unbind();
    }


    void Texture::Render(int _x, int _y, int _w, int _h)
    {

        Mat4 Transform = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
        Transform = glm::translate(Transform, Vec3(_x, _y, 0));
        Transform = glm::rotate(Transform, glm::radians(0.f), Vec3(1.0f, 0.0f, 0.0f));
        Transform = glm::rotate(Transform, glm::radians(0.f), Vec3(0.0f, 1.0f, 0.0f));
        Transform = glm::rotate(Transform, glm::radians(0.f), Vec3(0.0f, 0.0f, 1.0f));

        ///Shader::GetActiveShader()->SetUniform("ProjectionMatrix", OrthographicMatrix(640, 480));
        ///Shader::GetActiveShader()->SetUniform("ViewMatrix", Mat4(1));
        ///Shader::GetActiveShader()->SetUniform("ModelMatrix", Transform);

        ///Plane temp(Vec3(0), Vec3(0), (float)_w, (float)_h);
        ///Shader::GetActiveShader()->SetTextureUniform("DiffuseTexture", this);
        ///temp.Render(*Shader::GetActiveShader());

        debugShader->Bind();
        {
            OpenGL::bind_VAO(debugVAO);
            debugCamera.Bind();
            debugShader->SetUniform("Position", Vec4(_x, _y, _w, _h));

            glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        debugShader->Unbind();
    }

    _static std::string Texture::VdebugRenderer =
        "#version 330 core         \n\
layout(location = 0) in vec2 aPos; \n\
uniform vec4 Position;             \n\
uniform mat4 ProjectionMatrix;     \n\
uniform mat4 ViewMatrix;           \n\
out vec2 TexCoords;                \n\
void main()                        \n\
{                                  \n\
    TexCoords = aPos.xy;           \n\
    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
    gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
}";

    _static std::string Texture::FdebugRenderer =
        "#version 330 core \n\
uniform sampler2D DebugText;   \n\
in vec4 Col;                   \n\
in vec2 TexCoords;             \n\
out vec4 FragColor;            \n\
void main()                    \n\
{                              \n\
    vec4 Samp = texture2D(DebugText, TexCoords); \n\
    FragColor = Samp;   \n\
}";

  //texture2D(sampler2D(DiffuseTexture), TexCoords.xy); 

 
//Vec2 TexCoords[6] =
//{
//    Vec2(0.0f, 1.0f), Vec2(0.0f, 0.0f),        Vec2(1.0f, 0.0f),
//    Vec2(0.0f, 1.0f), Vec2(1.0f, 0.0f),        Vec2(1.0f, 1.0f)
//};








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

        InternalFormat = GL_RGBA;// GL_RGBA;GL_RED;// 
        Format = GL_RGBA;// _INTEGER;// GL_RGBA_INTEGER; GL_RED;//
        glTexImage2D(Target, 0, InternalFormat, (Picture->Width()), Picture->Height(), 0, Format, GL_UNSIGNED_BYTE, Picture->Data()); // 	_GL(glTexImage2D(Target, 0, GL_RGB, Picture.Size.x, Picture.Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, Picture.Data));





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
	}
	Texture::Texture(Bitmap& image) noexcept
		:
		Picture(&image),
		Type(0),
		GL_Handle(0),
		MipmapComplete(0)
	{

		glGenTextures(1, &GL_Handle);
		Bind();
		SetFiltering(GL_NEAREST);// GL_LINEAR);
		SetWrap( GL_CLAMP_TO_EDGE);
		//GL_RGBGL_REPEAT);//
		InternalFormat = GL_RGBA;// GL_RGBA;GL_RED;// 
		Format = GL_RGBA;// _INTEGER;// GL_RGBA_INTEGER; GL_RED;//
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

	//Texture Wrapping
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

	// Filtering
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
	
	// MipMapping
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





























    //  
    //  bool Texture::Load()
    //  {
    //  	Bind();
    //  
    //  	SetMinFiltering(GL_LINEAR_MIPMAP_LINEAR);  // Texture Layout when sampling past texture
    //  	SetMagFiltering(GL_LINEAR);
    //  
    //  	SetWrap(GL_REPEAT); // Texture Layout when sampling past texture
    //  
    //  
    //  	if (Picture->Channels == 4)
    //  	{
    //  		Format = GL_RGBA;
    //  		InternalFormat = GL_RGBA;
    //  	}
    //  	else if (Picture->Channels == 3)
    //  	{
    //  		Format = GL_RGB;
    //  		InternalFormat = GL_RGB;
    //  	}
    //  	_GL(glTexImage2D(Target, 0, InternalFormat, Picture->Size.x, Picture->Size.y, 0, Format, GL_UNSIGNED_BYTE, Picture->Data)); // 	_GL(glTexImage2D(Target, 0, GL_RGB, Picture.Size.x, Picture.Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, Picture.Data));
    //  	glGenerateMipmap(Target);
    //  	Handle = glGetTextureHandleARB(TextureID);
    //  	glMakeTextureHandleResidentARB(Handle);
    //  
    //  	Unbind();
    //  	return true;
    //  }
    //  
    //Target


///     Texture::Texture(Bitmap& image, int _x, int _y, int _width, int _height)
///     {// Generate a Texture from a Sub image 
///     	Picture = new Bitmap();
///     	///*Picture = Bitmap::FlippedImage(image);
///     
///     	glGenTextures(1, &GL_Handle);
///     	Bind();
///     
///     	SetMinFiltering(GL_LINEAR_MIPMAP_LINEAR);  // Texture Layout when sampling past texture
///     	SetMagFiltering(GL_LINEAR);
///     	SetWrap(GL_REPEAT); // Texture Layout when sampling past texture
///     
///     	size_t Stride = Picture->Channels();
///     	size_t SrcByteHeight = Picture->Height() * Stride;
///     	size_t SrcByteWidth = Picture->Width() * Stride;
///     
///     	size_t DestByteHeight = _height * Stride;
///     	size_t DestByteWidth = _width * Stride;
///     
///     	size_t RowSize = DestByteWidth;
///     
///     	size_t DestIndex = 0, SourceIndex = 0;// (_x * Stride);
///     
///     	unsigned char* TempImage = new unsigned char[DestByteHeight * DestByteWidth];
///     	size_t Buffered = 0;
///     	if (DestByteWidth + (_x * Stride) > SrcByteWidth)
///     	{
///     		RowSize -= (_x * Stride);
///     		DestByteWidth -= (_x * Stride);
///     		Buffered = (_x * Stride);
///     	}
///     
///     	int Counter = 0;
///     	for (int py = 0; py < _height; ++py)
///     	{
///     		unsigned char* Src = &Picture->Data[SourceIndex];
///     		unsigned char* Dest = &TempImage[DestIndex];
///     
///     		memcpy(Dest, Src, RowSize);
///     
///     		SourceIndex += SrcByteWidth;
///     		DestIndex += DestByteWidth;
///     
///     		Counter++;
///     	}
///     	Picture->Data = TempImage;
///     
///     
///     	*Picture = Bitmap::FlippedImage(*Picture);
///     
///     	if (Picture->Channels() == 4)
///     	{
///     		Format = GL_RGBA;
///     		InternalFormat = GL_RGBA;
///     	}
///     	else if (Picture->Channels() == 3)
///     	{
///     		Format = GL_RGB;
///     		InternalFormat = GL_RGB;
///     	}
///     
///     
///     	glTexImage2D(Target, 0, InternalFormat, (Picture->Width()), Picture->Height(), 0, Format, GL_UNSIGNED_BYTE, Picture->Data()); // 	_GL(glTexImage2D(Target, 0, GL_RGB, Picture.Size.x, Picture.Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, Picture.Data));
///     	glGenerateMipmap(Target);
///     
///     	Handle = glGetTextureHandleARB(GL_Handle);
///     	glMakeTextureHandleResidentARB(Handle);
///     	delete (TempImage);
///     	Unbind();
///     }
