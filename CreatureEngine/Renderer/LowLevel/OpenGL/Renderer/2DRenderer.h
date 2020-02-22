#include "Renderer.h"
#include "Core/Common.h"


namespace OpenGL
{
	class Renderer2D
		:
		public Renderer
	{
		Camera2D mainCamera;

	public:
		NO_COPY_OR_ASSIGNMENT(Renderer2D);
		enum  Surface_t { Normals, Albedo, Metallic };
		using SurfaceFragment = std::pair<Surface_t, Graphics::Texture>;
		using Surface = std::vector<SurfaceFragment>;
		using Material = std::pair<Surface, Shader>;
		using RenderPair = std::pair<Material, Mesh>;


		Renderer2D() = default;
		Renderer2D(Vec2 _size);

		/* Submits a Quad using the Current Draw Color */
		void renderQuad(Vec2 _topleft, Vec2 _bottomright);
		/* Submits a Quad using the provided Draw Color */
		void renderQuad(Vec2 _topleft, Vec2 _bottomright, Vec4 _color);

		/* Accepts a precreated Batch into the Renderer
		 ~WARNING~ Possibly Deprecation soon */
		void renderQuadBatch(const std::vector<Vec2> _batch);

       

		/* Renderers the Current batch */
		void Render();
		/* Updates the data in the Camera and the Batchs */
		void Update();

		/* Returns a vec4 of Normalized Colors for OpenGL Accepts 0-255*/
		Vec4 CreateColor(int _r, int _g, int _b, int _a);
		/* Clears the Buffers */
		void Flush();

		/* Might eliminate these in near future idk yet*/
        void SetRenderColor(int _r, int _g, int _b, int _a);

		/* Changes the size of the Renderer */
        void Resize(Vec2 _size);

		/* Gets the Camera the Renderer uses */
		Camera2D& g_Camera() { return mainCamera; }

    private:
		uint32_t QuadVAO{ 0 };
		size_t InstanceCount{ 0 };

		Shader* InstanceRenderer;

		uint32_t ColorVBO{ 0 };
		std::vector<Vec4> ColorData;

		uint32_t TransformVBO{ 0 };
		std::vector<Vec4> Positions;


		uint32_t QuadVBO{ 0 };
		Vec2 QuadData[6] =
		{
			Vec2(0, 0),  Vec2(1, 0),  Vec2(0, 1),
			Vec2(1, 1),  Vec2(0, 1),  Vec2(1, 0)
		};


		Vec4 CurrentRenderColor{ 1, 0, 0, 1 };

//===============================================================================================================
		std::string VinstanceRenderer =
			"#version 330 core     \n\
layout(location = 0) in vec2 aPos; \n\
layout(location = 1) in vec4 Position; \n\
layout(location = 2) in vec4 Color; \n\
uniform mat4 ProjectionMatrix;     \n\
uniform mat4 ViewMatrix;           \n\
out vec4 Col;                      \n\
void main()                        \n\
{                                  \n\
    Col = Color; \n\
    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
    gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
}";

		std::string FinstanceRenderer =
			"#version 330 core \n\
in vec4 Col;                   \n\
out vec4 FragColor;            \n\
void main()                    \n\
{                              \n\
    FragColor = Col;\n\
}";


	};// Class Renderer2D
}// NS OpenGLvec4(Col.rgb,1.0f); 




/*=======================================================================================================================================================
/*                                               TRASH
/*=======================================================================================================================================================*/