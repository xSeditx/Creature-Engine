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
		Renderer2D() = default;
		Renderer2D(Vec2 _size);

//  Renderer2D(Renderer2D&& _other) = default;
//	Renderer2D& operator=(Renderer2D&& _other) = default;


		std::vector<Mat4> Transforms;

		enum  Surface_t { Normals, Albedo, Metallic };
		using SurfaceFragment = std::pair<Surface_t, Graphics::Texture>;
		using Surface = std::vector<SurfaceFragment>;
		using Material = std::pair<Surface, Shader>;
		using RenderPair = std::pair<Material, Mesh>;
		std::vector<Vec2> QuadData;
		std::vector<Vec2> LineData;
       
        uint32_t QuadVAO{ 0 }; uint32_t QuadVBO{ 0 }; uint32_t ColorVBO{ 0 };
        uint32_t LineVAO{ 0 }; uint32_t LineVBO{ 0 };
       
		void renderQuad(Vec2 _topleft, Vec2 _bottomright);
		void renderLine(Vec2 _start, Vec2 _end);

		void renderQuadBatch(const std::vector<Vec2> _batch);
		void renderLineBatch(const std::vector<Vec2> _batch);


		Shader* QuadRenderer;
		Shader* LineRenderer;
		Transform ModelMatrix;

		void Render();
		void Update();

		Camera2D& g_Camera()
        {
            return mainCamera;
        }


        void Resize(Vec2 _size);

        void SetRenderColor(int _r, int _g, int _b, int _a);
        Vec3 CurrentRenderColor;
        std::vector<Vec3> ColorData;

    private:
 
		std::string VquadRenderer =
			"#version 330 core     \n\
layout(location = 0) in vec2 aPos; \n\
layout(location = 1) in vec3 Color;\n\
uniform mat4 ProjectionMatrix;     \n\
uniform mat4 ViewMatrix;           \n\
uniform mat4 ModelMatrix;          \n\
out vec3 Col;                      \n\
void main()                        \n\
{                                  \n\
    Col = Color;                   \n\
    mat4 ModelViewMatrix = (ViewMatrix * ModelMatrix);  \n\
    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
    gl_Position = ModelViewProjectionMatrix * vec4(aPos.x, aPos.y, -1.0, 1.0); \n\
}";

		std::string FquadRenderer =
			"#version 330 core \n\
out vec4 FragColor;            \n\
in vec3  Col;            \n\
void main()                    \n\
{                              \n\
    FragColor = vec4(Col, 1.0f); \n\
}";

//vec4(1.0, 1.0, 1.0, 1.0); vec4(1.0, 1.0, 1.0, 1.0) + Col; 
	};// Class Renderer2D

}// NS OpenGL




/*=======================================================================================================================================================
/*                                               TRASH
/*=======================================================================================================================================================
//gl_Position = (ProjectionMatrix * (ViewMatrix * ModelMatrix)) * vec4(aPos.x, aPos.y, -1.0, 1.0); \n\

std::vector<FrameBufferObject> RenderTargets;
std::vector<Shader> Programs;
//std::vector<VertexArrayObject> VAOs;
//std::vector<VertexBufferObject<Vec3>> VBOs;
std::vector<Mesh> Meshes;

VertexData.push_back(Vec2(Position.x - Offset.x, Position.y + Offset.y));
VertexData.push_back(Vec2(Position.x - Offset.x, Position.y - Offset.y));
VertexData.push_back(Vec2(Position.x + Offset.x, Position.y - Offset.y));
VertexData.push_back(Vec2(Position.x - Offset.x, Position.y + Offset.y));
VertexData.push_back(Vec2(Position.x + Offset.x, Position.y - Offset.y));
VertexData.push_back(Vec2(Position.x + Offset.x, Position.y - Offset.y));

Vec2 Position = Vec2(0);
//Vec4 Vertices[6] =
//{   // positions   // texCoords
//	Vec4(0 ,         Size.y   ,    0.0f,  1.0f),
//	Vec4(0 ,        -Size.y   ,    0.0f,  0.0f),
//	Vec4(Size.x ,  -Size.y   ,    1.0f,  0.0f),
//	Vec4(0 ,         Size.y   ,    0.0f,  1.0f),
//	Vec4(Size.x ,  -Size.y   ,    1.0f,  0.0f),
//	Vec4(Size.x ,   Size.y   ,    1.0f,  1.0f)
//};

//glGenBuffers(1, &VBO);
//glBindBuffer(GL_ARRAY_BUFFER, VBO);
//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
//VAO->Attach(VERTEX, new VertexBufferObject<Vec2>(&newVertices[0], 6));
//VAO->Attach(UVCOORD, new VertexBufferObject<Vec2>(&TexCoords[0], 6));
//Program = new Shader("Resources\\DebugQuad.sfx");

Vec2 Vertices[6];
Vec2 TexCoords[6] =
{
	Vec2(0.0f, 1.0f),
	Vec2(0.0f, 0.0f),
	Vec2(1.0f, 0.0f),
	Vec2(0.0f, 1.0f),
	Vec2(1.0f, 0.0f),
	Vec2(1.0f, 1.0f)
};

                   //VA.Bind();
                   //for(auto& VB: VBOs)
                   //{ }


		//for (auto& FB : RenderTargets)
		//{
		//    FB.Bind();
		//    for (auto& S: Programs)
		//    {
		//        S.Enable();
		//        for (auto& VA : VAOs)
		//        {
		//            VA.Bind();
		//            /*
		//            for(auto& VB: VA.Buffers)
		//            {
		//                VB->Bind();
		//            }
		//            for(auto& M: Meshes)
		//            {
		//                M.Bind();
		//            }*/
		//            Renderer::drawArray(QuadVBO, VertexData.size() * 0.5f);
		//        }
		//    }
		//}
///     struct RenderPass
///     {
///     	void Submit(Mesh& _mesh, Graphics::Texture& _tex)
///     	{
///           //  SurfaceMap[_tex.g_Handle()].push_back( _mesh );
///     	}
///     	void Render()
///     	{
///             for (auto& Te : SurfaceMap)
///             {
///                 int Slot{ 0 };
///             	for (auto& S : Te.first)
///             	{// Cycle over and fill all the Slots for the Surface,  Diffuse, the Bump map, Shine, Displacement etc..
///             		glActiveTexture(GL_TEXTURE0 + (Slot++));
///             		glBindTexture(GL_TEXTURE_2D, S);
///             	}
///             
///             	for(auto& M : Te.second)
///             	{ 
///             		//M.
///             	}
///             }
///     	}
///     	//std::unordered_map<std::vector<uint32_t>, std::vector<Mesh>> SurfaceMap;
///     	std::vector<std::vector<int>> Pairs;
///     	std::vector<Graphics::Texture*> Texts;
///     	std::vector<Mesh*> Meshes;
///     	Shader* GPUrenderer;
///     	FrameBufferObject FBO;
///     	DEBUG_CODE(const char* Name{""};)
///     };
/// 	struct Surface_s
/// 	{
/// 		struct Material {
/// 			Graphics::Texture Tex;
/// 		};
/// 		Shader* MaterialShader;
/// 	};
/// 	void Submit(RenderPair _matMesh)
/// 	{
/// 
/// 	}
///		std::unordered_map<Material, uint32_t> Pairs;






/*=======================================================================================================================================================*/








//LineRenderer->Bind();
//{
//    Shader::get().SetUniform("ModelMatrix", Mat4(1.0f));
//    mainCamera.Bind();
//    Renderer::drawArray(LineVBO, QuadData.size());
//    DEBUG_CODE(CheckGLERROR());
//}
//LineRenderer->Unbind();
//LineData.clear();



// LineRenderer = new Shader(VlineRenderer, FlineRenderer);
// 
// LineVAO = OpenGL::create_VAO();
// LineVBO = OpenGL::create_VBO();
// LineRenderer->Bind();
// {// Sets up the VAO for the Lines
//     OpenGL::bind_VAO(LineVAO);
//     OpenGL::bind_VBO(LineVBO);
//     OpenGL::set_Attribute(LineRenderer->g_ID(), 2, "aPos");  
// }
// LineRenderer->Unbind();


//		std::string VlineRenderer =
//			"#version 330 core     \n\
//layout(location = 0) in vec2 aPos; \n\
//uniform mat4 ProjectionMatrix;     \n\
//uniform mat4 ViewMatrix;           \n\
//uniform mat4 ModelMatrix[100];          \n\
//void main()                        \n\
//{                                  \n\
//    mat4 ModelViewMatrix = (ViewMatrix * ModelMatrix[gl_InstanceID]);  \n\
//    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
//    gl_Position = ModelViewProjectionMatrix * vec4(aPos.x, aPos.y, -1.0, 1.0); \n\
//}";
//
//		std::string FlineRenderer =
//			"#version 330 core \n\
//out vec4 FragColor;            \n\
//void main()                    \n\
//{                              \n\
//    FragColor = vec4(1.0, 1.0, 1.0, 1.0);  \n\
//}";
