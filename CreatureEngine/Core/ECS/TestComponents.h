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
        MeshComponent() noexcept;
 
        Vec3 *Data;
        size_t Size;
    };
    COMPONENT(SurfaceComponent)
    {
        SurfaceComponent() = default;
        SurfaceComponent(Graphics::Texture* _diffuse, Graphics::Texture* _normals, Graphics::Texture* _roughness, Graphics::Texture* _specular) noexcept;

        Graphics::Texture* Diffuse{ nullptr };
        Graphics::Texture* Normals{ nullptr };
        Graphics::Texture* Roughness{ nullptr };
        Graphics::Texture* Specular{ nullptr };
    };

    SYSTEM(MovementSystem)
    {
        MovementSystem() noexcept;
        virtual ~MovementSystem() = default;
        void UpdateComponents(float delta, BaseComponent** components);
    };



    SYSTEM(RenderingSystem)
    {
        RenderingSystem();
        virtual ~RenderingSystem() = default;
        void UpdateComponents(float delta, BaseComponent** components) override;
    };




}//NS ECS