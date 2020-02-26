/*===============================================================================================================================================================
//
//                             SHADER MODULE
//
=================================================================================================================================================================*/


#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

#include"Shader.h"

Shader::Shader(const char* filepath)
	:
	Filepath(filepath),
	GL_Handle(0),
	VertID(0),
	FragID(0)
{
	DEBUGPrint(CON_Green, "Called Shader Constructor" << Filepath);
	GL_Handle = Load();
	// _GL( glUseProgram(GL_Handle) );
}
Shader::~Shader()
{
	//Delete();
}
void Shader::Delete()
{
	TODO("Error creatd here if Shader is made on the Stack and destroyed after strings are gone");
	if(Filepath)DEBUGPrint(CON_Red, "Called the Shader Destructor: " << Filepath);
	//glDeleteProgram(GL_Handle);
}


void Shader::Bind()
{
	glUseProgram(GL_Handle);
	//Print("Enabling Shader: " << GL_Handle);
	Push(*this);
	DEBUG_CODE(CheckGLERROR(););
}
void Shader::Unbind()
{
	REFACTOR("Perhaps return just the Shader ID instead of dereferencing a pointer here.");
	//DEBUG_CODE(Print("Disabling Shader:" << Shader::get_CurrentShader()));
	int Prog = Pop().GL_Handle;
	glUseProgram(Prog);
	//DEBUG_CODE(Print("Reenabling Shader:" << Shader::get_CurrentShader()));
	//DEBUG_CODE(CheckGLERROR(););
}
//
//void Shader::Enable()
//{
//	glUseProgram(GL_Handle);
//	//Print("Enabling Shader: " << GL_Handle);
//	Push(*this);
//	DEBUG_CODE(CheckGLERROR(););
//}
//void Shader::Disable()
//{
//	REFACTOR("Perhaps return just the Shader ID instead of dereferencing a pointer here.");
//	//DEBUG_CODE(Print("Disabling Shader:" << Shader::get_CurrentShader()));
//	int Prog = Pop().GL_Handle;
//	glUseProgram(Prog);
//	//DEBUG_CODE(Print("Reenabling Shader:" << Shader::get_CurrentShader()));
//	//DEBUG_CODE(CheckGLERROR(););
//}

void Shader::Push(Shader& shad)
{
	ActiveShader.push(&shad);
}
Shader& Shader::Pop()
{
	ActiveShader.pop();
	//DEBUG_CODE(if (ActiveShader.empty()) __debugbreak(););
	//DEBUG_CODE(Print("Popping Shader: " << ActiveShader.top()->GL_Handle));
	return *ActiveShader.top();
}

GLint Shader::GetUniformLocation(const char  *name)
{
	return glGetUniformLocation(GL_Handle, name); //glGetProgramResourceLocation(GL_Handle, GL_UNIFORM, name);//
}
 
GLuint Shader::Load()
{

	enum ShaderType { VERTEX, FRAGMENT, PROGRAM };

	std::ifstream ShaderFile(Filepath, std::ios::in);
	if (!ShaderFile)
	{
		Print("Shader File Not Found:" << Filepath);
		__debugbreak();
		//    EngineErrorResponse(ERROR_FILE_NOT_FOUND, 0, (char*)Filepath);
	}
	std::string Line;
	std::string VertexShader;
	std::string FragmentShader;

	ShaderType Type = (ShaderType)0;
	std::stringstream SS[2];
	while (std::getline(ShaderFile, Line))
	{
		if (Line.find("[#SHADER:") != std::string::npos)
		{
			if (Line.find("VERTEX") != std::string::npos)
			{
				Type = VERTEX;
			}
			else if (Line.find("FRAGMENT") != std::string::npos)
			{
				Type = FRAGMENT;
			}
		}
		else
		{
			SS[(int)Type] << Line << '\n';
		}
	}
	std::string vert = SS[VERTEX].str();
	std::string frag = SS[FRAGMENT].str();

	CompileStrings(vert, frag);
	TODO("See TRASHED SHADER in Trash.txt");

	return GL_Handle;
}

void Shader::Reload()
{
	enum ShaderType { VERTEX, FRAGMENT, PROGRAM };

	std::ifstream ShaderFile(Filepath, std::ios::in);
	if (!ShaderFile)
	{
		Print("Shader File Not Found:" << Filepath);
		__debugbreak();
		//		EngineErrorResponse(ERROR_FILE_NOT_FOUND, 0, (char*)Filepath);
	}
	std::string Line;
	std::string VertexShader;
	std::string FragmentShader;

	ShaderType Type{ VERTEX };
	std::stringstream SS[2];
	while (std::getline(ShaderFile, Line))
	{
		if (Line.find("[#SHADER:") != std::string::npos)
		{
			if (Line.find("VERTEX") != std::string::npos)
			{
				Type = VERTEX;
			}
			else if (Line.find("FRAGMENT") != std::string::npos)
			{
				Type = FRAGMENT;
			}
		}
		else
		{
			SS[(int)Type] << Line << '\n';
		}
	}
	std::string vert = SS[VERTEX].str();
	std::string frag = SS[FRAGMENT].str();

	CompileStrings(vert, frag);
	TODO("See TRASHED SHADER in Trash.txt");
	return;
}

/*
Static Gets and Shader information
*/

// Associated Gets
//----------------
int Shader::get_FunctionCount(int _shadertype)
{//	To query the number of active subroutine functions, pname? must be GL_ACTIVE_SUBROUTINES.
	int RETURN;
	glGetProgramStageiv(GL_Handle, _shadertype, GL_ACTIVE_SUBROUTINES, &RETURN);
	return RETURN;
}
std::string Shader::get_FunctionName(int _shadertype, int _index)
{
	char RETURN[255] = { 0 };
	GLsizei Length;
	GLsizei Size = 20;
	glGetActiveSubroutineName(GL_Handle, _shadertype, _index, Size, &Length, RETURN);
	return RETURN;
}
void Shader::GetShaderError(ErrorType T)
{
	GLint length{ 0 };
	GLint result{ 0 };
	GLchar error[1028];

	switch (T)
	{
	case Vert:

		glGetShaderiv(VertID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(VertID, GL_INFO_LOG_LENGTH, &length);

		if (result == GL_FALSE)
		{

			glGetShaderiv(VertID, GL_INFO_LOG_LENGTH, &length);
			//std::vector<char> error(length);
			glGetShaderInfoLog(VertID, length, &length, &error[0]);
			std::cout << "Failed to compile VertexShader: " << &error[0] << std::endl;
			glDeleteShader(VertID);
			//EngineErrorResponse(0x10, VertID, (char*)Filepath);
		}
		break;
	case Frag:

		glGetShaderiv(FragID, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			glGetShaderiv(FragID, GL_INFO_LOG_LENGTH, &length);
			glGetShaderInfoLog(FragID, length, &length, &error[0]);
			Print("Failed to compile Fragment Shader:" << &error[0]);
			glDeleteShader(FragID);
			//	EngineErrorResponse(0x11, FragID, (char*)Filepath);
		}
		break;

	case Program:
		glGetProgramiv(GL_Handle, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			glGetProgramiv(GL_Handle, GL_INFO_LOG_LENGTH, &length);
			glGetProgramInfoLog(GL_Handle, length, &length, &error[0]);
			std::cout << "Link Fail " << &error[0] << std::endl;
			glDeleteShader(GL_Handle);
			//	EngineErrorResponse(0x12, ID, (char*)Filepath);
		}
		break;
	};
}

// Statics
//--------

std::stack<Shader *> Shader::ActiveShader;








void Shader::SetUniform(const char* _name, bool _val)
{
	_GL(glUniform1i(glGetUniformLocation(GL_Handle, _name), _val));
}
void Shader::SetUniform(const char* _name, float _val)
{
	_GL(glUniform1f(glGetUniformLocation(GL_Handle, _name), _val));
}
void Shader::SetUniform(const char* _name, float _val, float _val2)
{
	_GL(glUniform2f(glGetUniformLocation(GL_Handle, _name), _val, _val2));
}
void Shader::SetUniform(const char* _name, float _val, float _val2, float _val3)
{
	_GL(glUniform3f(glGetUniformLocation(GL_Handle, _name), _val, _val2, _val3));
}
void Shader::SetUniform(const char* _name, int _val)
{
	_GL(glUniform1i(glGetUniformLocation(GL_Handle, _name), _val));
}



void Shader::SetUniform(const char* _name, uint64_t _val)
{
	GLuint Location = glGetUniformLocation(GL_Handle, _name);
	//glUniform1ui(Location, _val);
	_GL(glProgramUniformui64NV(GL_Handle, glGetUniformLocation(GL_Handle, _name), _val));
}



void Shader::SetUniform(const char* _name, int _val, int _val2)
{
	_GL(glUniform2i(glGetUniformLocation(GL_Handle, _name), _val, _val2));
}
void Shader::SetUniform(const char* _name, int _val, int _val2, int _val3)
{
	GLuint Location = glGetUniformLocation(GL_Handle, _name);
	_GL(glUniform3i(glGetUniformLocation(GL_Handle, _name), _val, _val2, _val3));
}
void Shader::SetUniform(const char* _name, Vec2 _val)
{
	_GL(glUniform2fv(glGetUniformLocation(GL_Handle, _name), 1, &_val[0]));
}
void Shader::SetUniform(const char* _name, Vec3 _val)
{
	_GL(glUniform3fv(glGetUniformLocation(GL_Handle, _name), 1, &_val[0]));
}
void Shader::SetUniform(const char* _name, Vec4 _val)
{
	_GL(glUniform4fv(glGetUniformLocation(GL_Handle, _name), 1, &_val[0]));
}
void Shader::SetUniform(const char* _name, Mat3 _val)
{
	_GL(glUniformMatrix3fv(glGetUniformLocation(GL_Handle, _name), 1, GL_FALSE, glm::value_ptr(_val)));
}
void Shader::SetUniform(const char* _name, Mat4 _val)
{
	_GL(glUniformMatrix4fv(glGetUniformLocation(GL_Handle, _name), 1, GL_FALSE, glm::value_ptr(_val)));
}




void Shader::SetUniform(const char* _name, std::vector < Vec2>& _array)
{
	_GL(glUniform2fv(glGetUniformLocation(GL_Handle, _name), (GLsizei)_array.size(), glm::value_ptr(_array[0])));
}
void Shader::SetUniform(const char* _name, std::vector < Vec3>& _array)
{
	_GL(glUniform3fv(glGetUniformLocation(GL_Handle, _name), (GLsizei)_array.size(), glm::value_ptr(_array[0])));
}
void Shader::SetUniform(const char* _name, std::vector < Vec4>& _array)
{
	_GL(glUniform4fv(glGetUniformLocation(GL_Handle, _name), (GLsizei)_array.size(), glm::value_ptr(_array[0])));
}
void Shader::SetUniform(const char* _name, std::vector < Mat3>& _array)
{
	_GL(glUniformMatrix3fv(glGetUniformLocation(GL_Handle, _name), (GLsizei)_array.size(), GL_FALSE, glm::value_ptr(_array[0])));
}
void Shader::SetUniform(const char* _name, std::vector<Mat4>& _array)
{
	_GL(glUniformMatrix4fv(glGetUniformLocation(GL_Handle, _name),2, GL_FALSE, reinterpret_cast<GLfloat*>(&_array[0]))); // Passing 20 matrices
}



void Shader::SetTextureUniform(const char *_name, uint64_t _tex)
{
	_GL(glProgramUniformui64NV(GL_Handle, glGetUniformLocation(GL_Handle, _name), _tex));
}

int Shader::AttributeLocation(const char *_name)
{
	return glGetAttribLocation(GL_Handle, _name);
}
Shader::Shader(std::string _vertstring, std::string _fragstring)
{
	CompileStrings(_vertstring, _fragstring);
}
 
void Shader::CompileStrings(std::string _vertstring, std::string _fragstring)
{
	enum ShaderType { VERTEX, FRAGMENT, PROGRAM };

	VertID = glCreateShader(GL_VERTEX_SHADER);
	FragID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertSource = _vertstring.c_str();
	const char* fragSource = _fragstring.c_str();

	glShaderSource(VertID, 1, &vertSource, NULL);
	glCompileShader(VertID);
	GetShaderError(Vert);

	CheckGLERROR();

	glShaderSource(FragID, 1, &fragSource, NULL);
	glCompileShader(FragID);
	GetShaderError(Frag);
	uint32_t ERR = 0;

	CheckGLERROR();
	GL_Handle = glCreateProgram();
	glAttachShader(GL_Handle, VertID);
	glAttachShader(GL_Handle, FragID);
	glLinkProgram(GL_Handle);
	GetShaderError(Program);

	CheckGLERROR();

	glValidateProgram(GL_Handle);
	glDetachShader(GL_Handle, VertID);
	glDetachShader(GL_Handle, FragID);
	glDeleteShader(VertID);
	glDeleteShader(FragID);

	glUseProgram(GL_Handle);

	CheckGLERROR();

	return;
}

