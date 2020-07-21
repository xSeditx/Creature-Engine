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

	void push_ProjectionMatrix();
	void push_ViewMatrix();
	void pop_ProjectionMatrix();
	void pop_ViewMatrix();


	Mat4 ViewMatrix = Mat4(0);
	Mat4 ProjectionMatrix = Mat4(0);
    Mat4 ViewProjectionMatrix = Mat4(0);



	Mat4 get_ViewMatrix();
    Mat4 get_ProjectionMatrix();
    Mat4 get_ViewProjectionMatrix();

	void set_ViewMatrix(Mat4 mat);
	void set_ProjectionMatrix(Mat4 mat);


    virtual Vec2 get_Center()
    {
        //WARN_ME("get_Canter in Viewport Class is just returning junk: Be sure to correct this"); 
        return Vec2(0);
    }
    //=======================================================================================================================================================

	static Viewport& get()               { return *ActiveCamera; }
	static void      set(Viewport* _cam) { ActiveCamera = _cam; }
private:

	static Viewport *ActiveCamera;

	std::stack<Mat4> ViewMatrixStack;
	std::stack<Mat4> ProjectionMatrixStack;
};


Mat4 OrthographicMatrix(int _width, int _height);
Mat4 PerspectiveMatrix(float _fovY, float _aspect, float _near, float _far);
 


/*=======================================================================================================================================================
/*                                               TRASH
/*=======================================================================================================================================================
 
/*=======================================================================================================================================================*/
