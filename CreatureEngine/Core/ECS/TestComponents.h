#pragma once 

#include"ECS.h"
#include"../../Renderer/LowLevel/Materials/Image/Texture.h"
#include"../../Renderer/LowLevel/OpenGL/Renderer/Transform.h"
namespace ECStest {
    extern  EntityComponentSystem ECStest;

    void InitECS();
    COMPONENT(TransformComponent)
    {
    public:
        Transform Trans;
    };

    COMPONENT(MeshComponent)
    {
        MeshComponent();
 
        Vec3 *Data;
        size_t Size;
    };
    COMPONENT(SurfaceComponent)
    {
        SurfaceComponent() = default;
        SurfaceComponent(Graphics::Texture* _diffuse, Graphics::Texture* _normals, Graphics::Texture* _roughness, Graphics::Texture* _specular);

        Graphics::Texture* Diffuse;
        Graphics::Texture* Normals;
        Graphics::Texture* Roughness;
        Graphics::Texture* Specular;
    };

    SYSTEM(MovementSystem)
    {
        MovementSystem();
        void UpdateComponents(float delta, BaseComponent** components);
    };



    SYSTEM(RenderingSystem)
    {
        RenderingSystem();
        void UpdateComponents(float delta, BaseComponent** components);
    };




}//NS ECS