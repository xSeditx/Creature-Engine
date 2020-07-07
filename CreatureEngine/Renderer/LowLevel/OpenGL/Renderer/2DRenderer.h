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

/// Material = Shader && Bunch of Uniforms

namespace OpenGL
{


    // ================================================================================================================
    /// EXPERIMENTAL
    // ================================================================================================================

    struct Geometry
    {
        Geometry()
        {
            Vertices = new VertexBufferObject<Vec2>();
            Colors = new VertexBufferObject<Vec4>();
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

    struct Pixel_Format
    {
        uint32_t format;
        struct SDL_Palette {}*palette;
        uint8_t BitsPerPixel{ 32 };// 8, 15, 16, 24, 32
        uint8_t BytesPerPixel{ 4 };// the number of bytes required to hold a pixel value, eg: 1, 2, 3, 4; see Remarks for related data type

        iVec4 Mask
        { // Default 32bit Mask. 
            BITS_32_25, BITS_24_17, BITS_16_9, BITS_8_1
        };

    private:
        uint8_t  Red_Shift;
        uint8_t  Green_Shift;
        uint8_t  Blue_Shift;
        uint8_t  Alpha_Shift;
    };

    struct Surface
    {
        Surface(iVec2 _size)
            :
            FBO(new FrameBufferObject(_size.x, _size.y))
        {}



        /* Blits the contents of _source with area of _srcRect onto the _destRect of this Surface */
        int blit_Surface(Surface *_source, iVec4 _srcRect, iVec4 _destRect)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, _source->Handle());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Handle());

            glBlitFramebuffer
            (
                _srcRect.x, _srcRect.y, _srcRect.z, _srcRect.w,
                _destRect.x, _destRect.y, _destRect.z, _destRect.w,
                GL_COLOR_BUFFER_BIT , GL_LINEAR
            );
        }

        /* Blits the contents of _source with area of _srcRect onto the _destRect of this Surface */
        void blit_FrameBuffer(FrameBufferObject *_fbo, iVec4 _source, iVec4 _dest)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo->GL_Handle);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Handle());

            glBlitFramebuffer
            (
                _source.x, _source.y, _source.z, _source.w,
                _dest.x, _dest.y, _dest.z, _dest.w,
                GL_COLOR_BUFFER_BIT , GL_LINEAR
            );
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }

        /* Read a Rectangle of pixels from the Surface */
        void *read_Pixels(iVec4 _sourceRect, uint32_t _format = GL_RGBA, uint32_t _type = GL_UNSIGNED_SHORT_4_4_4_4) //format = GL_ALPHA, GL_RGB, and GL_RGBA. type = GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4, or GL_UNSIGNED_SHORT_5_5_5_1.
        {/// NOTE: Should I lock this first

            //void *results = new int[_sourceRect.z * _sourceRect.w * sizeof(int)];
            //glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO->GL_Handle);
            //glReadPixels(_sourceRect.x, _sourceRect.y, _sourceRect.z, _sourceRect.w, _format, _type, results);
            //glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
            //CheckGLERROR();
            return FBO->RenderTarget->get_Pixels();
        }

        void attach_ColorBuffer(Texture *_texture)
        {
            FBO->Bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->g_Handle(), 0);
            FBO->ValidateFrameBuffer();
            FBO->Unbind();

        }
        void attach_DepthBuffer(Texture *_texture)
        {
            FBO->Bind();

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _texture->g_Handle(), 0);
            FBO->ValidateFrameBuffer();
            FBO->Unbind();
        }

        Pixel_Format Format;

        void Lock() { Locked = true; }
        void Unlock() { Locked = false; }

        void *data() { return Pixels; }

        uint32_t Pitch() { return RowSize; }
        uint32_t Width() { return FBO->Width(); }
        uint32_t Height() { return FBO->Height(); }
        uint32_t Handle() { return FBO->GL_Handle; }

        FrameBufferObject *FBO{ nullptr };

        void *User_Data;//an arbitrary pointer you can set(read - write)
    private:
        iVec4 Clip_Rect{ 0,0,0,0 };
        bool Locked{ false };// For locking and Unlocking the FrameBuffer;
        uint32_t Flags;
        int Reference_count{ 0 };/// Implement count everytime it is referenced
        int RowSize{ 0 };
        void *Pixels;
    };



    // GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT.
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

        Surface *new_Surface(iVec2 _size)
        {
            return new Surface(_size);
        }
        /*================================
                   NEEDED DATA
        ==================================*/
        bool needs_Updated{ true };

        Geometry *my_Mesh;
        Camera2D *mainCamera;
        Shader   *GPU_Program{ nullptr };
        VertexArrayObject *VAO{ nullptr };
        FrameBufferObject *FBO{ nullptr };
    };


    // ================================================================================================================
    // ================================================================================================================


    class Renderer2D
        :
        public Renderer
    {
        Camera2D *mainCamera;

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
        using Mesh_ID_t = uint32_t;
        using Shader_ID_t = uint32_t;
        using Texture_ID_t = uint32_t;
        using Texture_ID_t = uint32_t;

        enum  Surface_t { Diffuse, Normals, Albedo, Metallic };
        using SurfaceFragment = std::pair   < Surface_t, Texture_ID_t >;
        using Surface = std::vector < SurfaceFragment         >;
        using Material = std::pair   < Surface, Shader_ID_t    >;
        using RenderPair = std::pair   < Material, Mesh_ID_t     >;

        std::vector<Shader*> Shaders;
        std::vector<Texture*> Textures;
        std::vector<Mesh*> Meshes;
        std::vector<RenderPair> Buckets;
        void Submit(Shader& _shader, Texture& _texture, Mesh& _mesh);

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
        Camera2D& g_Camera() { return *mainCamera; }
        void renderImage(Vec2 _pos, Vec2 _size, Texture *_image);

        /* Render a Line in the Current Render color as floats */
        void draw_Line(float _x1, float _y1, float _x2, float _y2);

        /* Render a Line in the Current Render color as two Points*/
        void draw_Line(Vec2 _p1, Vec2 _p2);

        /* Render a Line in the Current Render color as Vec4*/
        void draw_Line(Vec4 _line);

        void Attach(Camera2D *_camera)
        {
            mainCamera = _camera;
        }
        /// ========= STATICS ========= May very well change where these are located as they are more of a Utility than a rendering option

        Shader* InstanceRenderer;
        Shader* LineRenderer;

    private:
    //    uint32_t LineVAO{ 0 };
    //    uint32_t LineVBO{ 0 };
    //    VertexBufferObject<Vec2> *VBO_Test;

        VertexArrayObject VAO_Lines;


        std::vector<Vec2> Line_Data;

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
                layout(location = 0) in vec2 WorldPosition;     \n\
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
                    gl_Position = ModelViewProjectionMatrix * vec4( (WorldPosition.x * Position.z) + Position.x, (WorldPosition.y * Position.w) +  Position.y, -1.0, 1.0); \n\
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





struct Renderer_test
{
    Renderer_test(Camera2D *_cam)
    {
        TEST_ASSERT(_cam != nullptr, "Renderer_Test needs a Camera", "");

        Camera = _cam;
        FBO = new FrameBufferObject({ Camera->Width(), Camera->Height() });
    }


    void Attach(Camera2D *_cam)
    {
        Camera = _cam;
    }



    struct Material
    {
        Material(Shader *_program, Texture* _tex)
            :
            Program( _program)
        {
            Program = new Shader(V, F);
            Diffuse = _tex;
        }

        void Bind()
        {
            Program->Bind();
            Diffuse->Bind(0);
            //Program->SetUniform("DiffuseTexture", (int));
            Program->SetTextureUniform("DiffuseTexture", Diffuse->g_Handle(), 0);
        }

        Shader *Program;
        Texture *Diffuse;

        std::string V =
            "#version 330 core                             \n\
                layout(location = 0) in vec2 Position;     \n\
                layout(location = 1) in vec4 VColor;       \n\
                layout(location = 2) in vec2 TextureCoord; \n\
                uniform mat4 ProjectionMatrix;             \n\
                uniform mat4 ViewMatrix;                   \n\
                out vec2 TexCoords;                        \n\
                out vec4 Col;                              \n\
                void main()                                \n\
                {                                          \n\
                    Col = VColor;                          \n\
                    TexCoords = TextureCoord;              \n\
                    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
                    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);  \n\
                    gl_Position = ModelViewProjectionMatrix * vec4( Position.x, Position.y, -1.0, 1.0); \n\
                }";

        std::string F =
            "#version 330 core                    \n\
                uniform sampler2D DiffuseTexture; \n\
                in  vec2 TexCoords;               \n\
                in  vec4 Col;                     \n\
                out vec4 FragColor;               \n\
                void main()                       \n\
                {                                 \n\
                    FragColor = (Col ) + vec4(texture(DiffuseTexture, TexCoords.xy).xyz, .51);  \n\
                }";

    }; //vec4 color = texture(sampler2D(DiffuseTexture), IN.TCoord.xy);vec4(texture(DiffuseTexture, TexCoords.xy).xyz, 1.0);  );  vec4(TexCoords, 0.0,1.0
    struct Geometry
    {
        Geometry(VertexBufferObject<Vec2> *_verts, VertexBufferObject<Vec4> *_colors, VertexBufferObject<Vec2> *_uv)
        {
            VAO = new VertexArrayObject();
            Verts = _verts;
            Colors = _colors;
            UVs = _uv;
            Length = _verts->size();
         }
        void Validate()
        {
            VAO->Attach(BufferTypes::VERTEX, Verts);
            VAO->Attach(BufferTypes::COLOR, Colors);
            VAO->Attach(BufferTypes::UVCOORD, UVs);
        }
        void Bind()
        {
            VAO->Bind();
        }

        // size_t Start; // Should store the Start and the Length and push all Geometry into a single buffer with the Renderer defining that
        size_t Length;
        VertexArrayObject *VAO;
        VertexBufferObject<Vec2> *UVs;
        VertexBufferObject<Vec2> *Verts;
        VertexBufferObject<Vec4> *Colors;
    };

    Camera2D *Camera;
    FrameBufferObject *FBO;

    std::vector<Material *> Materials;
    std::vector<Geometry *> Models;

    void Submit(Geometry *_mesh, Material *_mat)
    {
        
        uint8_t MatIndex{ 0 }, MeshIndex{ 0 };
        bool found = false;
        for_loop(i, Materials.size())
        {
            found = (Materials[i] == _mat);
            if (found)
            {
                break;
            }

            ++MatIndex;
        }
        if (!found)
        {// Create New Entry for the Model and the Material
            Materials.push_back(_mat);
            Models.push_back(_mesh);
            Materials.back()->Bind();
            Models.back()->Validate();
           // Buckets.push_back(std::vector<uint32_t>());
        }
        else
        {// It was found Append the Mesh data to the Material Bucket
            Materials[MatIndex]->Bind();
            _mesh->Validate();

            Models[MatIndex]->Bind();
            Models[MatIndex]->VAO->Append(_mesh->VAO);
            Models[MatIndex]->Length += _mesh->Length;
        }
        
    }
    void Render()
    {
        int i=0, j=0;
        FBO->Bind(); // Bind the FrameBuffer
        {
            FBO->Clear();

            for_loop(i, Materials.size())
            {
                Materials[i]->Bind();
                Models[i]->Bind();
                Camera->Bind(); 
                OpenGL::Renderer::drawArray((uint32_t)Models[i]->Length);
            }

        }
        FBO->Unbind();
    }
};


std::vector<Vec2> make_Quad(Vec2 _center, Vec2 _sz);
std::vector<Vec2> make_Triangle(Vec2 _center, Vec2 _sz);

/*=======================================================================================================================================================
/*                                               TRASH
/*=======================================================================================================================================================
            //    Materials[i]->Bind();
            //    for_loop(j, Buckets[i].size())
            //    {
            //        auto B = Buckets[i];
            //        Models[B[j]]->Bind();
            //        Camera->Bind();            // Bind ProjectionMatrix and ViewMatrix
            //        OpenGL::Renderer::drawArray((uint32_t)Models[B[j]]->Length);
            //    }
            //}
        //auto Found = std::find(Materials.begin(), Materials.end(), _mat);
        //if (Found != Materials.end())
        //{// Found it
        //
        //}
        //else
        //{// New
        //
        //}
        //_mat->Bind();
        //_mesh->Bind();
        //Vec2 * MeshPTR = (Vec2*)_mesh->VAO->Buffers[0]->Map(GL_READ_BUFFER);
        // Check to see if the Material is already here, if it is use that Index, if not add it

        // Check to see if the Mesh is already here, if it is use that Index, if not add it

        //Buckets[MatIndex].push_back((uint32_t)Models.size() - 1);
//  Buckets
//  (
//      {
//          MatIndex,//(uint32_t)Materials.size()- 1
//          (uint32_t)Models.size() - 1
//      }
//  ); /
//
//
//  Buckets.push_back
//  (
//      {
//          MatIndex,//(uint32_t)Materials.size()- 1
//          (uint32_t)Models.size()   - 1
//      }
//  ); // Adds the Index of the Material and the Index of the Model to the Renderer
//

//for (auto&B : Buckets)
//{
//    j = 0;
//    for (auto &M : Buckets[i])
//    {
//        M[j]->Bind();
//        ++j;
//    }
//    ++i;
//    //Materials[B.first]->Bind();// Binds Shaders, Textures, Material Uniforms
//   // Models[B.second]->Bind();  // Binds VAO, VBOs
//    Camera->Bind();            // Bind ProjectionMatrix and ViewMatrix
//    OpenGL::Renderer::drawArray((uint32_t)Models[B.second]->Length);
//}
    //The mask​ parameter is a bitfield that specifies which kinds of buffers you want copied
    //Filter = GL_NEAREST or GL_LINEAR.
    //mask =    The bitwise OR of the flags indicating which buffers are to be copied.The allowed flags are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT.
    //uint32_t Red_Mask{ BIT_32 | BIT_31 | BIT_30 | BIT_29 | BIT_28 | BIT_27 | BIT_26 | BIT_25 };
    //uint32_t Green_Mask{ BIT_24 | BIT_23 | BIT_22 | BIT_21 | BIT_20 | BIT_19 | BIT_18 | BIT_17 };
    //uint32_t Blue_Mask{ BIT_16 | BIT_15 | BIT_14 | BIT_13 | BIT_12 | BIT_11 | BIT_10 | BIT_9 };
    //uint32_t Alpha_Mask{ BIT_8 | BIT_7 | BIT_6 | BIT_5 | BIT_4 | BIT_3 | BIT_2 | BIT_1 };
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

//using SurfaceFragment = std::pair<Surface_t, Texture>;
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
        Surface(Shader* _shader, Texture _textures[4])
        {}

        Shader *Progam;
        std::vector<const char*> Names{"Diffuse", "Normals", "Emissive", "Specular", "Metallic", "Gloss", "Opacity"};
        std::vector<Texture*> Textures;
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
//Models[MatIndex]->Validate();
//Models[MatIndex]->VAO->Buffers[0]->Append(_mesh->VAO->Buffers[0]->BufferPtr, _mesh->VAO->Buffers[0]->Size);
//Models[MatIndex]->VAO->Buffers[1]->Append(_mesh->VAO->Buffers[1]->BufferPtr, _mesh->VAO->Buffers[1]->Size);
//Models[MatIndex]->VAO->Buffers[2]->Append(_mesh->VAO->Buffers[2]->BufferPtr, _mesh->VAO->Buffers[2]->Size);
//Models[MatIndex]->VAO->Buffers[0]->Update
//  Buckets[MatIndex].front()       //push_back((uint32_t)Models.size() - 1);
