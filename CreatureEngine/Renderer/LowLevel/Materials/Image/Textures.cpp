#include"Renderer\LowLevel\Materials\Image\Texture.h"


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

	Texture::Texture(std::string file)
		:
		Type(0),
		Picture(nullptr),
		ImageFormatComplete(false)
	{
		///TextureID = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
	}
	Texture::Texture(Bitmap& image)
		:
		Picture(&image),
		Type(0),
		GL_Handle(0),
		MipmapComplete(0)
	{
		glGenTextures(1, &GL_Handle);
		///Load();
	}
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
	// State Handler
	void Texture::Bind()
	{
		glBindTexture(Target, GL_Handle);
	}
	void Texture::Unbind()
	{
		glBindTexture(Target, 0);
	}
	// MipMapping
	void Texture::MipmapOn()
	{
		glTexParameteri(Target, GL_GENERATE_MIPMAP, GL_TRUE);
	}
	void Texture::MipmapOff()
	{
		glTexParameteri(Target, GL_GENERATE_MIPMAP, GL_TRUE);
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
	}

}//End NS Graphics