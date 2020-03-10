

#include<string>
#include"Shader.h"
#include"../Camera/Camera2D.h"


GLuint ImageQuadVAO{ 0 };
GLuint ImageQuadVBO{ 0 };
Shader *TextureRenderer{ nullptr };

//===============================================================================================================

std::string VTextureRenderer =
"#version 330 core                 \n\
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

std::string FTextureRenderer =
"#version 330 core               \n\
uniform sampler2D DiffuseTexture;\n\
in vec2 TexCoords;               \n\
out vec4 FragColor;              \n\
void main()                      \n\
{                                \n\
    FragColor = texture(DiffuseTexture, TexCoords);\n\
}";

//===============================================================================================================
