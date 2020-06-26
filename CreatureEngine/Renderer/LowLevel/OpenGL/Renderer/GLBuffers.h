#pragma once

#include<vector>
/*
RenderTargets<FrameBuffers>
Pass<Depth Blending etc.>
Material<Shaders>
MaterialInstance<Textures>
Vertex Format<VAO/VBO>
Object<>
{
Uniforms
glDrawElementsBaseVertex()
}
*/

#include"Core/Common.h"
#include"../OpenGL.h"
#include"../Shader/Shader.h"

#define BUFFER_OFFSET(i)   ((char *)NULL + (i))

#define DEFAULT_BUFFER_ACCESS   GL_DYNAMIC_DRAW // For later implementation


enum BufferTypes
{
	VERTEX, COLOR, NORMAL, UVCOORD, TEXTURE, INDICE, TANGENT, BITANGENT, NONE
};

enum ShaderType
{/// COMPLETELY TEMPORARY
	TESSLATION_EVALUATION_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER,
	TESSLATION_CONTROL_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER,
	GEOMETRY_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER,
	FRAGMENT_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER,
	COMPUTE_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER,
	VERTEX_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER
};

 
class Attribute
{
public:
	NO_COPY_OR_ASSIGNMENT(Attribute);

	Attribute();
	Attribute(BufferTypes t);

	void Bind();
	void Unbind();

	void* Map(GLenum accessflags);
	void* MapRange(int offset, int count, GLenum accessflags);

	size_t MappedSize();
	bool isMapped();
	void Release();
	void Destroy();

	GLuint GL_Handle{ 0 };
	BufferTypes AttributeType{NONE};
	GLint Location{ 0 };
	GLuint ElementCount{ 0 };

	GLuint64EXT Address{ 0 };
	GLuint      Size{ 0 };
	GLuint      Stride{ 0 };
	void* BufferPtr{ nullptr };
};

/*=========================================================================================================================================================
           OpenGL Buffer Object which holds raw Buffer data which can be given to the VAO as a specific Attribute type by assigning it that usecase.
 ========================================================================================================================================================= */
template<class T = Vec3>
class  VertexBufferObject
	:	public Attribute
{ 
public:

	NO_COPY_OR_ASSIGNMENT(VertexBufferObject);

	using value_type = T;
	using pointer_type = T*;

	VertexBufferObject() 
    {
        DEBUG_CODE(CheckGLERROR());
        ElementCount = 0;
        Data.resize(ElementCount);

        Stride = sizeof(value_type);

        GL_Handle = OpenGL::new_VBO();
        Bind();
        OpenGL::set_BufferData(ElementCount*Stride, NULL);

        /// ------------------------Bindless Address stuff---------------------------------
        /// glGetBufferParameterui64vNV(GL_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &Address);
        /// glMakeBufferResidentNV(GL_ARRAY_BUFFER, GL_READ_ONLY);
        Size = ElementCount * Stride;
        ///--------------------------------------------------------------------------------
        Unbind();
        //BufferPtr = &Data[0];
    }

    /* Create a buffer object that stores data for OpenGL */
	VertexBufferObject(pointer_type data, GLsizei count)
	{
		ElementCount = count;
		Data.resize(ElementCount);
		Data.insert(Data.begin(), ElementCount, *data);

		Stride = sizeof(value_type);

        GL_Handle = OpenGL::new_VBO();
        Bind();
        OpenGL::set_BufferData(ElementCount*Stride, data);

		/// ------------------------Bindless Address stuff---------------------------------
 
        Address = OpenGL::get_Bindless_Address();
        OpenGL::make_Buffer_Resident();
 
		Size = ElementCount * Stride;
		///--------------------------------------------------------------------------------

        Unbind();
		BufferPtr = &Data[0];
	}

    /* Create a buffer object that stores data for OpenGL while allowing change of Default Access */
	VertexBufferObject(GLenum _access, pointer_type _data, GLsizei _count)
		: /// Specify default access
		ElementCount(_count),
		GL_Handle(0)
	{
		Data.resize(_count);
		Data.insert(Data.begin(), _count, *_data);
		Stride = sizeof(T);
        
        GL_Handle = OpenGL::new_VBO();
        Bind();
        OpenGL::set_BufferData(ElementCount*Stride, _data, _access);

        Unbind();
	}
	
    /* Create a VBO from an std::vector of Data */
    VertexBufferObject(std::vector<value_type> _vec)
	{
        __debugbreak(); /// MAKE SURE THIS IS WORKING CORRECTLY
		*this = &VertexBufferObject<value_type>(&_vec[0], _vec.size());
	}
	void Update(std::vector<value_type>& _data)
	{
        REFACTOR("DATA: Is simply debug shit, clean this up in the future. && This needs to have the Access changed from default Access and to the Access of this Buffer however I see no way to change the Acces type");
        Data = _data;
		Bind();
        {
            OpenGL::set_BufferData(_data);
        }
        Unbind();

	}
    void Update(void* _data, size_t _sz)
    {
        __debugbreak();// This is currently working wrong if the buffer was not previously created so come back to this later
        Bind();
        {
            OpenGL::set_Subbuffer_Data(_sz * sizeof(value_type), _data, 0);
        }
        Unbind();

        //glBufferSubData(GL_ARRAY_BUFFER, 0, _sz * sizeof(value_type), _data);
    }

	GLuint get_BufferSize()
	{
		GLint results;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &results);
		return results;
	}

	size_t size() const { return Data.size(); }

    pointer_type data_ptr()
    {
        return Data.data();
    }

    VertexBufferObject(VertexBufferObject&& _other)
    {
        __debugbreak(); /// NEED TO MAKE SURE THIS IS WORKING PROPERLY
        *this = std::move(_other);
    }
    VertexBufferObject& operator = (VertexBufferObject&& _other)
    {
        if (this != &_other)
        {
            Data = (std::move(_other.Data));
            ElementCount = _other.ElementCount;
            Stride = _other.Stride;
            BufferPtr = _other.BufferPtr;
            Size = _other.Size;

            _other.Data = std::vector< value_type>();
            _other.ElementCount = 0;
            _other.Stride = 0;
            _other.BufferPtr = 0;
            _other.Size = 0;
        }
        return *this;
    }
	VertexBufferObject& operator = (std::vector<T>& data)
	{// Map the whole buffer, resize if needed and make the data of the buffer equal to that of the Rvalue
        Data = data;
		Update(data);
        return *this;
	}

    VertexBufferObject operator += (VertexBufferObject& other)
	{ // Map the buffer and add to the end of it, updating the data, and size while retaining access type and GL_Handle
		Bind();
        OpenGL::set_BufferData((size() * sizeof(value_type)) + (other.size() * sizeof(other.value_type)), NULL);
		std::move(other.Data.begin(), other.Data.end(), std::back_inserter(Data));
        OpenGL::set_Subbuffer_Data(size() * sizeof(value_type), Data, 0);
 	}

protected:
	std::vector<value_type> Data;
};


class ElementArrayBuffer
{
public:
	NO_COPY_OR_ASSIGNMENT(ElementArrayBuffer);

	ElementArrayBuffer() = default;
	ElementArrayBuffer(GLuint* _data, size_t _count);
	ElementArrayBuffer(GLenum _access, GLuint* _data, size_t _count);

	void Bind();
	void Unbind();

	GLuint* Data;// Later make private and Accessors
	GLuint GL_Handle;
	size_t ElementCount;
};


class VertexArrayObject
{ // OpenGL VAO container which holds the state information for the Object contained on the GPU

public:
	NO_COPY_OR_ASSIGNMENT(VertexArrayObject);

	VertexArrayObject();
    template<typename _Ty>
	VertexArrayObject(VertexBufferObject<_Ty>& vbo);

	GLuint GL_Handle;
	int ElementCount;

	std::vector<Attribute*> Buffers;

	void Bind();
	void Unbind();

	template<typename T>
	void Attach(BufferTypes bufferT, VertexBufferObject<T>* buffer)
	{
		/// I HAD THIS BECAUSE COLOR WAS CAUSING ISSUES LIKELY BECAUSE ITS A VEC 3 and Not a Vec4		if (bufferT == COLOR) return;
		if (GL_Handle == NULL)
		{
            GL_Handle = OpenGL::new_VAO();
		}

		Bind(); // First Bind the buffer to make it Current in the OpenGL Context

		GLint Amount = sizeof(T) / sizeof(float);

		Buffers.push_back(buffer);

		switch (bufferT)
		{/// Case statement of Buffer types. Indices pushback then exit the function so that EnableVertexAttribArray and glVertexAttribPointer are never called.
        case BufferTypes::INDICE:
        {
            ElementCount = buffer->ElementCount;
            buffer->AttributeType = INDICE;
            OpenGL::bind_IBO(buffer->GL_Handle);

            return; //  If VBO is Indices then bind it to the VAO and return before attribute is set.
        }
        break;


		case BufferTypes::VERTEX:
		{
			buffer->Location = Shader::get().AttributeLocation("VertexPosition");
			if (buffer->Location == -1) return;
			buffer->AttributeType = VERTEX;
		}
	    break;

		case BufferTypes::COLOR:
		{
			buffer->Location = Shader::get().AttributeLocation("VertexColor");
			if (buffer->Location == -1) return;
			buffer->AttributeType = COLOR;
		}
		break;

		case BufferTypes::NORMAL:
		{
			buffer->Location = Shader::get().AttributeLocation("VertexNormal");
			if (buffer->Location == -1) return;
			buffer->AttributeType = NORMAL;
		}
		break;

		case BufferTypes::UVCOORD:
		{
			buffer->Location = Shader::get().AttributeLocation("TextureCoord");
			if (buffer->Location == -1) return;
			buffer->AttributeType = UVCOORD;
		}
		break;

		case BufferTypes::TANGENT:
		{
			buffer->Location = Shader::get().AttributeLocation("VertexTangent");
			if (buffer->Location == -1) return;
			buffer->AttributeType = TANGENT;

		}
		break;

		case BufferTypes::BITANGENT:
		{
			buffer->Location = Shader::get().AttributeLocation("VertexBitangent");
			if (buffer->Location == -1) return;
			buffer->AttributeType = BITANGENT;
		}
		break;

		}
        OpenGL::bind_VBO(buffer->GL_Handle);
		_GL(glEnableVertexAttribArray(buffer->Location));
		_GL(glVertexAttribPointer(buffer->Location, Amount, GL_FLOAT, GL_FALSE, 0, (char*)NULL));
	}

	void Render();
};


//uint32_t  set_Attribute(uint32_t _shaderID, uint8_t _elements, const char* _name)
//{
//    uint32_t Location = glGetAttribLocation(_shaderID, _name);
//    glEnableVertexAttribArray(Location);
//    glVertexAttribPointer(Location, _elements, GL_FLOAT, GL_FALSE, 0, (char*)NULL);
//    CheckGLERROR();
//    return Location;
//}
//uint32_t set_Attribute(uint8_t _elements, const char* _name)
//{
//    int H = Shader::getHandle();
//    int32_t Location = glGetAttribLocation(H, _name);
//
//    DEBUG_CODE
//    (
//        if (Location == -1)
//        {
//            Print("ERROR: Attribute does not exist");
//            __debugbreak();
//        }
//    );
//    glEnableVertexAttribArray(Location);
//    glVertexAttribPointer(Location, _elements, GL_FLOAT, GL_FALSE, 0, (char*)NULL);
//    CheckGLERROR();
//    return Location;
//}

















#include"../../Materials/Image/Texture.h"

enum Attachment_t
{
	Color = GL_COLOR_ATTACHMENT0,
	Depth = GL_DEPTH_ATTACHMENT,
	Stencil = GL_STENCIL_ATTACHMENT,
	DepthStencil = GL_DEPTH_ATTACHMENT | GL_STENCIL_ATTACHMENT //<- WARNING! Dunno if this is correct or not... GL_DEPTH_STENCIL_ATTACHMMENT
};


class RenderBufferObject
{
	/* DEFAULT STATE:
	If renderbuffer is 0, the current image, if any,
	attached to the attachment logical buffer of the currently bound framebuffer object is detached. 
	The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is set to GL_NONE. 
	The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME is set to 0.
	GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL and GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE are set to the default values 0 
	GL_TEXTURE_CUBE_MAP_POSITIVE_X, respectively. */
public:
	RenderBufferObject(iVec2 _size, uint32_t _internalFormat = GL_RGBA, uint32_t _samples = 0)
		:
		Size(_size),
		SampleCount(_samples),
		InternalFormat(_internalFormat)
	{
		glGenRenderbuffers(1, &GL_Handle);
		glBindRenderbuffer(GL_RENDERBUFFER, GL_Handle);

		if(Samples() == 0)
		{// If no multisampled
			glRenderbufferStorage(GL_RENDERBUFFER, InternalFormat, Width(),Height());
		}
		else
		{ // We have Enabled Multisampling
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, Samples(), InternalFormat, Width(), Height());
		}
	}

	void AttachtoFrameBuffer(uint32_t _fbo, Attachment_t _point = Color)
	{
		AttachmentPoint = _point;
		glBindRenderbuffer(GL_RENDERBUFFER, GL_Handle);

		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, AttachmentPoint, GL_RENDERBUFFER, GL_Handle);
	}
	/// Can i change the Read or Write functionality here?

	Attachment_t AttachmentPoint{ Color };
	uint32_t GL_Handle{ 0 };
	//TODO: Create a Read function here to return the contents of the Render Buffer
	// void *Read(){return Data;}
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Width(), Height());
		//glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GL_Handle);


	/* Returns the Width of the Render Buffer */
	const GLsizei Width() const  { return static_cast<GLsizei>(Size.x); }
	/* Returns the Height of the Render Buffer */
	const GLsizei Height()const  { return static_cast<GLsizei>(Size.y); }
	/* Number of Samples taken */
	const GLsizei Samples()const { return static_cast<GLsizei>(SampleCount); }
	/* Checks to see if our Render Buffer is Multisamples*/
	const bool isMultisampled() { return (Samples() != 0); }
private:
	uint32_t InternalFormat{ GL_RGBA };
	iVec2 Size{ 0,0 };
	size_t SampleCount{ 0 };
};


/* Sort or Bucket Visible Objects
foreach( render target )    // framebuffer
foreach( pass )             // depth, blending, etc... states
foreach( material )         // shaders
foreach( material instance) // textures, normals, diffuse,
foreach( vertex format )    // vertex buffers
foreach( object )           //
{
 WriteUniformData( object );
 glDrawElementBaseVertex(
     GL_TRIANGLES,
     object->indexCount,
     GL_UNSIGNED_SHORT,
     object->indexDataOffset,
     object->baseVertex );
}*/
class FrameBufferObject
{
public:
	FrameBufferObject() = default;

    /* Creates a Frame Buffer Object for the user to Render to */
	FrameBufferObject(int _width, int _height, GLenum _datatype = GL_FLOAT, GLenum _internal = GL_RGBA32F, GLenum _format = GL_RGBA);

	GLuint GL_Handle;

    Graphics::Texture *RenderTarget;
    Graphics::Texture *DepthTarget;

	Vec2 Size;


    /* Returns the Width of our Frame Buffer Object */
    uint32_t Width()  { return static_cast<uint32_t>(Size.x); }
    /* Returns the Height of our Frame Buffer Object */
    uint32_t Height() { return static_cast<uint32_t>(Size.y); }
   

    /* Binds the Frame Buffer Object to OpenGL*/
    void Bind();
	/* Bind for Reading */
	void BindWrite();
	/* Bind for Writing */
	void BindRead();
    /* Unbinds the Frame Buffer Object and falls back to OpenGL default Frame Buffer*/
    void Unbind();
    /* Clears the Color and Depth Channels for the Frame Buffer Object */
	void Clear();
	/* Clears Color Buffers */
	void ClearColorBuffer();
	/* Clears Depth Buffer */
	void ClearDepthBuffer();

	/* Destroys the Frame Buffer Object releasing its ID to OpenGL */
	void Destroy();

    /* Test the Frame Buffer to see if it is complete.
       Returns: True if complete.
       Returns: False if incomplete and displays a message to the console explaining why FrameBuffer creation failed */
	bool ValidateFrameBuffer();    

    /* Render to the Default FBO, Will likely change name of this to SwapBuffer */
    void Render();

    static Shader* ScreenShader;
    static uint32_t ScreenVAO;
    static uint32_t ScreenVBO;
    static uint32_t ScreenIBO;

    static std::string Frenderer;
	static std::string Vrenderer;
	static Vec2 ScreenQuad[6];
    static uint32_t Indices[6];
	/* Checks an ID to see if it is a Frame Buffer Object already */
	static bool isFBO(uint32_t _fbo)
	{
		bool results{ false };
		return static_cast<bool>(glIsFramebuffer((GLuint)_fbo));
	}


	enum Target_t
	{
	    Write = GL_DRAW_FRAMEBUFFER, 
	    Read  = GL_READ_FRAMEBUFFER,
	    Both  = GL_FRAMEBUFFER
	};
};


template<typename T>
struct UniformBufferObject
{
	UniformBufferObject() = default;
	UniformBufferObject(Shader* _program, const char* _name, unsigned int _slot)
		:
		BlockSize(sizeof(T)),
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
		//* Same as above glBindBufferRange(GL_UNIFORM_BUFFER, 0, GL_Handle, 0, BlockSize);
		SetSubData(0, BlockSize, NULL);
		Unbind();
	}

	void Create()
	{
		glCreateBuffers(1, &GL_Handle);
	}
	void BindBase(GLuint _slot)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, _slot, GL_Handle);
	}
	void SetBinding(GLuint _slot)
	{
		glUniformBlockBinding(Program, Index, _slot);
	}
	void Update()
	{
		Bind();
		SetSubData(0, BlockSize, &Data);
		Unbind();
	}

	void SetData(T* _data)
	{
		glBufferData(GL_UNIFORM_BUFFER, BlockSize, _data, GL_DYNAMIC_DRAW);
	}
	void SetSubData(size_t _offset, size_t _range, void* _data)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, _range, _data);
	}

	void Bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, GL_Handle);
	}
	void Unbind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void LockBuffer()
	{
		if (gSync)
		{
			glDeleteSync(gSync);
		}
		gSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}
	void WaitBuffer()
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

	int fetchIndex(const char* _name)
	{
		return glGetUniformBlockIndex(Program, _name);
	}
	inline GLuint g_Index()
	{
		return Index;
	}

	T Data;
private:
	// http://docs.gl/gl4/glBufferSubData
	unsigned int BlockSize;

	const char* Name;

	GLsync gSync;
	GLuint Slot;
	GLuint Index;
	GLuint GL_Handle;
	GLuint Program;
};

template<typename T>
struct PersistantUniformBufferObject
{
	PersistantUniformBufferObject() = default;
	PersistantUniformBufferObject(Shader* _program, const char* _name, unsigned int _bindLOC)
		:
		BlockSize(sizeof(T)),
		Name(_name),
		SyncObject(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0)),
		Slot(_bindLOC),
		Index(NULL),
		GL_Handle(NULL),
		Pointer(NULL),
		Program(_program->g_Handle())
	{
		Index = fetchIndex(Name);
		SetBinding(Slot);

		Create();
		Bind();
		BindBase(Slot);

		unsigned int flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		glBufferStorage(GL_UNIFORM_BUFFER, BlockSize, 0, flags);
		Pointer = Map(flags);
	}
	void Update()
	{// Ensures previous write has completed and copies the contents of Data over to Pointer
		WaitBuffer();
		SetData(&Data);
		LockBuffer();
	}

	T* Map(GLuint _flags)
	{// Since it is persistant this only needs to be called once and this abstraction is likely uneeded later on.
		return (T*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, BlockSize, _flags);
	}
	void Unmap()
	{
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
	void Create()
	{
		glCreateBuffers(1, &GL_Handle);
	}
	void BindBase(GLuint _slot)
	{

		glBindBufferBase(GL_UNIFORM_BUFFER, _slot, GL_Handle);
		//	glBindBufferRange(GL_UNIFORM_BUFFER, 0, GL_Handle, 0, BlockSize);
	}
	void SetBinding(GLuint _slot)
	{// Binds the Buffer to a Specified Uniform slot on the GPU.
		glUniformBlockBinding(Program, Index, _slot);

	}

	void SetData(T* _data)
	{// When class is finished likely this will just be replaced by proper syncing and direct access to memory block elements via T
		memcpy(Pointer, _data, BlockSize);
	}
	void SetSubData(size_t _offset, size_t _range, void* _data)
	{// Fills in specified range of data. ( likely not needed in future use as Pointer and Data may become a single entity and Access will be performed directly on respective elements)
		memcpy(Pointer + _offset, _data, _range);
	}

	void Bind()
	{// Need it at Initialization and potentially if the buffer is ever needed for other use
		glBindBuffer(GL_UNIFORM_BUFFER, GL_Handle);
	}
	void Unbind()
	{// Both likely uneeded due to persistancy of buffer
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void LockBuffer()
	{// Creates a Fence object 
		if (SyncObject)
		{// Destroys Sync Objects if it already exist
			glDeleteSync(SyncObject);
		}
		SyncObject = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}
	void WaitBuffer()
	{// Syncs operations to ensure Buffer write is finished before proceeding 
		if (SyncObject)
		{
			while (1)
			{
				GLenum waitReturn = glClientWaitSync(SyncObject, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
				if (waitReturn == GL_ALREADY_SIGNALED || waitReturn == GL_CONDITION_SATISFIED)
					return;
			}
		}
	}


	//TODO: Allow Direct Acces to pointer when working correctly
	T Data;
	T* Pointer;

	// OpenGL API for Uniform Buffer information

	GLint isReferenced(ShaderType _shadertype)
	{
		GLint results = 0;
		glGetActiveUniformBlockiv(Program, Index, _shadertype, &results);
		return results;
	}
	int fetchIndex(const char* _name)
	{// Return Index of the uniform block with specific name
		return glGetUniformBlockIndex(Program, _name);
	}
	int fetchActiveBlockBinding() {
		int results = 0;
		glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_BINDING, &results);
		return results;

	}
	int fetchBlockSize()
	{
		int results = 0;
		glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_DATA_SIZE, &results);
		return results;
	}
	int fetchNameLength()
	{
		int results = 0;
		glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_NAME_LENGTH, &results);
		return results;
	}
	int fetchActiveUniforms()
	{
		int results = 0;
		glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &results);
		return results;
	}
	int fetchActiveUniformIndices()
	{
		int results = 0;
		glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &results);
		return results;
	}

private:

	unsigned int BlockSize;

	const char* Name;

	GLsync SyncObject;
	GLuint Slot;
	GLuint Index;
	GLuint GL_Handle;
	GLuint Program;
};














#define SetMember(name) offsetof(T, name)




/// Not currently in use yet however Intentions are to Stack the entire State of OpenGL as much as possible for Efficient Scene Transversal
struct OpenGLState
{
	OpenGLState()
		:
		BlendingEnabled(true),
		CullFaceEnabled(true)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		BlendingOperation.push({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
		CullFace.push(GL_BACK);
	}

	//______________ BLENDING ________________________
	bool BlendingEnabled;
	struct blending
	{
		GLenum
			Sfactor = GL_SRC_ALPHA,
			Dfactor = GL_ONE_MINUS_SRC_ALPHA;
	};


	std::stack<blending> BlendingOperation;
	void SetBlendingState(GLenum _sfactor, GLenum _dfactor)
	{//void glBlendFunc(GLenum sfactor, GLenum dfactor);
		BlendingOperation.push({ _sfactor ,_dfactor });
		glBlendFunc(_sfactor, _dfactor);
	}
	//_________________________________________________

	//void glCullFace(GLenum mode);
	//_____________ CULL FACES ________________________

	bool CullFaceEnabled = true;
	std::stack<GLenum> CullFace;

	void SetCullFaceMode(GLenum _mode)
	{
		glCullFace(_mode);
	}
	//_________________________________________________


};






/*  #FRAMEBUFFEROBJECT   2-26-2020


    enum AttachmentPoint_t
    {
        COLOR = GL_COLOR_ATTACHMENT0, // These are an implementation - dependent number of attachment points.You can query GL_MAX_COLOR_ATTACHMENTS to determine the number of color attachments that an implementation will allow.The minimum value for this is 8, so you are guaranteed to be able to have at least color attachments 0 - 7. These attachment points can only have Images bound to them with color - renderable formats.All compressed Image formats are not color - renderable, and thus cannot be attached to an FBO.
        COLOR1 = GL_COLOR_ATTACHMENT1,
        COLOR2 = GL_COLOR_ATTACHMENT2,
        COLOR3 = GL_COLOR_ATTACHMENT3,
        COLOR4 = GL_COLOR_ATTACHMENT4,
        COLOR5 = GL_COLOR_ATTACHMENT5,
        COLOR6 = GL_COLOR_ATTACHMENT6,
        COLOR7 = GL_COLOR_ATTACHMENT7,
        COLOR10 = GL_COLOR_ATTACHMENT10,
        COLOR11 = GL_COLOR_ATTACHMENT11,
        COLOR12 = GL_COLOR_ATTACHMENT12,
        COLOR13 = GL_COLOR_ATTACHMENT13,
        COLOR14 = GL_COLOR_ATTACHMENT14,
        COLOR15 = GL_COLOR_ATTACHMENT15,
        COLOR16 = GL_COLOR_ATTACHMENT16,
        COLOR17 = GL_COLOR_ATTACHMENT17,
        COLOR18 = GL_COLOR_ATTACHMENT18,
        COLOR19 = GL_COLOR_ATTACHMENT19,
        COLOR20 = GL_COLOR_ATTACHMENT20,
        COLOR21 = GL_COLOR_ATTACHMENT21,
        COLOR22 = GL_COLOR_ATTACHMENT22,
        COLOR23 = GL_COLOR_ATTACHMENT23,
        COLOR24 = GL_COLOR_ATTACHMENT24,
        COLOR25 = GL_COLOR_ATTACHMENT25,
        COLOR26 = GL_COLOR_ATTACHMENT26,
        COLOR27 = GL_COLOR_ATTACHMENT27,
        COLOR28 = GL_COLOR_ATTACHMENT28,
        COLOR29 = GL_COLOR_ATTACHMENT29,
        COLOR30 = GL_COLOR_ATTACHMENT30,
        COLOR31 = GL_COLOR_ATTACHMENT31,
        DEPTH = GL_DEPTH_ATTACHMENT,//GL_DEPTH_ATTACHMENT : This attachment point can only have Images with depth formats bound to it.The Image attached becomes the Depth Buffer for the FBO. **NOTE** Even if you don't plan on reading from this depth_attachment, an off screen buffer that will be rendered to should have a depth attachment.
        STENCIL = GL_STENCIL_ATTACHMENT,// This attachment point can only have Images with stencil formats bound to it.The Image attached becomes the stencil buffer for the FBO.
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT// This is shorthand for "both depth and stencil".The Image attached becomes both the depth and stencil buffers. Note : If you use GL_DEPTH_STENCIL_ATTACHMENT, you should use a packed depth - stencil internal format for the texture or renderbuffer you are attaching.
    };
    AttachmentPoint_t Attachments[10];
*/



/*
Striped down version https://pastebin.com/Kr8U7mGu
Persistantly Mapped Uniform Buffer with Coherent memory access making writes immediately availible to the GPU
Usage:
PersistantUniformBufferObject<STRUCT> UBOobject(Pointer to Shader, Uniforms Block Name, Uniform Binding Location);
 Members of Data can be access and Updated at will however currently visibility will not take place until void Update() is called
 NOTE: 7/5/19
 Current Implementation is incorrect as the second Buffer I create comes through Corrupted. The first I pass a Matrices Struct containing Projection and View Matrices,
 the Second I pass lighting information such as Position and Color. All Images on Screen turn bright green as the Light information is corrupted completely and has
 Random values.
*/


//glBufferStorage(GL_UNIFORM_BUFFER, BlockSize, 0, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
//BufferHandle = (GPUptr*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, BlockSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

//  1280 GL_INVALID_ENUM
//  1281 GL_INVALID_VALUE
//  1282 GL_INVALID_OPERATION
//  1283 GL_STACK_OVERFLOW
//  1284 GL_STACK_UNDERFLOW
//  1285 GL_OUT_OF_MEMORY
//  
//  //  switch (bufferT)
//  {/// Case statement of Buffer types. Indices pushback then exit the function so that EnableVertexAttribArray and glVertexAttribPointer are never called.
//  
//  case BufferTypes::VERTEX:
//  	Amount = sizeof(T) / sizeof(float);
//  	buffer->AttributeType = VERTEX;
//  	glBindBuffer(GL_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	break;
//  
//  case BufferTypes::COLOR:
//  	Amount = sizeof(T) / sizeof(float);
//  	buffer->AttributeType = COLOR;
//  	glBindBuffer(GL_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	break;
//  
//  case BufferTypes::NORMAL:
//  	Amount = sizeof(T) / sizeof(float);
//  	buffer->AttributeType = NORMAL;
//  	glBindBuffer(GL_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	break;
//  
//  case BufferTypes::UVCOORD:
//  	Amount = sizeof(T) / sizeof(float);
//  	buffer->AttributeType = UVCOORD;
//  	glBindBuffer(GL_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	break;
//  
//  case BufferTypes::INDICE:
//  	ElementCount = buffer->ElementCount;
//  	buffer->AttributeType = INDICE;
//  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	return; //  Exit since the Element Buffer is not an Attribute and continuing through in the function would result in attempting to bind the element buffer to an attribute slot
//  }



/// RANDOM CODE FROM OLDER PROJECT THAT I MIGHT USE OR AT LEAST WANT TO KEEP IN ORDER TO HAVE IDEAS
//  GameObject::GameObject()
//  {
//  	Set_Position(Vec3(0.0f));
//  	Set_Rotation(Vec3(0.0f));
//  	Set_Scale(Vec3(1.0f));
//  }
//  
//  
//  void GameObject::Set_Transform(Vec3 pos, Vec3 rot, Vec3 scale)
//  {
//  	Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
//  	Transform = glm::translate(Transform, pos);
//  	Transform = glm::rotate(Transform, glm::radians(rot.x), Vec3(1.0f, 0.0f, 0.0f));
//  	Transform = glm::rotate(Transform, glm::radians(rot.y), Vec3(0.0f, 1.0f, 0.0f));
//  	Transform = glm::rotate(Transform, glm::radians(rot.z), Vec3(0.0f, 0.0f, 1.0f));
//  	Transform = glm::scale(Transform, scale);
//  	// WARNING: NOT SURE SCALING IN THE PROPER ORDER
//  	Print("GameObject::Set_Transform() -  Unsure if Scaling is being applied properly. Double check.");
//  }