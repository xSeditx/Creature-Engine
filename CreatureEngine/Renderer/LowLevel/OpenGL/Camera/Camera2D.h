#pragma once

#include"Camera.h"


class Camera2D
	: public Viewport
{
public:
	Camera2D() = default;
	Camera2D(Vec2 size);
	Camera2D(float _left, float _right);
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

	/* Resizes the Projection Matrix to the given _size */
	void Resize(Vec2 _size);

	/* Gets the Width of the Cameras Dimensions*/
	const int Width() { return  static_cast<int>(Size.x); }
	/* Gets the Height of the Cameras Dimensions*/
	const int Height() { return static_cast<int>(Size.y); }


    void set_ProjectionMatrix(float _left, float _right, float _bottom, float _top)
    {
        ProjectionMatrix = glm::ortho(_left, _right, _bottom, _top, Near, Far);
        ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    }



    Vec2  g_Position()            { return Position;   }
    void  s_Position (Vec2 _val)  { Position   = _val; }

    float g_PositionX() { return Position.x;}
    float g_PositionY() { return Position.y;}
    void  s_PositionX(float _val) { Position.x = _val; }
    void  s_PositionY(float _val) { Position.y = _val; }



    virtual Vec2 get_Center() override { return { Width() * 0.5f, Height() * 0.5f }; }



	/*  Zooms in or out of a scene by manipulating the Projection Matrix */
	void Zoom(float _amount);
	/*  Zooms in on a scene by manipulating the Projection Matrix */
	void ZoomIn(float _amount);
	/*  Zooms out on a scene by manipulating the Projection Matrix */
	void ZoomOut(float _amount);
    /*  Zooms in or out of a scene by manipulating the Projection Matrix */
    void ZoomInto(Vec2 _pos, float _amount);
    /*  Zooms in or out of a scene by manipulating the Projection Matrix */
    void ZoomOutFrom(Vec2 _pos, float _amount);

    void s_Zoom(float _amount) {  ZoomLevel = _amount;  } //(_amount + 1000) / 1000;
    float g_Zoom() { return ZoomLevel; }
            

    // Only hit the one Axis with the Aspect ratio
private:
	Vec2  Size{ 0,0 };

	float ZoomLevel{ 1.0f };

    Vec2  Position{ 0,0 };
	float Rotation{ 0 };

	float
		Top{ 0.0f },
		Bottom{ 0.0f },
		Right{ 0.0f },
		Left{ 0.0f };

	Vec2  TargetSize{ 0,0 };
	Vec2  TargetPosition{ 0,0 };
	float TargetRotation{ 0 };


	float LERP_Speed{ .1f };

	/* Imprecise method, which does not guarantee v = v1 when t = 1, due to floating-point arithmetic error.
       This form may be used when the hardware has a native fused multiply-add instruction.*/
	Vec2 lerp()
	{
		return TargetPosition + Vec2(LERP_Speed) * (Position - TargetPosition);
	}
	/* Precise method, which guarantees v = v1 when t = 1 */
	Vec2 precise_lerp()
	{
		return (Vec2(1) - Vec2(LERP_Speed)) * TargetPosition + Vec2(LERP_Speed) * Position;
	}
};






/*=======================================================================================================================================================
/*                                               TRASH
/*=======================================================================================================================================================
/*=======================================================================================================================================================*/




