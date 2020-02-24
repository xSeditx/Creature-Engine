#include "2DRenderer.h"


namespace OpenGL
{
    
    void QuadBatch::Render()
    {
    }


    void QuadBatch::renderQuad(Vec2 _topleft, Vec2 _size, Vec4 _color)
    {

        QuadData.push_back(Vec2(_topleft.x, _topleft.y));
        QuadData.push_back(Vec2(_topleft.x + _size.x, _topleft.y));
        QuadData.push_back(Vec2(_topleft.x, _topleft.y + _size.y));
        QuadData.push_back(Vec2(_topleft.x + _size.x, _topleft.y + _size.y));
        QuadData.push_back(Vec2(_topleft.x, _topleft.y + _size.y));
        QuadData.push_back(Vec2(_topleft.x + _size.x, _topleft.y));

        ColorData.push_back(_color);
        Transform trans = Transform(Vec3(0), Vec3(0), "ModelMatrix"); //_topleft + (_size * 0.5f);
        Transforms.push_back(trans.get());
    }











    Renderer2D::Renderer2D(Vec2 _size)
    {
        CurrentRenderColor = Vec4(1, 0, 0, 1);
        mainCamera = Camera2D(_size);
        ModelMatrix = Transform(Vec3(0), Vec3(0), "ModelMatrix");

        QuadRenderer = new Shader(VquadRenderer, FquadRenderer);

        QuadVAO = OpenGL::create_VAO();
        QuadVBO = OpenGL::create_VBO();
        ColorVBO = OpenGL::create_VBO();

        QuadRenderer->Bind();
        {// Sets up the VAO for the Quads
            OpenGL::bind_VAO(QuadVAO);
            OpenGL::bind_VBO(ColorVBO);
            uint32_t A = OpenGL::set_Attribute(4, "Color");
            OpenGL::set_Divisor(A, 1);

            OpenGL::bind_VBO(QuadVBO);
            OpenGL::set_Attribute(2, "aPos");
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

        ColorData.push_back(CurrentRenderColor);
    }
    void Renderer2D::renderQuadBatch(const std::vector<Vec2> _batch)
    {
        QuadData.insert(QuadData.end(), _batch.begin(), _batch.end());
        ColorData.insert(ColorData.end(), _batch.size() / 3, CurrentRenderColor);
    }
    void Renderer2D::Render()
    {
        OpenGL::bind_VAO(QuadVAO);
        Update();
        SetRenderColor(255, 0, 0, 255);
        QuadRenderer->Bind();
        {   
            Shader::get().SetUniform("ModelMatrix", Mat4(1.0f));
            mainCamera.Bind();
            Renderer::drawArray(QuadVBO, QuadData.size());
            DEBUG_CODE(CheckGLERROR());
        }
        QuadRenderer->Unbind();
        SetRenderColor(255,255, 0, 255);

        QuadData.clear();
        ColorData.clear();
    }

    void Renderer2D::Update()
    {
        mainCamera.Update();
        OpenGL::bind_VBO(QuadVBO); 
        OpenGL::set_BufferData(QuadData);
        SetRenderColor(255, 0, 255, 255);

        OpenGL::bind_VBO(ColorVBO);
        OpenGL::set_BufferData(ColorData);
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

}//NS OpenGL





//
//
//void Renderer2D::renderLineBatch(const std::vector<Vec2> _batch)
//{
//    LineData.insert(LineData.end(), _batch.begin(), _batch.end());
//}
//
//
//ColorData.push_back(CurrentRenderColor);
//ColorData.push_back(CurrentRenderColor);
//ColorData.push_back(CurrentRenderColor);
//ColorData.push_back(CurrentRenderColor);
//ColorData.push_back(CurrentRenderColor);
//
// void Renderer2D::renderLine(Vec2 _start, Vec2 _end)
// {
//     LineData.push_back(_start);
//     LineData.push_back(_end);
// }

//		Shader* LineRenderer;
//       Vec2
//           _topleft = Vec2(30, 30),
//           _bottomright = Vec2(200.f, 200.f);
//
//       Vec2 Offset = Vec2
//       (// Later we may perform a check to ensure values are properly ordered
//           (_bottomright.x - _topleft.x) * 0.5f,
//           (_bottomright.y - _topleft.y) * 0.5f
//       );
//       Vec2 Position = { 100, 100 };
//
// VertexData.push_back({ 200.0, 200.0 });
// VertexData.push_back({ 400.0, 200.0 });
// VertexData.push_back({ 0.0, 400.0 });
// VertexData.push_back(Vec2(Position.x - Offset.x, Position.y + Offset.y));
// VertexData.push_back(Vec2(Position.x + Offset.x, Position.y - Offset.y));
// VertexData.push_back(Vec2(Position.x + Offset.x, Position.y - Offset.y));

 //       renderQuad(Vec2(100,100), Vec2(200,200));
        //==============================================================================

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

       //VertexData.push_back(Vec2(-Offset.x, +Offset.y));
       //VertexData.push_back(Vec2(-Offset.x, -Offset.y));
       //VertexData.push_back(Vec2(+Offset.x, -Offset.y));
    /// Currently Memcpy is far slower than Insert. Like 1/2 the speed
       /// size_t Sz = QuadData.size() + _batch.size();
       /// size_t Start = QuadData.size();
       /// QuadData.reserve(Sz);
       /// memcpy(&QuadData[Start ], &_batch[0], _batch.size() * sizeof(Vec2));
//  for_loop(y, 10)
//  {
//      for_loop(x, 10)
//      {
//          Transform Tr = Transform({ x,y,0 }, { 0,0,0 }, "Tr");
//          Transforms.push_back(Tr.get());
//      }
//  }