#include "2DRenderer.h"



GLuint DebugQuadVAO{ 0 };
GLuint DebugQuadVBO{ 0 };
Shader *QuadRenderer{ nullptr };
Camera2D *debugCamera{ nullptr };


//===============================================================================================================

std::string VQuadRenderer =
"#version 330 core     \n\
layout(location = 0) in vec2 aPos; \n\
uniform vec4 Position; \n\
uniform mat4 ProjectionMatrix;     \n\
uniform mat4 ViewMatrix;           \n\
out vec2 TexCoords; \n\
void main()                        \n\
{                                  \n\
 TexCoords = aPos; \n\
    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
    gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
}";

std::string FQuadRenderer =
"#version 330 core \n\
uniform sampler2D DiffuseTexture;\n\
in vec2 TexCoords; \n\
out vec4 FragColor;            \n\
void main()                    \n\
{                              \n\
    FragColor = texture(DiffuseTexture, TexCoords);\n\
}";



namespace OpenGL
{

    Renderer2D::Renderer2D(Vec2 _size)
    {
        mainCamera = Camera2D(_size);
        InstanceRenderer = new Shader(VinstanceRenderer, FinstanceRenderer);

        QuadVAO      = OpenGL::new_VAO();

        QuadVBO      = OpenGL::new_VBO();
        ColorVBO     = OpenGL::new_VBO();
        TransformVBO = OpenGL::new_VBO();

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
        InstanceRenderer->Bind();

        OpenGL::bind_VBO(QuadVBO); 
        OpenGL::set_BufferData(sizeof(QuadData), QuadData);



        /* Load a Test Texture to Test With */
        {
            Graphics::Bitmap *Bmp = new Graphics::Bitmap("../Resources/Test.bmp");
            TestTexture = new Graphics::Texture(*Bmp);
        }

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
            Renderer::drawArrayInstanced(6, (uint32_t)InstanceCount);
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
     
        Buckets.emplace_back(Mat, Mesh_Index); //  RenderPair Bucket = { Mat, Mesh_Index };

    }
    void  Renderer2D::Render_Buckets()
    {
        std::vector<FrameBufferObject*> FrameBuffers;

        for (auto& F : FrameBuffers)
        {// This and the Following Loop are literally the same thing
            for (auto& S : Shaders)
            {
                S->Bind();
                {
                    int TextureSlot{ 0 };
                    for (auto& T : Textures)
                    {// Binds all relavent Textures. Not needed in AZDO
                        glActiveTexture(GL_TEXTURE0 + (TextureSlot++));
                        glBindTexture(GL_TEXTURE_2D, T->g_Handle());
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


    void Renderer2D::renderImage(Vec2 _pos, Vec2 _size, Graphics::Texture *_image)
    {
        OpenGL::bind_VAO(DebugQuadVAO);   
        mainCamera.Update();

        QuadRenderer->Bind();
        {
            mainCamera.Bind();
            QuadRenderer->SetUniform("Position", _pos.x, _pos.y, _size.x, _size.y);
            QuadRenderer->SetTextureUniform("DiffuseTexture", _image->g_Handle(), 1);
            Renderer::drawArray(DebugQuadVBO,  6 );
        }
        QuadRenderer->Unbind();
    }
}//NS OpenGL



bool Init_DefaultShaders()
{
    QuadRenderer = new Shader(VQuadRenderer, FQuadRenderer);
    DebugQuadVAO = OpenGL::new_VAO();
    DebugQuadVBO = OpenGL::new_VBO();
    debugCamera = new Camera2D(SCREEN_X, SCREEN_Y);
    Vec2 QuadData[6] =
    {
        Vec2(0, 0),  Vec2(1, 0),  Vec2(0, 1),
        Vec2(1, 1),  Vec2(0, 1),  Vec2(1, 0)
    };
    QuadRenderer->Bind();
    {
        OpenGL::bind_VAO(DebugQuadVAO);

        OpenGL::bind_VBO(DebugQuadVBO);
        OpenGL::set_Attribute(2, "aPos");
        OpenGL::set_BufferData(sizeof(QuadData), QuadData);
    }
    QuadRenderer->Unbind();


    return true;
}


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