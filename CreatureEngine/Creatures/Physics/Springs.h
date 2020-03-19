#pragma once

#include"Core/Common.h"
#include"Core/ECS/ECS.h"

#define _COUNT 100
//https://coronavirus-realtime.com/?fbclid=IwAR1zcaNjPZRfOGurpMG_0ySipVNBtDcuIQn_n4mjdMPeSz3-mEieYAVwSxg


namespace Creatures
{
    
    using Joint_id = uint32_t;

    extern  EntityComponentSystem CreatureECS;

    const float Friction{ 0.95f };
    // COMPONENT(Spring)
    // {
    //     Vec3 Start{ 0 };
    //     float RestLength{ 0 };
    //     float Tension{ 1.0f };
    // };

    float get_Timestep();
    float get_Distance(Vec3 _p1, Vec3 _p2);


    /*  ============================================================================================================================================ 
    /*                        EVERYTHING BETWEEN THESE IS PART OF THE SPRING CLASS- A DOD APPROCH TO OPTIMIZE PERFORMANCE
    /*  ============================================================================================================================================ */


    struct _Mass
    {
         float  X[ _COUNT ];
         float  Y[ _COUNT ];
         float  Z[ _COUNT ];
    };
    extern _Mass Mass;


    struct _Acceleration
    {
        float  X[ _COUNT ];
        float  Y[ _COUNT ];
        float  Z[ _COUNT ];
    };
    extern _Acceleration Acceleration;


    struct _Velocity
    {
        float  X[ _COUNT ];
        float  Y[ _COUNT ];
        float  Z[ _COUNT ];
    };
    extern _Velocity Velocity;


    struct _Force
    {
         float  X[ _COUNT ];
         float  Y[ _COUNT ];
         float  Z[ _COUNT ];
    };
    extern _Force Force;


    struct _Position
    {
        float X[ _COUNT ];
        float Y[ _COUNT ];
        float Z[ _COUNT ];
        void Update()
        {
            for_loop(i, _COUNT)
            {
                Velocity.X[i] *= Friction;
                auto Last_Acceleration = Acceleration.X[i];
                X[i] += Velocity.X[i] * get_Timestep() + (Last_Acceleration * 0.5f * Squared(get_Timestep()));
                Acceleration.X[i] = Force.X[i] / Mass.X[i];
                float  Avg_Acceleration = (Last_Acceleration + Acceleration.X[i]) * 0.5f;
                Velocity.X[i] += Avg_Acceleration * get_Timestep();

                Force.X[i] = 0.0f;
            }

            for_loop(i, _COUNT)
            {
                Velocity.Y[i] *= Friction;
                auto Last_Acceleration = Acceleration.Y[i];
                Y[i] += Velocity.Y[i] * get_Timestep() + (Last_Acceleration * 0.5f * Squared(get_Timestep()));
                Acceleration.Y[i] = Force.Y[i] / Mass.Y[i];
                float  Avg_Acceleration = (Last_Acceleration + Acceleration.Y[i]) * 0.5f;
                Velocity.Y[i] += Avg_Acceleration * get_Timestep();

                Force.Y[i] = 0.0f;
            }
            for_loop(i, _COUNT)
            {
                Velocity.Z[i] *= Friction;
                auto Last_Acceleration = Acceleration.Z[i];
                Z[i] += Velocity.Z[i] * get_Timestep() + (Last_Acceleration * 0.5f * Squared(get_Timestep()));
                Acceleration.Z[i] = Force.Z[i] / Mass.Z[i];
                float  Avg_Acceleration = (Last_Acceleration + Acceleration.Z[i]) * 0.5f;
                Velocity.Z[i] += Avg_Acceleration * get_Timestep();

                Force.Z[i] = 0.0f;
            }
        }
    };
    extern _Position Positions;

    extern float Radius[_COUNT];


    /*  ============================================================================================================================================ */
    /*                                      END OF SPRING DOD                                                                                        */
    /*  ============================================================================================================================================ */
    
    struct Spring
    {
        Spring() = default;
        Joint_id Start;
        Joint_id End;
        Vec3 Center{ 0.0f };
        float Damper{ 1.0f };
        float RestLength{ 0.0f };
        float Length{ 0.0f };
        float Tension{ 0.5f };



        explicit Spring(Vec3 _p1, Vec3 _p2, float _radiusA, float _radiusB, float _tension, float _damping) noexcept
            :
            Start(Joint_Count++),
            End(Joint_Count++),        
            Damper{ _damping },
            RestLength{ get_Distance( _p1, _p2) }, // I know, Fucking Dangerous.
            Length { RestLength },      // They are in the right order chill out
            Tension{ _tension }
        {
            Positions.X[Start] = _p1.x;
            Positions.Y[Start] = _p1.y;
            Positions.Z[Start] = _p1.z;

            Positions.X[End] = _p1.x;
            Positions.Y[End] = _p1.y;
            Positions.Z[End] = _p1.z;

            Radius[Start] = _radiusA;
            Radius[End] = _radiusB;
        }


        void Update(float _dt) noexcept
        {
            Positions.Update();
        }
        Vec3 get_Displacement(Vec3 _p1, Vec3 _p2)
        {
            return Vec3((_p1.x -_p2.x), (_p1.y - _p2.y), (_p1.z - _p2.z));
        }

        /*- Applies Hookes Law to the Masses connected to this Spring -*/
        void apply_Force(Vec3 _force)
        {// F = (-Tensions * Displacement) - (Damper - Velocity);

            Vec3 P1 = Vec3
            (
                Positions.X[Start],
                Positions.Y[Start],
                Positions.Z[Start]
            );

            Vec3 P2 = Vec3
            (
                Positions.X[End],
                Positions.Y[End],
                Positions.Z[End]
            );

            Vec3 Displacement = get_Displacement
            (
                P1, P2
            );
            
            Vec3 F1
            {
              (-Tension * Displacement.x) - (Damper * Velocity.X[Start]),
              (-Tension * Displacement.y) - (Damper * Velocity.Y[Start]),
              (-Tension * Displacement.z) - (Damper * Velocity.Z[Start])
            };

            Vec3 Displacement2 = get_Displacement
            (
                P2, P1
            );

            Vec3 F2
            {
              (-Tension * Displacement2.x) - (Damper * Velocity.X[End]),
              (-Tension * Displacement2.y) - (Damper * Velocity.Y[End]),
              (-Tension * Displacement2.z) - (Damper * Velocity.Z[End])
            };


            Force.X[Start] = F1.x;
            Force.Y[Start] = F1.y;
            Force.Z[Start] = F1.z;

            Force.X[End] = F2.x;
            Force.Y[End] = F2.y;
            Force.Z[End] = F2.z;
        }


        static Joint_id Joint_Count;
    };
    extern Spring Springs[(size_t)(_COUNT / 2)]; 


    void create_Springs(uint32_t _count);

    bool TEST_SPRINGS();
} // NS Creatures




/*  ============================================================================================================================================ */
/*                                                   TRASH                                                                                       */
/*  ============================================================================================================================================ 

  var springForceY = -k * (positionY - anchorY);
  var dampingForceY = damping * velocityY;
  var forceY = springForceY + mass * gravity - dampingForceY;
  var accelerationY = forceY / mass;
  velocityY = velocityY + accelerationY * timeStep;
  positionY = positionY + velocityY * timeStep;
    void Apply_Forces(Spring& _spring, Vec3 _forceA, Vec3 _forceB)
    {
       // _spring.
       // Body.Velocity = Body.Velocity * .95f;
       //
       // Vec3  Last_Acceleration = Body.Acceleration;
       // Body.Position += Body.Velocity *  Mass::Get_Timestep() + (Last_Acceleration * 0.5f * Squared(Mass::Get_Timestep()));
       // Body.Acceleration = Body.Force / Body.Kg;
       // Body.Force = 0.0f;
       // Vec3  Avg_Acceleration = (Last_Acceleration + Body.Acceleration) / 2;
       // Body.Velocity += Avg_Acceleration * Mass::Get_Timestep();
    }
    void  CollisionSphere::Update()
    {
        Body.Velocity = Body.Velocity * .95f;
    
        Vec3  Last_Acceleration = Body.Acceleration;
        Body.Position += Body.Velocity *  Mass::Get_Timestep() + (Last_Acceleration * 0.5f * Squared(Mass::Get_Timestep()));
        Body.Acceleration = Body.Force / Body.Kg;
        Body.Force = 0.0f;
        Vec3  Avg_Acceleration = (Last_Acceleration + Body.Acceleration) / 2;
        Body.Velocity += Avg_Acceleration * Mass::Get_Timestep();
    
        CollisionDetection();
    }
    void Default_Update(Collider *_object, float _deltaTime)
    {
      _object->Velocity *= .95;//#PERF: Warm Cache, create Reference object for _object and operate on that
      _object->Position += _object->Velocity * _deltaTime;
      TODO("I could now move the Position and Velocity back to the GameObject class if I want to. It is HACKED for now");
      Forward Euler Vt + dt = Vt + m-1 FORCE t dt
      Implicit Euler Vt + dt = Vt + m -1 FORCE t + dtDT
      deltaTime = 1.0 / _deltaTime;
      ollider &O = *_object;
      .Velocity = O.Velocity * .95f; // Perform adhoc Friction to slow object down
      
      ec2 Last_Acceleration = O.Acceleration;
      .Position += O.Velocity * _deltaTime + (Last_Acceleration * 0.5f * Squared(_deltaTime));
      .Acceleration = O.Force / O.Weight;
      
      .Force = Vec2(0.0f);
      
      ec2 Avg_Acceleration = (Last_Acceleration + O.Acceleration) * 0.5f;
      .Velocity += Avg_Acceleration * _deltaTime;
    }
       Body.Velocity = Body.Velocity * .95f;
       Vec3  Last_Acceleration = Body.Acceleration;
       Body.Position += Body.Velocity *  Mass::Get_Timestep() + (Last_Acceleration * 0.5f * Squared(Mass::Get_Timestep()));
       Body.Acceleration = Body.Force / Body.Kg;
       Body.Force = 0.0f;
       Vec3  Avg_Acceleration = (Last_Acceleration + Body.Acceleration) / 2;
       Body.Velocity += Avg_Acceleration * Mass::Get_Timestep();
    struct Joint
    {
        Joint(Vec3 _position, float _size)
            :
            Position(_position),
            Radius(_size)
        {}
    
        
        Vec3 Position{ 0 };
        Vec3 Degrees_of_Movement{ RADIANS(360.0) };
        Vec3 operator -(Joint _other)
        {
            return Position - _other.Position;
        }
    
        void s_Radius(float _size)
        {
            Radius = _size;
        }
    
        float Radius{ 1.0f };
    };
    void calculate_Length() noexcept
    {
       // Vec3 result = Squared(End - Start);
       // Length = std::sqrt(result.x + result.y + result.z );
    }

*/