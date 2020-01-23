#pragma once

#include"Core/Common.h"
#include"../Shader/Shader.h"
//namespace OpenGL
//{
	class Transform
	{
	public:
		Transform()
			:
			Scale(0),
			Position(0),
			Rotation(0),
			Matrix(Mat4(0))
		{
			Matrix = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
			Matrix = glm::translate(Matrix, Position);
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
		}

		Transform(Vec3 _pos, Vec3 _rot, Vec3 _scale)
			:
			Position(_pos),
			Rotation(_rot),
			Scale(_scale)
		{
			Matrix = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
			Matrix = glm::translate(Matrix, _pos);
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
		}
		Transform(Vec3 _pos, Vec3 _rot, std::string _name)
			:
			Position(_pos),
			Rotation(_rot),
			Scale(Vec3(1.0f)),
			Name(_name)
		{
			Matrix = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
			Matrix = glm::translate(Matrix, _pos);
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
		}




		void set_Position(Vec3 _position)   { Position = _position; }
		void set_Rotation(Vec3 _rotation)   { Rotation = _rotation; }
		void set_Scale(Vec3 _scale)         { Scale = _scale;       } 
		void set_Scale(float _scale)        { Scale = Vec3(_scale); }
		void set_Transform(Mat4 _transform) { Matrix = _transform;  }
		void set_Name(std::string _name)    { Name = _name;         }



		void Bind()
		{
			Shader::get().SetUniform(Name.c_str(), Matrix);
		}
		void Update()
		{
			Matrix = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
			Matrix = glm::translate(Matrix, Position);
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
		}
		void UpdateBy(Vec3 _pos, Vec3 _rot)
		{
			Matrix = glm::translate(Matrix, _pos);
			Matrix = glm::rotate(Matrix, glm::radians(_rot.x), Vec3(1.0f, 0.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(_rot.y), Vec3(0.0f, 1.0f, 0.0f));
			Matrix = glm::rotate(Matrix, glm::radians(_rot.z), Vec3(0.0f, 0.0f, 1.0f));
		}

		void Identity()
		{
			Matrix = Mat4(1);
		}
		void Translate(Vec3 _pos)
		{
			Matrix = glm::translate(	glm::mat4(1.0f), _pos);
		}
		void Rotate_Translate(Vec3 _pos, Vec3 _rot)
		{
			Matrix = glm::rotate
			( // ensure unneeded copying as the results from the last transform are immediately used in the next step
				glm::rotate
				(
					glm::rotate
					(
						glm::translate
						( // Starts with the Translation here
							glm::mat4(1.0f),
							_pos
						),
						glm::radians(_rot.x),
						Vec3(1.0f, 0.0f, 0.0f)),
					glm::radians(_rot.y),
					Vec3(0.0f, 1.0f, 0.0f)),
				glm::radians(_rot.z),
				Vec3(0.0f, 0.0f, 1.0f)
			);
		}
		void Rotate(Vec3 _rot)
		{
			Matrix = glm::rotate
			( // ensure unneeded copying as the results from the last transform are immediately used in the next step
				glm::rotate
				(
					glm::rotate
					(
						Matrix,
						glm::radians(_rot.x),
						Vec3(1.0f, 0.0f, 0.0f)),
					glm::radians(_rot.y),
					Vec3(0.0f, 1.0f, 0.0f)),
				glm::radians(_rot.z),
				Vec3(0.0f, 0.0f, 1.0f)
			);
		}
		void RotateX(float _rot)
		{
		    Matrix = glm::rotate(Matrix, glm::radians(_rot), Vec3(1.0f, 0.0f, 0.0f));
		}
		void RotateY(float _rot)
		{
			Matrix = glm::rotate(Matrix, glm::radians(_rot), Vec3(0.0f, 1.0f, 0.0f));
		}
		void RotateZ(float _rot)
		{
			Matrix = glm::rotate(Matrix, glm::radians(_rot), Vec3(0.0f, 0.0f, 1.0f));
		}


		Mat4 get() { return Matrix; }
		Vec3 scale() { return Scale; }
		Vec3 position() { return Position; }
		Vec2 rotation() { return Rotation; }


		Mat4 Matrix;
		Vec3 Position, Rotation, Scale;
		std::string Name;
		static int ObjectCount;
	};
//}