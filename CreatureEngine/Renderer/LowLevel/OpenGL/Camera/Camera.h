#pragma once

#include<stack>

#include"Core/Common.h"


class Viewport
{
public:
	Viewport() = default;

	float
		FOV{ 60.0f },
		AspectRatio{ 1.0f},
		Near{ 0.1f },
		Far{ 1000.0f };

	void PushProjectionMatrix();
	void PushViewMatrix();
	void PopProjectionMatrix();
	void PopViewMatrix();

	Mat4 ProjectionMatrix = Mat4(0);
	Mat4 ViewMatrix = Mat4(0);
	Mat4 Identity = Mat4(1.0f);

	Mat4 GetViewMatrix();

	//=======================================================================================================================================================

	void Set_ViewMatrix(Mat4 mat);
	void Set_ProjectionMatrix(Mat4 mat);

	static Viewport& get()               { return *ActiveCamera; }
	static void      set(Viewport* _cam) { ActiveCamera = _cam; }
private:

	static Viewport *ActiveCamera;

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
		Position{ 0,0,0 },
		Rotation{ 0,0,0 },
		Forward{ 0,0,0 },
		Right{ 0,0,0 },
		Up{ 0,0,0 };

	Vec3
		Target_Position{ 0 },
		Target_Rotation{ 0 };

	Vec3  Camera_Direction{ 0 };
	float Delta_Time = 0.0f;
	float Current_Time = 0.0f;

	void Bind();
	void Update();

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

	void Update( );
	void Bind( );

	float
		Top,
		Bottom,
		Right,
		Left;

	void Translate(Vec2 pos);
	void Rotate(float _angle);

	const int Height() { return static_cast<int>(Size.y); }
	const int Width() { return  static_cast<int>(Size.x); }

private:
	Vec2  Position{ 0,0 };
	Vec2  Size    { 0,0 };
	float Rotation{ 0   };
};




/*=======================================================================================================================================================
/*                                               NOTES
/*=======================================================================================================================================================

/*=======================================================================================================================================================*/
