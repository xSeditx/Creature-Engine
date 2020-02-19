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

		//Renderer2D(Renderer2D&& _other) = default;
	//	Renderer2D& operator=(Renderer2D&& _other) = default;

		//{}
		//{
		//	return _other;
		//}
		// 
		std::vector<Mat4> Transforms;

		enum  Surface_t { Normals, Albedo, Metallic };
		using SurfaceFragment = std::pair<Surface_t, Graphics::Texture>;
		using Surface = std::vector<SurfaceFragment>;
		using Material = std::pair<Surface, Shader>;
		using RenderPair = std::pair<Material, Mesh>;


		std::vector<Vec2> QuadData;
		std::vector<Vec2> LineData;

		uint32_t QuadVBO{ 0 };
		uint32_t QuadVAO{ 0 };

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
    private:


		std::string VquadRenderer =
			"#version 330 core     \n\
layout(location = 0) in vec2 aPos; \n\
uniform mat4 ProjectionMatrix;     \n\
uniform mat4 ViewMatrix;           \n\
uniform mat4 ModelMatrix;          \n\
void main()                        \n\
{                                  \n\
    mat4 ModelViewMatrix = (ViewMatrix * ModelMatrix);  \n\
    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
    gl_Position = ModelViewProjectionMatrix * vec4(aPos.x, aPos.y, -1.0, 1.0); \n\
}";

		std::string FquadRenderer =
			"#version 330 core \n\
out vec4 FragColor;            \n\
void main()                    \n\
{                              \n\
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);  \n\
}";



		std::string VlineRenderer =
			"#version 330 core     \n\
layout(location = 0) in vec2 aPos; \n\
uniform mat4 ProjectionMatrix;     \n\
uniform mat4 ViewMatrix;           \n\
uniform mat4 ModelMatrix[100];          \n\
void main()                        \n\
{                                  \n\
    mat4 ModelViewMatrix = (ViewMatrix * ModelMatrix[gl_InstanceID]);  \n\
    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
    gl_Position = ModelViewProjectionMatrix * vec4(aPos.x, aPos.y, -1.0, 1.0); \n\
}";

		std::string FlineRenderer =
			"#version 330 core \n\
out vec4 FragColor;            \n\
void main()                    \n\
{                              \n\
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);  \n\
}";
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


/*=======================================================================================================================================================*/
