#include"Camera3D.h"
#include"../Shader/Shader.h"


Camera3D::Camera3D(Vec2 _screen, Vec3 position, Vec3 rotation)
{
	FOV = 60.0;
	Far = 1024.0f;
	Near = .10f;
	AspectRatio = _screen.x / _screen.y;

	Position = position;
	Rotation = rotation;
    Target_Position = position;
    Target_Rotation = rotation;

	Up = Vec3(0.0f, 1.0f, 0.0f);
	Right = Vec3(0.1f, 0.0f, 0.0f);
	Forward = Vec3(0.0f, 0.0f, 1.0f);

	ViewMatrix = Mat4(1.0f);
	ProjectionMatrix = glm::perspective(glm::radians(FOV), AspectRatio, Near, Far);

	set(this);
}


void Camera3D::Bind()
{
//	Shader::get().SetUniform("EyePosition", Position); This is for Lighting later
	Shader::get().SetUniform("ProjectionMatrix", ProjectionMatrix);
	Shader::get().SetUniform("ViewMatrix", ViewMatrix);
}

// Updates the Camera to the new Target Position 
void Camera3D::Update()
{
	WARN_ME(" I am Changing GetTime to a flat number to make this work Change Immediately ");
	float Time = .1f; //GetTime();
    /// We are using Fixed time for Debug purposes however this needs to change. 

    Delta_Time = 0.1f;// (Time - Current_Time);
	Current_Time = Time;

	Position += (Target_Position - Position) * 10.f * Delta_Time;
	Rotation += (Target_Rotation - Rotation) * 10.f * Delta_Time;

	ViewMatrix = Mat4(1.0);

	RotateX(Rotation.x);
	RotateY(Rotation.y);

	Translate(Position);

	Up = glm::normalize(Vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]));
	Right = glm::normalize(Vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]));
	Forward = glm::normalize(Vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]));
	ViewMatrix = glm::lookAt(Position, Position - Forward, Up);
}


// CAMERA MANIPULATIONS
void Camera3D::Rotate(float pitch, float yaw)
{
	Target_Rotation.x -= yaw   * static_cast<float>(RADIANS(.9));
	Target_Rotation.y -= pitch * static_cast<float>(RADIANS(.9));
}
void Camera3D::MoveBack(float speed)
{
	Target_Position -= (speed * Forward);
}
void Camera3D::MoveLeft(float speed)
{
	Target_Position -= (speed)*Right;
}
void Camera3D::MoveRight(float speed)
{
	Target_Position += (speed)*Right;

}
void Camera3D::MoveForward(float speed)
{
	Target_Position += (speed * Forward);
}

// MATRIX TRANSFORMERS
void Camera3D::RotateX(GLfloat angle)
{
	ViewMatrix *= Mat4
	{
		1.0f,       0.0f,        0.0f, 0.0f,
		0.0f, cos(angle), -sin(angle), 0.0f,
		0.0f, sin(angle),  cos(angle), 0.0f,
		0.0f,       0.0f,        0.0f, 1.0f
	};
}
void Camera3D::RotateY(GLfloat angle)
{
	ViewMatrix *= Mat4
	{
		cos(angle), 0.0f,-sin(angle), 0.0f,
			  0.0f, 1.0f,       0.0f, 0.0f,
		sin(angle), 0.0f, cos(angle), 0.0f,
			  0.0f, 0.0f,       0.0f, 1.0f
	};
}
void Camera3D::RotateZ(GLfloat angle)
{
	ViewMatrix *= Mat4
	{
		cos(angle), 0.0f,-sin(angle), 0.0f,
		sin(angle), 1.0f, cos(angle), 0.0f,
			  0.0f, 0.0f,       1.0f, 0.0f,
			  0.0f, 0.0f,       0.0f, 1.0f
	};
}

void Camera3D::Translate(Vec3 pos)
{
	ViewMatrix *= (glm::translate(glm::mat4(1.0f), pos));
}
Mat4 Camera3D::LookAt(Vec3 eye, Vec3 target, Vec3 up)
{
	Vec3 vz = glm::normalize(eye - target);
	Vec3 vx = glm::normalize(glm::cross(up, vz));
	Vec3 vy = glm::cross(vz, vx);
	Mat4 results =  
		Mat4
		(
			Vec4(vx, 0),
			Vec4(vy, 0),
			Vec4(vz, 0),
			Vec4(eye, 1)
		);
	return results; 
}// MATRIX STATE HANDLING

