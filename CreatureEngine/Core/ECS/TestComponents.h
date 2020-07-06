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
        SurfaceComponent(Texture* _diffuse, Texture* _normals, Texture* _roughness, Texture* _specular) noexcept;

        Texture* Diffuse{ nullptr };
        Texture* Normals{ nullptr };
        Texture* Roughness{ nullptr };
        Texture* Specular{ nullptr };
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











/*        REMOVED FROM MAIN.CPP and NOT YET FULLY TESTED



EntityComponentSystem *TestECS;
SystemList MainSystems;

COMPONENT(MovementComponent)
{
    Vec3 Velocity;
    Vec3 Acceleration;
};
COMPONENT(PositionComponent)
{
    Vec3 Position;
};
struct MovementSystem
    :
    public BaseSystem
{
    MovementSystem()
        :
        BaseSystem()
    {
        AddComponentType(PositionComponent::ID);
        AddComponentType(MovementComponent::ID);

        DEBUG_CODE(SystemName = (typeid(this).name()));
    }

    virtual void UpdateComponents(float _delta, BaseComponent** _components)
    {
        PositionComponent* Pos      = (PositionComponent*)_components[0];
        MovementComponent* Movement = (MovementComponent*)_components[1];

        Movement->Velocity += Movement->Acceleration;
        Pos->Position += Movement->Velocity;
        Movement->Velocity *= .9;
    };
};

MovementSystem    movementSys;
PositionComponent PosComponent;
MovementComponent TestMovementComponent;



/// App class
    EntityComponentSystem *ECS{ nullptr };

        ///Entity Component System
        {
            //    ECS = new EntityComponentSystem();
            //    SystemList mainSystems;
            //    ECStest::TransformComponent transformComp;
            //    ECS->AddComponent(transformComp);
            //    ECStest::InitECS();
            //    TestECS = new EntityComponentSystem();
            /// Create Entities
            //    EntityPTR Entity = TestECS->MakeEntity(PosComponent, TestMovementComponent);
            /// Create Systems
            //    MainSystems.AddSystem(movementSys);
            //    TestECS->AddComponent(Entity, &PosComponent);
            //    TestECS->AddComponent(Entity, &TestMovementComponent);
        }



                delete(ECS);


*/