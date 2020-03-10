



#include"Core/Common.h"












namespace Physics
{
    void verlet(Vec3& pos, Vec3 &velocity, const Vec3 &acceleration, float delta)
    {
        float halfDelta = delta * 0.5f;
        pos += velocity * halfDelta;
        velocity += acceleration * delta;
        pos += velocity * halfDelta;
    }

    void forestRuth(Vec3& pos, Vec3& velocity, const Vec3 &acceleration, float delta)
    {
        static const float frCoefficient = 1.0f / (2.0f - std::pow(2.0f, 1.0f / 3.0f));
        static const float frComplement = 1.0f - 2.0f*frCoefficient;
        verlet(pos, velocity, acceleration, delta*frCoefficient);
        verlet(pos, velocity, acceleration, delta*frComplement);
        verlet(pos, velocity, acceleration, delta*frCoefficient);
    }

    void modifiedEuler(Vec3& pos, Vec3& velocity, const Vec3& acceleration, float delta)
    {
        velocity += acceleration * delta;
        pos += velocity * delta;
    }
}