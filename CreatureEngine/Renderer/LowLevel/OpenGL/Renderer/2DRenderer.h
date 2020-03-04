#include "Renderer.h"
#include "Core/Common.h"


namespace OpenGL
{

/*     foreach(render target)    // framebuffer
        foreach(pass)             // depth, blending, etc... states

        foreach(material)         // shaders
        foreach(material instance) // textures, normals, diffuse,

        foreach(vertex format)    // vertex buffers
        foreach(object)           //
    {
        WriteUniformData(object);
        glDrawElementBaseVertex(
            GL_TRIANGLES,
            object->indexCount,
            GL_UNSIGNED_SHORT,
            object->indexDataOffset,
            object->baseVertex);
    }
 */

    struct Object
    {
        uint32_t GL_Handle;
        Object(Vec2 *_data, size_t _size)
            : 
            Size(_size), 
            Data(_data),
            VAO (OpenGL::new_VAO())
        {}
         

        void Render()
        {
            OpenGL::Renderer::drawArray((uint32_t)Size);
        }

        size_t size() { return Size; }

    private:
        uint32_t VAO;
        Vec2 *Data;
        size_t Size{ 0 };
    };
    struct Surface
    {
        Surface(Shader* _shader, Graphics::Texture _textures[4]) 
        {}

        Shader *Progam;
        std::vector<const char*> Names{"Diffuse", "Normals", "Emissive", "Specular", "Metallic", "Gloss", "Opacity"};
        std::vector<Graphics::Texture*> Textures;
    };

    struct RenderPass
    {
        FrameBufferObject *FBO;
        void Submit(Surface* _surface, Object* _mesh)
        {
            RenderPairs.insert({ _surface, _mesh });
        }
        std::unordered_map<Surface*, Object*> RenderPairs;

        void Render()
        {
            for (auto &P : RenderPairs)
            {
                uint32_t Slot{ 0 };
                Shader *Temp = P.first->Progam;

                Temp->Bind();
                {// Activate the Shader
                    for (auto &S : P.first->Textures)
                    {// Bind the Material Maps
                        Temp->SetTextureUniform(P.first->Names[Slot], S->g_Handle(), Slot++);
                    }

                    P.second->Render();
                }
                Temp->Unbind();
            }
        }
        //GPU_Program *Program;
        //std::vector<GPU_Program> ShaderPasses;
    };


	class Renderer2D
		:
		public Renderer
	{
		Camera2D mainCamera;
        
	public:
		NO_COPY_OR_ASSIGNMENT(Renderer2D);


        Graphics::Texture *TestTexture{ nullptr };
   
        using Texture_ID_t = uint32_t;
        using Mesh_ID_t = uint32_t;
        using Shader_ID_t = uint32_t;
        using Texture_ID_t = uint32_t;
        using Texture_ID_t = uint32_t;

		enum  Surface_t { Diffuse, Normals, Albedo, Metallic };
        using SurfaceFragment = std::pair<Surface_t, Texture_ID_t>;
        using Surface = std::vector<SurfaceFragment>;
        using Material = std::pair<Surface, Shader_ID_t>;
        using RenderPair = std::pair<Material, Mesh_ID_t>;

        std::vector<Shader*> Shaders;
        std::vector<Graphics::Texture*> Textures;
        std::vector<Mesh*> Meshes;
        std::vector<RenderPair> Buckets;
        void Submit(Shader& _shader, Graphics::Texture& _texture, Mesh& _mesh);

		Renderer2D() = default;
		Renderer2D(Vec2 _size);

		/* Submits a Quad using the Current Draw Color */
		void renderQuad(Vec2 _topleft, Vec2 _bottomright);

		/* Submits a Quad using the provided Draw Color */
		void renderQuad(Vec2 _topleft, Vec2 _bottomright, Vec4 _color);

		/* Accepts a precreated Batch into the Renderer
		 ~WARNING~ Possibly Deprecation soon */
		void renderQuadBatch(const std::vector<Vec2> _batch);

        /* Render every bucket to the screen Mesh + Material = Bucket */
        void Render_Buckets();

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
        void renderImage(Vec2 _pos, Vec2 _size, Graphics::Texture *_image);

    private:
		uint32_t QuadVAO{ 0 };
		uint32_t InstanceCount{ 0 };

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

        std::string VTextureRenderer =
            "#version 330 core     \n\
                layout(location = 0) in vec2 aPos; \n\
                layout(location = 1) in vec4 Position; \n\
                uniform mat4 ProjectionMatrix;     \n\
                uniform mat4 ViewMatrix;           \n\
                out  vec2 TexCoords;               \n\
                void main()                        \n\
                {                                  \n\
                    Col = Color; \n\
                    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
                    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
                    gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
                }";

        std::string FTextureRenderer =
            "#version 330 core \n\
        uniform sampler2D DiffuseTexture; \n\
        in  vec2 TexCoords;               \n\
          out vec4 FragColor;            \n\
          void main()                    \n\
          {                              \n\
                      FragColor = vec4(texture(DiffuseTexture,TexCoords.xy).xyz, 1.0);    \n\
          }";



        // vec4(Col.rgb,1.0f); 
	};// Class Renderer2D

}// NS OpenGL  


bool Init_DefaultShaders();
extern Shader *QuadRenderer;
extern GLuint DebugQuadVAO;
extern GLuint DebugQuadVBO;
extern Camera2D *debugCamera;




/*=======================================================================================================================================================
/*                                               TRASH
/*=======================================================================================================================================================




//         std::string Vrenderer =
//             "#version 330 core     \n\
// layout(location = 0) in vec2 aPos; \n\
// layout(location = 1) in vec4 Position; \n\
// layout(location = 2) in vec4 Color; \n\
// uniform mat4 ProjectionMatrix;     \n\
// uniform mat4 ViewMatrix;           \n\
// out vec4 Col;                      \n\
// void main()                        \n\
// {                                  \n\
//     Col = Color; \n\
//     mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
//     mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
//     gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
// }";
//
//         std::string Frenderer =
//             "#version 330 core    \n\
// uniform sampler2D DiffuseTexture; \n\
// in  vec2 TexCoords;               \n\
// out vec4 FragColor;               \n\
// void main()                       \n\
// {                                 \n\
//     FragColor = vec4(texture(FrameBufferTexture,TexCoords.xy).xyz, 1.0);    \n\
// }";
        //using SurfaceFragment = std::pair<Surface_t, Graphics::Texture>;
        //using Surface = std::vector<SurfaceFragment>;
        //using Material = std::pair<Surface, Shader>;
        //using RenderPair = std::pair<Material, Mesh>;








        //Instancer::get().Bind();
        //Instancer::get().Unbind();


            struct GPU_Program
    {
        GPU_Program(std::string v, std::string f)
        {
            Program = new Shader(v, f);
        }

        Shader *Program{ nullptr };
        Camera2D *Camera{ nullptr };
        static std::string VertexProgram;
        static std::string FragmentProgram;
        static GPU_Program& get()
        {
            static GPU_Program instance = GPU_Program(VertexProgram, FragmentProgram);
            return instance;
        }
        virtual void BindUniforms() = pure_virtual;
    };

             Instancer::get().Bind();
         Instancer::get().Unbind();

    struct Instancer
        : public GPU_Program
    {
        virtual void BindUniforms() override
        {
            Camera->Bind();
        }


    private:
        Instancer()
        {}
    };

    std::string Instancer::Vert =
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

    std::string Instancer::Frag =
        "#version 330 core \n\
            in vec4 Col;                   \n\
            out vec4 FragColor;            \n\
            void main()                    \n\
            {                              \n\
                FragColor = Col;\n\
            }";

*/