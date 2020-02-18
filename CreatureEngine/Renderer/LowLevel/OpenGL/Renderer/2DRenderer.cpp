#include "2DRenderer.h"


namespace OpenGL
{

    Renderer2D::Renderer2D(Vec2 _size)
    {
        mainCamera = Camera2D(_size);
        ModelMatrix = Transform(Vec3(0), Vec3(0), "ModelMatrix");
        
        QuadRenderer = new Shader(vRenderer, fRenderer);
        QuadRenderer->Bind();  
        {
            QuadVAO = OpenGL::create_VAO();
            OpenGL::bind_VAO(QuadVAO);

            QuadVBO = OpenGL::create_VBO();
            OpenGL::bind_VBO(QuadVBO);
            OpenGL::set_Attribute(QuadRenderer->g_ID(),2,"aPos");
        }
        QuadRenderer->Unbind();
    }

    void Renderer2D::renderQuad(Vec2 _topleft, Vec2 _size)
    {
        QuadData.push_back(Vec2(_topleft.x          , _topleft.y          ));
        QuadData.push_back(Vec2(_topleft.x + _size.x, _topleft.y          ));
        QuadData.push_back(Vec2(_topleft.x          , _topleft.y + _size.y));
        QuadData.push_back(Vec2(_topleft.x + _size.x, _topleft.y + _size.y));
        QuadData.push_back(Vec2(_topleft.x          , _topleft.y + _size.y));
        QuadData.push_back(Vec2(_topleft.x + _size.x, _topleft.y          ));
    }

    void Renderer2D::Render()
    {
        OpenGL::bind_VAO(QuadVAO);
        Update();
        QuadRenderer->Bind();
        {   
            ModelMatrix.Bind();
            mainCamera.Bind();
            Renderer::drawArray(QuadVBO, QuadData.size()); 
        }
        QuadRenderer->Unbind();

        QuadData.clear();
    }
    void Renderer2D::Update()
    {
        mainCamera.Update();
        OpenGL::bind_VBO(QuadVBO); 
        OpenGL::set_BufferData(QuadData);
    }
}//NS OpenGL











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

//VertexData.push_back({ 200.0, 200.0 });
//VertexData.push_back({ 400.0, 200.0 });
//VertexData.push_back({ 0.0, 400.0 });
//VertexData.push_back(Vec2(Position.x - Offset.x, Position.y + Offset.y));
//VertexData.push_back(Vec2(Position.x + Offset.x, Position.y - Offset.y));
//VertexData.push_back(Vec2(Position.x + Offset.x, Position.y - Offset.y));
//OpenGL::set_BufferData(QuadData);
//VertexData.push_back(Vec2(-Offset.x, +Offset.y));
//VertexData.push_back(Vec2(-Offset.x, -Offset.y));
//VertexData.push_back(Vec2(+Offset.x, -Offset.y));
//Vec2
//_topleft = Vec2(30, 30),
//_bottomright = Vec2(200.f, 200.f);
//Vec2 Offset = Vec2
//(// Later we may perform a check to ensure values are properly ordered
//(_bottomright.x - _topleft.x) * 0.5f,
//(_bottomright.y - _topleft.y) * 0.5f
//);
//Vec2 Position = { 100, 100 };
//renderQuad(Vec2(100, 100), Vec2(200, 200));
