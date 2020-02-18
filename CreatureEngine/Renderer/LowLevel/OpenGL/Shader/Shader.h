#pragma once

#include<stack>
#include<unordered_map>

#include"../OpenGL.h"
#include"Core/Common.h"

class uniform_t;
//class Texture;

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

	void Enable();
	void Disable();
	void Reload();
	virtual void Bind() {}

	void CompileStrings(std::string _vertstring, std::string _fragstring);

	std::unordered_map<std::string, GLuint> ShaderAttribute;

	/* returns the Location slow of a named Uniform */
	GLint GetUniformLocation(const char  *name);

	/* returns the OpenGL ID name */
	GLuint g_ID() { return GL_Handle; }

	void Push(Shader *shad);
	Shader *Pop();

	GLuint GetResourceLocation();

	int get_FunctionCount(int _shadertype);
	std::string get_FunctionName(int _shadertype, int _index);

	void SetUniform(const char* _name, bool _val);
	void SetUniform(const char* _name, float _val);
	void SetUniform(const char* _name, float _val, float _val2);
	void SetUniform(const char* _name, float _val, float _val2, float _val3);
	void SetUniform(const char* _name, uint64_t _val);
	void SetUniform(const char* _name, int _val);
	void SetUniform(const char* _name, int _val, int _val2);
	void SetUniform(const char* _name, int _val, int _val2, int _val3);
	void SetUniform(const char* _name, Vec2 _val);
	void SetUniform(const char* _name, Vec3 _val);
	void SetUniform(const char* _name, Vec4 _val);
	void SetUniform(const char* _name, Mat3 _val);
	void SetUniform(const char* _name, Mat4 _val);

	void SetTextureUniform(const char *_name, uint64_t _tex);
	//void SetTextureUniform(const char *_name, Texture *_tex);

	int AttributeLocation(const char *_name);


	static int  get_CurrentShader() { int result{ 0 }; glGetIntegerv(GL_CURRENT_PROGRAM, &result);    return result; }
	static void set_Shader(uint32_t _program)	{ glUseProgram(_program);	}

	static Shader& get()       { return *ActiveShader.top();        }
	static GLuint  getHandle() { return  ActiveShader.top()->g_ID();}


protected:
	GLuint GL_Handle{ 0 };//8
	unsigned int RecursionCount{ 0 };//8
	GLuint VertID, FragID; // Now these are destroyed at the end.. Find a better solution to handling the Shader errors as Currently I need to store this in order to pass to the Error response

	const char* Filepath{nullptr};
	bool Active{ false };

	static std::stack<Shader *> ActiveShader;

	GLuint Load();
	void GetShaderError(ErrorType t);
};