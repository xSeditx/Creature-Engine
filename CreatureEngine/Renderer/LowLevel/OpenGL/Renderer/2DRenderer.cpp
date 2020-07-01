#include "2DRenderer.h"


//GLuint DebugQuadVAO{ 0 };
//GLuint DebugQuadVBO{ 0 };
//Shader *QuadRenderer{ nullptr };
//Camera2D *debugCamera{ nullptr };




namespace OpenGL
{



    /* Constructs the Render pass object */
    RenderPass::RenderPass(int _width, int _height, Shader *_shader, GLenum _datatype, GLenum _internal, GLenum _format)
        :
        GPU_Program{ _shader }
    {
        std::vector<Vec2> VertData;
        std::vector<Vec4> ColorData;
        float MaxX{ 100 }, MaxY{ 100 };
        float grid = 1;
        float size = 100;
        for_loop(y, MaxY)
        {
            for_loop(x, MaxX)
            {
                Vec2 TL = { x* grid,y * grid };
                Vec2 BR = { x * grid + size, y * grid + size };

                VertData.push_back({ TL.x, TL.y });
                VertData.push_back({ BR.x, TL.y });
                VertData.push_back({ TL.y, BR.x });
                VertData.push_back({ BR.x ,BR.y });
                VertData.push_back({ TL.x, BR.y });
                VertData.push_back({ BR.x, TL.y });
                ColorData.push_back(OpenGL::Renderer::normalize_RGBA_Color((int)RANDOM(255), (int)RANDOM(255), (int)RANDOM(255), 255));
                ColorData.push_back(OpenGL::Renderer::normalize_RGBA_Color((int)RANDOM(255), (int)RANDOM(255), (int)RANDOM(255), 255));
                ColorData.push_back(OpenGL::Renderer::normalize_RGBA_Color((int)RANDOM(255), (int)RANDOM(255), (int)RANDOM(255), 255));
                ColorData.push_back(OpenGL::Renderer::normalize_RGBA_Color((int)RANDOM(255), (int)RANDOM(255), (int)RANDOM(255), 255));
                ColorData.push_back(OpenGL::Renderer::normalize_RGBA_Color((int)RANDOM(255), (int)RANDOM(255), (int)RANDOM(255), 255));
                ColorData.push_back(OpenGL::Renderer::normalize_RGBA_Color((int)RANDOM(255), (int)RANDOM(255), (int)RANDOM(255), 255));

            }
        }

        FBO = new FrameBufferObject(_width, _height, _datatype, _internal, _format);
        FBO->Bind();
        {
            OpenGL::set_Viewport(0, 0, _width, _height);
        }
        FBO->Unbind();

        VAO = new VertexArrayObject();
        mainCamera = new Camera2D();
        my_Mesh = new Geometry();

        GPU_Program->Bind();
        {
            VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec2>(VertData));
            VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec4>(ColorData));
        }
        GPU_Program->Unbind();
    }

    void RenderPass::Render()
    {
        Vec4 Stored_Color = OpenGL::get_ClearColor();
        // Can likely do away with Unbind in the future but for now it is needed
        FBO->Bind();
        {// Our FrameBuffer
            OpenGL::set_ClearColor(Stored_Color);
            FBO->Clear();
            GPU_Program->Bind();
            {// Our Shader
                /// GEOMETRY NEEDS TO GO HERE
                VAO->Bind();
                {
                    mainCamera->Bind();
                    //GPU_Program->SetUniform("ModelMatrix", Mat4(1.0f));

                    Renderer::drawArray(10000);
                }
                VAO->Unbind();
            }
            GPU_Program->Bind();
        }
        FBO->Unbind();
        OpenGL::set_ClearColor(Stored_Color);
    }
    void RenderPass::attach(Camera2D *_camera)
    {
        mainCamera = _camera;
    }
    void RenderPass::attach(Geometry *_mesh)
    {
        GPU_Program->Bind();
        {
            VAO->Bind();
            {
                my_Mesh = _mesh;
                VAO->Attach(BufferTypes::VERTEX, _mesh->Vertices);
                VAO->Attach(BufferTypes::COLOR, _mesh->Colors);
            }
            VAO->Unbind();
        }
    }
    void RenderPass::attach(VertexArrayObject* _vao)
    {
        VAO = _vao;
    }



    



    void RenderPass::set_Attributes(VertexBufferObject<Vec2>* _vertices, VertexBufferObject<Vec4>* _colors)
    {
        __debugbreak();
        my_Mesh->Add(_vertices);
        my_Mesh->Add(_colors);

        GPU_Program->Bind();
        {
            VAO->Attach(BufferTypes::VERTEX, _vertices);
            VAO->Attach(BufferTypes::COLOR, _colors);
        }
        GPU_Program->Unbind();
    }
    void RenderPass::update_Geometry(std::vector<Vec2> _vertices)
    {
        __debugbreak();
        my_Mesh->Vertices->Update(_vertices);
    }
    void RenderPass::Update()
    {
        __debugbreak();
        if (needs_Updated)
        {
            /// UPDATE THE BUFFERS IF WE HAVE CHANGED ANYTHING IN THEM'
        }
    }

































    Renderer2D::Renderer2D(Vec2 _size)
    {
        WARN_ME("When Initializing likely prior to this ctor even being called due to overloading the Memory_pool new/delete this fails and I can not figure out why");
        mainCamera = Camera2D(_size);

        QuadVAO      = OpenGL::new_VAO();
        QuadVBO      = OpenGL::new_VBO();
        ColorVBO     = OpenGL::new_VBO();
        TransformVBO = OpenGL::new_VBO();
        InstanceRenderer = new Shader(VinstanceRenderer, FinstanceRenderer);

        InstanceRenderer->Bind();
        {// Sets up the VAO for the Quads

            OpenGL::bind_VAO(QuadVAO);
            OpenGL::bind_VBO(QuadVBO);
            OpenGL::set_Attribute(2, "aPos");

            OpenGL::bind_VBO(TransformVBO);
            OpenGL::set_Divisor(OpenGL::set_Attribute(4, "Position"), 1 ); // Divisor, number of vertices before skipping to next Transform
            
            OpenGL::bind_VBO(ColorVBO);
            OpenGL::set_Divisor(OpenGL::set_Attribute(4, "Color"), 1);
        }
        InstanceRenderer->Unbind();

        WARN_ME("Did you mean Unbind here?");

        OpenGL::bind_VBO(QuadVBO); 
        OpenGL::set_BufferData(sizeof(QuadData), QuadData);


///     Layers.push(new Layer("Root Layer"));

        LineVAO = OpenGL::new_VAO();
        LineVBO = OpenGL::new_VBO();

        LineRenderer = new Shader(Line_shader_v, Line_shader_f);
        VBO_Test = new VertexBufferObject<Vec4>();

        LineRenderer->Bind();
        {
            OpenGL::bind_VAO(LineVAO);
        //  VBO_Test->Bind();
        //  OpenGL::bind_VBO(LineVBO);
            OpenGL::bind_VBO(VBO_Test->GL_Handle);
            OpenGL::set_Attribute(2, "Position");
        }
        LineRenderer->Unbind();
     // OpenGL::set_LineWidth(6);
    }
    void Renderer2D::renderQuad(Vec2 _topleft, Vec2 _size, Vec4 _color)
    {
        Positions.emplace_back(Vec4(_topleft.x, _topleft.y, _size.x, _size.y));
        ColorData.emplace_back(_color);
        ++InstanceCount;
    }
    void Renderer2D::renderQuad(Vec2 _topleft, Vec2 _size)
    {
        Positions.emplace_back(Vec4(_topleft.x, _topleft.y, _size.x, _size.y));
        ColorData.emplace_back(CurrentRenderColor);
        ++InstanceCount;
    }
    void Renderer2D::renderQuadBatch(const std::vector<Vec2> _batch)
    {
        ColorData.insert(ColorData.end(), _batch.size() / 6, CurrentRenderColor);
    }
    void Renderer2D::Render()
    {

        OpenGL::bind_VAO(QuadVAO);
        Update();

        InstanceRenderer->Bind();
        {
            mainCamera.Bind();
            Renderer::drawArrayInstanced(6, InstanceCount);
        }
        InstanceRenderer->Unbind();

        OpenGL::bind_VAO(LineVAO);

        LineRenderer->Bind();
        {
            mainCamera.Bind(); 
            // Renderer::drawArrayLines(LineVBO, (uint32_t)Line_Data.size() * 2);
            Renderer::drawArrayLines(VBO_Test->GL_Handle, (uint32_t)Line_Data.size() * 2);
        }
        LineRenderer->Unbind();
    }
    void Renderer2D::Flush()
    {// Clears buffer, perhaps change this idk
        __debugbreak();
        ColorData.clear();
        Positions.clear();
        Line_Data.clear();
    }
    void Renderer2D::Update()
    {
        mainCamera.Update();
 
        if (Positions.size())
        {
            OpenGL::bind_VBO(ColorVBO);
            OpenGL::set_BufferData(ColorData);

            OpenGL::bind_VBO(TransformVBO);
            OpenGL::set_BufferData(Positions);
        }

        if (Line_Data.size())
        {
            // OpenGL::bind_VBO(LineVBO);
            // OpenGL::set_BufferData(Line_Data);
            OpenGL::bind_VBO(VBO_Test->GL_Handle);
            OpenGL::set_BufferData(Line_Data);
            //*VBO_Test = Line_Data;
        }
    }
    void Renderer2D::Resize(Vec2 _size)
    {
        mainCamera.Resize(_size);
    }
    void Renderer2D::SetRenderColor(int _r, int _g, int _b, int _a)
    {
        float coef = 1.0f / 255.0f;
        CurrentRenderColor.r = _r * coef;
        CurrentRenderColor.g = _g * coef;
        CurrentRenderColor.b = _b * coef;
        CurrentRenderColor.a = _a * coef;
    }

    void Renderer2D::Submit(Shader& _shader, Texture& _texture, Mesh& _mesh)
    {
        uint32_t Shader_Index = static_cast<uint32_t>(Shaders.size());
        Texture_ID_t Texture_Index = static_cast<uint32_t>(Textures.size());
        uint32_t Mesh_Index = static_cast<uint32_t>(Meshes.size());

        Shaders.emplace_back(&_shader);
        Textures.emplace_back(&_texture);
        Meshes.emplace_back(&_mesh);

        Surface Surf;
        Surf.emplace_back( Surface_t::Diffuse, Texture_Index );

        Material Mat = { Surf, Shader_Index };
     
        Buckets.emplace_back(Mat, Mesh_Index); //  RenderPair Bucket = { Mat, Mesh_Index };

    }
    void Renderer2D::renderImage(Vec2 _pos, Vec2 _size, Texture *_image)
    {
        OpenGL::bind_VAO(DebugQuadVAO);   
        mainCamera.Update();

        shader_QuadRenderer->Bind();
        {
            mainCamera.Bind();
            shader_QuadRenderer->SetUniform("Position", _pos.x, _pos.y, _size.x, _size.y);
            shader_QuadRenderer->SetTextureUniform("DiffuseTexture", _image->g_Handle(), 1);
            Renderer::drawArray(DebugQuadVBO,  6 );
        }
        shader_QuadRenderer->Unbind();
    }

    void Renderer2D::draw_Line(float x1, float y1, float x2, float y2)
    {// Render a Line in the Current Render color as floats
        Line_Data.emplace_back(x1,y1,x2,y2);
    }
    void Renderer2D::draw_Line(Vec2 _p1, Vec2 _p2)
    {// Render a Line in the Current Render color as two Points
        Line_Data.emplace_back(_p1.x, _p1.y, _p2.x, _p2.y);
    }
    void Renderer2D::draw_Line(Vec4 _line)
    { // Render a Line in the Current Render color as Vec4
        Line_Data.emplace_back(_line.x, _line.y, _line.z, _line.w);
    }

    std::vector<Vec4> Line_Data;






    void Renderer2D::Render_Buckets()
    {

        for (auto& F : RenderPasses)
        {// This and the Following Loop are literally the same thing
            F.FBO->Bind();

            for (auto& S : Shaders)
            {
                S->Bind();
                {
                    int TextureSlot{ 0 };
                    for (auto& T : Textures)
                    {// Binds all relavent Textures. Not needed in AZDO

                        OpenGL::bind_Texture(T->g_Handle(), ++TextureSlot);

                    }// PBR Material is now bound and Active in the Shader 
                }
                for (auto& B : Buckets)
                {
                    // B.first.first.
                }
                S->Unbind();
            }
        }
    }

    RenderPass& Renderer2D::new_RenderPass(int _width, int _height, Shader *_shader, GLenum _datatype, GLenum _internal , GLenum _format )
    {
        return  RenderPasses.emplace_back(RenderPass(_width, _height,_shader, _datatype, _internal, _format)); //RenderPasses.back();
    }

}//NS OpenGL



//    using Texture_ID_t = uint32_t;
//    using Mesh_ID_t    = uint32_t;
//    using Shader_ID_t  = uint32_t;
//    using Texture_ID_t = uint32_t;
//    using Texture_ID_t = uint32_t;
//    enum  Surface_t { Diffuse, Normals, Albedo, Metallic };
//    using SurfaceFragment = std::pair   < Surface_t, Texture_ID_t >;
//    using Surface    = std::vector < SurfaceFragment         >;
//    using Material   = std::pair   < Surface, Shader_ID_t    >;
//    using RenderPair = std::pair   < Material, Mesh_ID_t     >;
//    std::vector<Shader*> Shaders;
//    std::vector<Texture*> Textures;
//    std::vector<Mesh*> Meshes;
//    std::vector<RenderPair> Buckets;


// 1:) Create Render Pass: new_RenderPass returns a Handle to the FrameBuffer Bucket and Rendering to that Pass will be done using that Handle
// 2:) Add Shaders to that Render Pass so that Geometry can be rendered using that Shader







/*
/* Sort or Bucket Visible Objects
foreach( render target )    // framebuffer
foreach( pass )             // depth, blending, etc... states
foreach( material )         // shaders
foreach( material instance) // textures, normals, diffuse,
foreach( vertex format )    // vertex buffers
foreach( object )           //
{
 WriteUniformData( object );
 glDrawElementBaseVertex(
     GL_TRIANGLES,
     object->indexCount,
     GL_UNSIGNED_SHORT,
     object->indexDataOffset,
     object->baseVertex );
}

*/









//  FrameBuffers.emplace_back(new FrameBufferObject(_width, _height, _datatype, _internal, _format));
//  return FrameBuffers.size() - 1;
