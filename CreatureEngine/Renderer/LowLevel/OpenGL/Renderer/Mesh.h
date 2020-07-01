#pragma once

//#include"Window.h"

///#include"Assimp.h"

#include"GLBuffers.h"
#include"Core/Common.h"

#include"GameObject.h"


class Shader;
struct MatProps
{
	unsigned int Semantic;
	unsigned int Index;
	unsigned int DataLength;
///	aiPropertyTypeInfo Type;
	char* Data;
};



class  Mesh
	: public GameObject
{
public:

	unsigned int MaterialID;
	unsigned int PrimativeType = GL_TRIANGLES;

	VertexArrayObject* VAO; /// Figure something out here

	std::vector<Mesh> Children;

	Mesh();
	Mesh(std::string file);

	void Bind() override;
	void Render() override;

	void Unbind() override;
	void Update() override;

	void GetPrimativeType(unsigned int input)
	{
///switch (input)
///{
///case aiPrimitiveType_LINE:
///	PrimativeType = GL_LINES;
///	break;
///case aiPrimitiveType_POINT:
///	PrimativeType = GL_POINTS;
///	break;
///case aiPrimitiveType_TRIANGLE:
///	PrimativeType = GL_TRIANGLES;
///	break;
///case aiPrimitiveType_POLYGON:
///	PrimativeType = GL_TRIANGLE_STRIP;
///	/// "I think this is wrong, there appears to be no GL_POLYGONS so Triangle strip seems the closest thing I can think of");
///	__debugbreak();
///	break;
///}
	}


	std::string VertexShader = " #version 330 core \n\
layout(location = 0) in vec3 aPos; 			\n\
out vec4 VColor; 						\n\
void main()									\n\
{											\n\
	gl_Position = vec4(aPos, 1.0);			\n\
	VColor = vec4(0.5, 1.0, 1.0, 1.0); \n\
}";

	std::string FragmentShader = "#version 330 core \n\
out vec4 FragColor;                             \n\
in vec4 VColor;                            \n\
void main()                                     \n\
{                                               \n\
	FragColor = VColor;                    \n\
}";

protected:

	std::string Filepath;
	bool needsUpdated = false; // #ATOMIC NEEDED
	bool Static = true;
};
