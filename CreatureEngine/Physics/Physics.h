#pragma once


#include<vector>
#include<utility>

#include"Core/Common.h"
#include"../Renderer/LowLevel/OpenGL/Renderer/GameObject.h"

#define M_PI   3.141592653589
//Uses


class  QuadTree;

/* Requirements for Object_Type is that it needs functions
g_PositionX'Y'Z'(); 
std::vector<Object_Type> Children ;
g_Collider() Inorder to find the Position of the Object
*/
using Object_type = GameObject;

class Simulation
{

public:
    NO_COPY_OR_ASSIGNMENT(Simulation);

    Simulation(double _timestep);

    void Add(Object_type *_collider);
    void Remove(Object_type *_collider);
    void Clean();


    void Update(float _timestep);
    void Render();
    void Resolve();


    std::vector<Object_type *> Colliders;
    std::vector<Object_type *> KillList;


    inline static Simulation& get() { return *Instance; }
    inline static void set(Simulation *_simulation)
    {
        Instance = _simulation;
    }
    inline static bool Initialize(double _timestep)
    {
        Static_Simulation_Mtx.lock();
        {/* ~ CRITICAL SECTION ~ Must Ensure only single object is Initialized at once */
            if (Instance == nullptr)
            {
                Instance = new Simulation(_timestep);
                return true;
            }
        }
        Static_Simulation_Mtx.lock();
        return false;
    }

    void AddtoQuadTree(Object_type *_object);


    std::mutex Simulation_Mtx;
    static std::mutex Static_Simulation_Mtx;

    QuadTree *SpatialTree;

private:
    std::vector<std::pair<Object_type *, Object_type *>> CollisionPairs;
    static Simulation *Instance;
};






class Mass
{
public:
    Mass() {}
    Mass(float weight, Vec3 pos)
        :
        Force(0, 0, 0),
        Acceleration(0, 0, 0),
        Position ( pos),
        Velocity(0, 0, 0),
        Kg(weight)
    {}

    Mass(float weight, float x, float y, float z)
        : Mass(weight, Vec3(x, y, z))
    {}

    ~Mass() {}


    Vec3
        Velocity,
        Position,
        Acceleration,
        Angular,
        Force;

    float  Kg;

    void Apply_Gravity();
    void Apply_Physics();
    void s_Force(float x, float y);
    void s_Position(float x, float y);

    Vec3 g_Position();
    inline float g_PositionX() { return Position.x; }
    inline float g_PositionY() { return Position.y; }
    inline float g_PositionZ() { return Position.z; }

    struct convert
    {
        inline float Pounds_to_Kg(float lb) { return (float)(lb * .453); }
        inline float Newtons_to_Kg(float N) { return (float)(N / 9.8); }
        inline float Kg_to_Newtons(float Kg) { return (float)(Kg * 9.8); }
    }Convert;

    struct volume
    {
        inline float ofBox(Vec3 min, Vec3 max) { return (float)((max.x - min.x) * (max.y - min.y) * (max.z - min.z)); }
        inline float ofSphere(float radius)    { return (float)((4 / 3) * (M_PI * (radius * radius * radius))); }
        //inline float ofBox(AABB &box){return (box.MaxPoint.x - box.MinPoint.x) * (box.MaxPoint.y - box.MinPoint.y) * (box.MaxPoint.z - box.MinPoint.x);}
    }Volume;


    static void  s_Timestep(float time) { Current_Timestep = time; }
    static void  s_Gravity(float g) { Gravity = g; }

    static float g_Timestep() { return Current_Timestep; }
    static float g_Gravity() { return Gravity; }
private:

    static float Current_Timestep;
    static float Gravity;
};