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


Shader::Shader(std::string _vertstring, std::string _fragstring)
{
    CompileStrings(_vertstring, _fragstring);
}

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
	WARN_ME("Error creatd here if Shader is made on the Stack and destroyed after strings are gone");
	if (Filepath)
	{
		DEBUGPrint(CON_Red, "Called the Shader Destructor: " << Filepath);
	}
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
 }
 
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
void Shader::SetUniform(const char* _name, float _val, float _val2, float _val3, float _val4)
{
    _GL(glUniform4f(glGetUniformLocation(GL_Handle, _name), _val, _val2, _val3, _val4));
}

 

void Shader::SetUniform(const char* _name, int _val)
{
	_GL(glUniform1i(glGetUniformLocation(GL_Handle, _name), _val));
}

void Shader::SetUniform(const char* _name, uint64_t _val)
{
	GLuint Location = glGetUniformLocation(GL_Handle, _name);
	glProgramUniformui64NV(GL_Handle, glGetUniformLocation(GL_Handle, _name), _val);
}



void Shader::SetUniform(const char* _name, int _val, int _val2)
{
	_GL(glUniform2i(glGetUniformLocation(GL_Handle, _name), _val, _val2));
}
void Shader::SetUniform(const char* _name, int _val, int _val2, int _val3)
{
	glUniform3i(glGetUniformLocation(GL_Handle, _name), _val, _val2, _val3);
}
void Shader::SetUniform(const char* _name, Vec2 _val)
{
	glUniform2fv(glGetUniformLocation(GL_Handle, _name), 1, &_val[0]);
}
void Shader::SetUniform(const char* _name, Vec3 _val)
{
	glUniform3fv(glGetUniformLocation(GL_Handle, _name), 1, &_val[0]);
}
void Shader::SetUniform(const char* _name, Vec4 _val)
{
	glUniform4fv(glGetUniformLocation(GL_Handle, _name), 1, &_val[0]);
}
void Shader::SetUniform(const char* _name, Mat3 _val)
{
	glUniformMatrix3fv(glGetUniformLocation(GL_Handle, _name), 1, GL_FALSE, glm::value_ptr(_val));
}
void Shader::SetUniform(const char* _name, Mat4 _val)
{
	glUniformMatrix4fv(glGetUniformLocation(GL_Handle, _name), 1, GL_FALSE, glm::value_ptr(_val));
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
void Shader::SetTextureUniform(const char *_name, uint32_t _textureID, int _slot)
{
    glActiveTexture(GL_TEXTURE0 + _slot);
    glBindTexture(GL_TEXTURE_2D, _textureID);
   SetUniform(_name, _slot);
}

int Shader::AttributeLocation(const char *_name)
{
	return glGetAttribLocation(GL_Handle, _name);
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













/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================
/// =================================================================================================================================





#include"../Camera/Camera2D.h"


//===============================================================================================================
/* Built in Shaders for Rapid usecases
NOTE: Might Enumerate these in the future to make loading and unloading of specific ones
      simpler   */           
//===============================================================================================================

Shader *shader_Basic{ nullptr };     // Simple basic Shader for rapid rendering and prototyping
Shader *shader_Blur{ nullptr };      // Gaussian Blurs the provided texture
Shader *shader_Collider{ nullptr };  // Renders GL_LINES around an objects collider the color of the normals for the colliders surface  
Shader *shader_Debug{ nullptr };     // Basic Debug shader providing various data not otherwise rendered
Shader *shader_DebugQuad{ nullptr }; // Renders a Texture to the Screen for debug purposes
Shader *shader_Light{ nullptr };     // Provides basic Lighting for a Scene
Shader *shader_Shadow{ nullptr };    // Renders the Shadowmap provided 
Shader *shader_Skybox{ nullptr };    // Renders the Skybox around the world
Shader *shader_Sprite{ nullptr };    // Renders a Specified Sprite to the screen

Shader *shader_TextureRenderer{nullptr};
Shader *shader_QuadRenderer{ nullptr };
Shader *shader_BasicRenderer{ nullptr };

//===============================================================================================================

GLuint ImageQuadVAO{ 0 };
GLuint ImageQuadVBO{ 0 };

GLuint DebugQuadVAO;
GLuint DebugQuadVBO;

Camera2D *debug_Camera;
//===============================================================================================================


//===============================================================================================================
/* Built in Shaders for Rapid usecases
NOTE: Might Enumerate these in the future to make loading and unloading of specific ones
      simpler */
//===============================================================================================================

bool load_Builtin_Shaders()
{
    /// ======================================================================================================================================================================================
    ///   Usage Name                     Location                          Filename                  Description
    /// ======================================================================================================================================================================================
    shader_Blur = new Shader("Resources/Blur.sfx");             // Blur.sfx           Gaussian Blurs the provided texture
    shader_Debug = new Shader("Resources/Debug.sfx");           // Debug.sfx          Basic Debug shader providing various data not otherwise rendered
    shader_Light = new Shader("Resources/Light.sfx");           // Light Shader.      Provides basic Lighting for a Scene
    shader_Shadow = new Shader("Resources/Shadow.sfx");         // Shadow.sfx         Renders the Shadowmap provided
    shader_Skybox = new Shader("Resources/Skybox.sfx");         // Skybox.sfx         Renders the Skybox around the world
    shader_Sprite = new Shader("Resources/Sprite.sfx");         // Sprite.sfx         Renders a Specified Sprite to the screen
    shader_Collider = new Shader("Resources/Collider.sfx");     // Collider.sfx       Renders GL_LINES around an objects collider the color of the normals for the colliders surface
    shader_DebugQuad = new Shader("Resources/DebugQuad.sfx");   // DebugQuad          Renders a Texture to the Screen for debug purposes
    shader_Basic = new Shader("Resources/BasicShader.sfx");     // BasicShader.sfx    Simple basic Shader for rapid rendering and prototyping
/// ======================================================================================================================================================================================
    return true;
}

/* Frees the memory held by the built in Shaders*/
bool cleanup_Builting_Shaders()
{
    bool Result = true;
    if (shader_Blur)         delete(shader_Blur);      else  Result = false;
    if (shader_Debug)        delete(shader_Debug);     else  Result = false;
    if (shader_Light)        delete(shader_Light);     else  Result = false;
    if (shader_Shadow)       delete(shader_Shadow);    else  Result = false;
    if (shader_Skybox)       delete(shader_Skybox);    else  Result = false;
    if (shader_Sprite)       delete(shader_Sprite);   else  Result = false;
    if (shader_Collider)     delete(shader_Collider); else  Result = false;
    if (shader_DebugQuad)    delete(shader_DebugQuad); else  Result = false;
    if (shader_Basic)        delete(shader_Basic);  else  Result = false;
    return Result;
}



//===============================================================================================================
/* DUNNO IF HTIS HAS BEEN SETUP BUT IT APPEARS TO HAVE*/
std::string VInstance_TextureRenderer =
"#version 330 core                                                                                                                                \n\
         layout(location = 0) in vec2 aPos;                                                                                                       \n\
         layout(location = 1) in vec4 Position;                                                                                                   \n\
         uniform mat4 ProjectionMatrix;                                                                                                           \n\
         uniform mat4 ViewMatrix;                                                                                                                 \n\
         out  vec2 TexCoords;                                                                                                                     \n\
         void main()                                                                                                                              \n\
         {                                                                                                                                        \n\
             mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));                                                                                     \n\
             mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);                                                               \n\
             gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
         }";


std::string VTextureRenderer =
"#version 330 core                          \n\
         layout(location = 0) in vec2 aPos; \n\
         uniform vec4 Position;             \n\
         uniform mat4 ProjectionMatrix;     \n\
         uniform mat4 ViewMatrix;           \n\
         out vec2 TexCoords;                \n\
         void main()                        \n\
         {                                  \n\
             TexCoords = aPos;              \n\
             mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
             mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
             gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
         }";


std::string FAlphaBlendTextureRenderer =
"#version 330 core                        \n\
         uniform sampler2D DiffuseTexture;\n\
         in vec2  TexCoords;              \n\
         out vec4 FragColor;              \n\
         void main()                      \n\
         {                                \n\
             FragColor = texture(DiffuseTexture, TexCoords);\n\
         }";

std::string FTextureRenderer =
"#version 330 core                                                             \n\
         uniform sampler2D DiffuseTexture;                                     \n\
         in  vec2 TexCoords;                                                   \n\
         out vec4 FragColor;                                                   \n\
         void main()                                                           \n\
         {                                                                     \n\
             FragColor = vec4(texture(DiffuseTexture,TexCoords.xy).xyz, 1.0);  \n\
         }";


//===============================================================================================================

std::string VQuadRenderer =
"#version 330 core                           \n\
          layout(location = 0) in vec2 aPos; \n\
          uniform vec4 Position;             \n\
          uniform mat4 ProjectionMatrix;     \n\
          uniform mat4 ViewMatrix;           \n\
          out vec2 TexCoords;                \n\
          void main()                        \n\
          {                                  \n\
              TexCoords = aPos;              \n\
              mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
              mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
              gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
          }";

std::string FQuadRenderer =
"#version 330 core                                           \n\
          uniform sampler2D DiffuseTexture;                  \n\
          in vec2 TexCoords;                                 \n\
          out vec4 FragColor;                                \n\
          void main()                                        \n\
          {                                                  \n\
              FragColor = texture(DiffuseTexture, TexCoords);\n\
          }";
 

//===============================================================================================================


std::string VBasicRenderer =
    "#version 330 core                         \n\
        layout(location = 0) in vec2 Position; \n\
        layout(location = 1) in vec4 VColor; \n\
        uniform mat4 ProjectionMatrix;     \n\
        uniform mat4 ViewMatrix;           \n\
        out vec4 Col;                      \n\
        void main()                        \n\
        {                                  \n\
            Col = VColor; \n\
            mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
            mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
            gl_Position = ModelViewProjectionMatrix * vec4( Position.x, Position.y, -1.0, 1.0); \n\
        }";

std::string FBasicRenderer =
    "#version 330 core            \n\
        in vec4 Col;          \n\
        out vec4 FragColor;   \n\
        void main()           \n\
        {                     \n\
            FragColor = vec4(Col.x, Col.y, Col.z,1);  \n\
        }";
//===============================================================================================================




/* DEPRECATED: please use load builting shaders */
bool init_DefaultShaders()
{
    debug_Camera = new Camera2D(SCREEN_X, SCREEN_Y);

    DebugQuadVAO = OpenGL::new_VAO();
    DebugQuadVBO = OpenGL::new_VBO();

    ImageQuadVAO = OpenGL::new_VAO();
    ImageQuadVBO = OpenGL::new_VBO();

    Vec2 QuadData[6] =
    {
        Vec2(0, 0),  Vec2(1, 0),  Vec2(0, 1),
        Vec2(1, 1),  Vec2(0, 1),  Vec2(1, 0)
    };



    shader_QuadRenderer = new Shader( VQuadRenderer, FQuadRenderer);
    shader_QuadRenderer->Bind();
    {
        OpenGL::bind_VAO(DebugQuadVAO);

        OpenGL::bind_VBO(DebugQuadVBO);
        OpenGL::set_Attribute(2, "aPos");
        OpenGL::set_BufferData(sizeof(QuadData), QuadData);
    }
    shader_QuadRenderer->Unbind();


    //======================================================================================
    shader_TextureRenderer = new Shader(VTextureRenderer, FTextureRenderer);
    shader_TextureRenderer->Bind();
    {
        OpenGL::set_Attribute(2, "aPos");
    }
    shader_TextureRenderer->Unbind();
    //======================================================================================
    shader_BasicRenderer = new Shader(VBasicRenderer, FBasicRenderer);
    return true;
}

// In Header
//enum default_Shader
//{
//    Basic,
//    Blur,
//    Collider,
//    Debug,
//    DebugQuad,
//    Light,
//    Shadow,
//    Skybox,
//    Sprite,
//    TextureRenderer,
//    QuadRenderer,
//    BasicRenderer
//};
//Shader& DefaultShader(default_Shader _type);
// In CPP
//Shader& DefaultShader(default_Shader _type)
//{
//    switch (_type)
//    {
//    case Basic:
//    {
//        static Shader *Basic = new Shader(VBasicRenderer, FBasicRenderer);
//        return *Basic;
//    }break;
//    case Blur:
//    {}break;
//    case Collider:
//    {}break;
//    case Debug:
//    {}break;
//    case DebugQuad:
//    {
//        Shader *Quad = new Shader(VQuadRenderer, FQuadRenderer);
//        return *Quad;
//    }break;
//    case Light:
//    {}break;
//    case Shadow:
//    {}break;
//    case Skybox:
//    {}break;
//    case Sprite:
//    {}break;
//    case TextureRenderer:
//    {
//        static Shader *Tex = new Shader(VTextureRenderer, FTextureRenderer);
//        return *Tex;
//    }break;
//    case QuadRenderer:
//    {}break;
//    case BasicRenderer:
//    {}break;
//    };
//}