

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

Shader *shader_Basic        = nullptr; // Simple basic Shader for rapid rendering and prototyping
Shader *shader_Blur         = nullptr; // Gaussian Blurs the provided texture
Shader *shader_Collider     = nullptr; // Renders GL_LINES around an objects collider the color of the normals for the colliders surface  
Shader *shader_Debug        = nullptr; // Basic Debug shader providing various data not otherwise rendered
Shader *shader_DebugQuad    = nullptr; // Renders a Texture to the Screen for debug purposes
Shader *shader_Light        = nullptr; // Provides basic Lighting for a Scene
Shader *shader_Shadow       = nullptr; // Renders the Shadowmap provided 
Shader *shader_Skybox       = nullptr; // Renders the Skybox around the world
Shader *shader_Sprite       = nullptr; // Renders a Specified Sprite to the screen
 
/* Built in Shaders for Rapid usecases
NOTE: Might Enumerate these in the future to make loading and unloading of specific ones
      simpler */
bool load_Builtin_Shaders()             
{
        
    /// ======================================================================================================================================================================================
    ///   Usage Name                     Location                          Filename                  Description
    /// ======================================================================================================================================================================================
        shader_Blur      = new Shader("Resources/Blur.sfx");           // Blur.sfx           Gaussian Blurs the provided texture
        shader_Debug     = new Shader("Resources/Debug.sfx");          // Debug.sfx          Basic Debug shader providing various data not otherwise rendered
        shader_Light     = new Shader("Resources/Light.sfx");          // Light Shader.      Provides basic Lighting for a Scene
        shader_Shadow    = new Shader("Resources/Shadow.sfx");         // Shadow.sfx         Renders the Shadowmap provided
        shader_Skybox    = new Shader("Resources/Skybox.sfx");         // Skybox.sfx         Renders the Skybox around the world
        shader_Sprite    = new Shader("Resources/Sprite.sfx");         // Sprite.sfx         Renders a Specified Sprite to the screen
        shader_Collider  = new Shader("Resources/Collider.sfx");       // Collider.sfx       Renders GL_LINES around an objects collider the color of the normals for the colliders surface
        shader_DebugQuad = new Shader("Resources/DebugQuad.sfx");      // DebugQuad          Renders a Texture to the Screen for debug purposes
        shader_Basic     = new Shader("Resources/BasicShader.sfx");    // BasicShader.sfx    Simple basic Shader for rapid rendering and prototyping
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


