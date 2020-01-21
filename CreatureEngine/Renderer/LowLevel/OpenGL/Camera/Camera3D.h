#pragma once

#include"Camera.h"



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