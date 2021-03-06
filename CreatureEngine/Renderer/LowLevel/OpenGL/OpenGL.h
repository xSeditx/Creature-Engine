#pragma once

#include"Core/Common.h"

#include<GL/gl.h>
#include<GL/glu.h>

#define _USING_OPENGL_API /* Are we using OpenGL or something else like DX*/
#pragma comment(lib, "OpenGL32.lib") 
#pragma comment(lib, "glu32.lib")

/* Subject to change default buffer access hint */
#define DEFAULT_BUFFER_ACCESS  GL_STATIC_DRAW


extern std::string OpenGL_ErrorList;




typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext,
    const int *attribList);
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList,
    const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
extern wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;
extern wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt for all values
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B




namespace OpenGL
{
	CREATURE_API enum Buffer_Access
	{/* Why are we missing E3-E7-EB? Is there Undocumented modes?*/
		StreamDraw = GL_STREAM_DRAW,   // 0
		StreamRead = GL_STREAM_READ,   // 1
		StreamCopy = GL_STREAM_COPY,   // 2	
		StaticDraw = GL_STATIC_DRAW,   // 4
		StaticRead = GL_STATIC_READ,   // 5
		StaticCopy = GL_STATIC_COPY,   // 6
		DynamicDraw = GL_DYNAMIC_DRAW, // 8
		DynamicRead = GL_DYNAMIC_READ, // 9
		DynamicCopy = GL_DYNAMIC_COPY  // A
	};

	CREATURE_API enum Pixel_flags
	{
		Null = 0,
		Stereo = PFD_STEREO,
		Generic = PFD_GENERIC_FORMAT,
		GDI_Support = PFD_SUPPORT_GDI,
		Double_Buffer = PFD_DOUBLEBUFFER,
		Draw_to_Window = PFD_DRAW_TO_WINDOW,
		Draw_to_Bitmap = PFD_DRAW_TO_BITMAP,
		OpenGL_Support = PFD_SUPPORT_OPENGL,
		Default_Stereo = PFD_STEREO_DONTCARE,
		Swap_Copy_Hint = PFD_SWAP_COPY,
		Require_Palette = PFD_NEED_PALETTE,
		Swap_Layer_Buffers = PFD_SWAP_LAYER_BUFFERS,
		Swap_Exchange_Hint = PFD_SWAP_EXCHANGE,
		Genaric_Accelerated = PFD_GENERIC_ACCELERATED,
		Default_Depth_Buffer = PFD_DEPTH_DONTCARE,
		Default_Double_Buffer = PFD_DOUBLEBUFFER_DONTCARE,
		Require_System_Palette = PFD_NEED_SYSTEM_PALETTE
	};
	CREATURE_API enum Hint_t
	{
		SmoothLines = GL_LINE_SMOOTH_HINT,
		SmoothPolygons = GL_POLYGON_SMOOTH_HINT,
		CompressTexture = GL_TEXTURE_COMPRESSION_HINT,
		FragmentDerivationHint = GL_FRAGMENT_SHADER_DERIVATIVE_HINT
	};
	CREATURE_API enum Mode_t
	{
		Nicest = GL_NICEST,
		Fastest = GL_FASTEST,
		Default = GL_DONT_CARE
	};



	struct PFD
	{
		uint32_t nSize;     // sizeof(PIXELFORMATDESCRIPTOR).
		uint32_t nVersion;  // Specifies the version of this data structure.
		uint32_t dwFlags;
		uint8_t  iPixelType;
		uint8_t  cColorBits;
		uint8_t  cRedBits;
		uint8_t  cRedShift;
		uint8_t  cGreenBits;
		uint8_t  cGreenShift;
		uint8_t  cBlueBits;
		uint8_t  cBlueShift;
		uint8_t  cAlphaBits;
		uint8_t  cAlphaShift;
		uint8_t  cAccumBits;
		uint8_t  cAccumRedBits;
		uint8_t  cAccumGreenBits;
		uint8_t  cAccumBlueBits;
		uint8_t  cAccumAlphaBits;
		uint8_t  cDepthBits;
		uint8_t  cStencilBits;
		uint8_t  cAuxBuffers;
		uint8_t  iLayerType;
		uint8_t  bReserved;
		uint32_t dwLayerMask;
		uint32_t dwVisibleMask;
		uint32_t dwDamageMask;
	};

    ///=============================================================    A    ==================================================================================
    CREATURE_API void Attribute_Pointer(uint32_t _location, uint32_t _elements);


    /* Attaches a Colorbuffer to the bound Framebuffer Object at Attachment point defined */
    CREATURE_API void attach_ColorBuffer(uint32_t _texture, uint32_t _attachmentPoint = 0);

    /* Attaches Color Texture to Specified FrameBuffer ID at Attachment Point */
    CREATURE_API void attach_ColorBuffer(uint32_t _fboID, uint32_t _texture, uint32_t _attachmentPoint); 

    /* Attaches a Depthbuffer to the currently bound FrameBufferObject */
    CREATURE_API void attach_DepthBuffer(uint32_t _texture);

    /* Attaches Depth Texture to Specified FrameBuffer */
    CREATURE_API void attach_DepthBuffer(uint32_t _fboID, uint32_t _texture);
 
    ///=============================================================    B    ==================================================================================

    /* Sets Buffer Object as Current using the given target */
    CREATURE_API void bind_Buffer(uint32_t _bufferID, uint32_t _target);

    /* Sets Vertex Buffer Object as Current */
    CREATURE_API void bind_FBO(uint32_t _vboID);

    /*  Sets Vertex Buffer Object as Current */
    CREATURE_API void bind_IBO(int32_t _IboID);

    /* Creates a New VAO and bind it as Current */
    CREATURE_API void bind_new_VAO(); 

    /* Binds the given Texture to currently Active Slot */
    CREATURE_API void bind_Texture(uint32_t _handle);

    /* Binds the given Texture to Provided Slot */
    CREATURE_API void bind_Texture(uint32_t _handle, uint32_t _slot);

    CREATURE_API void bind_TextureBuffer(uint32_t _id);
    template<typename _Ty>  CREATURE_API void set_TextureBuffer_Data(std::vector<_Ty> _data)
    {
        glBufferData(GL_TEXTURE_BUFFER, sizeof(_Ty) * _data.size(), _data.data(), GL_STATIC_DRAW);
    }

    /* Binds the Texture ID to the specified target */
    CREATURE_API void bind_Texture_Target(uint32_t _target, uint32_t _handle);

    /* Binds the Texture ID to the specified target in the given Slot */
    CREATURE_API void bind_Texture_Target(uint32_t _target, uint32_t _handle, uint32_t _slot);

    /* Sets Vertex Buffer Object as Current */
    CREATURE_API void bind_VBO(uint32_t _vboID);

    /* Sets Vertex Array Object as Current */
    CREATURE_API void bind_VAO(int32_t _vaoID);


    ///=============================================================    C    ==================================================================================

	/* Creates an OpenGL context from an OS native Device Context */
	CREATURE_API HGLRC create_OpenGLContext(HDC _dc);


    CREATURE_API void clear_DepthBuffer();
    CREATURE_API void clear_ColorBuffer();
    CREATURE_API void clear_FrameBuffer();
    /* Checks to see if FrameBuffer Object is complete */
    CREATURE_API int check_FBO_Status();


    ///=============================================================    D    ==================================================================================

    /* Frees ID for a Vertex Array Object*/
    CREATURE_API void delete_VBO(uint32_t _id);
    /* Frees the ID for the Element Array Object */
    CREATURE_API void delete_IBO(uint32_t _id);

    CREATURE_API void delete_Texture(uint32_t _handle);
    /* Creates a Unique ID for a Vertex Array Object*/
    CREATURE_API void delete_VAO(uint32_t _id);
    CREATURE_API void disable_DebugOutput();

    /* Frees ID for a Vertex Array Object*/
    CREATURE_API void delete_FBO(uint32_t _id);

    ///=============================================================    E    ==================================================================================

	/* Disables states for OpenGL */
    CREATURE_API void enable_DebugOutput();
	CREATURE_API void enable_Blending();
	CREATURE_API void enable_ColorLogicOperation();
	CREATURE_API void enable_Culling();
	CREATURE_API void enable_DepthClamp();
	CREATURE_API void enable_DepthTest();
	CREATURE_API void enable_Dithering();
	CREATURE_API void enable_FramebufferSRGB();
	CREATURE_API void enable_LineSmoothing();
	CREATURE_API void enable_Multisampling();
	CREATURE_API void enable_PolygonOffsetFill();
	CREATURE_API void enable_PolygonOffsetLine();
	CREATURE_API void enable_PolygonOffsetPoint();
	CREATURE_API void enable_PolygonOffsetSmooth();
	CREATURE_API void enable_PrimativeRestart();
	CREATURE_API void enable_SampleAlphatToCoverage();
	CREATURE_API void enable_SampleAlphaToOne();
	CREATURE_API void enable_SampleCoverage();
	CREATURE_API void enable_ScissorTest();
	CREATURE_API void enable_StencilTest();
	CREATURE_API void enable_SeemlessTextureCubemap();
	CREATURE_API void enable_ProgramPointSize();
    CREATURE_API void enable_Attribute(uint32_t _location);


    ///=============================================================    F    ==================================================================================



    ///=============================================================    G    ==================================================================================
    CREATURE_API size_t get_Bindless_Address();

    /* Creates Mip map for bound 2D Texture  */
    CREATURE_API void generate_MipMap();

    /* Returns the Viewport of the OpenGL context */
    CREATURE_API iVec4 get_Viewport();

    /* Returns the maximum number of Vertex buffers that can be bound as attributes */
    CREATURE_API int get_MaxAttributes();

    /* Returns the Currently active Shader Object */
    CREATURE_API int  get_CurrentShader();

    /* Recommended maximum number of vertex array indices  */
    CREATURE_API int  get_RecommendedIndices();

    /* Recommended maximum number of vertex array vertices */
    CREATURE_API int  get_RecommendedVertices();

    /* Returns maximum viewport dimensions */
    CREATURE_API iVec2 get_MaximumViewportDimensions();

    /* Returns the company responsible for this GL implementation.This name does not change from release to release. */
    CREATURE_API std::string get_Vendor();

    /* Returns the name of the renderer */
    CREATURE_API std::string get_Renderer();

    /* Returns a version or release number */
    CREATURE_API std::string get_Version();

    /*Returns a version or release number for the shading language.*/
    CREATURE_API std::string get_GLSL_version();
    CREATURE_API Vec4 get_ClearColor();

    /* Return the Active Texture unit */
    CREATURE_API int  get_ActiveTexture();
    /* returns Maximum Texture Units*/
    CREATURE_API int  get_MaximumTextureUnits();


    CREATURE_API uint32_t get_VAO();
    CREATURE_API uint32_t get_VertexVBO();
    CREATURE_API uint32_t get_ColorVBO();


    ///=============================================================    H    ==================================================================================



    ///=============================================================    I    ==================================================================================


    /* Is an ID a Vertex Buffer Object */
    CREATURE_API bool isFBO(int _array);

	/* Enable State feature for OpenGL */
	CREATURE_API bool isBlendEnabled();
	CREATURE_API bool isCullEnabled();
	CREATURE_API bool isDepthClampEnabled();
	CREATURE_API bool isDepthTestEnabled();
	CREATURE_API bool isDitherEnabled();
	CREATURE_API bool isLineSmoothEnabled();
    CREATURE_API bool is_Mapped(uint32_t _target = GL_ARRAY_BUFFER);
	CREATURE_API bool isMultisampleEnabled();
	CREATURE_API bool isPolygonSmoothEnabled();
	CREATURE_API bool isPolygonOffsetFill();
	CREATURE_API bool isPolygonOffsetLine();
	CREATURE_API bool isPolygonOffsetPoint();
	CREATURE_API bool isSampleMaskEnabled();
	CREATURE_API bool isScissorTestEnabled();
	CREATURE_API bool isStencilTestEnabled();
	CREATURE_API bool isSeemlessTextureCubemapEnabled();

    /* Is an ID a Vertex Array Object */
    CREATURE_API bool isVAO(int _array);

    /* Is an ID a Vertex Buffer Object */
    CREATURE_API bool isVBO(int _array);

    /*  Is an ID a Index Buffer Object */
    CREATURE_API bool isIBO(int _array);


    ///=============================================================    J    ==================================================================================

    ///=============================================================    K    ==================================================================================


    ///=============================================================    L    ==================================================================================


    CREATURE_API GLenum glCheckError_(const char *file, int line);

    ///=============================================================    M    ==================================================================================


    CREATURE_API bool make_Context_Current(HDC _device, HGLRC _context);

    CREATURE_API void make_Buffer_Resident();
    template<typename _Ty>
    CREATURE_API _Ty *map_Buffer(uint32_t _target = GL_ARRAY_BUFFER)
    {
        return (_Ty*)glMapBuffer(_target, GL_READ_BUFFER);
    }
    template<typename _Ty>
    CREATURE_API _Ty *map_Range(uint32_t _start, uint32_t _length, uint32_t _target = GL_ARRAY_BUFFER)
    {//target offset length access
        return (_Ty*)glMapBufferRange(_target, _start, _length * sizeof(_Ty), GL_READ_BUFFER);
    }

    template<typename _Ty>
    CREATURE_API _Ty *map_Buffer(int _target = GL_ARRAY_BUFFER, GLenum _access = GL_READ_BUFFER)
    {
        return (_Ty*)glMapBuffer(_target, _access);
    }


    ///=============================================================    N    ==================================================================================


	/* Creates a Unique ID for a Vertex Array Object*/
	CREATURE_API uint32_t new_VAO();
    CREATURE_API uint32_t new_Buffer();
	/* Creates a Unique ID for a Vertex Array Object*/
	CREATURE_API uint32_t new_VBO();
    /* Creates a Unique ID for a Element Array Object */
    CREATURE_API uint32_t new_IBO();
    CREATURE_API uint32_t new_ShaderHandle();
    CREATURE_API uint32_t new_FBO();
    /* Generates a New Texture ID for OpenGL */
    CREATURE_API uint32_t new_TextureHandle();


    ///=============================================================    O    ==================================================================================



    ///=============================================================    P    ==================================================================================

    /* Set the Alignment of the Pixel pack and unpack processed for a texture */
    CREATURE_API void PixelAlignment(uint32_t _pack, uint32_t _unpack);

    /* Set the Alignment of the Pixel Pack processe for a texture */
    CREATURE_API void PixelAlignmentPack(uint32_t _align);

    /* Set the Alignment of the Pixel Unpack processed for a texture */
    CREATURE_API void PixelAlignmentUnpack(uint32_t _align);

    ///=============================================================    Q    ==================================================================================



    ///=============================================================    R    ==================================================================================



    ///=============================================================    S    ==================================================================================
    /* Sets the active Texture Unit */
    CREATURE_API void set_ActiveTexture(uint32_t _slot);
    CREATURE_API void set_LineWidth(float _size);
    CREATURE_API void set_ClearColor(float r, float g, float b, float a);
    CREATURE_API void set_ClearColor(Vec4 _color);
	CREATURE_API uint32_t set_Attribute(uint32_t _shaderID, uint8_t _elements, const char* _name);
    CREATURE_API uint32_t set_Attribute(uint8_t _elements, const char* _name);

	/* Sets a specific Hint for OpenGL */
	CREATURE_API void set_Hint(Hint_t _hint, Mode_t _mode);
	/* Sets the Current Shader */
	CREATURE_API void set_Shader(uint32_t _program);
    CREATURE_API void set_DepthFunction(uint32_t _func);
    CREATURE_API void set_Divisor(uint8_t _location, uint32_t _divisor);
    CREATURE_API void set_Viewport(int _x, int _y, int _width, int _height);

    /* Sets the Data in the currently bound Vertex Buffer */
    template<typename _Ty>
    CREATURE_API void set_BufferData(std::vector<_Ty>& _data)
    { // Sets the Data in the currently bound Vertex Buffer sizeof(_data)
        glBufferData(GL_ARRAY_BUFFER, _data.size() * sizeof(_Ty), &_data[0], DEFAULT_BUFFER_ACCESS);
        DEBUG_CODE(CheckGLERROR());
    }
    /* Sets the Data in the currently bound Vertex Buffer */
    CREATURE_API void set_BufferData(uint32_t _size, const void* _data);

    /* Sets the Data in the currently bound Vertex Buffer With defined access */
    CREATURE_API void set_BufferData(uint32_t _size, const void* _data, GLenum _access);

    /* Sets the Data in the currently bound Vertex Buffer */
    template<typename _Ty>
    CREATURE_API void set_Subbuffer_Data(std::vector<_Ty>& _data, uint32_t _offset = 0)
    {
        glBufferSubData(GL_ARRAY_BUFFER, _offset, _data.size() * sizeof(_Ty), &_data[0]);
    }
    CREATURE_API void set_Subbuffer_Data(uint32_t _size, const void* _data, uint32_t _offset = 0);
    CREATURE_API void set_Magnification(uint32_t _param);
    CREATURE_API void set_Minification(uint32_t _param);
    CREATURE_API void set_Texture_Magnification(GLenum _target, uint32_t _param);
    CREATURE_API void set_Texture_Minification(GLenum _target, uint32_t _param);
    CREATURE_API void set_WrapX(unsigned int param);
    CREATURE_API void set_WrapY(unsigned int param);
    CREATURE_API void set_Texture_WrapX(uint32_t _target, unsigned int param);
    CREATURE_API void set_Texture_WrapY(uint32_t _target, unsigned int param);

    ///=============================================================    T    ==================================================================================



    /* Turns Mip Map on for bound 2D texture */
    CREATURE_API void turn_Mipmap_On();
    /* Turns Mip Map off for bound 2D Texture */
    CREATURE_API void turn_Mipmap_Off();

    /* Turns Mip Map on for bound texture of _target type*/
    CREATURE_API void turn_Mipmap_On(GLenum _target);
    /* Turns Mip Map off for bound 2D Texture of _target type*/
    CREATURE_API void turn_Mipmap_Off(GLenum _target);



    ///=============================================================    U    ==================================================================================
    
    /* Clears any VAO Binding */
	CREATURE_API void unbind_VAO();  
    CREATURE_API void unbind_Texture_Target(uint32_t _target);
    CREATURE_API void unbind_Texture();
    /*  Unbinds all Vertex Buffer Objects from OpenGL */   
    CREATURE_API void unbind_IBO();
    /*  Unbinds all Vertex Buffer Objects from OpenGL */
    CREATURE_API void unbind_FBO();

	/*  Unbinds all Vertex Buffer Objects from OpenGL */
	CREATURE_API void unbind_VBO();
    /* Update a Sub part of the bound Texture */
    CREATURE_API void update_SubImage(const void* _pixels, iVec2 _size, iVec2 _offset, GLenum _format = GL_RGBA, GLenum _type = GL_UNSIGNED_BYTE);

    CREATURE_API void update_Texture(const void* _pixels, iVec2 _size, GLenum _internal_format = GL_RGBA, GLenum _format = GL_RGBA, GLenum _type = GL_UNSIGNED_BYTE);


    ///=============================================================    V    ==================================================================================

    ///=============================================================    W    ==================================================================================
    ///=============================================================    X    ==================================================================================
    ///=============================================================    Y    ==================================================================================
    ///=============================================================    Z    ==================================================================================

}


 


namespace OpenGL_Debug
{
    void GetFirstNMessages(GLuint numMsgs);
}


/* Sets _slot as the Currently Active Texture */
//CREATURE_API void ActivateTexture(uint32_t _slot);


//int fetchIndex(const char* _name)
//{// Return Index of the uniform block with specific name
//	return glGetUniformBlockIndex(Program, _name);
//}
//int fetchActiveBlockBinding() {
//	int results = 0;
//	glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_BINDING, &results);
//	return results;
//
//}
//int fetchBlockSize()
//{
//	int results = 0;
//	glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_DATA_SIZE, &results);
//	return results;
//}
//int fetchNameLength()
//{
//	int results = 0;
//	glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_NAME_LENGTH, &results);
//	return results;
//}
//int fetchActiveUniforms()
//{
//	int results = 0;
//	glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &results);
//	return results;
//}
//int fetchActiveUniformIndices()
//{
//	int results = 0;
//	glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &results);
//	return results;
//}




 

 




/* GMS2 GPU State Functions
get_blendenable
get_ztestenable
get_zfunc
get_zwriteenable
get_fog
get_cullmode
get_blendmode
get_blendmode_ext
get_blendmode_ext_sepalpha
get_blendmode_src
get_blendmode_dest
get_blendmode_srcalpha
get_blendmode_destalpha
get_alphatestenable
get_colourwriteenable
get_alphatestref
get_texfilter
get_texfilter_ext
get_texrepeat
get_texrepeat_ext
set_blendenable
set_ztestenable
set_zfunc
set_zwriteenable
set_fog
set_cullmode
set_blendmode
set_blendmode_ext
set_blendmode_ext_sepalpha
set_colorwriteenable
set_alphatestenable
set_alphatestref
set_texfilter
set_texfilter_ext
set_texrepeat
set_texrepeat_ext
push_state
pop_state
get_state
set_state
*/


