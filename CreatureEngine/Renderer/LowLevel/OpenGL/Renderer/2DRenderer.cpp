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

}//NS OpenGL
