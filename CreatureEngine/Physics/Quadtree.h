#pragma once

#include"Core/Common.h"
#include"../Renderer/LowLevel/OpenGL/Renderer/GameObject.h"

#include<vector>

#define MAX_CAPACITY 1
#define _RENDER_QUADTREE_



// TODO Need to make the object inside of Quad tree something that can derive from the Object you are attempting to insert 
// as it will make implementation as simple as changing a single word in the file

enum NodeTag
{
    NE = 0,
    SE = 1,
    NW = 2,
    SW = 3,
};

/* Can we Potentially just make this a Void pointer with a Templated caster get<_Ty>() in our class */
using Object_type = GameObject;

class Node
{
public:
    Node() = default;

    Node(Vec2 Position, Vec2 size);

    Node *SubNodes[4]; // 32( 8 * 4)
    Vec2 Position;     // 8
    Vec2 Size;         // 8
    int Capacity;      // 4
    NodeTag Facing;    // 4
    bool IsLeaf;       // 1

    std::vector<Object_type *> Entities;

    /* Give us the Ability to Range For the Subnodes*/
    auto begin() { return SubNodes[0]; }
    auto end()   { return SubNodes[3]; }


    /* Break the current Node into 4 Children Nodes */
    void Subdivide();

    /* Renders a Debug Map of the Current Quadtree */
    void Render();

    /* Finds the center for the new Node if Created in _direction*/
    Vec2 NewPos(Vec2 _position, NodeTag _direction);

    /* Moves the Object DOWN to it's appropriate Child Node */
    bool PushDown(Object_type *Object);

    /* Moves the Object UP the Tree into its Parent Node */
    void Moveup(Object_type *Object);

    /* Adds an Object to the Tree */
    bool Insert(Object_type *object);

    /* Checks to see if the Object is contained inside this Node */
    bool IsContained(Object_type *object);

    /* Cuts off this Node and all of its Children Reinserting them back into the Quadtree at 
    /* the appropriate Location */
    void Prune(Node *node);

    /* Returns true if the Rect Described intersects this Node 
    /* NOTE: Nested Rects will return True */
    bool Intersects(Vec2 position, Vec2 size);

    /* Returns a List of All Objects contained inside the Rect with its center at Position */
    std::vector<Object_type*> QueryRange(Vec2 position, Vec2 size);

    /* Syntactic Sugar that Returns True if Node is not a Leaf Node */
    bool is_Not_a_Leaf_Node() { return !IsLeaf; }
};

class QuadTree
{
public:
    QuadTree() = default;
    QuadTree(Vec2 _position, Vec2 _size);

    Vec2 Position;
    Vec2 Size;
    Node *RootNode;

    /* Creates the Root Node and Adds all the Collidable objects to the Tree */
    void Init();

    /* Rebuilds the tree from scratch every Update() */
    void Update();

    /* Adds an Object to the Root Node which places it in the appropriate location */
    void ChildAdd(Object_type *_object);

    /* Main QuadTree for the Entire Engine although others can Exist */
    static QuadTree *QT;
};