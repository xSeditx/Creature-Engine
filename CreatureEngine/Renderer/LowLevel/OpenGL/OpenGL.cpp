#include"OpenGL.h"

#include"../Materials/Image/Texture.h"
#include"Shader/Shader.h"
//../Image/Texture.h"

#include<utility>//std::pair
#include<vector>//std::vector

using namespace Graphics;

namespace OpenGL
{
	/*
	Experimental nonsense more than likely
	========================================================*/
	struct Mesh {};
 	//struct Texture {};


	enum  Surface_t { Normals, Albedo, Metallic };
	using SurfaceFragment = std::pair<Surface_t, Texture>;
	using Surface = std::vector<SurfaceFragment>;
	using Material = std::pair<Surface, Shader>;
	using RenderPair = std::pair<Material, Mesh>;
	//======================================================
	/*void InitOpenGL()
	{
		Print(gladLoadGL());
		RenderPair TestObject =
		{
			Material
			(
				{
					{
						{
							Normals, Texture()
						},
						{
							Albedo, Texture()
						},
						{
							Metallic, Texture()
						}
					},
					Shader()
				}
			),
			Mesh()
		};
	}
	*/

	void InitOpenGL()
	{
		// Init GLFW
		//glfwInit();
		// ... <snip> ... setup a window and a context

		// Load all OpenGL functions using the glfw loader function
		// If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
	//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	//		std::cout << "Failed to initialize OpenGL context" << std::endl;
	//		return -1;
	//	}
		// Alternative use the builtin loader, e.g. if no other loader function is available
		/*
		if (!gladLoadGL()) {
			std::cout << "Failed to initialize OpenGL context" << std::endl;
			return -1;
		}
		*/

		// glad populates global constants after loading to indicate,
		// if a certain extension/version is available.
	//	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	//	if (GLAD_GL_EXT_framebuffer_multisample) {
			/* GL_EXT_framebuffer_multisample is supported */
	//	}
	//	if (GLAD_GL_VERSION_3_0) {
			/* We support at least OpenGL version 3 */
	//	}

		// ... <snip> ... more code
	}

	HGLRC create_OpenGLContext(HDC _dc)
	{ // Create OpenGL Rendering Context
		HGLRC results{ nullptr };
		results = wglCreateContext(_dc);
		if (!wglMakeCurrent(_dc, results))
		{
			std::cout << "Making Current Rendering Context Failed" << "\n";
		}
		if (!results)
		{
			MessageBox
			(/// Turn all this into a Macro for Clearity
				NULL,
				"GL Context Creation Failed  "
				"Cannot Create Renderer",
				"Error",
				MB_OK
			);
		}
		if (!gladLoadGL())
		{// If the Loading of OpenGL functions fails report it and exit
			int error_code = glGetError();
			std::cout << "Failed to initialize GLAD" << error_code << std::endl;
			__debugbreak();
		};

		return results;
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


	//void InitOpenGL();
	HGLRC create_OpenGLContext(HDC _dc);

	int  get_ActiveTexture()
	{//  Return the Active Texture unit 
		int result{ 0 };
		glGetIntegerv(GL_ACTIVE_TEXTURE, &result);
		return result;
	}
	int  get_MaximumTextureUnits()
	{
		int result{ 0 };
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &result);
		return result;
	}
	void set_ActiveTexture(uint32_t _slot)
	{
		glActiveTexture(_slot);
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


	void set_Hint(Hint_t _hint, Mode_t _mode)
	{
		glHint(_hint, _mode);
		DEBUG_CODE(CheckGLERROR());
	}
	/* ==========================================================
					 Vertex Array Object Management
	/* ==========================================================*/
	uint32_t create_VAO()
	{ // Creates a Unique ID for a Vertex Array Object
		uint32_t result;
		glGenVertexArrays(1, &result);
		DEBUG_CODE(CheckGLERROR());
		return result;
	}
	void bind_VAO(int32_t _vaoID)
	{
		glBindVertexArray(_vaoID);
		DEBUG_CODE(CheckGLERROR());
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
					 Vertex Buffer Object Management
	/* ==========================================================*/
	
	uint32_t create_VBO()
	{// Creates a Unique ID for a Vertex Array Object
		uint32_t result;
		glGenBuffers(1, &result);
		DEBUG_CODE(CheckGLERROR());
		return result;
	}
	void bind_VBO(int32_t _vboID)
	{ // Sets Vertex Buffer Object as Current 
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
				Texture Management
	/* ==========================================================*/
	CREATURE_API void ActivateTexture(uint32_t _slot)
	{// Sets _slot as the Currently Active Texture 
		glActiveTexture(GL_TEXTURE0 + _slot);
		DEBUG_CODE(CheckGLERROR());
	}



	//return glIsEnabled(GL_CLIP_DISTANCEi);




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


	/* Enable State Functions */
	void  EnableBlending() { glEnable(GL_BLEND); }
	void  EnableColorLogicOperation() { glEnable(GL_COLOR_LOGIC_OP); }
	void  EnableCulling() { glEnable(GL_CULL_FACE); }
	void  EnableDepthClamp() { glEnable(GL_DEPTH_CLAMP); }
	void  EnableDepthTest() { glEnable(GL_DEPTH_TEST); }
	void  EnableDithering() { glEnable(GL_DITHER); }
	void  EnableFramebufferSRGB() { glEnable(GL_FRAMEBUFFER_SRGB); }
	void  EnableLineSmoothing() { glEnable(GL_LINE_SMOOTH); }
	void  EnableMultisampling() { glEnable(GL_MULTISAMPLE); }
	void  EnablePolygonOffsetFill() { glEnable(GL_POLYGON_OFFSET_FILL); }
	void  EnablePolygonOffsetLine() { glEnable(GL_POLYGON_OFFSET_LINE); }
	void  EnablePolygonOffsetPoint() { glEnable(GL_POLYGON_OFFSET_POINT); }
	void  EnablePolygonOffsetSmooth() { glEnable(GL_POLYGON_SMOOTH); }
	void  EnablePrimativeRestart() { glEnable(GL_PRIMITIVE_RESTART); }
	void  EnableSampleAlphatToCoverage() { glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE); }
	void  EnableSampleAlphaToOne() { glEnable(GL_SAMPLE_ALPHA_TO_ONE); }
	void  EnableSampleCoverage() { glEnable(GL_SAMPLE_COVERAGE); }
	void  EnableScissorTest() { glEnable(GL_SCISSOR_TEST); }
	void  EnableStencilTest() { glEnable(GL_STENCIL_TEST); }
	void  EnableSeemlessTextureCubemap() { glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); }
	void  EnableProgramPointSize() { glEnable(GL_PROGRAM_POINT_SIZE); }

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



	void  set_Attribute(uint32_t _shaderID, uint8_t _elements, const char* _name)
	{
		uint32_t Location = glGetAttribLocation(_shaderID, _name);
		glEnableVertexAttribArray(Location);
		glVertexAttribPointer(Location, _elements, GL_FLOAT, GL_FALSE, 0, (char*)NULL);
		CheckGLERROR();
	}
    void  set_Attribute(uint8_t _elements, const char* _name)
    {
        uint32_t Location = glGetAttribLocation(Shader::getHandle(), _name);
        glEnableVertexAttribArray(Location);
        glVertexAttribPointer(Location, _elements, GL_FLOAT, GL_FALSE, 0, (char*)NULL);
        CheckGLERROR();
    }





	void set_BufferData(uint32_t _size, const void* _data)
	{ // Sets the Data in the currently bound Vertex Buffer sizeof(_data)
		glBufferData(GL_ARRAY_BUFFER, _size, _data, DEFAULT_BUFFER_ACCESS);
		DEBUG_CODE(CheckGLERROR());
	}


}


//glGet with argument GL_LOGIC_OP_MODE.



/*
Mathmatics for Lighting in OpenGL
https://www.tjhsst.edu/~dhyatt/supercomp/n310.html

Vertexbanana Basic-OpenGL-Tutorials
https://github.com/vertexbanana/Basic-OpenGL-Tutorials/blob/master/BasicTutorials/DEPENDENCIES/include/GL/GL.H
*/


