#include"Camera2D.h"
#include"../Shader/Shader.h"


Camera2D::Camera2D(float _left, float _right, float _top, float _bottom, float _near, float _far)
	:
	Top(_top),
	Left(_left),
	Right(_right),
	Bottom(_bottom),
	Size({
			_right - _left,
			_bottom - _top
		})
{
	//FOV = 60.0;
	Near = _near;
	Far = _far;

	AspectRatio = Size.x / Size.y;

	Position = Vec3(0);
	Rotation = 0;

	ProjectionMatrix = glm::ortho(Left, Right, Bottom, Top, Near, Far);  //OrthographicMatrix(_size.x, _size.y);
	ViewMatrix = glm::translate(Mat4(1.0f), Vec3(0.0f));

	set(this);
}



Camera2D::Camera2D(float _left, float _right, float _top, float _bottom)
	:
	Camera2D(_left, _right, _top, _bottom , -1.0f, 1000.0f)
{}
Camera2D::Camera2D(Vec2 _size)
	:
	Camera2D(0,  _size.x, 0, _size.y)
{}

void Camera2D::Move(Vec2 _amount)
{// Move Camera by _amount
	Position += _amount;
}
void Camera2D::MoveX(float _amount)
{// Move Camera on X axis by _amount
	Position.x += _amount;
}
void Camera2D::MoveY(float _amount)
{// Move Camera on Y axis by _amount
	Position.y += _amount;
}
void Camera2D::Rotate(float _angle)
{// Rotate the Camera by _angle
	Rotation += _angle;
}
void Camera2D::Translate(Vec2 _pos)
{// Move Camera to Position _pos
	ViewMatrix = (glm::translate(glm::mat4(1.0f), Vec3(-_pos.x, -_pos.y, 1.0f)));
}


void Camera2D::Bind()
{// Binds our Camera to OpenGL context and Sets Uniforms
	Shader::get().SetUniform("ProjectionMatrix", ProjectionMatrix);
	Shader::get().SetUniform("ViewMatrix", ViewMatrix);
}
void Camera2D::Update()
{// Updates the interpolation of the Camera
	ViewMatrix = Mat4(1.0f);
	Translate(Position);
	ViewMatrix = glm::rotate(ViewMatrix, glm::radians(Rotation), Vec3(0, 0, 1.0f));
}


void Camera2D::Resize(Vec2 _size)
{
    Right = _size.x;
    Bottom = _size.y;
    ProjectionMatrix = glm::ortho(0.0f, Right, Bottom, 0.0f, Near, Far); 
}


void Camera2D::Zoom(float _amount)
{//  Zooms in or out of a scene by manipulating the Projection Matrix
    REFACTOR("Zoom is slightly broken leading to elongation on the Y Axis. Not important ATM but something that should be fixed");
    
    ZoomLevel += _amount;
    Vec2 Sz
    {
        Right * (ZoomLevel * AspectRatio),
        Bottom  * ZoomLevel,
    };
    ProjectionMatrix =
        glm::ortho
        (
            Position.x   - ZoomLevel,
            Right  + ZoomLevel,
            Bottom + ZoomLevel,
            Position.y   - ZoomLevel,
            Near,
            Far
        );
}
void Camera2D::ZoomIn(float _amount) {  Zoom(_amount);  }
void Camera2D::ZoomOut(float _amount){  Zoom(-_amount); }


//  Sz.x,
//Sz.y,
//    Sz *= _amount;
//	float
//		Sx = Right * ZoomLevel * 0.5f,
//		Sy = Bottom * ZoomLevel * 0.5f;
//

///ProjectionMatrix =
///	glm::ortho
///	(
///		0.0f - Sx,
///		Right * ZoomLevel,
///		Bottom * ZoomLevel,
///		0.0f - Sy,
///		Near,
///		Far
///	);  //OrthographicMatrix(_size.x, _size.y);
///}
   // TODO("This is not currently setup correctly 2/18/2020: The Left and Right should zoom in on that current location of the Camera");

/*=======================================================================================================================================================
/*                                               NOTES
/*=======================================================================================================================================================

template <typename valType>
GLM_FUNC_QUALIFIER detail::tmat4x4<valType> ortho
(
    valType const & left,
    valType const & right,
    valType const & bottom,
    valType const & top,
    valType const & zNear,
    valType const & zFar
)
{
    detail::tmat4x4<valType> Result(1);
    Result[0][0] = valType(2) / (right - left);
    Result[1][1] = valType(2) / (top - bottom);
    Result[2][2] = - valType(2) / (zFar - zNear);
    Result[3][0] = - (right + left) / (right - left);
    Result[3][1] = - (top + bottom) / (top - bottom);
    Result[3][2] = - (zFar + zNear) / (zFar - zNear);
    return Result;
}

/*=======================================================================================================================================================*/
//ViewMatrix = glm::translate(ViewMatrix, Vec3(-_amount.x, -_amount.y, 0));
//ViewMatrix = glm::translate(ViewMatrix, Vec3(-_amount, 0, 0));
//ViewMatrix = glm::translate(ViewMatrix, Vec3(0, -_amount, 0));
//  ZoomLevel += _amount;
//  
//  float
//  Sx = Right * ZoomLevel * 0.5f,
//  Sy = Bottom * ZoomLevel * 0.5f;
//  
//  
//  
//  ProjectionMatrix =
//  glm::ortho
//  (
//  	0.0f - Sx,
//  	Right * ZoomLevel,
//  	Bottom * ZoomLevel,
//  	0.0f - Sy,
//  	Near,
//  	Far
//  );  //OrthographicMatrix(_size.x, _size.y);
