#pragma once

#include"Core/Common.h"
#include"Core/ECS/ECS.h"

namespace Creatures
{
  extern  EntityComponentSystem CreatureECS;


// COMPONENT(Spring)
// {
//     Vec3 Start{ 0 };
//     float RestLength{ 0 };
//     float Tension{ 1.0f };
// };



    struct Joint
    {
        Joint(Vec3 _position, float _size)
            :
            Position(_position),
            Radius(_size)
        {}
        Vec3 Position{ 0 };

        Vec3 operator -(Joint _other)
        {
            return Position - _other.Position;
        }
        float Radius{ 1.0f };
    };

    struct Spring
    {
        Joint Start;
        Joint End;

        void calculate_Length()
        {
            Vec3 result= Squared(End - Start);
            Length = std::sqrt(result.x + result.y + result.z );
        }

        void Update(float _dt)
        {
        }

        float Length{ 0.0 };
        float RestLength{ 0.0f };
        float Tension{ 0.5f };
    };



}// NS Creatures