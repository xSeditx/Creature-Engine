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
	TODO("Implement a 2D Camera Abstraction");
	//FOV = 60.0;
	Near = _near;
	Far = _far;

	AspectRatio = Size.x / Size.y;

	Position = Vec3(0);
	Rotation = 0;

	ProjectionMatrix = glm::ortho(Left, Right, Bottom, Top, Near, Far);  //OrthographicMatrix(_size.x, _size.y);
	ViewMatrix = glm::translate(Mat4(1.0f), Vec3(0));

	set(this);
}



Camera2D::Camera2D(float _left, float _right, float _top, float _bottom)
	:
	Camera2D(_left, _right, _top, _bottom , -1.0f, 1000.0f)
{}
Camera2D::Camera2D(Vec2 _size)
	:
	Camera2D(0,  _size.x, 0, _size.y)
{}

void Camera2D::Move(Vec2 _amount)
{// Move Camera by _amount
	ViewMatrix = glm::translate(ViewMatrix, Vec3(-_amount.x, -_amount.y, 0));
}
void Camera2D::MoveX(float _amount)
{// Move Camera on X axis by _amount
	ViewMatrix = glm::translate(ViewMatrix, Vec3(-_amount, 0, 0));
}
void Camera2D::MoveY(float _amount)
{// Move Camera on Y axis by _amount
	ViewMatrix = glm::translate(ViewMatrix, Vec3(0, -_amount, 0));
}
void Camera2D::Rotate(float _angle)
{// Rotate the Camera by _angle
	ViewMatrix = glm::rotate(ViewMatrix, glm::radians(-_angle), Vec3(0, 0, 1));
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
	ViewMatrix = Mat4(1.0);
	Translate(Position);
}





/*=======================================================================================================================================================
/*                                               NOTES
/*=======================================================================================================================================================



/*=======================================================================================================================================================*/
