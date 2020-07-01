#include"TestComponents.h"


namespace ECStest
{
    EntityComponentSystem ECStest;
    SurfaceComponent Surf;
    TransformComponent Trans;
    MeshComponent Mes;

    void InitECS()
    {
        EntityPTR RenderingObject = ECStest.MakeEntity(Mes, Surf, Trans);// BaseComponent **_comp, const Component_Handle *_componentIDs, size_t _size);
    }

    SurfaceComponent::SurfaceComponent
    (Texture* _diffuse, Texture* _normals, Texture* _roughness, Texture* _specular) noexcept
        :
        Diffuse(_diffuse),
        Normals(_normals),
        Roughness(_roughness),
        Specular(_specular)
    {}
 
    MeshComponent::MeshComponent() noexcept
        :
        Component()
    {
        Size = 10;
        Data = new Vec3[Size];
    }

    MovementSystem::MovementSystem() noexcept
    {
        AddComponentType(TransformComponent::ID);
        AddComponentType(MeshComponent::ID);
    }
    void MovementSystem::UpdateComponents(float delta, BaseComponent** components)
    {
        Print("Updating Transforms");
    }


    RenderingSystem::RenderingSystem()
        : BaseSystem()
    {
        AddComponentType(TransformComponent::ID);
        AddComponentType(MeshComponent::ID);

    }
    void RenderingSystem::UpdateComponents(float delta, BaseComponent** components)  
    {
        Print("Updating Rendering System");
    }

}