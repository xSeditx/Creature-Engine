#include"Renderer.h"
#include"../../../../Core/Common.h"
#include"../../../../Core/Threading/Threadpool.h"
#include"../../../../Profiling/Timing/Benchmark.h"

#include"../Camera/Camera3D.h"

#include"../Shader/Shader.h"

namespace OpenGL
{
    /* ===========================================================================================================
    /*              Test bed of a Renderer Class for 3D Geometry                              
    /*  Creation Date: 7/10/20
    /* =========================================================================================================== */
    struct Renderer3D
    {
        Renderer3D(iVec2 _size);

        void Update();
        void Render();

        /* Gets the Camera the Renderer uses */
        Camera3D& g_Camera() { return *Main_Camera; }

        /* Returns Reference to the Frame Buffer Object for the Renderer */
        FrameBufferObject& g_FBO() { return *Main_FBO; }

        /* Gets a Reference to the Default Shader for this Renderer class */
        Shader& g_Program() { return *Main_Program; }


        /* Submits a Cube using the Current Draw Color */
        void renderCube(Vec3 _center, float _size);


        Shader  *InstanceRenderer{ nullptr };
        Shader   *Main_Program  { nullptr };
        Camera3D *Main_Camera   { nullptr };
        FrameBufferObject *Main_FBO{ nullptr };
        VertexArrayObject *Main_VAO{ nullptr };


        std::vector<Vec3> Vertices; // The xyz components to our Geometry
        std::vector<Vec4> Colors;   // The Colors of our Geometry

        /* Returns a new buffer which is a combination of the two old buffers */
        static Vec3 *Concat_Vertices(Vec3 *_dataA, size_t _szA, Vec3 *_dataB, size_t _szB);


        Vec3 *Vertex_Data;
        Vec4 *Color_Data;
        uint32_t Vertex_Elements{ 0 };

        VertexArrayObject *CubeVAO;
        VertexBufferObject<Vec3> *CubeVBO;
        std::vector<Vec4> Cube_Positions;
        std::vector<uint32_t> CubeInds;

        uint32_t InstanceCount{ 0 };

 
        // ================================================================================================================

        std::string VinstanceRenderer =
            "#version 330 core                         \n\
                layout(location = 0) in vec3 WorldPosition; \n\
                layout(location = 1) in vec4 Position; \n\
                layout(location = 2) in vec4 Color; \n\
                uniform mat4 ProjectionMatrix;     \n\
                uniform mat4 ViewMatrix;           \n\
                out vec4 Col;                      \n\
                void main()                        \n\
                {                                  \n\
                    Col = Color; \n\
                    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
                    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
                    gl_Position = ModelViewProjectionMatrix * vec4( (WorldPosition.x * Position.w) + Position.x, (WorldPosition.y * Position.w) +  Position.y, Position.z * WorldPosition.z, 1.0); \n\
                }";

        std::string FinstanceRenderer =
            "#version 330 core \n\
                in vec4 Col;                   \n\
                out vec4 FragColor;            \n\
                void main()                    \n\
                {                              \n\
                    FragColor = Col;\n\
                }";


    };

    std::pair<std::vector<uint32_t>, std::vector<Vec3>> make_Cube(Vec3 _center, float _size);
}// NS OpenGL