#include"Mesh.h"
#include"../Shader/Shader.h"
#include"../../Materials/Image/Texture.h"


int GameObject::ObjectCount{ 0 };

Mesh::Mesh()
	:
	GameObject(Vec3(0), Vec3(0), Vec3(1.0f)),
	MaterialID(NULL)
{
}


Mesh::Mesh(std::string file)
	:
	GameObject(Vec3(0), Vec3(0), Vec3(1.0f)),
	Filepath(file),
	MaterialID(NULL)
{
	__debugbreak();
}


std::string GetFileName(const  std::string& s)
{
	char sep = '/';
#ifdef _WIN32
	sep = '\\';
#endif
	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}
	return("");
}

void Mesh::Bind()
{
	VAO->Bind();
	Shader::get().SetUniform("ModelMatrix", Transform);
}
void Mesh::Unbind()
{
	//	VAO->Unbind();
}

void Mesh::Render()
{
	VAO->Bind();
	Shader::get().SetUniform("ModelMatrix", Transform);

	glDrawElements(PrimativeType, VAO->ElementCount, GL_UNSIGNED_INT, nullptr);

	for (auto& C : Children)
	{
		C.Render();
	}
}


void Mesh::Update()
{//  Set Identity and Rotate all axis followed with the Translation.


	Transform = glm::rotate
	( // ensure unneeded copying as the results from the last transform are immediately used in the next step
		glm::rotate
		(
			glm::rotate
			(
				glm::translate
				( // Starts with the Translation here
					glm::mat4(1.0f),
					Position
				),
				glm::radians(Rotation.x),
				Vec3(1.0f, 0.0f, 0.0f)),
			glm::radians(Rotation.y),
			Vec3(0.0f, 1.0f, 0.0f)),
		glm::radians(Rotation.z),
		Vec3(0.0f, 0.0f, 1.0f)
	);
	needsUpdated = false; // Tell Application we Finished Updating. May need to make this Atomic in the future.
	// #ATOMIC NEEDED
}