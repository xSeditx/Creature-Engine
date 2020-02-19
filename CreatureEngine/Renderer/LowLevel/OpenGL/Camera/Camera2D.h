#pragma once

#include"Camera.h"

class Camera2D
	: public Viewport
{
public:
	Camera2D() = default;
	Camera2D(Vec2 size);
	Camera2D(float _left, float _right, float _top, float _bottom);
	Camera2D(float _left, float _right, float _top, float _bottom, float _near, float _far);

	/* Updates the Cameras Interpolation */
	void Update();
	/* Binds the Camera and makes it Active on our OpenGL context */
	void Bind();

	/* Move Camera by _amount            */
	void Move(Vec2 _pos);
	/* Move Camera on X axis by _amount  */
	void MoveX(float _amount);
	/* Move Camera on Y axis by _amount  */
	void MoveY(float _amount);

	/* Move Camera to Position _pos	  */
	void Translate(Vec2 pos);
	/* Rotate the Camera by _angle		  */
	void Rotate(float _angle);

	/* Gets the Width of the Cameras Dimensions*/
	const int Width() { return  static_cast<int>(Size.x); }
	/* Gets the Height of the Cameras Dimensions*/
	const int Height() { return static_cast<int>(Size.y); }

	void ZoomIn (float _amount);
	void ZoomOut(float _amount);
	void Zoom(float _amount);

	void Resize(Vec2 _size);
private:

	float ZoomLevel{ 1.0f };
	Vec2  Size{ 0,0 };
	Vec2  Position{ 0,0 };

	Vec2  TargetSize{ 0,0 };
	Vec2  TargetPosition{ 0,0 };

	float Rotation{ 0 };
	float TargetRotation{ 0 };

	float
		Top,
		Bottom,
		Right,
		Left;

	float LERP{ .1f };

	/* Imprecise method, which does not guarantee v = v1 when t = 1, due to floating-point arithmetic error.
       This form may be used when the hardware has a native fused multiply-add instruction.*/
	Vec2 lerp()
	{
		return TargetPosition + Vec2(LERP) * (Position - TargetPosition);
	}
	/* Precise method, which guarantees v = v1 when t = 1 */
	Vec2 precise_lerp()
	{
		return (Vec2(1) - Vec2(LERP)) * TargetPosition + Vec2(LERP) * Position;
	}
};





/*=======================================================================================================================================================
/*                                               TRASH
/*=======================================================================================================================================================
	//	ProjectionMatrix = glm::ortho(0.0f, size.x, size.y, 0.0f, -1.0f, 1.0f);  //OrthographicMatrix(size.x, size.y);
	//	ViewMatrix = glm::translate(Mat4(1.0f), Vec3(-100, 0, 0));
//Top = _top;
//Left = _left;
//ProjectionMatrix = glm::ortho(0.0f, Right, Bottom, 0.0f, -1.0f, 100.0f);  //OrthographicMatrix(_size.x, _size.y);
//ViewMatrix = glm::translate(Mat4(1.0f), Vec3(0, 0, 0));//200, 200, 0))
//Size(size)


/*=======================================================================================================================================================*/




