#pragma once

#include<stack>
#include<unordered_map>

#include"../OpenGL.h"
#include"Core/Common.h"

class uniform_t;


class Shader
{
	enum ErrorType
	{
		Vert,
		Frag,
		Program
	};
public:
	Shader() = default;
	Shader(const char* filepath);
	Shader(std::string _vertstring, std::string _fragstring);
	~Shader();

	void Delete();
	void Reload();

	void Bind();
	void Unbind();

    /* Turns a String representing the Vertex and Fragment Shaders into a Compiled program */
	void CompileStrings(std::string _vertstring, std::string _fragstring);

	/* returns the Location slow of a named Uniform */
	GLint GetUniformLocation(const char  *name);

	/* returns the OpenGL ID name */
	GLuint g_Handle() { return GL_Handle; }

    /* Pushes a Shader to the Shader Stack and Activates it */
	static void Push(Shader& shad);
    /* Pops a Shader from the Shader Stack and Activates the next one */
	static Shader& Pop();

	GLuint GetResourceLocation();

	int get_FunctionCount(int _shadertype);
	std::string get_FunctionName(int _shadertype, int _index);

	void SetUniform(const char* _name, bool _val);
	void SetUniform(const char* _name, float _val);
	void SetUniform(const char* _name, float _val, float _val2);
    void SetUniform(const char* _name, float _val, float _val2, float _val3);
    void SetUniform(const char* _name, float _val, float _val2, float _val3, float _val4);

    void SetUniform(const char* _name, uint64_t _val);
	void SetUniform(const char* _name, int _val);
	void SetUniform(const char* _name, int _val, int _val2);
	void SetUniform(const char* _name, int _val, int _val2, int _val3);
	void SetUniform(const char* _name, Vec2 _val);
	void SetUniform(const char* _name, Vec3 _val);
	void SetUniform(const char* _name, Vec4 _val);
	void SetUniform(const char* _name, Mat3 _val);
	void SetUniform(const char* _name, Mat4 _val);


	/* Sets Arrays of Values as the Uniform */
	void SetUniform(const char* _name, std::vector < Vec2 >& _array);
	void SetUniform(const char* _name, std::vector < Vec3 >& _array);
	void SetUniform(const char* _name, std::vector < Vec4 >& _array);
	void SetUniform(const char* _name, std::vector < Mat3 >& _array);
	void SetUniform(const char* _name, std::vector < Mat4 >& _array);



	uint32_t SetAttribute(uint8_t _elements, const char* _name)
	{
		return OpenGL::set_Attribute(GL_Handle, _elements, _name);
	}


	void SetArrayUniform(const char* _name, std::vector<Mat4>& _array);

	void SetTextureUniform(const char *_name, uint64_t _tex);
	void SetTextureUniform(const char *_name, uint32_t _textureID, int _slot);

	int AttributeLocation(const char *_name);


	static int  get_CurrentShader() { int result{ 0 }; glGetIntegerv(GL_CURRENT_PROGRAM, &result);    return result; }
	static void set_Shader(uint32_t _program)	{ glUseProgram(_program);	}

	static Shader& get()       { return *ActiveShader.top();        }
	static GLuint  getHandle() { return  ActiveShader.top()->g_Handle();}


protected:
	GLuint GL_Handle{ 0 };//8
	unsigned int RecursionCount{ 0 };//8
	GLuint VertID{ 0 }, FragID{ 0 }; // Now these are destroyed at the end.. Find a better solution to handling the Shader errors as Currently I need to store this in order to pass to the Error response

	const char* Filepath{nullptr};
	bool Active{ false };

	static std::stack<Shader *> ActiveShader;

	GLuint Load();
	void GetShaderError(ErrorType t);
};


//class GPU_program
//    : public Shader
//{
//    std::unordered_map<std::string, GLuint> ShaderAttribute;
//
//
//};
//

//bool Init_DefaultShaders();
//extern Shader *QuadRenderer;
//extern GLuint DebugQuadVAO;
//extern GLuint DebugQuadVBO;
//class Camera2D;
//extern Camera2D *debugCamera;
//







/*
NO_COPY_OR_ASSIGNMENT(Shader);
Shader(Shader&& _other)
{
    GL_Handle = _other.GL_Handle;
    RecursionCount = _other.RecursionCount;
    VertID = _other.VertID;
    FragID = _other.FragID;
    Filepath = _other.Filepath;
    Active = _other.Active;
    _other.GL_Handle = 0;
    _other.RecursionCount = 0;
    _other.VertID = 0;
    _other.FragID = 0;
    _other.Filepath = "";
    _other.Active = false;
}
const Shader& operator =(Shader&& _other)
{
    return std::move(_other);
} */
