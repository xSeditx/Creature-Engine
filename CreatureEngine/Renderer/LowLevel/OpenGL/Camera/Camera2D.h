#pragma once

#include"Camera.h"

class Camera2D
	: public Viewport
{
public:
	Camera2D() = default;
	Camera2D(Vec2 size);
	Camera2D(float _left, float _right, float _top, float _bottom);

	void Update();
	void Bind();

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
	Vec2  Size{ 0,0 };
	float Rotation{ 0 };
};

