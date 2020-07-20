#include"GLBuffers.h"


///=================================================================================================================
///=================================================================================================================

Attribute::Attribute(void *_data, uint32_t _count, uint32_t _stride)
    :
    ElementCount(_count),
    AttributeType(NONE),
    Stride(_stride),
    GL_Handle(OpenGL::new_VBO()),
    Size(_stride * _count),
    BufferPtr(malloc(_stride * _count))
{
    memcpy(BufferPtr, _data, Size);
    Bind();
    {
      //  glBufferStorage(GL_ARRAY_BUFFER, Size, _data, GL_DYNAMIC_STORAGE_BIT);
        OpenGL::set_BufferData((uint32_t)Size, _data);
    }
    Unbind();
}


void Attribute::Bind()
{
    OpenGL::bind_VBO(GL_Handle);
}
void Attribute::Unbind()
{
    OpenGL::unbind_VBO();
}
void Attribute::Release()
{// Release the GL_Handle for reuse by OpenGL
    OpenGL::delete_VBO(GL_Handle);
}
void Attribute::Destroy()
{// Totaly remove the VBO from Graphic Memory and recover it by the Operating system
    OpenGL::delete_VBO(GL_Handle);
	delete(this);  
}

void* Attribute::Map(GLenum accessflags)
{// Map the OpenGL Buffer and return pointer to that memory
	Bind();
    REFACTOR("Setup OpenGL::map_Buffer(GL_ARRAY_BUFFER, accessflags) to deal with this, Currently there is an Error ");
    void* results =  glMapBuffer(GL_ARRAY_BUFFER, accessflags);
	return results;
}
void* Attribute::MapRange(int offset, int count, GLenum accessflags)
{// Possibly only avalible on later versions of OpenGL > 3.0
	Bind();
	return glMapBufferRange(GL_ARRAY_BUFFER, offset, count, accessflags);
}
void Attribute::Update(void* _data, size_t _sz)
{
    Size = _sz;
    Bind();
    {
        OpenGL::set_BufferData((uint32_t)_sz, _data);
    }
    Unbind();
}
bool Attribute::isMapped()
{
	Bind();
    int result{ 0 };
	if (AttributeType == INDICE)
	{
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_MAPPED, &result);
	}
	else
	{
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_MAPPED, &result);
	}
	return (result != 0);
}

size_t Attribute::MappedSize()
{
	Bind();
    int sz{ 0 };
    REFACTOR("Add a Target to avoid this Conditional so it being an IBO or VBO is choosen early in the Attributes Lifetime || 7/7/20 Perhaps remove the IBO from this class completely");
	if (AttributeType == INDICE)
	{
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &sz);
	}
	else
	{
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &sz);
	}
	return sz;
}

size_t Attribute::size() const { return Size / Stride; }
GLuint Attribute::get_SizeOnGPU()
{
    GLint results;
    Bind();
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &results);
    return results;
}
void Attribute::Append(void * _data, size_t _sz)
{
    size_t newSize = Size + _sz;
    uint8_t *newBuffer = (uint8_t*)malloc(newSize);

    memcpy(newBuffer, BufferPtr, Size);
    memcpy((uint8_t*)newBuffer + Size, _data, _sz);

    delete(BufferPtr);

    BufferPtr = newBuffer;

    Size = newSize;
    ElementCount = (uint32_t)Size / Stride;

    Update(newBuffer, newSize);
}


//  Print("DataSize: " << _sz);
//  Print("NewSize: " << NewSize);
//  Print("Addresses: " << (void*)newBuffer << " : " << (void*)mod);
//  Print("Difference: " <<  (int)((void*)mod)- (int)((void*)newBuffer) );
//template<typename _Ty>
//void Update(std::vector<_Ty>& _data)
//{
//    __debugbreak();
//    REFACTOR("DATA: Is simply debug shit, clean this up in the future. && This needs to have the Access changed from default Access and to the Access of this Buffer however I see no way to change the Acces type");
//    BufferPtr = &_data[0];
//    Bind();
//    {
//        OpenGL::set_BufferData(_data);
//    }
//    Unbind();
//    Size = _data.size() * sizeof(_Ty);
//}
 
///=================================================================================================================



///=================================================================================================================
///=================================================================================================================

ElementArrayBuffer::ElementArrayBuffer(GLuint* _data, size_t _count)
	:
	GL_Handle(NULL),
	ElementCount(_count)
{
	Data = new GLuint[_count];
	Data = _data;/// HMMM....

	glGenBuffers(1, &GL_Handle);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), _data, DEFAULT_BUFFER_ACCESS);
}
ElementArrayBuffer::ElementArrayBuffer(GLenum _access, GLuint* _data, size_t _count)
	:
	GL_Handle(NULL),
	ElementCount(_count)
{
	Data = new GLuint[_count];
	Data = _data;/// HMMM....

	glGenBuffers(1, &GL_Handle);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), _data, _access);
}

void ElementArrayBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_Handle);
}
void ElementArrayBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
///=================================================================================================================


GLuint GL_Handle;
int ElementCount;
std::vector<Attribute*> Buffers;


VertexArrayObject::VertexArrayObject()
	:
	GL_Handle(NULL),
	ElementCount(NULL)
{
	Buffers.reserve(5);
}

template<typename _Ty>
VertexArrayObject::VertexArrayObject(VertexBufferObject<_Ty>& vbo)
	:
	ElementCount(0),
	GL_Handle(OpenGL::new_VAO())
{
	Buffers.reserve(5);
}

#ifdef BINDLESS_ATTRIBUTES
    void VertexArrayObject::Bind()   {}
    void VertexArrayObject::Unbind() {}
#else
    void VertexArrayObject::Bind()
    {
        OpenGL::bind_VAO(GL_Handle);
    }
    void VertexArrayObject::Unbind()
    {
        OpenGL::unbind_VAO();
    }
#endif
 


 #include"Renderer.h"

//FrameBufferObject::FrameBufferObject(Vec2 _size)
//    : FrameBufferObject(_size.x, _size.y)
//{}


/* Creates a Frame Buffer Object for the user to Render to */
FrameBufferObject::FrameBufferObject(int _width, int _height, GLenum _datatype, GLenum _internal, GLenum _format)
	:
    GL_Handle(OpenGL::new_FBO())
{

	TODO("Need to create : \n\
		[x]Color : the outputs written with the output variables from the fragment shader \n\
		[x]Depth : this works as the Z buffer for the framebuffer object \n\
		[ ]Stencil : the stencil buffer");


    glBindFramebuffer(GL_FRAMEBUFFER, GL_Handle);
    {
        RenderTarget = new Texture({ _width, _height }, GL_RGBA);
        DepthTarget  = new Texture({ _width, _height }, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24);

        OpenGL::attach_ColorBuffer(RenderTarget->g_Handle());
        OpenGL::attach_DepthBuffer(DepthTarget->g_Handle());

        ValidateFrameBuffer();
        OpenGL::enable_DepthTest();
        OpenGL::set_DepthFunction(GL_LEQUAL);
        OpenGL::set_Viewport(0, 0, _width, _height);
    }
    Unbind();

    DEBUG_CODE(CheckGLERROR());
}




/*
NOTE: Should just perhaps have a resize for the Texture which will Resize the Bitmap and work our way through the abstraction
*/
void FrameBufferObject::Resize(Vec2 _size)
{
    if (GL_Handle)
    {
        OpenGL::delete_FBO(GL_Handle);
    }

    GL_Handle = OpenGL::new_FBO();
    if (RenderTarget != nullptr) 
    {
        delete(RenderTarget);
    }
    RenderTarget = new Texture(_size, GL_RGBA);


    if (DepthTarget != nullptr)
    {
        delete(DepthTarget);
    }
    DepthTarget = new Texture(_size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24);

    OpenGL::attach_ColorBuffer(RenderTarget->g_Handle());
    OpenGL::attach_DepthBuffer(DepthTarget->g_Handle());
}

void FrameBufferObject::Destroy()
{// Destroys the Frame Buffer Object releasing its ID to OpenGL 
    DEBUG_CODE(DEBUGPrint(CON_Red, "Deleting FrameBuffer: " << GL_Handle));
	OpenGL::delete_FBO(GL_Handle);
    delete(RenderTarget); 
    delete(DepthTarget); 
}

void FrameBufferObject::Bind()
{
	OpenGL::bind_FBO(GL_Handle);
    OpenGL::set_Viewport(0, 0, Width(), Height());
}
void FrameBufferObject::BindWrite()
{// Bind for Reading 
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_Handle);
}
void FrameBufferObject::BindRead()
{// Bind for Writing 
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_Handle);
}

void FrameBufferObject::Clear()
{// Clears Color and Depth Buffer 
    Bind();
    OpenGL::clear_FrameBuffer();
}
void FrameBufferObject::ClearColorBuffer()
{//Clears Color Buffers 
    Bind();
    OpenGL::clear_ColorBuffer();
}
void FrameBufferObject::ClearDepthBuffer()
{// Clears Depth Buffer 
    Bind();
    OpenGL::clear_DepthBuffer();
}
void FrameBufferObject::Unbind()
{// Disables FrameBuffer and uses Default FrameBuffer
    OpenGL::unbind_FBO();
}



bool FrameBufferObject::ValidateFrameBuffer()
{// Checks the status of the FrameBuffer to ensure it is complete
	std::string
        ErrorString{ "" },
        PossibleSolution{ "" };

    int error_number = OpenGL::check_FBO_Status();
	switch (error_number)
	{
	case GL_FRAMEBUFFER_COMPLETE:

		ErrorString = "No Error, FrameBuffer Healthy";
		PossibleSolution = "You did something right for a change, High five yourself!";
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:

		ErrorString = "Incomplete FrameBuffer";
		PossibleSolution = "Check FrameBuffer Textures and Attachment Points";
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:

		ErrorString = "Incomplete FrameBuffer Dimensions";
		PossibleSolution = "Not all attached Images have the same width and height. Check all Image sizes";
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:

		ErrorString = "FrameBuffer Missing Attachment";
		PossibleSolution = "Attach an Image to the FrameBuffer";
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED:
		ErrorString = "FrameBuffer Unsupported";
		PossibleSolution = "Try a Different format, or combination of, for the Attached Textures";
		break;
	}

    isValid = error_number == GL_FRAMEBUFFER_COMPLETE;

    if (!isValid)
    {
        Print("ERROR: " << ErrorString.c_str());
        Print("Possible Fix: " << PossibleSolution.c_str());
    }

    return isValid;
}





/// ONLY ACTIVE OPENGL > 4.0
//glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, _width);
//glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, _height);
//glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);

//
//_static Shader* FrameBufferObject::ScreenShader{ nullptr };
//_static uint32_t FrameBufferObject::ScreenVAO{ 0 };
//_static uint32_t FrameBufferObject::ScreenVBO{ 0 };
//_static uint32_t FrameBufferObject::ScreenIBO{ 0 };
//_static Vec2 FrameBufferObject::ScreenQuad[6] =
//{
//    Vec2(-1.0f,-1.0f),  Vec2(1.0f,-1.0f),  Vec2(-1.0f, 1.0f),
//    Vec2(1.0f, 1.0f),  Vec2(-1.0f, 1.0f),  Vec2(1.0f,-1.0f)
//};
//_static uint32_t FrameBufferObject::Indices[6] =
//{
//    0,1,3, 1,2,0
//};
//_static std::string
//FrameBufferObject::Vrenderer = "#version 330 core  \n\
//layout(location = 0) in vec2 aPos;                 \n\
//out vec2 TexCoords;                                \n\
//void main()                                        \n\
//{                                                  \n\
//    TexCoords = (aPos.xy + 1.0f) * 0.5f;           \n\
//    gl_Position = vec4( aPos.x , aPos.y  , -1.0,  1.0); \n\
//}";
//_static std::string
//FrameBufferObject::Frenderer = "#version 330 core \n\
//uniform sampler2D FrameBufferTexture;             \n\
//in  vec2 TexCoords;                               \n\
//out vec4 FragColor;                               \n\
//void main()                                       \n\
//{                                                 \n\
//    vec4 col1 = vec4(texture(FrameBufferTexture,TexCoords.xy+1).xyz, 1.0); \n\
//    FragColor = vec4(texture(FrameBufferTexture,TexCoords.xy).xyz, 1.0);  \n\
//}";
/////* Returns the Width of our Frame Buffer Object 
//uint32_t Width()  { return static_cast<uint32_t>(Size.x); }
///* Returns the Height of our Frame Buffer Object 
//uint32_t Height() { return static_cast<uint32_t>(Size.y); }
    /* Render to the Default FBO, Will likely change name of this to SwapBuffer */
//void Render();
/* Render to the Default FBO, Will likely change name of this to SwapBuffer */
//void FrameBufferObject::Render()
//{
//    REFACTOR("This should likely be removed and instead move to Renderer Class Render_FBO or something like that.");
//    Unbind();
//    ScreenShader->Bind();
//    {
//        OpenGL::bind_VAO(ScreenVAO);   
//        ScreenShader->SetTextureUniform("FrameBufferTexture", RenderTarget->g_Handle(), 2);
//        OpenGL::Renderer::drawArray(6);
//    }
//    ScreenShader->Unbind();
//}
//
//static Shader*  ScreenShader;
//static uint32_t ScreenVAO;
//static uint32_t ScreenVBO;
//static uint32_t ScreenIBO;
//
//static std::string Frenderer;
//static std::string Vrenderer;
//static Vec2  ScreenQuad[6];
//static uint32_t Indices[6];
////
//static bool _Initialized__ = false;
//if (_Initialized__ == false)
//{// Initialize the default Shader for FBOs the first time we create a FrameBufferObject
//    REFACTOR("This shit is Deprecated. There is an imageRender(x,y,w,h) which can be used for the FBO rendering");
//    ScreenShader = new Shader(Vrenderer, Frenderer);
//    ScreenVAO = OpenGL::new_VAO();
//    ScreenVBO = OpenGL::new_VBO();
//    ScreenIBO = OpenGL::new_IBO();
//
//    OpenGL::bind_VAO(ScreenVAO);
//    OpenGL::bind_VBO(ScreenVBO);
//    ScreenShader->SetAttribute(2, "aPos");
//    OpenGL::set_BufferData(sizeof(ScreenQuad), ScreenQuad);
//
//    //OpenGL::bind_IBO(ScreenIBO);
//    //OpenGL::set_BufferData(sizeof(Indices), Indices);
//
//    _Initialized__ = true;
//}
//
//


//glGenTextures(1, &TextureID);
//glBindTexture(GL_TEXTURE_2D, TextureID);
////glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);

// glGenTextures(1, &DepthTexture);
// glBindTexture(GL_TEXTURE_2D, DepthTexture);
////glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture, 0);

