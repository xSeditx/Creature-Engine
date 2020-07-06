#include"Mesh.h"
#include"../Shader/Shader.h"
#include"../../Materials/Image/Texture.h"


int GameObject::ObjectCount{ 0 };

Mesh::Mesh()
	:
	GameObject(Vec3(0), Vec3(0), Vec3(1.0f)),
	VAO(nullptr)
{}


Mesh::Mesh(std::string file)
	:
	GameObject(Vec3(0), Vec3(0), Vec3(1.0f)),
	Filepath(file),
	VAO(nullptr)
{
	__debugbreak();
}


void Mesh::Bind()
{
	VAO->Bind();
    model_Matrix.Bind();
}
void Mesh::Unbind()
{
	VAO->Unbind();
}

void Mesh::Update(Vec3 _pos, Vec3 _rot)
{//  Set Identity and Rotate all axis followed with the Translation.

    model_Matrix.Update(_pos, _rot);
	needsUpdated = false; // Tell Application we Finished Updating. May need to make this Atomic in the future.
	// #ATOMIC NEEDED
}




















// =
//	glm::rotate
//	( // ensure unneeded copying as the results from the last transform are immediately used in the next step
//		glm::rotate
//		(
//			glm::rotate
//			(
//				glm::translate
//				( // Starts with the Translation here
//					glm::mat4(1.0f),
//                     model_Matrix.Position
//				),
//				glm::radians(model_Matrix.Rotation.x),
//				Vec3(1.0f, 0.0f, 0.0f)),
//			glm::radians(Rotation.y),
//			Vec3(0.0f, 1.0f, 0.0f)),
//		glm::radians(Rotation.z),
//		Vec3(0.0f, 0.0f, 1.0f)
//	);
