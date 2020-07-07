#include"Camera2D.h"
#include"../Shader/Shader.h"


Camera2D::Camera2D(float _left, float _right, float _top, float _bottom, float _near, float _far)
	:
	Top(_top),
	Left(_left),
	Right(_right),
	Bottom(_bottom),
	Size({
			_right - _left,
			_bottom - _top
		})
{
    ZoomLevel = 1.0f;
	Near = _near;
	Far = _far;

	AspectRatio = Size.x / Size.y;

	Position = Vec3(0);
	Rotation = 0;

	ProjectionMatrix = glm::ortho(Left, Right, Bottom, Top, Near, Far);  //OrthographicMatrix(_size.x, _size.y);
	ViewMatrix = glm::translate(Mat4(1.0f), Vec3(0.0f));
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

   // glViewport(0,0, (GLsizei)Right, (GLsizei)Bottom); // Dunno if I should put this here
	set(this);
}
Camera2D::Camera2D(float _left, float _right, float _top, float _bottom)
	:
	Camera2D(_left, _right, _top, _bottom , -1.0f, 10.0f)
{}
Camera2D::Camera2D(float _left, float _right)
    :
    Camera2D(0, _left, 0, _right)
{}
Camera2D::Camera2D(Vec2 _size)
	:
	Camera2D(0,  _size.x, 0, _size.y)
{}

void Camera2D::Move(Vec2 _amount)
{// Move Camera by _amount
	Position += _amount;
    Update();
}
void Camera2D::MoveX(float _amount)
{// Move Camera on X axis by _amount
	Position.x += _amount;
    Update();
}
void Camera2D::MoveY(float _amount)
{// Move Camera on Y axis by _amount
	Position.y += _amount;
    Update();
}
void Camera2D::Rotate(float _angle)
{// Rotate the Camera by _angle
	Rotation += _angle;
    Update();
}
void Camera2D::Translate(Vec2 _pos)
{// Move Camera to Position _pos
    Position = _pos;
    Update();
}


void Camera2D::Bind()
{// Binds our Camera to OpenGL context and Sets Uniforms
    Shader& Current = Shader::get();
	Shader::get().SetUniform("ProjectionMatrix", ProjectionMatrix);
	Shader::get().SetUniform("ViewMatrix", ViewMatrix);
    DEBUG_CODE(CheckGLERROR());
}
void Camera2D::Update()
{// Updates the interpolation of the Camera
    Mat4 Transform =
         
        glm::translate(Mat4(1.0f), Vec3(Position.x, Position.y, -1.0f)) *
        glm::rotate(Mat4(1.0f), Rotation, Vec3(0, 0, 1.0f));

    ViewMatrix = glm::inverse(Transform);
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void Camera2D::Resize(Vec2 _size)
{
    AspectRatio = _size.x / _size.y; 
    ZoomLevel = (ZoomLevel > 0.1f) ? ZoomLevel : 0.1f;
    float Level = 1.0f / ZoomLevel;
    float HalfH = _size.y * 0.5f;
    Size = _size;
    set_ProjectionMatrix
    (
        (-AspectRatio * Level) * HalfH,
        ( AspectRatio * Level) * HalfH,
        -Level * HalfH,
         Level * HalfH
    );
    glViewport(0,0, _size.x , _size.y );
}


void Camera2D::Zoom(float _amount)
{//  Zooms in or out of a scene by manipulating the Projection Matrix
    TODO("Make a Zoom In To Function that Zooms to an XY location regardless of specifically where the cameras Center is at that moment");
    REFACTOR("I believe it is fixed although it appears ever so slightly off on the Y axis. Perhaps I need to adjust for aspect ratio or something idk");
    ZoomLevel += (_amount / 100);
    __debugbreak();
    if (ZoomLevel < 0)
    {// Prevent Projection Matrix from Inverting
        ZoomLevel = 0;  
        return;
    }
}
void Camera2D::ZoomIn(float _amount) {  Zoom(-_amount);  }
void Camera2D::ZoomOut(float _amount){  Zoom(_amount); }
void Camera2D::ZoomInto(Vec2 _pos, float _amount)
{//  Zooms in or out of a scene by manipulating the Projection Matrix 

    ZoomLevel += (-(_amount) / 100);
    if (ZoomLevel < 0)
    {// Prevent Projection Matrix from Inverting
        ZoomLevel = 0;
        return;
    }
}
void Camera2D::ZoomOutFrom(Vec2 _pos, float _amount)
{//  Zooms in or out of a scene by manipulating the Projection Matrix 

    ZoomLevel += ((_amount) / 100);
    if (ZoomLevel < 0)
    {// Prevent Projection Matrix from Inverting
        ZoomLevel = 0;
        return;
    }
}





/*=======================================================================================================================================================
/*                                               NOTES:
/*=======================================================================================================================================================
/*=======================================================================================================================================================


Transformations:
https://www.opengl.org/archives/resources/faq/technical/transformations.htm


*/
