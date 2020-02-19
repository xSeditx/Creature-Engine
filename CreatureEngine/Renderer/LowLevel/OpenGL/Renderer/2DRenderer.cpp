#include "2DRenderer.h"


namespace OpenGL
{

    Renderer2D::Renderer2D(Vec2 _size)
    {

        mainCamera = Camera2D(_size);
        ModelMatrix = Transform(Vec3(0), Vec3(0), "ModelMatrix");

        QuadRenderer = new Shader(VquadRenderer, FquadRenderer);
        LineRenderer = new Shader(VlineRenderer, FlineRenderer);

        QuadRenderer->Bind();
        {
            QuadVAO = OpenGL::create_VAO();
            OpenGL::bind_VAO(QuadVAO);

            QuadVBO = OpenGL::create_VBO();
            OpenGL::bind_VBO(QuadVBO);
            OpenGL::set_Attribute(QuadRenderer->g_ID(), 2, "aPos");
        }
        QuadRenderer->Unbind();
        for_loop (y, 10)
        {
            for_loop (x, 10)
            {
                Transform Tr = Transform({ x,y,0 }, { 0,0,0 }, "Tr");
                Transforms.push_back(Tr.get());
            }
        }
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

    void Renderer2D::renderLine(Vec2 _start, Vec2 _end)
    {
        LineData.push_back(_start);
        LineData.push_back(_end);
    }
    void Renderer2D::renderLineBatch(const std::vector<Vec2> _batch)
    {
        LineData.insert(LineData.end(), _batch.begin(), _batch.end());
    }

    void Renderer2D::renderQuadBatch(const std::vector<Vec2> _batch)
    {
        QuadData.insert(QuadData.end(), _batch.begin(), _batch.end());
    }
    void Renderer2D::Render()
    {
        OpenGL::bind_VAO(QuadVAO);
        Update();
        QuadRenderer->Bind();
        {   
            Shader::get().SetUniform("ModelMatrix", Mat4(1.0f));
            mainCamera.Bind();
            Renderer::drawArray(QuadVBO, QuadData.size());
            DEBUG_CODE(CheckGLERROR());
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


    void Renderer2D::Resize(Vec2 _size)
    {
        mainCamera.Resize(_size);
    }

}//NS OpenGL







        


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
