#include"Camera.h"

Viewport *Viewport::ActiveCamera; // CURRENT MAIN CAMERA IN USE. Possibly not going to be needed soon.


void Viewport::PushProjectionMatrix()
{
	ProjectionMatrixStack.push(ProjectionMatrix);
}
void Viewport::PushViewMatrix()
{
	ViewMatrixStack.push(ViewMatrix);
}
void Viewport::PopProjectionMatrix()
{
	ProjectionMatrix = ProjectionMatrixStack.top();
	ProjectionMatrixStack.pop();
}
void Viewport::PopViewMatrix()
{
	ViewMatrix = ViewMatrixStack.top();
	ViewMatrixStack.pop();
}
void Viewport::Set_ViewMatrix(Mat4 mat)
{
	ViewMatrix = mat;
}
void Viewport::Set_ProjectionMatrix(Mat4 mat)
{
	ProjectionMatrix = mat;
}

Mat4 Viewport::GetViewMatrix()
{
	return ViewMatrix;
}

// HELPER FUNCTIONS
Mat4 OrthographicMatrix(int width, int height)
{
	return glm::ortho(0.0f, (float)width, (float)height, 0.0f, -0.0f, 1000.0f);
}


Mat4 PerspectiveMatrix(float _fovY, float _aspect, float _near, float _far)// Eigen::Matrix4f& projectionMatrix)
{
	float range = _far - _near;
	float invtan = 1.0f / tan(_fovY * 0.5f);

	Mat4 results;
	results[0][0] = invtan / _aspect;
	results[1][1] = invtan;
	results[2][2] = -(_near + _far) / range;
	results[2][3] = -1;
	results[3][2] = -2 * _near * _far / range;
	results[3][3] = 0;

	return results;
}