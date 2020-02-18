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




/*=======================================================================================================================================================
/*                                               TRASH
/*=======================================================================================================================================================
 
/*=======================================================================================================================================================*/
