#include"OpenGL.h"

#include"../Materials/Image/Texture.h"
#include"Shader/Shader.h"

#include<utility>//std::pair
#include<vector> //std::vector


std::string OpenGL_ErrorList;


 wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;
 wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;

///bool is_ActiveVAO(int _vao)
///{
///    int32_t result;
///void glGetVertexAttribiv( index,
///    GL_CURRENT_VERTEX_ATTRIB,
///    &result);
///}
///
///void glGetVertexAttribfv(GLuint index,
///    GLenum pname,
///    GLfloat *params);



namespace OpenGL
{

    /* ========================================================
    /*         Experimental nonsense more than likely
    /* ======================================================== */
    struct Mesh {};

    enum  Material_type { Normals, Albedo, Metallic };
    using Material_Fragment = std::pair<Material_type, Texture>;
    using Material = std::pair<std::vector<Material_Fragment>, Shader>;
    using RenderPair = std::pair<Material, Mesh>;
    /* =============================================================================================================
        THIS MEANS A RENDERABLE OBJECT CONSIST OF YOUR VERTICES STORED ON A SERVER SIDE VBO AND A MATERIAL INSTANCE 
        WHICH CONSIST OF A SERIES OF TEXTURES WITH A TYPE ASSIGNED TO THEM INSIDE OF A VECTOR AS WELL AS A SHADER
        DESIGNED TO RENDER THAT SERIES OF TEXTURES APPROPRIATELY.
    /* ============================================================================================================= */



    HGLRC create_OpenGLContext(HDC _dc)
    { // Create OpenGL Rendering Context
        HGLRC results{ nullptr };
        results = wglCreateContext(_dc);
        if (!wglMakeCurrent(_dc, results))
        {
            DEBUGPrint(CON_Red, "Making Current Rendering Context Failed");
        }
        if (!results)
        {
            MessageBox
            (/// Turn all this into a Macro for Clearity 
                /// DEBUGPRINT THIS and Get rid of the damn box- 11/13/20 ~ Stillll fucking here isn't it!
                NULL,
                "GL Context Creation Failed  "
                "Cannot Create Renderer",
                "Error",
                MB_OK
            );
        }

        bool static first_Load_STATIC = false;
        if (!first_Load_STATIC)
        {// Only load up GLAD a single time

            first_Load_STATIC = true;
            if (!gladLoadGL())
            {// If the Loading of OpenGL functions fails report it and exit
                DEBUGPrint(CON_Red, "Failed to initialize GLAD" << glGetError());
                __debugbreak();
            };
        }

        DEBUG_CODE(CheckGLERROR());
        return results;
    }
    bool make_Context_Current(HDC _device, HGLRC _context)
    {// Sets the Provided Context as Current on the given Device
        bool result = wglMakeCurrent(_device, _context);
        if (!result)
        {
            DEBUGPrint(CON_Red, "Making Current Rendering Context Failed");
        }

        DEBUG_CODE(CheckGLERROR());
        return result;
    }

    /* Far from complete */
    struct DeviceContext_t
    {
        DeviceContext_t() {}

        int PixelFormat{ 0 };
        Pixel_flags Flags{ OpenGL_Support };

        HDC DeviceContext{ nullptr };
        PFD PixelForma;
        PIXELFORMATDESCRIPTOR PixelFormatDescriptor;
    };





    void set_PixelFormat()
    {// Creating and Setting Pixel Format Scope
        /* 	NOTE:  A good pixel format to choose for the dummy context is a simple 32-bit RGBA color buffer, with a 24-bit depth buffer and 8-bit stencil, as we did in the above sample PFD. This will usually get a hardware accelerated pixel format. */
        int PixelFormat;
        PIXELFORMATDESCRIPTOR PixelFormatDescriptor;
        uint32_t _flags{ 0 };
        HDC DeviceContext{ nullptr };

        memset(&PixelFormatDescriptor, 0, sizeof(PixelFormatDescriptor));
        PixelFormatDescriptor.nSize = sizeof(PixelFormatDescriptor);
        PixelFormatDescriptor.nVersion = 1;
        PixelFormatDescriptor.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | _flags;
        PixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
        PixelFormatDescriptor.cColorBits = 32;

#pragma warning(suppress : 4996) 
        PixelFormat =
            ChoosePixelFormat
            (
                DeviceContext,
                &PixelFormatDescriptor
            );

        if (PixelFormat == NULL)
        {
            MessageBox
            (
                NULL,
                "ChoosePixelFormat() failed:  "
                "Cannot find a suitable pixel format.",
                "Error",
                MB_OK
            );
            __debugbreak();
        }
#pragma warning(suppress : 4996) 
        if (SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDescriptor) == FALSE)
        {
            MessageBox
            (
                NULL,
                "SetPixelFormat() failed:  "
                "Cannot set format specified.",
                "Error",
                MB_OK
            );
            __debugbreak();
        }

#pragma warning(suppress : 4996) 
        DescribePixelFormat
        (
            DeviceContext,
            PixelFormat,
            sizeof(PIXELFORMATDESCRIPTOR),
            &PixelFormatDescriptor
        );
    }




    void Attribute_Pointer(uint32_t _location, uint32_t _elements)
    {
        glVertexAttribPointer(_location, _elements, GL_FLOAT, GL_FALSE, 0, (char*)NULL);
    }
    void bind_VAO(int32_t _vaoID)
    {
        DEBUG_CODE(CheckGLERROR());
        assert(_vaoID != NULL);
        glBindVertexArray(_vaoID);
        DEBUG_CODE(CheckGLERROR());
    } 
    void delete_VAO(uint32_t _id)
    {// Frees ID for a Vertex Array Object
        glDeleteVertexArrays(1, &_id);
    }
    void enable_Attribute(uint32_t _location)
    {
        glEnableVertexAttribArray(_location);
    }


    int  get_RecommendedIndices()
    {// Recommended maximum number of vertex array indices  */
        int result{ 0 };
        glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &result);
        return result;
    }
    int  get_RecommendedVertices()
    {// Recommended maximum number of vertex array vertices */
        int result; glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &result); return result;
    }
    iVec2 get_MaximumViewportDimensions()
    {// Returns maximum viewport dimensions
        iVec2 result{ 0 };
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, (int*)&result);
        return result;
    }
    std::string get_Vendor()
    { // Returns the company responsible for this GL implementation.This name does not change from release to release. 
        return (char*)glGetString(GL_VENDOR);
    }
    std::string get_Renderer()
    {// Returns the name of the renderer 
        return (char*)glGetString(GL_RENDERER);
    }
    std::string get_Version()
    {	// Returns a version or release number 
        return (char*)glGetString(GL_VERSION);
    }
    std::string get_GLSL_version()
    { //Returns a version or release number for the shading language.
        return (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    }
    iVec4 get_Viewport()
    {	// Returns the Viewport of the OpenGL context 
        iVec4 result;

        glGetIntegerv(GL_VIEWPORT, (int*)&result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }
    int get_MaxAttributes()
    {	// Returns the maximum number of Vertex buffers that can be bound as attributes 
        int result;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }

    uint32_t get_VAO()
    {
        int32_t result;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &result);
        return result;
    }
    uint32_t get_VertexVBO()
    {
        int32_t result;
        glGetIntegerv(GL_VERTEX_ARRAY_BUFFER_BINDING, &result);
        return result;
    }  
    uint32_t get_ColorVBO()
    {
        int32_t result;
        glGetIntegerv(GL_COLOR_ARRAY_BUFFER_BINDING, &result);
        return result;
    }

    // 
  //  GL_VERTEX_ARRAY_BUFFER_BINDING

    void set_Hint(Hint_t _hint, Mode_t _mode)
    {// Sets a Hint for the OpenGL Context
        glHint(_hint, _mode);
        DEBUG_CODE(CheckGLERROR());
    }

    /* ==========================================================
                     Vertex Array Object Management
    /* ==========================================================*/
    uint32_t new_VAO()
    { // Creates a Unique ID for a Vertex Array Object
        uint32_t result;
        glGenVertexArrays(1, &result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }
    void unbind_VAO()
    {
        glBindVertexArray(0);
        DEBUG_CODE(CheckGLERROR());
    }
    bool isVAO(int _array)
    {// Is an ID a Vertex Array Object 
        return (bool)(glIsVertexArray(_array));
    }



    /* ==========================================================
                     Generic Buffer Object Management
    /* ==========================================================*/

    void bind_Buffer(uint32_t _bufferID, uint32_t _target = GL_ARRAY_BUFFER)
    {// Binds an OpenGL Buffer Object of type _target
        glBindBuffer(_target, _bufferID);
        DEBUG_CODE(CheckGLERROR());
    }
    bool is_Mapped(uint32_t _target)
    {
        int Result{ 0 };
        glGetBufferParameteriv(_target, GL_BUFFER_MAPPED, &Result);
        return (Result != 0);
    }

    /* ==========================================================
                     Vertex Buffer Object Management
    /* ==========================================================*/

    uint32_t new_VBO()
    {// Creates a Unique ID for a Vertex Array Object
        uint32_t result;
        glGenBuffers(1, &result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }
    void delete_VBO(uint32_t _id)
    {// Frees ID for a Vertex Buffer Object
        glDeleteBuffers(1, &_id);
        DEBUG_CODE(CheckGLERROR());
    }

    void bind_VBO(uint32_t _vboID)
    { // Sets Vertex Buffer Object as Current 
        assert(_vboID != NULL);
        glBindBuffer(GL_ARRAY_BUFFER, _vboID);
        DEBUG_CODE(CheckGLERROR());
    }
    void unbind_VBO()
    { // Unbinds all Vertex Buffer Objects from OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        DEBUG_CODE(CheckGLERROR());
    }

    bool isVBO(int _array)
    {// Is an ID a Vertex Buffer Object 
        return (bool)(glIsBuffer(_array));
    }


    /* ==========================================================
                     Index Buffer Object Management
    /* ==========================================================*/

    uint32_t new_IBO()
    {// Creates a Unique ID for a Vertex Array Object
        uint32_t result;
        glGenBuffers(1, &result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }
    void delete_IBO(uint32_t _id)
    {// Frees ID for a Element Array Object
        assert(_id != NULL);
        glDeleteBuffers(1, &_id);
        DEBUG_CODE(CheckGLERROR());
    }


    void bind_IBO(int32_t _iboID)
    { // Sets Vertex Buffer Object as Current 
        assert(_iboID != NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboID);
        DEBUG_CODE(CheckGLERROR());
    }
    void unbind_IBO()
    { // Unbinds all Vertex Buffer Objects from OpenGL 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        DEBUG_CODE(CheckGLERROR());
    }
    bool isIBO(int _array)
    {// Is an ID a Vertex Buffer Object 
        __debugbreak(); // Deactivated as I am unsure if there is functionality for determining if a buffer can be a valid Element Array Buffer 
        return (bool)(glIsBuffer(_array));
    }

    ///===============================================================
    /* ==========================================================
                Texture Management
    /* ==========================================================*/
    ///===============================================================


    void bind_Texture(uint32_t _handle) 
    {
        glBindTexture(GL_TEXTURE_2D, _handle);
        DEBUG_CODE(CheckGLERROR());
    }
    void bind_Texture(uint32_t _handle, uint32_t _slot)
    {
        glActiveTexture(GL_TEXTURE0 + _slot);
        glBindTexture(GL_TEXTURE_2D, _handle);
        DEBUG_CODE(CheckGLERROR());
    }
    void unbind_Texture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        DEBUG_CODE(CheckGLERROR());
    }


    void bind_Texture_Target(uint32_t _target, uint32_t _handle)
    {
        assert(_handle != NULL);
        glBindTexture(_target, _handle);
        DEBUG_CODE(CheckGLERROR());
    }
    void bind_Texture_Target(uint32_t _target, uint32_t _handle, uint32_t _slot)
    {
        assert(_handle != NULL);
        glActiveTexture(GL_TEXTURE0 + _slot);
        glBindTexture(_target, _handle);
        DEBUG_CODE(CheckGLERROR());
    }
    void unbind_Texture_Target(uint32_t _target)
    {
        glBindTexture(_target, 0);
        DEBUG_CODE(CheckGLERROR());
    }



    /* Sets the Active Texture Slot */
    void set_ActiveTexture(uint32_t _slot)
    {
        glActiveTexture(GL_TEXTURE0 + _slot);
        DEBUG_CODE(CheckGLERROR());
    }

    /* Update a specified Area of an OpenGL Texture */
    void update_SubImage(const void* _pixels, iVec2 _size, iVec2 _offset, GLenum _format , GLenum _type )
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, _offset.x, _offset.y, _size.x, _size.y, _format, _type, _pixels);
        DEBUG_CODE(CheckGLERROR());
    }

    /* Update all of the Bound OpenGL Texture */
    void update_Texture(const void* _pixels, iVec2 _size, GLenum _internal_format ,GLenum _format , GLenum _type )
    {
        glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _size.x, _size.y, 0, _format, _type, _pixels);
        DEBUG_CODE(CheckGLERROR());
    }

    /* Creates a new Handle for an OpenGL Texture Object */
    uint32_t new_TextureHandle()
    {
        uint32_t result;
        glGenTextures(1, &result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }

    void PixelAlignment(uint32_t _pack, uint32_t _unpack)
    {
        glPixelStorei(GL_PACK_ALIGNMENT, _pack);
        glPixelStorei(GL_UNPACK_ALIGNMENT, _unpack);
        DEBUG_CODE(CheckGLERROR());
    }
    void PixelAlignmentPack(uint32_t _align)
    {
        glPixelStorei(GL_PACK_ALIGNMENT, _align);
        DEBUG_CODE(CheckGLERROR());
    }
    void PixelAlignmentUnpack(uint32_t _align)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, _align);
        DEBUG_CODE(CheckGLERROR());
    }

    int  get_ActiveTexture()
    {//  Return the Active Texture unit 
        int result{ 0 };
        glGetIntegerv(GL_ACTIVE_TEXTURE, &result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }
    int  get_MaximumTextureUnits()
    {
        int result{ 0 };
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }


    void generate_MipMap()
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        DEBUG_CODE(CheckGLERROR());
    }
    void turn_Mipmap_On() 
    {// Turns on Mipmap for this texture
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        DEBUG_CODE(CheckGLERROR());
    }
    void turn_Mipmap_Off() 
    {// Turns off Mipmaps for this Texture
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
        DEBUG_CODE(CheckGLERROR());
    }
    void turn_Mipmap_On(GLenum _target)
    {// Turns on Mipmap for this texture
        glTexParameteri(_target, GL_GENERATE_MIPMAP, GL_TRUE);
        DEBUG_CODE(CheckGLERROR());
    }
    void turn_Mipmap_Off(GLenum _target)
    {// Turns off Mipmaps for this Texture
        glTexParameteri(_target, GL_GENERATE_MIPMAP, GL_FALSE);
        DEBUG_CODE(CheckGLERROR());
    }

    void set_Magnification(uint32_t _param)                         { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _param); }
    void set_Minification(uint32_t _param)                          { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _param); }
    void set_Texture_Magnification(GLenum _target, uint32_t _param) { glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, _param);       }
    void set_Texture_Minification(GLenum _target, uint32_t _param)  { glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, _param);       }

    void set_WrapX(unsigned int param) 
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
        DEBUG_CODE(CheckGLERROR());
    }
    void set_WrapY(unsigned int param) 
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
        DEBUG_CODE(CheckGLERROR());
    }
    void set_Texture_WrapX(uint32_t _target, unsigned int param) 
    {
        glTexParameteri(_target, GL_TEXTURE_WRAP_S, param);
        DEBUG_CODE(CheckGLERROR());
    }
    void set_Texture_WrapY(uint32_t _target, unsigned int param) 
    {
        glTexParameteri(_target, GL_TEXTURE_WRAP_T, param);
        DEBUG_CODE(CheckGLERROR());
    }
    void delete_Texture(uint32_t _handle)
    {
        assert(_handle != 0);
        glDeleteTextures(1, &_handle);
        DEBUG_CODE(CheckGLERROR());
    }
    ///=================================================================================
    /// Texture Buffer Object 
    ///=================================================================================

    void bind_TextureBuffer(uint32_t _handle)
    {
        assert(_handle != 0);
        glBindTexture(GL_TEXTURE_BUFFER, _handle);
    }
    void attach_TextureBuffer(uint32_t _buffer, uint32_t _format)
    {
          glTexBuffer(GL_TEXTURE_BUFFER, _format, _buffer);
    }
    ///=================================================================================
    ///=================================================================================

    //return glIsEnabled(GL_CLIP_DISTANCEi);

    uint32_t new_Buffer()
    {
        uint32_t Result;
        glGenBuffers(1, &Result);
        DEBUG_CODE(CheckGLERROR());
        return Result;
    }
    

    bool isBlendEnabled()
    {
        return glIsEnabled(GL_BLEND);
    }
    bool isCullEnabled()
    {
        return glIsEnabled(GL_CULL_FACE);
    }
    bool isDepthClampEnabled()
    {
        return glIsEnabled(GL_DEPTH_CLAMP);
    }
    bool isDepthTestEnabled()
    {
        return glIsEnabled(GL_DEPTH_TEST);
    }
    bool isDitherEnabled()
    {
        return glIsEnabled(GL_DITHER);
    }
    bool isLineSmoothEnabled()
    {
        return glIsEnabled(GL_LINE_SMOOTH);
    }
    bool isMultisampleEnabled()
    {
        return glIsEnabled(GL_MULTISAMPLE);
    }
    bool isPolygonSmoothEnabled()
    {
        return glIsEnabled(GL_POLYGON_SMOOTH);
    }
    bool isPolygonOffsetFill()
    {
        return glIsEnabled(GL_POLYGON_OFFSET_FILL);
    }
    bool isPolygonOffsetLine()
    {
        return glIsEnabled(GL_POLYGON_OFFSET_LINE);
    }
    bool isPolygonOffsetPoint()
    {
        return glIsEnabled(GL_POLYGON_OFFSET_POINT);
    }
    bool isSampleMaskEnabled()
    {
        return glIsEnabled(GL_SAMPLE_MASK);
    }
    bool isScissorTestEnabled()
    {
        return glIsEnabled(GL_SCISSOR_TEST);
    }
    bool isStencilTestEnabled()
    {
        return glIsEnabled(GL_STENCIL_TEST);
    }
    bool isSeemlessTextureCubemapEnabled()
    {
        return glIsEnabled(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }


    //bool is            (){return glIsEnabled(GL_COLOR_LOGIC_OP);		   }
    //bool is            (){return glIsEnabled(GL_FRAMEBUFFER_SRGB);		   }
    //bool is            (){return glIsEnabled(GL_PROGRAM_POINT_SIZE);	   }
    //bool is            (){return glIsEnabled(GL_PRIMITIVE_RESTART);		   }
    //bool is            (){return glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE); }
    //bool is            (){return glIsEnabled(GL_SAMPLE_ALPHA_TO_ONE);	   }
    //bool is            (){return glIsEnabled(GL_SAMPLE_COVERAGE);		   }


    //GL_CLIP_DISTANCEi
    void set_DepthFunction(uint32_t _func)
    {
        glDepthFunc(_func);
        CheckGLERROR();
    }


    /* Enable State Functions */
    void  enable_Blending() { glEnable(GL_BLEND); }
    void  enable_ColorLogicOperation() { glEnable(GL_COLOR_LOGIC_OP); }
    void  enable_Culling() { glEnable(GL_CULL_FACE); }
    void  enable_DepthClamp() { glEnable(GL_DEPTH_CLAMP); }
    void  enable_DepthTest() { glEnable(GL_DEPTH_TEST); }
    void  enable_Dithering() { glEnable(GL_DITHER); }
    void  enable_FramebufferSRGB() { glEnable(GL_FRAMEBUFFER_SRGB); }
    void  enable_LineSmoothing() { glEnable(GL_LINE_SMOOTH); }
    void  enable_Multisampling() { glEnable(GL_MULTISAMPLE); }
    void  enable_PolygonOffsetFill() { glEnable(GL_POLYGON_OFFSET_FILL); }
    void  enable_PolygonOffsetLine() { glEnable(GL_POLYGON_OFFSET_LINE); }
    void  enable_PolygonOffsetPoint() { glEnable(GL_POLYGON_OFFSET_POINT); }
    void  enable_PolygonOffsetSmooth() { glEnable(GL_POLYGON_SMOOTH); }
    void  enable_PrimativeRestart() { glEnable(GL_PRIMITIVE_RESTART); }
    void  enable_SampleAlphatToCoverage() { glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE); }
    void  enable_SampleAlphaToOne() { glEnable(GL_SAMPLE_ALPHA_TO_ONE); }
    void  enable_SampleCoverage() { glEnable(GL_SAMPLE_COVERAGE); }
    void  enable_ScissorTest() { glEnable(GL_SCISSOR_TEST); }
    void  enable_StencilTest() { glEnable(GL_STENCIL_TEST); }
    void  enable_SeemlessTextureCubemap() { glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); }
    void  enable_ProgramPointSize() { glEnable(GL_PROGRAM_POINT_SIZE); }

    /* Disable State Functions */
    void  DisableBlending() { glDisable(GL_BLEND); }
    void  DisableColorLogicOperation() { glDisable(GL_COLOR_LOGIC_OP); }
    void  DisableCulling() { glDisable(GL_CULL_FACE); }
    void  DisableDepthClamp() { glDisable(GL_DEPTH_CLAMP); }
    void  DisableDepthTest() { glDisable(GL_DEPTH_TEST); }
    void  DisableDithering() { glDisable(GL_DITHER); }
    void  DisableFramebufferSRGB() { glDisable(GL_FRAMEBUFFER_SRGB); }
    void  DisableLineSmoothing() { glDisable(GL_LINE_SMOOTH); }
    void  DisableMultisampling() { glDisable(GL_MULTISAMPLE); }
    void  DisablePolygonOffsetFill() { glDisable(GL_POLYGON_OFFSET_FILL); }
    void  DisablePolygonOffsetLine() { glDisable(GL_POLYGON_OFFSET_LINE); }
    void  DisablePolygonOffsetPoint() { glDisable(GL_POLYGON_OFFSET_POINT); }
    void  DisablePolygonOffsetSmooth() { glDisable(GL_POLYGON_SMOOTH); }
    void  DisablePrimativeRestart() { glDisable(GL_PRIMITIVE_RESTART); }
    void  DisableSampleAlphatToCoverage() { glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE); }
    void  DisableSampleAlphaToOne() { glDisable(GL_SAMPLE_ALPHA_TO_ONE); }
    void  DisableSampleCoverage() { glDisable(GL_SAMPLE_COVERAGE); }
    void  DisableScissorTest() { glDisable(GL_SCISSOR_TEST); }
    void  DisableStencilTest() { glDisable(GL_STENCIL_TEST); }
    void  DisableSeemlessTextureCubemap() { glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS); }
    void  DisableProgramPointSize() { glDisable(GL_PROGRAM_POINT_SIZE); }


    void set_Shader(uint32_t _program) {
        glUseProgram(_program);
    }

    uint32_t set_Attribute(uint32_t _shaderID, uint8_t _elements, const char* _name)
    {
        assert(_shaderID != NULL);

        uint32_t Location = glGetAttribLocation(_shaderID, _name);
        assert(Location != -1);

        glEnableVertexAttribArray(Location);
        glVertexAttribPointer(Location, _elements, GL_FLOAT, GL_FALSE, 0, (char*)NULL);

        CheckGLERROR();
        return Location;
    }
    uint32_t set_Attribute(uint8_t _elements, const char* _name)
    {
        CheckGLERROR();

        int H = Shader::getHandle();
        int32_t Location = glGetAttribLocation(H, _name);
        CheckGLERROR();

        DEBUG_CODE
        (
            if (Location == -1)
            {
                Print("ERROR: Attribute does not exist, Your shader did not compile or you are attempting an attribute that was never used and erased by GLSL");
                __debugbreak();
            }
        );
        CheckGLERROR();

        enable_Attribute(Location);
        CheckGLERROR();
        Attribute_Pointer(Location, _elements);

        CheckGLERROR();
        return Location;
    }

    void set_Divisor(uint8_t _location, uint32_t _divisor)
    {
        OpenGL4_6(glVertexAttribDivisor(_location, _divisor));
        OpenGL3_1(glVertexAttribDivisorARB(_location, _divisor));
    }



    void set_BufferData(uint32_t _size, const void* _data)
    { // Sets the Data in the currently bound Vertex Buffer sizeof(_data)
        glBufferData(GL_ARRAY_BUFFER, _size, _data, DEFAULT_BUFFER_ACCESS);
        DEBUG_CODE(CheckGLERROR());
    }
    void set_BufferData(uint32_t _size, const void* _data, GLenum _access)
    { // Sets the Data in the currently bound Vertex Buffer sizeof(_data)
        glBufferData(GL_ARRAY_BUFFER, _size, _data, _access);
        DEBUG_CODE(CheckGLERROR());
    }
    void set_Subbuffer_Data(uint32_t _size, const void* _data, uint32_t _offset)
    {
        glBufferSubData(GL_ARRAY_BUFFER, _offset, _size, _data);
        DEBUG_CODE(CheckGLERROR());
    }

    GLenum glCheckError_(const char *file, int line)
    {
        GLenum errorCode{ 0 };
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (errorCode)
            {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            std::string output = "OpenGL Error: " + error + " \n File: " + file + "\n Line: [" + std::to_string(line) + "]" + "\n \n";
            OpenGL_ErrorList += output;
            std::cout << output << std::endl; 

        }
       // OpenGL_Debug::GetFirstNMessages(5);
        return errorCode;
    }

    void set_Viewport(int _x, int _y, int _width, int _height)
    {
        glViewport(_x, _y, _width, _height);
    }



    //============================================================================================
    //     SHADER FUNCTIONALITY
    //============================================================================================

    uint32_t new_ShaderHandle()
    {
        return glCreateProgram();
    }
    uint32_t new_VertexShader()
    {
        return glCreateShader(GL_VERTEX_SHADER);

    }
    uint32_t new_FragmentShader()
    {
        return glCreateShader(GL_FRAGMENT_SHADER);
    }




    void set_LineWidth(float _size)
    {
        glLineWidth(_size);
    }




    //============================================================================================
    // FRAME BUFFER OBJECT 
    //============================================================================================
    /* Creates a Unique ID for a Frame Buffer Object*/
    uint32_t new_FBO()
    {
        uint32_t result;
        glGenFramebuffers(1, &result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }

    /* Creates a Unique ID for a Frame Buffer Object*/
    uint32_t new_FBO(uint32_t _count)
    {
        uint32_t result;
        glGenFramebuffers(_count, &result);
        DEBUG_CODE(CheckGLERROR());
        return result;
    }

    /* Checks the Status of our FrameBuffer */
    int check_FBO_Status()
    { 
        return glCheckFramebufferStatus(GL_FRAMEBUFFER); 
    }

    /* Frees ID for a Frame Buffer Object*/
    void delete_FBO(uint32_t _id)
    {
        assert(_id != NULL);
        glDeleteFramebuffers(1, &_id);
        DEBUG_CODE(CheckGLERROR());
    }  

    /* Sets Frame Buffer Object as Current */
    void bind_FBO(uint32_t _fboID)
    {
        assert(_fboID != NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
        DEBUG_CODE(CheckGLERROR());
    }
    /*  Unbinds all Frame Buffer Objects from OpenGL */
    void unbind_FBO()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        DEBUG_CODE(CheckGLERROR());
    }

    /* Is an ID a Vertex Buffer Object */
  //  bool isFBO(int _array) { return true; }

    /* Gets a pointer to the Bindless Buffer on the GPU */
    size_t get_Bindless_Address()
    {
        size_t Result{ 0 };
        glGetBufferParameterui64vNV(GL_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &Result);
        DEBUG_CODE(CheckGLERROR());
        return Result;
    }
    void make_Buffer_Resident() {  glMakeBufferResidentNV(GL_ARRAY_BUFFER, GL_READ_ONLY);   }



    void clear_DepthBuffer() { glClear(GL_DEPTH_BUFFER_BIT);  }
    void clear_ColorBuffer() { glClear(GL_COLOR_BUFFER_BIT);  }
    void clear_FrameBuffer() { glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);    }

    Vec4 get_ClearColor()
    {
        GLfloat results[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE, results);
        return { results[0], results[1], results[2], results[3] };
    }
    void set_ClearColor(float r, float g, float b, float a)
    {
        set_ClearColor({ r,g,b,a });
    }
    void set_ClearColor(Vec4 _color)
    {
        glClearColor(_color.x, _color.y, _color.z, _color.w);
    }

    void attach_ColorBuffer(uint32_t _texture, uint32_t _attachmentPoint)
    { 
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _attachmentPoint, GL_TEXTURE_2D, _texture, 0);
    }

    void attach_DepthBuffer(uint32_t _texture)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _texture, 0);
    }
    void attach_ColorBuffer(uint32_t _fboID, uint32_t _texture, uint32_t _attachmentPoint)
    {
        OpenGL::bind_FBO(_fboID);
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _attachmentPoint, GL_TEXTURE_2D, _texture, 0);
        }
        OpenGL::unbind_FBO();
    }

    void attach_DepthBuffer(uint32_t _fboID, uint32_t _texture)
    {
        OpenGL::bind_FBO(_fboID);
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _texture, 0);
        }
        OpenGL::unbind_FBO();
    }






   
    void enable_DebugOutput()
    {
        glEnable(GL_DEBUG_OUTPUT);
    }
    void disable_DebugOutput()
    {
        glDisable(GL_DEBUG_OUTPUT);
    }
}






namespace OpenGL_Debug
{

    void GetFirstNMessages(GLuint numMsgs)
    {
        GLint maxMsgLen = 0;
        glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);

        std::vector<GLchar> msgData(numMsgs * maxMsgLen);
        std::vector<GLenum> sources(numMsgs);
        std::vector<GLenum> types(numMsgs);
        std::vector<GLenum> severities(numMsgs);
        std::vector<GLuint> ids(numMsgs);
        std::vector<GLsizei> lengths(numMsgs);

        uint32_t Size = 255;//msgs.size() made this Size'
        GLuint numFound = glGetDebugMessageLog(numMsgs, Size, &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);

        sources.resize(numFound);
        types.resize(numFound);
        severities.resize(numFound);
        ids.resize(numFound);
        lengths.resize(numFound);

        std::vector<std::string> messages;
        messages.reserve(numFound);

        std::vector<GLchar>::iterator currPos = msgData.begin();
        for (size_t msg = 0; msg < lengths.size(); ++msg)
        {
            messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
            currPos = currPos + lengths[msg];
        }

        for (auto& M : messages)
        {
            Print("Debug Message:" << M);
        }
    }

}




/* =========================================================================================================================================
/*                                                       REFERENCES 
/*  =========================================================================================================================================
/*  Mathmatics for Lighting in OpenGL
/*  https://www.tjhsst.edu/~dhyatt/supercomp/n310.html
/*  
/*  Vertexbanana Basic-OpenGL-Tutorials
/*  https://github.com/vertexbanana/Basic-OpenGL-Tutorials/blob/master/BasicTutorials/DEPENDENCIES/include/GL/GL.H
/* ========================================================================================================================================= */