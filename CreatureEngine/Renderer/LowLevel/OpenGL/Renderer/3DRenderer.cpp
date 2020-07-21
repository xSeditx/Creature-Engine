#include<utility>

#include"3DRenderer.h"
namespace OpenGL {


    Renderer3D::Renderer3D(iVec2 _size)
        trace(1) 
    {
        Main_FBO = new FrameBufferObject(_size.x, _size.y);
        Main_Camera = new Camera3D(_size, Vec3(0, 0, 1000), Vec3(0, 0, 0));

        CubeVAO = new VertexArrayObject();

        InstanceRenderer = new Shader(VinstanceRenderer, FinstanceRenderer);
        auto C = make_Cube({ 0,0,0 }, 1);
        CubeInds = C.first;
        InstanceRenderer->Bind();
        {
            CubeVBO = new VertexBufferObject<Vec3>(C.second);
            CubeVAO->Attach(BufferTypes::VERTEX, CubeVBO);
        }
        InstanceRenderer->Unbind();
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
            Main_VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(VertexBuffer));
            Main_VAO->Attach(BufferTypes::COLOR, new VertexBufferObject<Vec4>(ColorBuffer));
        }
        Main_Program->Unbind();

        Return();
    }

    void Renderer3D::Render()
        trace(1)
    {
        Update();
        Main_FBO->Bind();
        Main_FBO->Clear();
        {// Bind and clear the Frame Buffer
       //   Main_Program->Bind();
       //   {// Bind our basic Shader
       //       Main_VAO->Bind(); // Set the Vertex Array Object
       //       Main_Camera->Bind(); // Bind our Camera Uniforms
       //
       //       OpenGL::Renderer::drawArray(Main_VAO->Buffers[0]->ElementCount); // Render Geometry
       //   }
       //   Main_Program->Unbind();
        }


        CubeVAO->Bind();
        Update();

        InstanceRenderer->Bind();
        {
            CubeVAO->Bind();
            Main_Camera->Bind();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            Renderer::drawElementsInstanced(&CubeInds[0], 32, InstanceCount);
        }// void drawElementsInstanced(void *_indices, uint32_t _indicesCount, uint32_t _instanceCount)
        InstanceRenderer->Unbind();

        Main_FBO->Unbind();

        Return();
    }








    /* Submits a Quad using the Current Draw Color */
    void Renderer3D::renderCube(Vec3 _center, float _size)
    {
        auto Cube = make_Cube(_center, _size);
        Cube_Positions.push_back({ _center.x,_center.y,_center.z,_size });
       // CubeInds.insert(CubeInds.end(), Cube.first.begin(), Cube.first.end());
        ++InstanceCount;
    }



    std::pair<std::vector<uint32_t>, std::vector<Vec3>> make_Cube(Vec3 _center, float _size)
    {
        std::vector<Vec3> Vresult;
        std::vector<uint32_t> Iresult;

        Vec3 V[] =
        {
            Vec3(_center.x + -_size, _center.y + -_size, _center.z + _size), // FRONT    // 0                                             
            Vec3(_center.x + _size, _center.y + -_size, _center.z + _size),             // 1                                    
            Vec3(_center.x + _size, _center.y + _size, _center.z + _size),             // 2                                    
            Vec3(_center.x + -_size, _center.y + _size, _center.z + _size),             // 3                                    
            Vec3(_center.x + _size, _center.y + _size, _center.z + _size), // RIGHT    // 4    2                                        
            Vec3(_center.x + _size, _center.y + _size, _center.z + -_size),             // 5                                    
            Vec3(_center.x + _size, _center.y + -_size, _center.z + -_size),             // 6                                    
            Vec3(_center.x + _size, _center.y + -_size, _center.z + _size),             // 7    1                                
            Vec3(_center.x + -_size, _center.y + -_size, _center.z + -_size), // BACK     // 8                                            
            Vec3(_center.x + _size, _center.y + -_size, _center.z + -_size),             // 9    6                               
            Vec3(_center.x + _size, _center.y + _size, _center.z + -_size),             // 10   5                                
            Vec3(_center.x + -_size, _center.y + _size, _center.z + -_size),             // 11
            Vec3(_center.x + -_size, _center.y + -_size, _center.z + -_size), // LEFT     // 12   8                                        
            Vec3(_center.x + -_size, _center.y + -_size, _center.z + _size),             // 13   7                              
            Vec3(_center.x + -_size, _center.y + _size, _center.z + _size),             // 14   3                                
            Vec3(_center.x + -_size, _center.y + _size, _center.z + -_size),             // 15  11                                
            Vec3(_center.x + _size, _center.y + _size, _center.z + _size), // TOP      // 16   2                                      
            Vec3(_center.x + -_size, _center.y + _size, _center.z + _size),             // 17   3                                
            Vec3(_center.x + -_size, _center.y + _size, _center.z + -_size),             // 18   11                                
            Vec3(_center.x + _size, _center.y + _size, _center.z + -_size),             // 19   5                                
            Vec3(_center.x + -_size, _center.y + -_size, _center.z + -_size), // BOTTOM   // 20   8                                   
            Vec3(_center.x + _size, _center.y + -_size, _center.z + -_size),             // 21   9                          
            Vec3(_center.x + _size, _center.y + -_size, _center.z + _size),             // 22   1                          
            Vec3(_center.x + -_size, _center.y + -_size, _center.z + _size),             // 23   0                               
        };


        uint32_t Indexdata[] =
        {// 36
            0,1,2,    0,2,3,     // FRONT
            6,5,4,    4,7,6,     // RIGHT 
            10,9,8,   8,11,10,   // BACK
            12,13,14, 12,14,15,  // LEFT
            18,17,16, 16,19,18,  // TOP
            20,21,22, 20,22,23   // BOTTOM
        };
        for_loop(i, 24)
        {
            Vresult.push_back(V[i]);
        }
        for_loop(i, 36)
        {
            Iresult.push_back(Indexdata[i]);
        }
        return { Iresult, Vresult };
    }


    _static Vec3 *Renderer3D::Concat_Vertices(Vec3 *_dataA, size_t _szA, Vec3 *_dataB, size_t _szB)
        trace(1)
    {
        size_t newSize = _szA + _szB;

        Vec3 *newBuffer = (Vec3*)malloc(newSize);
        memcpy(newBuffer, _dataA, _szA);

        memcpy((uint8_t*)newBuffer + _szA, _dataB, _szB);

        // Destroy our old buffer so we can concatnate it with the new one
        delete(_dataB);
        delete(_dataA);

        Return(newBuffer);
    }


    void Renderer3D::Update()
        trace(1)
    {
        Main_Camera->Update();

        if (Cube_Positions.size())
        {// We have submitted some Cubes to be batched
            trace_scope("Cube_Data");
            CubeVBO->Update(Cube_Positions);
            CubeVAO->Buffers[0]->Update(Cube_Positions);
        }
        Return();
    }



}// NS OpenGL











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
