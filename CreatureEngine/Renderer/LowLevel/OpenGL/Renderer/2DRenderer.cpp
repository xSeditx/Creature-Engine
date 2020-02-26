#include "2DRenderer.h"


namespace OpenGL
{

    Renderer2D::Renderer2D(Vec2 _size)
    {
        mainCamera = Camera2D(_size);
        InstanceRenderer = new Shader(VinstanceRenderer, FinstanceRenderer);

        QuadVAO      = OpenGL::create_VAO();
        QuadVBO      = OpenGL::create_VBO();
        ColorVBO     = OpenGL::create_VBO();
        TransformVBO = OpenGL::create_VBO();

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

        OpenGL::bind_VBO(QuadVBO); 
        OpenGL::set_BufferData(sizeof(QuadData), QuadData);
    }
    void Renderer2D::renderQuad(Vec2 _topleft, Vec2 _size, Vec4 _color)
    {
        ++InstanceCount;
        Positions.emplace_back(Vec4(_topleft.x, _topleft.y, _size.x, _size.y));
        ColorData.emplace_back(_color);
    }
    void Renderer2D::renderQuad(Vec2 _topleft, Vec2 _size)
    {
        ++InstanceCount;
        Positions.emplace_back(Vec4(_topleft.x, _topleft.y, _size.x, _size.y  ));
        ColorData.emplace_back(CurrentRenderColor);
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
    }
    void Renderer2D::Flush()
    {// Clears buffer, perhaps change
        ColorData.clear();
        Positions.clear();
    }
    void Renderer2D::Update()
    {
        mainCamera.Update();
 
        OpenGL::bind_VBO(ColorVBO);
        OpenGL::set_BufferData(ColorData);

        OpenGL::bind_VBO(TransformVBO);
        OpenGL::set_BufferData(Positions);
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
    Vec4 Renderer2D::CreateColor(int _r, int _g, int _b, int _a)
    {
        float coef = 1.0f / 255.0f;
        Vec4 result;
        result.r = _r * coef;
        result.g = _g * coef;
        result.b = _b * coef;
        result.a = _a * coef;
        return result;
    }





    /*
    
    	enum  Surface_t { Normals, Albedo, Metallic };

        using SurfaceFragment = std::pair<Surface_t, Texture_ID_t>;
        using Surface = std::vector<SurfaceFragment>;
        using Material = std::pair<Surface, Shader_ID_t>;
        using RenderPair = std::pair<Material, Mesh_ID_t>;

        std::vector<Shader*> Shaders;
        std::vector<Graphics::Texture*> Textures;
        std::vector<Mesh*> Meshes;
    */

    void Renderer2D::Submit(Shader& _shader, Graphics::Texture& _texture, Mesh& _mesh)
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
        RenderPair Bucket = { Mat, Mesh_Index };
    }


    void  Renderer2D::Render_Buckets()
    {
        std::vector<FrameBufferObject*> FrameBuffers;

        for (auto& F : FrameBuffers)
        {// This and the Following Loop are literally the same thing
            for (auto& S : Shaders)
            {
                int TextureSlot{ 0 };
                for (auto& T : Textures)
                {// Binds all relavent Textures. Not needed in AZDO
                    glActiveTexture(GL_TEXTURE0 + TextureSlot);
                    glBindTexture(GL_TEXTURE_2D, T->g_Handle());
                    ++TextureSlot;
                }
            }
        }
    }

}//NS OpenGL

/*



for(auto& S: Shaders)
        {
            for(auto& T: Textures)
            { }
        }
*/



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