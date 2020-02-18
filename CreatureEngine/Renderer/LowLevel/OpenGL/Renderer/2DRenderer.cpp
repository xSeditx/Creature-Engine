#include "2DRenderer.h"


namespace OpenGL
{

    Renderer2D::Renderer2D(Vec2 _size)
    {
        Vec2
            _topleft = Vec2(30, 30),
            _bottomright = Vec2(200.f, 200.f);

        Vec2 Offset = Vec2
        (// Later we may perform a check to ensure values are properly ordered
            (_bottomright.x - _topleft.x) * 0.5f,
            (_bottomright.y - _topleft.y) * 0.5f
        );
        Vec2 Position = { 100, 100 };

        VertexData.push_back({ 200.0, 200.0 });
        VertexData.push_back({ 400.0, 200.0 });
        VertexData.push_back({ 0.0, 400.0 });
        VertexData.push_back(Vec2(Position.x - Offset.x, Position.y + Offset.y));
        VertexData.push_back(Vec2(Position.x + Offset.x, Position.y - Offset.y));
        VertexData.push_back(Vec2(Position.x + Offset.x, Position.y - Offset.y));

        renderQuad(Vec2(100,100), Vec2(200,200));
        //==============================================================================

        mainCamera = Camera2D(_size);
        ModelMatrix = Transform(Vec3(0), Vec3(0), "ModelMatrix");
        
        QuadRenderer = new Shader(vRenderer, fRenderer);
        QuadRenderer->Bind(); // glUseProgram(QuadRenderer->g_ID());
        {
            QuadVAO = OpenGL::create_VAO();
            OpenGL::bind_VAO(QuadVAO);

            QuadVBO = OpenGL::create_VBO();
            OpenGL::bind_VBO(QuadVBO);
            OpenGL::set_BufferData(VertexData);
            OpenGL::set_Attribute(QuadRenderer->g_ID(),2,"aPos");
        }
        QuadRenderer->Unbind();


        CheckGLERROR();
    }

    void Renderer2D::renderQuad(Vec2 _topleft, Vec2 _size)
    {
        VertexData.push_back(Vec2(_topleft.x          , _topleft.y          ));
        VertexData.push_back(Vec2(_topleft.x + _size.x, _topleft.y          ));
        VertexData.push_back(Vec2(_topleft.x          , _topleft.y + _size.y));

        VertexData.push_back(Vec2(_topleft.x + _size.x, _topleft.y + _size.y));
        VertexData.push_back(Vec2(_topleft.x          , _topleft.y + _size.y));
        VertexData.push_back(Vec2(_topleft.x + _size.x, _topleft.y          ));

        



        //Vec2 Offset = Vec2
       //(// Later we may perform a check to ensure values are properly ordered
       //    (_size.x - _topleft.x) * 0.5f,
       //    (_size.y - _topleft.y) * 0.5f
       //);
       //
       //VertexData.push_back(Vec2(-Offset.x, +Offset.y));
       //VertexData.push_back(Vec2(-Offset.x, -Offset.y));
       //VertexData.push_back(Vec2(+Offset.x, -Offset.y));
       //VertexData.push_back(Vec2(-Offset.x, +Offset.y));
       //VertexData.push_back(Vec2(+Offset.x, -Offset.y));
       //VertexData.push_back(Vec2(+Offset.x, -Offset.y));
    }


    void Renderer2D::Render()
    {
        OpenGL::bind_VAO(QuadVAO);
        QuadRenderer->Bind();
        {  // Print("Active Shader: " << Shader::get_CurrentShader());
            Shader::get().SetUniform("ModelMatrix", Mat4(1.0f));
            mainCamera.Bind();
            Renderer::drawArray(QuadVBO, VertexData.size()); //(uint32_t)();
        }
        QuadRenderer->Unbind();

        VertexData.clear();
    }
    void Renderer2D::Update()
    {
        mainCamera.Update();
        OpenGL::bind_VBO(QuadVBO); 
        renderQuad(Vec2(100, 100), Vec2(200, 200));
        renderQuad(Vec2(400, 400), Vec2(10, 200));
        OpenGL::set_BufferData(VertexData);

    }


    void Renderer2D::Resize(Vec2 _size)
    {
        mainCamera.Resize(_size);
    }

}//NS OpenGL








       //VertexData.push_back(Vec2(-Offset.x, +Offset.y));
       //VertexData.push_back(Vec2(-Offset.x, -Offset.y));
       //VertexData.push_back(Vec2(+Offset.x, -Offset.y));
