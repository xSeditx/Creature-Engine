#pragma once

#include<stack>
//#include"Renderer.h"
#include"../../../../Core/Common.h"


class Viewport
{
public:
	Viewport() = default;

	float
		FOV = 60,
		AspectRatio,
		Near,
		Far;

	void PushProjectionMatrix();
	void PushViewMatrix();
	void PopProjectionMatrix();
	void PopViewMatrix();

	Mat4 ProjectionMatrix = Mat4(0);
	Mat4 ViewMatrix = Mat4(0);
	Mat4 Identity = Mat4(1.0f);
	//=======================================================================================================================================================
	// FUNCTIONS FOR THE CAMERA CLASS
	//=======================================================================================================================================================

	Mat4 GetViewMatrix();

	//=======================================================================================================================================================

	void Set_ViewMatrix(Mat4 mat);
	void Set_ProjectionMatrix(Mat4 mat);

	static Viewport *Camera;
	static void SetCamera(Viewport* _cam) { Camera = _cam; }
private:

	std::stack<Mat4> ViewMatrixStack;
	std::stack<Mat4> ProjectionMatrixStack;
};


Mat4 OrthographicMatrix(int width, int height);

class Camera3D 
	: public Viewport
{
public:
	Camera3D() = default;
	Camera3D(Vec3 position, Vec3 Rotation);

	Mat4 LookAt(Vec3 eye, Vec3 target, Vec3 up);

	void RotateX(GLfloat Angle);
	void RotateY(GLfloat Angle);
	void RotateZ(GLfloat Angle);
	void Translate(Vec3 pos);

	void Rotate(float pitch, float yaw);
	void MoveBack(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveForward(float speed);
	Vec3
		Position = { 0,0,0 },
		Rotation = { 0,0,0 },
		Forward = { 0,0,0 },
		Right = { 0,0,0 },
		Up = { 0,0,0 };

	Vec3
		Target_Position = Vec3(0),
		Target_Rotation = Vec3(0);

	Vec3 Camera_Direction = Vec3(0);
	float Delta_Time = 0.0f;
	float Current_Time = 0.0f;

	void Update();
	void Bind();
	void Bind(Shader* _shader);

	void Unbind();
	void Render();
};

class Camera2D 
	: public Viewport
{
public:
	Camera2D() = default;
	Camera2D(Vec2 size);
	Camera2D(float _left, float _right, float _top, float _bottom);
	Vec2 Position;
	float Rotation;

	void Update();
	void Bind(Shader *_shader);

	float
		Top,
		Bottom,
		Right,
		Left;

	void Rotate(float _angle);
	const int Height() { return Size.y; }
	const int Width() { return Size.x; }

	void Translate(Vec2 pos);
private:
	Vec2 Size;
};




/*=======================================================================================================================================================
/*                                               NOTES
/*=======================================================================================================================================================

/*=======================================================================================================================================================*/
