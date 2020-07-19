#include"Renderer.h"

#include"../Camera/Camera3D.h"

#include"../Shader/Shader.h"
#include"../../../../Core/Threading/Threadpool.h"

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



            //===================================================================
            // VERTEX BUFFER ONE
            //===================================================================
/*
 
            auto VertexBuffer =
                Core::Threading::ThreadPool::get().Async(
                    []()
            { 
                float sz = 500; // Size of the Cluster
                size_t Count = 10000;
                Vec3 *result =(Vec3*)malloc(Count * sizeof(Vec3));

                // Create a Load of Random Geometry to Test 
                for_loop(i, Count)
                {
                    Vec3 Vert = // Random Vertices from -sz to sz range
                    { 
                        RANDOM_RANGE(sz), 
                        RANDOM_RANGE(sz),
                        RANDOM_RANGE(sz)
                    };
                    result[i] = Vert;
                }
                return result;
            });

            //===================================================================
            // VERTEX BUFFER TWO
            //===================================================================
            auto VertexBuffer2 =
                Core::Threading::ThreadPool::get().Async(
                    []()
            {
                float sz = 500; // Size of the Cluster
                size_t Count = 10000;
                Vec3 *result = (Vec3*)(malloc(Count * sizeof(Vec3)));

                for_loop(i, Count)
                {
                    Vec3 Vert = // Random Vertices from -sz to sz range
                    {
                        RANDOM_RANGE(sz),
                        RANDOM_RANGE(sz),
                        RANDOM_RANGE(sz)
                    };
                    result[i] = Vert;
                }
                return result;
            });



            //===================================================================
            // COLOR BUFFER 
            //===================================================================

            auto ColorBuffer =
                Core::Threading::ThreadPool::get().Async(
                    []()
            {
                size_t Count = 10000;
                Vec4 *result = (Vec4*)malloc(Count * sizeof(Vec4));

                for_loop(i, Count)
                {
                    Vec4 Col = // Also use Random Colors
                        OpenGL::Renderer::normalize_RGBA_Color
                        (
                        (int)RANDOM(255),
                            (int)RANDOM(255),
                            (int)RANDOM(255),
                            (int)RANDOM(255)
                        );
                    result[i] = Col;
                }
                return result;
            });


                //===================================================================
                auto FullBuffer = Core::Threading::ThreadPool::get().Async
            (
                Concat_Vertices, 
                VertexBuffer.get(), 10000 * sizeof(Vec3),
                VertexBuffer2.get(), 10000 * sizeof(Vec3)
            );
            auto Buff = FullBuffer.get();
  */    
            //===================================================================
            std::vector<Vec3> VertexBuffer;
            std::vector<Vec3> VertexBuffer2;
            std::vector<Vec4> ColorBuffer;
            float sz = 500; // Size of the Cluster
            size_t Count = 10000;
 
            // Create a Load of Random Geometry to Test 
            for_loop(i, Count)
            {
                Vec3 Vert = // Random Vertices from -sz to sz range
                {
                    RANDOM_RANGE(sz),
                    RANDOM_RANGE(sz),
                    RANDOM_RANGE(sz)
                };

                VertexBuffer.push_back(Vert);
            }

//  for_loop(i, Count)
//  {
//      Vec3 Vert = // Random Vertices from -sz to sz range
//      {
//          RANDOM_RANGE(sz),
//          RANDOM_RANGE(sz),
//          RANDOM_RANGE(sz)
//      };
//      VertexBuffer2.push_back(Vert);
//  }


            for_loop(i, Count)
            {
                Vec4 Col = // Also use Random Colors
                    OpenGL::Renderer::normalize_RGBA_Color
                    (
                    (int)RANDOM(255),
                        (int)RANDOM(255),
                        (int)RANDOM(255),
                        (int)RANDOM(255)
                    );
                ColorBuffer.push_back(Col);
            }


            Main_VAO = new VertexArrayObject();

            Main_Program = shader_Basic3DRenderer;
            Main_Program->Bind();
            {
                Main_VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(VertexBuffer ));
                Main_VAO->Attach(BufferTypes::COLOR,  new VertexBufferObject<Vec4>(ColorBuffer ));
            }
            Main_Program->Unbind();


           // static Vec3 *Concat_Vertices(Vec3 *_dataA, size_t _szA, Vec3 *_dataB, size_t _szB)
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

        /* Returns a new buffer which is a combination of the two old buffers */
        static Vec3 *Concat_Vertices(Vec3 *_dataA, size_t _szA, Vec3 *_dataB, size_t _szB)
        {
            size_t newSize = _szA + _szB;

            Vec3 *newBuffer = (Vec3*)malloc(newSize);
            memcpy(newBuffer, _dataA, _szA);

            memcpy((uint8_t*)newBuffer + _szA, _dataB, _szB);

            // Destroy our old buffer so we can concatnate it with the new one
            delete(_dataB);
            delete(_dataA);

            return newBuffer;
        }


        Vec3 *Vertex_Data;
        Vec4 *Color_Data;
        uint32_t Vertex_Elements{ 0 };
    };

}// NS OpenGL