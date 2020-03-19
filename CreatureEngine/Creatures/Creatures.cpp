#include"Creatures.h"




namespace Creatures
{
    Shader* Creature::CreatureShader{ nullptr };
    EntityComponentSystem CreatureECS;



///    std::string VCreatureRenderer =
///        "#version 330 core     \n\
///layout(location = 0) in vec3 aPos; \n\
///layout(location = 1) in vec4 Color; \n\
///uniform mat4 ProjectionMatrix;     \n\
///uniform mat4 ViewMatrix;           \n\
///out vec4 Col;                      \n\
///void main()                        \n\
///{                                  \n\
///    Col = Color; \n\
///    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
///    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
///    gl_Position = ModelViewProjectionMatrix * vec4(aPos.xyz, 1.0f); \n\
///}";
///
///    std::string FCreatureRenderer =
///        "#version 330 core \n\
///in vec4 Col;                   \n\
///out vec4 FragColor;            \n\
///void main()                    \n\
///{                              \n\
///    FragColor = Col;\n\
///}";
///
///    std::string VCreatureInstanceRenderer = // This is breaking on Exit when using the Overloaded New/Delete Why?
///        "#version 330 core     \n\
///layout(location = 0) in vec2 aPos; \n\
///layout(location = 1) in vec4 Position; \n\
///layout(location = 2) in vec4 Color; \n\
///uniform mat4 ProjectionMatrix;     \n\
///uniform mat4 ViewMatrix;           \n\
///out vec4 Col;                      \n\
///void main()                        \n\
///{                                  \n\
///    Col = Color; \n\
///    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
///    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
///    gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
///}";
///
///    std::string FCreatureInstanceRenderer =
///        "#version 330 core \n\
///in vec4 Col;                   \n\
///out vec4 FragColor;            \n\
///void main()                    \n\
///{                              \n\
///    FragColor = Col;\n\
///}";
///
    void InitCreatureShader()
    {
       // Creature::CreatureShader = new Shader(VCreatureInstanceRenderer, FCreatureRenderer);
    }
}