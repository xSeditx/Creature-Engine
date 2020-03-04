//
//
//#include<string>
//#include"Shader.h"
//#include"../Camera/Camera2D.h"
//
//
//
//GLuint DebugQuadVAO{0};
//GLuint DebugQuadVBO{0};
//Shader *QuadRenderer{ nullptr };
//Camera2D *debugCamera{ nullptr };
//
//
////===============================================================================================================
//
//std::string VQuadRenderer =
//"#version 330 core     \n\
//layout(location = 0) in vec2 aPos; \n\
//uniform vec4 Position; \n\
//uniform mat4 ProjectionMatrix;     \n\
//uniform mat4 ViewMatrix;           \n\
//void main()                        \n\
//{                                  \n\
//    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
//    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
//    gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
//}";
//
//std::string FQuadRenderer =
//"#version 330 core \n\
//out vec4 FragColor;            \n\
//void main()                    \n\
//{                              \n\
//    FragColor = vec4(1,1,1,1);\n\
//}";
 

