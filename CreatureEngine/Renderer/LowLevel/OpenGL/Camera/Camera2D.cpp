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
	Near = _near;
	Far = _far;

	AspectRatio = Size.x / Size.y;

	Position = Vec3(0);
	Rotation = 0;

	ProjectionMatrix = glm::ortho(Left, Right, Bottom, Top, Near, Far);  //OrthographicMatrix(_size.x, _size.y);
	ViewMatrix = glm::translate(Mat4(1.0f), Vec3(0.0f));

	set(this);
}
Camera2D::Camera2D(float _left, float _right, float _top, float _bottom)
	:
	Camera2D(_left, _right, _top, _bottom , -1.0f, 1000.0f)
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
}
void Camera2D::MoveX(float _amount)
{// Move Camera on X axis by _amount
	Position.x += _amount;
}
void Camera2D::MoveY(float _amount)
{// Move Camera on Y axis by _amount
	Position.y += _amount;
}
void Camera2D::Rotate(float _angle)
{// Rotate the Camera by _angle
	Rotation += _angle;
}
void Camera2D::Translate(Vec2 _pos)
{// Move Camera to Position _pos
	ViewMatrix = (glm::translate(glm::mat4(1.0f), Vec3(-_pos.x, -_pos.y, 1.0f)));
}

void Camera2D::Bind()
{// Binds our Camera to OpenGL context and Sets Uniforms
	Shader::get().SetUniform("ProjectionMatrix", ProjectionMatrix);
	Shader::get().SetUniform("ViewMatrix", ViewMatrix);
}
void Camera2D::Update()
{// Updates the interpolation of the Camera
	ViewMatrix = Mat4(1.0f);
	Translate(Position);
	ViewMatrix = glm::rotate(ViewMatrix, glm::radians(Rotation), Vec3(0, 0, 1.0f));
}

void Camera2D::Resize(Vec2 _size)
{
    glViewport(0, 0, (GLsizei)_size.x, (GLsizei)_size.y);
    Zoom(0.0f);
}

void Camera2D::Zoom(float _amount)
{//  Zooms in or out of a scene by manipulating the Projection Matrix
    REFACTOR("I believe it is fixed although it appears ever so slightly off on the Y axis. Perhaps I need to adjust for aspect ratio or something idk");
    ZoomLevel += (_amount / 100);
    if (ZoomLevel < 0)
    {// Prevent Projection Matrix from Inverting
        ZoomLevel = 0;  
        return;
    }

    ProjectionMatrix = glm::ortho(0.0f, Size.x, Size.y, 0.0f, Near, Far);
 	ProjectionMatrix = glm::translate(ProjectionMatrix, Vec3(Size.x * 0.5f, Size.y * 0.5f,0));
    ProjectionMatrix = glm::scale(ProjectionMatrix, Vec3(ZoomLevel, ZoomLevel, ZoomLevel));
 	ProjectionMatrix = glm::translate(ProjectionMatrix, Vec3(-(Size.x * 0.5f), -(Size.y * 0.5f), 0));
}
void Camera2D::ZoomIn(float _amount) {  Zoom(-_amount);  }
void Camera2D::ZoomOut(float _amount){  Zoom(_amount); }