



#include"Core/Common.h"




#include"Springs.h"





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


namespace Creatures
{
    _static Joint_id Spring::Joint_Count{ 0 };
    _Position Positions;
    _Mass Mass;
    _Acceleration Acceleration;
    _Velocity Velocity;
    _Force Force;
    float Radius[_COUNT];

    Spring Springs[(size_t)(_COUNT / 2)];


    void create_Springs(uint32_t _count)
    {
        _count = (size_t)(_COUNT / 2);// Just Temp until I get the System Working
        for_loop(i, _count)
        {
            Springs[i] = Spring(Vec3(0), Vec3(0), 1.0, 1.0f, 0.5f, 0.5f);
        }
    }

    float get_Timestep()
    {
        return .1f;
    }
    float get_Distance(Vec3 _p1, Vec3 _p2)
    {
        Vec3 result = Squared(_p2 - _p1);
        return std::sqrt(result.x + result.y + result.z);
    }



    bool TEST_SPRINGS()
    {
        
        create_Springs(_COUNT);
        TEST_ASSERT(Springs[0].Length _EQUALS_ 0, "Incorrect Initialization of Springs","Correct Initialization of Springs");


        return true;
    }

}