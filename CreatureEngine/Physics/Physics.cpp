#include"Physics.h"

_static Simulation *Simulation::Instance{ nullptr };

// Set the default for the Static variables
_static float Mass::Current_Timestep = 2.0f;
_static float Mass::Gravity = 0.1f;




#include<algorithm>
#include"Quadtree.h"


//#include"Sprite.h"
//#include"Camera.h"
//#include"Collider.h"
//#include"QuadTree.h"


size_t CollisionTestCounter = 0;


Simulation::Simulation(double _timestep)
{
    SpatialTree = new QuadTree({ 0,0 }, { 2000,2000 });
}


void Simulation::Update(float _timestep)
{// Update each of the Rigidbodies in our simulation with the given timestep
    for (auto& Body : Colliders)
    {// Raw Update of all Colliders, In perfect system we would perform some sort of Clipping or flag to say if it needs updating
        Body->g_Collider().Update(_timestep);
    }
    SpatialTree->Update();

    // Empty the Collisionpairs vector and prepare it for new Collisions
    WARN_ME("I believe Orphaning this Vector is the best option however pay attention to ensure we are not leaking Memory." << \
            " since we are only storing pointers to whole objects and not the objects themselves it should not be capable of leaking memory");
    std::vector<std::pair<Object_type *, Object_type*>> TempVec;
    CollisionPairs = TempVec; //CollisionPairs.clear(); // Surely a better way out there such as orphaning this buffer for a new one and/or double buffering

    for (auto &C : Colliders)
    {// Crude O(log(N)) Test for Collisions. Replace with Quad tree
        std::vector<Parent_t*> EMPTYVEC;
        C->g_Collider().Sweep(EMPTYVEC);// *&SpatialTree->RootNode->QueryRange(C->g_Position(), { 100,100 }));
        TODO("Physics.cpp Line 30: I can likely put the Child sweeps here with better results and easier readability");
        TODO("Instead of std::vector best if we had some reusable Memory model that can be Allocated and Deleted every frame with little overhead ")
    }
    Clean();

    DEBUG_CODE(CollisionTestCounter = 0;)
    DEBUG_CODE(SpatialTree->RootNode->Render());


    TODO("After the Gamejam reactivate this and fix the collision pairs issues");
    WARN_ME("Resolve is Currently Disabled in the Simulation Update Inorder to keep it so I can later Multithread it" <<\
            " I need to store all collision Pairs then when that is done Resolved can be called either here or in a different thread does not matter");

    ///Resolve();
}
void Simulation::Resolve()
{// Perform Specific Collision Response given to each collider. These can range from Physics Calculations, Triggers to Individual Game Pillers

    WARN_ME("All of the Simulation::Resolve() function is Commented out");
 //  for (auto& P : CollisionPairs)
//  {// Perform "physics" Calculations adjusting the Velocity of the Collider after Collision
//      P.first->g_Collider().g_CollisionResponse()(*P.first, *P.second);
//      P.second->g_Collider().g_CollisionResponse()(*P.second, *P.first);
//      TODO("Is this correct... Am I passing the proper object to the Collision Reponse and not a copy: I think I am going to Call the response twice however...");
//  }
}
void Simulation::Render()
{// Debug feature to Render the Colliders on the Screen
    for (auto& C : Colliders)
    {
        TODO("Need to incorperate a Debug render here which outlines the Colliders for the Object_type being simulated");
        //C->Render();
    }
}





void Simulation::Add(Object_type *_object)
{// Add an Object to our physics simulation
    Colliders.push_back(_object);
}
void Simulation::Remove(Object_type *_deadObject)
{// Removes an Object from the Simulation
    KillList.push_back(_deadObject);
    _deadObject->Kill(); // Immediately prevents Object from being Updated in the Simulation and Clean() will remove the Objects from the Simulation at a later date
}
void Simulation::Clean()
{
    REFACTOR("This to could greatly benefit from some Memory Model which allows Rapid Allocation and Deallocation")
        for (auto &K : KillList)
        {
            auto It = std::find(Colliders.begin(), Colliders.end(), K);
            if (It != Colliders.end())
            {
                Colliders.erase(It);
            }
        }
    KillList.clear();
}



void Simulation::AddtoQuadTree(Object_type *_object)
{
    SpatialTree->RootNode->Insert(_object);
    for (auto& C : _object->Children)
    {
        AddtoQuadTree(C);
    }
}
