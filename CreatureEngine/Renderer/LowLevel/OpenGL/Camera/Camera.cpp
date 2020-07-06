#include"Camera.h"

Viewport *Viewport::ActiveCamera; // CURRENT MAIN CAMERA IN USE. Possibly not going to be needed soon.


void Viewport::push_ProjectionMatrix()
{
	ProjectionMatrixStack.push(ProjectionMatrix);
}
void Viewport::push_ViewMatrix()
{
	ViewMatrixStack.push(ViewMatrix);
}
void Viewport::pop_ProjectionMatrix()
{
	ProjectionMatrix = ProjectionMatrixStack.top();
	ProjectionMatrixStack.pop();
}
void Viewport::pop_ViewMatrix()
{
	ViewMatrix = ViewMatrixStack.top();
	ViewMatrixStack.pop();
}
void Viewport::set_ViewMatrix(Mat4 mat)
{
	ViewMatrix = mat;
}
void Viewport::set_ProjectionMatrix(Mat4 mat)
{
	ProjectionMatrix = mat;
}

Mat4 Viewport::get_ViewMatrix()
{
	return ViewMatrix;
}



Mat4 Viewport::get_ProjectionMatrix()
{
    return ProjectionMatrix;
}
Mat4 Viewport::get_ViewProjectionMatrix() { return ViewProjectionMatrix; }



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

