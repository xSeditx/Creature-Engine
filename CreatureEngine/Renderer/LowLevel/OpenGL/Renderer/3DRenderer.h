#include"Renderer.h"

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
        Renderer3D(iVec2 _size)
        {
            Main_FBO = new FrameBufferObject(_size.x, _size.y);
            Main_Camera = new Camera3D( _size, Vec3(0, 0, 1000), Vec3(0, 0, 0));

            float sz = 500;

            // Create a Load of Random Geometry to Test 
            for_loop(i, 10000)
            {
                Vec3 Vert = // Random Vertices from -sz to sz range
                { 
                    RANDOM_RANGE(sz), 
                    RANDOM_RANGE(sz),
                    RANDOM_RANGE(sz)
                };

                Vec4 Col = // Also use Random Colors
                    OpenGL::Renderer::normalize_RGBA_Color
                    (
                        (int)RANDOM(255),
                        (int)RANDOM(255),
                        (int)RANDOM(255),
                        (int)RANDOM(255)
                    );


                Vertices.push_back(Vert);
                Colors.push_back(Col);
            }

            Main_VAO = new VertexArrayObject();

            Main_Program = shader_Basic3DRenderer;
            Main_Program->Bind();
            {
                Main_VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(Vertices));
                Main_VAO->Attach(BufferTypes::COLOR, new VertexBufferObject<Vec4>(Colors));
            }
            Main_Program->Unbind();
        }


        void Update()
        {
            Main_Camera->Update();
        }

        void Render()
        {
            Update();
            Main_FBO->Bind();
            Main_FBO->Clear();
            {// Bind and clear the Frame Buffer
                Main_Program->Bind();
                {// Bind our basic Shader
                    Main_VAO->Bind(); // Set the Vertex Array Object
                    Main_Camera->Bind(); // Bind our Camera Uniforms

                    OpenGL::Renderer::drawArray(Main_VAO->Buffers[0]->ElementCount); // Render Geometry
                }
                Main_Program->Unbind();
            }
            Main_FBO->Unbind();
        }


        /* Gets the Camera the Renderer uses */
        Camera3D& g_Camera() { return *Main_Camera; }

        /* Returns Reference to the Frame Buffer Object for the Renderer */
        FrameBufferObject& g_FBO() { return *Main_FBO; }

        /* Gets a Reference to the Default Shader for this Renderer class */
        Shader& g_Program() { return *Main_Program; }

        Shader   *Main_Program  { nullptr };
        Camera3D *Main_Camera   { nullptr };
        FrameBufferObject *Main_FBO{ nullptr };
        VertexArrayObject *Main_VAO{ nullptr };


        std::vector<Vec3> Vertices; // The xyz components to our Geometry
        std::vector<Vec4> Colors;   // The Colors of our Geometry
    };

}// NS OpenGL

 