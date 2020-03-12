#pragma once


#include<vector>
#include<utility>

#include"Core/Common.h"
#include"../Renderer/LowLevel/OpenGL/Renderer/GameObject.h"

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