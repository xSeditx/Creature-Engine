#pragma once


#include"Core\Common.h"

#include<GL/gl.h>
#include<GL/glu.h>
#define _USING_OPENGL_API /* Are we using OpenGL or something else like DX*/
#pragma comment(lib, "OpenGL32.lib") 
#pragma comment(lib, "glu32.lib")

namespace OpenGL
{

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
		Nices = GL_NICEST,
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


	CREATURE_API void InitOpenGL();
	
	/* Creates an OpenGL context from an OS native Device Context */
	CREATURE_API HGLRC create_OpenGLContext(HDC _dc);
	
	/* Returns the Viewport of the OpenGL context */
	CREATURE_API iVec4 get_Viewport();

	/* Returns the maximum number of Vertex buffers that can be bound as attributes */
	CREATURE_API int get_MaxAttributes();

	/* Return the Active Texture unit */
	CREATURE_API int  get_ActiveTexture();

	/* returns Maximum Texture Units*/
	CREATURE_API int  get_MaximumTextureUnits();

	/* Sets the active Texture Unit */
	CREATURE_API void set_ActiveTexture(uint32_t _slot);

	/* Returns the Currently active Shader Object */
	CREATURE_API int  get_CurrentShader();

	/* Sets the Current Shader */
	CREATURE_API void set_Shader(uint32_t _program);

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

	/* Is an ID a Vertex Array Object */
	CREATURE_API bool isVAO(int _array);

	/* Sets a specific Hint for OpenGL */
	CREATURE_API void set_Hint(Hint_t _hint, Mode_t _mode);

	/* Enable State feature for OpenGL */
	CREATURE_API bool isBlendEnabled();
	CREATURE_API bool isCullEnabled();
	CREATURE_API bool isDepthClampEnabled();
	CREATURE_API bool isDepthTestEnabled();
	CREATURE_API bool isDitherEnabled();
	CREATURE_API bool isLineSmoothEnabled();
	CREATURE_API bool isMultisampleEnabled();
	CREATURE_API bool isPolygonSmoothEnabled();
	CREATURE_API bool isPolygonOffsetFill();
	CREATURE_API bool isPolygonOffsetLine();
	CREATURE_API bool isPolygonOffsetPoint();
	CREATURE_API bool isSampleMaskEnabled();
	CREATURE_API bool isScissorTestEnabled();
	CREATURE_API bool isStencilTestEnabled();
	CREATURE_API bool isSeemlessTextureCubemapEnabled();

	/* Disables states for OpenGL */
	CREATURE_API void  EnableBlending();
	CREATURE_API void  EnableColorLogicOperation();
	CREATURE_API void  EnableCulling();
	CREATURE_API void  EnableDepthClamp();
	CREATURE_API void  EnableDepthTest();
	CREATURE_API void  EnableDithering();
	CREATURE_API void  EnableFramebufferSRGB();
	CREATURE_API void  EnableLineSmoothing();
	CREATURE_API void  EnableMultisampling();
	CREATURE_API void  EnablePolygonOffsetFill();
	CREATURE_API void  EnablePolygonOffsetLine();
	CREATURE_API void  EnablePolygonOffsetPoint();
	CREATURE_API void  EnablePolygonOffsetSmooth();
	CREATURE_API void  EnablePrimativeRestart();
	CREATURE_API void  EnableSampleAlphatToCoverage();
	CREATURE_API void  EnableSampleAlphaToOne();
	CREATURE_API void  EnableSampleCoverage();
	CREATURE_API void  EnableScissorTest();
	CREATURE_API void  EnableStencilTest();
	CREATURE_API void  EnableSeemlessTextureCubemap();
	CREATURE_API void  EnableProgramPointSize();



}




















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





/* 
int Shader::GetCSMaxBlocks()
{//	data returns one value, the maximum number of active shader storage blocks that may be accessed by a compute shader.
	int results = 0;
	glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &results);
	return results;
}
int Shader::GetMaxShaderStorageBlocks()
{//	data returns one value, the maximum total number of active shader storage blocks that may be accessed by all active shaders.
	int results = 0;
	glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &results);
	return results;
}
	int GetCSMaxBlocks();
	int GetMaxShaderStorageBlocks();



*/






//		data returns one value, the name of the program object that is currently active, or 0 if no program object is active.See glUseProgram.


