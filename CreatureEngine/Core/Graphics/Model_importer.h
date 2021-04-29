#include"assimp/Importer.hpp"
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing flags

#include"../Common.h"
#include"../../Renderer/LowLevel/OpenGL/OpenGL.h"
#include"../../Renderer/LowLevel/OpenGL/Renderer/GLBuffers.h"
#include"../../Renderer/LowLevel/OpenGL/Renderer/Renderer.h"
#include"../../Renderer/LowLevel/OpenGL/Camera/Camera3D.h"


struct Model
{
    struct Bone
    {

        Bone(aiBone* _bone)
        {
            Name = _bone->mName.C_Str();
            Weight_Count = _bone->mNumWeights;

            for_loop(i, Weight_Count)
            {
                Weights.emplace_back(Weight(_bone->mWeights[i].mVertexId, _bone->mWeights[i].mWeight));
            }
            Mat4 Offset;
            memcpy(&Offset, (void*)&_bone->mOffsetMatrix[0][0], sizeof(float) * 16);
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][1] = _bone->mOffsetMatrix[0][1];
    //     Offset[0][2] = _bone->mOffsetMatrix[0][2];
    //     Offset[0][3] = _bone->mOffsetMatrix[0][3];
    //
    //     Offset[1][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];
    //     Offset[0][0] = _bone->mOffsetMatrix[0][0];

        }

        struct Weight
        {
            Weight(uint32_t _vertID, float _w)
                :
                Vertex_ID(_vertID), Vertex_Weight(_w)
            {}
            uint32_t Vertex_ID;
            float Vertex_Weight;
        };        // Armature

        // Node
        Mat4 Offset;
        std::string  Name;
        uint32_t Weight_Count;
        std::vector<Weight> Weights;

    };

     aiMesh *Mesh;

    std::vector<Bone> Bones;
    uint32_t MaterialIndex;
    std::string Name;
    std::vector<Vec2> UvCoords;
    std::vector<Vec3> Vertices;
    std::vector<Vec3> Normals;
    std::vector<Vec3> Bitangents;
    std::vector<Vec3> Tangents;
    std::vector<Vec4> Colors;
    std::vector<uint32_t> Faces;
    //Method;
    uint32_t Animesh_Count{ 0 };
    uint32_t Bone_Count{ 0 };
    uint32_t Face_Count{ 0 };
    uint32_t Vertices_Count{ 0 };
    uint32_t UV_Count{ 0 };


    void get_Counts()
    {
        Animesh_Count = Mesh->mNumAnimMeshes;
        Bone_Count = Mesh->mNumBones;
        Face_Count = Mesh->mNumFaces;
        Vertices_Count = Mesh->mNumVertices;
        UV_Count = Mesh->mNumUVComponents[0];
    }


    Model(aiMesh *_mesh)
        :
        Mesh(_mesh)
    {
        std::vector<Vec3> Vresult;
        std::vector<uint32_t> Iresult;
        Vec3 _center = { 0, 0, 0 };
    
        float _size = 100;
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
            Vertices.push_back(V[i]);
        }
        for_loop(i, 36)
        {
            Faces.push_back(Indexdata[i]);
        }



        TODO(Get the Channels of the UV coords in the future);
        get_Counts();


        // for_loop(i, Animesh_Count)
        // {  };
        for_loop(i, Bone_Count)
        {
          //  Bones.emplace_back(Mesh->mBones[i]);
        };
        for_loop(i, Face_Count)
        {
            for_loop(j, Mesh->mFaces->mNumIndices)
            {
              //  Faces.emplace_back(Mesh->mFaces[i].mIndices[j]);
            }
        };
        for_loop(i, Vertices_Count)
        {
            Vec3 V =
            {
                Mesh->mVertices[i].x,
                Mesh->mVertices[i].y,
                Mesh->mVertices[i].z
            };
          //  Vertices.emplace_back(V);
        };
        for_loop(i, UV_Count)
        {
            // UvCoords.emplace_back(Vec2((float)Mesh->mTextureCoords[i]->x, (float)Mesh->mTextureCoords[i]->y));
        };
        VAO = new VertexArrayObject();
        VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(Vertices));
        VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<uint32_t>(Faces));
    }


    void Render()
    {
        VAO->Bind();
        OpenGL::Renderer::drawIndices(VAO->Buffers[1]->GL_Handle, (uint32_t)Faces.size());
    }
    VertexArrayObject *VAO;

};
struct Model_Importer
{
    Shader *Program{ nullptr };
    FrameBufferObject *FBO;
    Camera3D *Cam;

    void Render()
    {
        FBO->Bind();
        {
            OpenGL::set_ClearColor(1, 0, 0, 1);
            FBO->Clear();
            Program->Bind();
            {
                Cam->Bind();
                for (auto& M : Models)
                {
                    M.Render();
                }
            }
            Program->Unbind();
        }
        FBO->Unbind();
    }


    Model_Importer(std::string _file)
    {
        Program = new Shader(V, F);
        FBO = new FrameBufferObject({ SCREEN_X, SCREEN_Y });
        Cam = new Camera3D(Vec2( SCREEN_X, SCREEN_Y ), Vec3( 0,0,-100 ), Vec3( 0 ));

        Program->Bind();
        {

            Assimp::Importer importer;
            Scene = importer.ReadFile
            (
                _file,
                aiProcess_CalcTangentSpace |
                aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_SortByPType
            );

            if (!Scene)
            {
                DEBUGPrint(CON_Red, "Error loading Model " << _file << "\n Assimp Error: " << importer.GetErrorString());
            }
            get_Counts();
            if (Scene->HasMeshes())
            {
                import_Meshes();
            }

            if (Scene->HasMaterials())
            {
                import_Materials();
            }

        }
        Program->Unbind();
    }

    void import_Meshes()
    {
        for_loop(i, Mesh_Count)
        {
            Models.push_back(Model(Scene->mMeshes[i]));
        }
    }
    void import_Materials()
    {}

    std::vector<Model> Models;


    void get_Counts() 
    {
        Animation_Count = Scene->mNumAnimations;
        Material_Count = Scene->mNumMaterials;
        Mesh_Count = Scene->mNumMeshes;
        Camera_Count = Scene->mNumCameras;
        Light_Count = Scene->mNumLights;
        Texture_Count = Scene->mNumTextures;
    }

    const aiScene* Scene;
    uint32_t Animation_Count{ 0 };
    uint32_t Material_Count{ 0 };
    uint32_t Mesh_Count{ 0 };
    uint32_t Camera_Count{ 0 };
    uint32_t Light_Count{ 0 };
    uint32_t Texture_Count{ 0 };




    std::string V =
        "#version 330 core                             \n\
                layout(location = 0) in vec3 Position;     \n\
                uniform mat4 ProjectionMatrix;             \n\
                uniform mat4 ViewMatrix;                   \n\
                void main()                                \n\
                {                                          \n\
                    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));  \n\
                    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);  \n\
                    gl_Position = ModelViewProjectionMatrix * vec4( Position.x, Position.y, Position.z, 1.0); \n\
                }";

    std::string F =
        "#version 330 core                    \n\
                out vec4 FragColor;               \n\
                void main()                       \n\
                {                                 \n\
                    FragColor = vec4(1,1,1,1);  \n\
                }";
};










//Scene->mMeshes[i]->mBones[9]->
//Scene->mMeshes[i]->mColors;
//Scene->mMeshes[i]->mFaces;
//Scene->mMeshes[i]->mMaterialIndex;
//Scene->mMeshes[i]->mMethod;
//Scene->mMeshes[i]->mName;
    //uint32_t Normals_Count{ 0 };

    //NumAnimMeshes;
    //NumBones;
    //NumFaces;
    //NumUVComponents;
    //NumVertices;
    //PrimitiveTypes;
    //TextureCoords;

//Scene->mMeshes[i]->mPrimitiveTypes;
//Scene->mMeshes[i]->mTangents;
//Scene->mMeshes[i]->mTextureCoords;
//Scene->mMeshes[i]->mVertices;


