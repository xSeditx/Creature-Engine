#include"Camera2D.h"
#include"../Shader/Shader.h"



Camera2D::Camera2D(Vec2 size)
	:
	Size(size)
{
	TODO("Implement a 2D Camera Abstraction");
	//FOV = 60.0;
	AspectRatio = 640.0f / 480.0f;
	Near = -1.0f;
	Far = 1.0f;

	Position = Vec3(0);
	Rotation = 0;

	Top = 0;
	Left = 0;
	Right = size.x;
	Bottom = size.y;

//	ProjectionMatrix = glm::ortho(0.0f, size.x, size.y, 0.0f, -1.0f, 1.0f);  //OrthographicMatrix(size.x, size.y);
//	ViewMatrix = glm::translate(Mat4(1.0f), Vec3(-100, 0, 0));

	ProjectionMatrix = glm::ortho(0.0f, Right, Bottom, 0.0f, -1.0f, 100.0f);  //OrthographicMatrix(_size.x, _size.y);
	ViewMatrix = glm::translate(Mat4(1.0f), Vec3(0, 0, 0));//200, 200, 0))

	set(this);
}
Camera2D::Camera2D(float _left, float _right, float _top, float _bottom)
	:
	Top(_top),
	Left(_left),
	Bottom(_bottom),
	Right(_right),
	Size
	(
		Vec2(_right - _left, _bottom - _top)
	)
{
}

void Camera2D::Move(Vec2 _pos)
{
	ViewMatrix = glm::translate(ViewMatrix, Vec3(-_pos.x, -_pos.y, 0));
}
void Camera2D::MoveX(float _amount)
{
	ViewMatrix = glm::translate(ViewMatrix, Vec3(-_amount, 0, 0));
}
void Camera2D::MoveY(float _amount)
{
	ViewMatrix = glm::translate(ViewMatrix, Vec3(0, -_amount, 0));
}
void Camera2D::Rotate(float _angle)
{
	ViewMatrix = glm::rotate(ViewMatrix, glm::radians(-_angle), Vec3(0, 0, 1));
}
void Camera2D::Translate(Vec2 pos)
{
	ViewMatrix = (glm::translate(glm::mat4(1.0f), Vec3(-pos.x, -pos.y, 1.0f)));
}




void Camera2D::Bind()
{
	Shader::get().SetUniform("ViewMatrix", ViewMatrix);
	Shader::get().SetUniform("ProjectionMatrix", ProjectionMatrix);
}
void Camera2D::Update()
{
	ViewMatrix = Mat4(1.0);
	Translate(Position);
}
