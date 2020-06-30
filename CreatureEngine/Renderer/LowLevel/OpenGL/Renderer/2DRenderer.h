#pragma once

#include "Renderer.h"
#include "Core/Common.h"
#include"GLBuffers.h"

/// Think I should add my Layers here and make the Renderer handle the Layer. The Application can have a Renderer
/// And that Renderer handles the layers and the reponse to Messages passed to it. I might even use my Pub/Sub setup
/// On a Modified variation of my Layers Module.
    
        //  uint32_t new_RenderPass(int _width, int _height, GLenum _datatype = GL_FLOAT, GLenum _internal = GL_RGBA32F, GLenum _format = GL_RGBA);
         // uint32_t new_RenderPass(int _width, int _height, GLenum _datatype, GLenum _internal, GLenum _format);
          //std::vector<FrameBufferObject*> FrameBuffers;


namespace OpenGL
{


    // ================================================================================================================
    /// EXPERIMENTAL
    // ================================================================================================================

    struct Geometry
    {
        Geometry()
        {
            Vertices = new VertexBufferObject<Vec2> ();
            Colors   = new VertexBufferObject<Vec4> ();
        }
        Geometry(VertexBufferObject<Vec2> _vertices, VertexBufferObject<Vec4> _colors)
            :
            Vertices(&_vertices),
            Colors(&_colors)
        {}

        void Add(VertexBufferObject<Vec2>* _vertices)
        {
            Vertices = _vertices;
        }
        void Add(VertexBufferObject<Vec4>* _colors)
        {
            Colors = _colors;
        }

        VertexBufferObject<Vec2> *Vertices;
        VertexBufferObject<Vec4> *Colors;
    };

    struct RenderPass
    {

        /* Constructs the Render pass object */
        RenderPass(int _width, int _height, Shader *_shader, GLenum _datatype = GL_FLOAT, GLenum _internal = GL_RGBA32F, GLenum _format = GL_RGBA);

        void set_Attributes(VertexBufferObject<Vec2>* _vertices, VertexBufferObject<Vec4>* _colors);
        /* Updates the Vertices for the Render Scene */
        void update_Geometry(std::vector<Vec2> _vertices);

        /* Updates all the buffers and Cameras if the User calls for it */
        void Update();
        /* Executes the Render Pass. NOTE: Might Change the name to Execute */
        void Render();

        /* Adds a Camera to the Current RenderPass */
        void attach(Camera2D *_camera);

        /* Adds a Mesh to the Current RenderPass */
        void attach(Geometry *_mesh);

        /* Attaches an ArrayObject Format. Likely combine this with the Shader and pass it in differently */
        void attach(VertexArrayObject* _vao);


        /*================================
                   NEEDED DATA 
        ==================================*/
        bool needs_Updated{ true };

        Geometry *my_Mesh;
        Camera2D *mainCamera;
        Shader   *GPU_Program { nullptr };
        VertexArrayObject *VAO{ nullptr };
        FrameBufferObject *FBO{ nullptr };
    };


    // ================================================================================================================
    // ================================================================================================================


	class Renderer2D
		:
		public Renderer
	{
		Camera2D mainCamera;
        
	public:

        // ================================================================================================================
        /// EXPERIMENTAL
        // ================================================================================================================
        // Maybe default the shader as well? That could make things easier and could eliminate need for other default shaders all over the code
        RenderPass& new_RenderPass(int _width, int _height, Shader *_shader, GLenum _datatype = GL_FLOAT, GLenum _internal = GL_RGBA32F, GLenum _format = GL_RGBA);
        std::vector<RenderPass> RenderPasses;

        // ================================================================================================================



		NO_COPY_OR_ASSIGNMENT(Renderer2D);

       // layerStack Layers;

        using Texture_ID_t = uint32_t;
        using Mesh_ID_t    = uint32_t;
        using Shader_ID_t  = uint32_t;
        using Texture_ID_t = uint32_t;
        using Texture_ID_t = uint32_t;

		enum  Surface_t { Diffuse, Normals, Albedo, Metallic };
        using SurfaceFragment = std::pair   < Surface_t, Texture_ID_t >;
        using Surface         = std::vector < SurfaceFragment         >;
        using Material        = std::pair   < Surface, Shader_ID_t    >;
        using RenderPair      = std::pair   < Material, Mesh_ID_t     >;

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

		/* Clears the Buffers */
		void Flush();

		/* Might eliminate these in near future idk yet*/
        void SetRenderColor(int _r, int _g, int _b, int _a);

		/* Changes the size of the Renderer */
        void Resize(Vec2 _size);

		/* Gets the Camera the Renderer uses */
		Camera2D& g_Camera() { return mainCamera; }
        void renderImage(Vec2 _pos, Vec2 _size, Graphics::Texture *_image);

        /* Render a Line in the Current Render color as floats */
        void draw_Line(float _x1, float _y1, float _x2, float _y2);

        /* Render a Line in the Current Render color as two Points*/
        void draw_Line(Vec2 _p1, Vec2 _p2);

        /* Render a Line in the Current Render color as Vec4*/
        void draw_Line(Vec4 _line);


        /// ========= STATICS ========= May very well change where these are located as they are more of a Utility than a rendering option

        Shader* InstanceRenderer;
        Shader* LineRenderer;

    private:
        uint32_t LineVAO{ 0 };
        uint32_t LineVBO{ 0 };
        VertexBufferObject<Vec4> *VBO_Test;

        std::vector<Vec4> Line_Data;

		uint32_t QuadVAO{ 0 };
		uint32_t InstanceCount{ 0 };


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

        // ===============================================================================================================

        std::string  Line_shader_v = 
            "#version 330 core                                                                          \n\
                 layout(location = 0) in vec2 Position;                                                 \n\
                 uniform mat4 ProjectionMatrix;                                                         \n\
                 uniform mat4 ViewMatrix;                                                               \n\
                 void main()                                                                            \n\
                 {                                                                                      \n\
                     mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));                                   \n\
                     mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);             \n\
                     gl_Position = ModelViewProjectionMatrix * vec4(Position.x, Position.y, -1.0, 1.0); \n\
                 }";


        std::string Line_shader_f = 
            "#version 330 core                              \n\
                 out vec4 FragColor;                        \n\
                 void main()                                \n\
                 {                                          \n\
                     FragColor = vec4(0.0, 1.0, 1.0, 1.0);  \n\
                 }";
                 


        // ================================================================================================================

		std::string VinstanceRenderer =
            "#version 330 core                         \n\
                layout(location = 0) in vec2 aPos;     \n\
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


        // ================================================================================================================
        std::string VTextureRenderer =
            "#version 330 core                         \n\
                layout(location = 0) in vec2 aPos;     \n\
                layout(location = 1) in vec4 Position; \n\
                uniform mat4 ProjectionMatrix;         \n\
                uniform mat4 ViewMatrix;               \n\
                out  vec2 TexCoords;                   \n\
                void main()                            \n\
                {                                      \n\
                    Col = Color;                       \n\
                    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
                    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);  \n\
                    gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
                }";
        std::string FTextureRenderer =
            "#version 330 core                    \n\
                uniform sampler2D DiffuseTexture; \n\
                in  vec2 TexCoords;               \n\
                out vec4 FragColor;               \n\
                void main()                       \n\
                {                                 \n\
                    FragColor = vec4(texture(DiffuseTexture,TexCoords.xy).xyz, 1.0);  \n\
                }";

    };// Class Renderer2D
}// NS OpenGL  












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












/*


foreach(render target)         // framebuffer
    foreach(pass)              // depth, blending, etc... states

    foreach(material)          // shaders
    foreach(material instance) // textures, normals, diffuse,

    foreach(vertex format)     // vertex buffers
    foreach(object)            //
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
///  struct Texture;
///  struct Material
///  {
///      std::vector<Vec2> UV_list;
///      std::vector<Texture*> Texture_list;
///      std::vector<Texture*> Normal_list;
///      std::vector<Texture*> Shine_list;
///      std::vector<Texture*> Texture_list;
///      std::vector<Texture*> Texture_list;
///  };
///  struct Program
///  {   
///      std::vector<Material> Material_list;
///      std::vector<VertexArrayObject> VAOs;
///      std::vector<VertexBufferObject<Vec2>> Geometry; 
///  };
///  struct Surface
///  {
///      std::vector<Program> Shader_Programs;
///  };









/// 6/25/ 2020
/// NEED TO CLEAN UP THE  2D RENDERER CLASS TO IMPLEMENT REAL SOLUTIONS

    /*
    template<typename _Ty>
    struct Geometry
    {
        Geometry(_Ty *_data, size_t _sz)
            :
            GL_Handle(OpenGL::new_VBO()),
            Vertices(_data),
            ElementCount(_sz)
        {}
        Geometry(size_t _sz)
            :
            GL_Handle(OpenGL::new_VBO()),
            Vertices(new _Ty[_sz]),
            ElementCount(_sz)
        {}
        Geometry(std::vector<_Ty>& _data)
            :
            GL_Handle(OpenGL::new_VBO()),
            Vertices(_data),
            ElementCount(_data.size())
        {}


        void set_Data(std::vector<_Ty> _data)
        {
            ElementCount = _data.size();
            OpenGL::set_BufferData(_data);

        }
        void set_Data(_Ty* _data, size_t _sz)
        {
            ElementCount = _sz;
            OpenGL::bind_VBO(GL_Handle);
            OpenGL::set_BufferData( _data, _sz);
        }

        void Bind()
        {
            OpenGL::bind_VBO(GL_Handle);
        }
        void Unbind()
        {
            OpenGL::bind_VBO(0);
        }


        // Number of Elements in the Vertex Array
        size_t count() { return ElementCount; }

        // Returns size in bytes of Vertex Data
        size_t size() { return ElementCount * sizeof(_Ty); }

        // Returns pointer to Start of Vertices
        _Ty* data_Ptr() { return Vertices; }
    private:
        uint32_t GL_Handle{ 0 };

        _Ty *Vertices;
        size_t ElementCount;
    };
    struct GPU_MemoryPool
    {
        // Create a pool of Memory on the GPU
        GPU_MemoryPool(uint32_t _size)
        {
            GL_Handle = OpenGL::new_VBO();
            OpenGL::bind_VBO(GL_Handle);
            OpenGL::set_BufferData(_size, nullptr);
        }
        // Make memory buffer current in OpenGL state
        void Bind()
        {
            OpenGL::bind_VBO(GL_Handle);
        }

        void *Map()
        {
            return glMapBuffer(Target, GL_READ_BUFFER);
        }
        void *MapRange(uint32_t _start, uint32_t _length)
        {//target offset length access
            return glMapBufferRange(Target, _start, _length, GL_READ_BUFFER);
        }
        uint32_t GL_Handle{ 0 };
        uint32_t Target{ GL_ARRAY_BUFFER };
    };
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

/*
        uint32_t size() { return Length; }
        char* get() { return Data; }

    private:
        uint32_t Start{ 0 };
        uint32_t Length{ 0 };
        char *Data;


    private:
        uint32_t Start{ 0 };
        uint32_t Length{ 0 };


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

*/