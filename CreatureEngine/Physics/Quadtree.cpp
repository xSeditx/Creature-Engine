#include"QuadTree.h"
#include"Physics.h"


#include"../Renderer/LowLevel/OpenGL/Camera/Camera.h"

_static QuadTree *QuadTree::QT;

Node::Node(Vec2 pos, Vec2 size)
    :
    Position(pos),
    Size(size),
    Capacity(MAX_CAPACITY),
    IsLeaf(true),
    Facing(NE)
{
    SubNodes[NW] = nullptr; // Maybe a memset for the size of SubNodes could help
    SubNodes[NE] = nullptr; // zero this a bit faster
    SubNodes[SW] = nullptr;
    SubNodes[SE] = nullptr;
    Entities.reserve(MAX_CAPACITY);
}


void Node::Subdivide()
{
    for_loop(Index, 4)
    {
        SubNodes[Index] =
            new  Node
            (
                NewPos
                (
                    Position,
                    (NodeTag)Index
                ),
                Vec2
                (
                    Size.x * 0.5f,
                    Size.y * 0.5f
                )
            );
        SubNodes[Index]->Facing = (NodeTag)Index;
    }

}

bool  Node::Insert(Object_type *object)
{
    if (Size.x < 1.0)
    {
        Entities.push_back(object);
        return true;
    }
    //================================================================================
    //   The Following two checks may not be needed in the Future
    //================================================================================
    REFACTOR(" Check to see if these following Test are still Applicable ");
    for (auto& C : object->Children)
    {// Inserts all of the Parent Objects Children into this Node as well
    	Insert(C);
    }
    if (!object->g_Position().x && !object->g_Position().y )
    {// Prevents objects at {0,0} from Infinitely attempting to subdivide the Tree
    	return false;
    }
    //================================================================================


    if (IsContained(object))
    {// If the Object Fits in this Square
        if (IsLeaf _IS_EQUAL_TO_ true)
        {// And its a Leaf Node
            if (Entities.size() < Capacity)
            {// And the Capacity of the Square is not full 
                Entities.push_back(object);
                return true;
            }  // Than add this Object to the Current Object list and exit the function
            else
            { // Else If its full than make the Node a Branch
                IsLeaf = false;
                Subdivide();    // And Subdivide it

                for (Object_type *object : Entities)
                {// Put the Particles that were in this Node in their respective SubNodes
                    PushDown(object);
                }

                Entities.clear();    // And Delete the particles from this Now branch node
                if (SubNodes[NE]->Insert(object) _IS_EQUAL_TO_ true) return true; // Attempt to place the Particle in one of the newly
                if (SubNodes[SE]->Insert(object) _IS_EQUAL_TO_ true) return true; // Created Subnodes
                if (SubNodes[NW]->Insert(object) _IS_EQUAL_TO_ true) return true;
                if (SubNodes[SW]->Insert(object) _IS_EQUAL_TO_ true) return true;

            }
        }
        else
        {// If this node is a Branch and not a leaf than try the Subnodes and see if any of them are leaves
            if (SubNodes[NE]->Insert(object) _IS_EQUAL_TO_ true) return true;
            if (SubNodes[SE]->Insert(object) _IS_EQUAL_TO_ true) return true;
            if (SubNodes[NW]->Insert(object) _IS_EQUAL_TO_ true) return true;
            if (SubNodes[SW]->Insert(object) _IS_EQUAL_TO_ true) return true;
        }
    }
    else
    { // Finally if the Particle is not in this quadrent at all just return false    
        return false;
    }  // This is done for completeness as it would fall through to a false return anyway by default
    return false;
}


/* Currently the QT only test points and considers Objects to be Dimensionless. 
/* At the very least this is something that should be kept in mind and possibly
/* Reconsidered at a later date */
bool Node::IsContained(Object_type *object)
{
    REFACTOR("Possibly alter this test to include other dimensions instead of assuming object is a single point ");
    return
        (
            object->g_PositionX() >= (Position.x - Size.x) _AND_
            object->g_PositionX() <= (Position.x + Size.x) _AND_
            object->g_PositionY() >= (Position.y - Size.y) _AND_
            object->g_PositionY() <= (Position.y + Size.y)
            );
}


bool  Node::PushDown(Object_type *_object)
{
    if (SubNodes[NE]->Insert(_object) _IS_EQUAL_TO_ true) return true;
    if (SubNodes[SE]->Insert(_object) _IS_EQUAL_TO_ true) return true;
    if (SubNodes[NW]->Insert(_object) _IS_EQUAL_TO_ true) return true;
    if (SubNodes[SW]->Insert(_object) _IS_EQUAL_TO_ true) return true;
    return false;
}


void  Node::Moveup(Object_type *_object)
{
    // Needs work and possibly is not needed at all
}


void  Node::Prune(Node *_node)
{
    if (_node _NOT_EQUAL_TO_ nullptr)
    {
        for_loop(i, 4)
        {
            if (_node->IsLeaf _IS_EQUAL_TO_ false)
            {
                Prune(_node->SubNodes[i]);
            }
            delete _node->SubNodes[i];
            *_node->SubNodes = nullptr; // Not sure this is needed or even helpful for that matter
        }
        _node->IsLeaf = true;
        _node->Entities.clear();
    }
}



void  Node::Render()
{
    float X1 = (Position.x - (Size.x));
    float Y1 = (Position.y - (Size.y));
    float X2 = ((Size.x) * 2); 
    float Y2 = ((Size.y) * 2); 

    Vec3 Color;
    if (Facing _IS_EQUAL_TO_ NE) { Color.r = 255; Color.g = 0;  Color.b = 0; }
    if (Facing _IS_EQUAL_TO_ NW) { Color.r = 0;   Color.g = 255; Color.b = 0; }
    if (Facing _IS_EQUAL_TO_ SW) { Color.r = 0;   Color.g = 0;  Color.b = 255; }
    if (Facing _IS_EQUAL_TO_ SE) { Color.r = 255; Color.g = 255; Color.b = 255; }
    TODO("Call the Collider Render HERE");

    Vec2 Offset = Viewport::get().get_Center();

    REFACTOR(" Include a Debug Renderer here That can draw Wire Frames of our Nodes Boarders ");
   // SDL_Rect R = {
   //     X1 + Offset.x,
   //     Y1 + Offset.y,
   //     X2,
   //     Y2
   // };
   // SDL_SetRenderDrawColor(Renderer::get().g_Context(), Color.r, Color.g, Color.b, 80); //0, 255, 0, 100);
   //
   // SDL_RenderDrawRect(Renderer::get().g_Context(), &R);
   // SDL_RenderDrawLine(Renderer::get().g_Context(), Position.x + Offset.x, R.y, Position.x + Offset.x, R.y + (Size.y * 2));
   // SDL_RenderDrawLine(Renderer::get().g_Context(), R.x, Position.y + Offset.y, R.x + (Size.x * 2), Position.y + Offset.y);
   //
   // SDL_SetRenderDrawColor(Renderer::get().g_Context(), DEFAULT_DRAW_COLOR);

    if (is_Not_a_Leaf_Node())
    {
        for_loop(Index, 4)
        {
            SubNodes[Index]->Render();
        }
    }
}
Vec2  Node::NewPos(Vec2 pos, NodeTag direction) {
    Vec2 NewPos = pos;
    if (direction _IS_EQUAL_TO_ NE)
    {
        NewPos.x += (Size.x * .5f);
        NewPos.y -= (Size.y * .5f);
    }
    if (direction _IS_EQUAL_TO_ SE)
    {
        NewPos.x += (Size.x * .5f);
        NewPos.y += (Size.y * .5f);
    }
    if (direction _IS_EQUAL_TO_ NW)
    {
        NewPos.x -= (Size.x * .5f);
        NewPos.y -= (Size.y * .5f);
    }
    if (direction _IS_EQUAL_TO_ SW)
    {
        NewPos.x -= (Size.x * .5f);
        NewPos.y += (Size.y * .5f);
    }
    return NewPos;
}
bool  Node::Intersects(Vec2 position, Vec2 size)
{// Position is Center of Quad
    return
        _NOT_(
            position.x - size.x > Position.x + Size.x _OR_
            position.x + size.x < Position.x - Size.x _OR_
            position.y - size.y > Position.y + Size.y _OR_
            position.y + size.y < Position.y - Size.y);

}
std::vector<Object_type*> Node::QueryRange(Vec2 position, Vec2 size)
{

    std::vector<Object_type *> results;

    if (_NOT_(Intersects(position, size)) _IS_EQUAL_TO_ true)
    {
        return results;
    }
    for_loop(Index, Entities.size())
    {
        if (IsContained(Entities[Index]))
        {
            results.push_back(Entities[Index]);
        }
    }

    if (IsLeaf _IS_EQUAL_TO_ true)
    {
        return results;
    }

    for_loop(SubIndex, 4)
    {
        std::vector<Object_type*> S;
        S = SubNodes[SubIndex]->QueryRange(position, size);
        if (S.size() > 0)
        {
            results.insert(results.end(), S.begin(), S.end());
        }
    }

#ifdef _RENDER_QUADTREE_
///    SDL_SetRenderDrawColor(Renderer::get().g_Context(), 0, 255, 0, 100);
///    Vec2 Offset = Viewport::get().get_Center();
///    SDL_Rect dest =
///    {
///        position.x - (size.x *.5) + Offset.x,
///        position.y - (size.y *.5) + Offset.y,
///         size.x,
///         size.y
///    };
///    SDL_RenderDrawRect(Renderer::get().g_Context(), &dest);
///    SDL_SetRenderDrawColor(Renderer::get().g_Context(), DEFAULT_DRAW_COLOR);
#endif

    return results;
}


QuadTree::QuadTree(Vec2 _position, Vec2 _size)
    :
    Position(_position),
    Size(_size)
{
    RootNode = new Node(_position, _size);
    QT = this;
}
void QuadTree::Init()
{
    RootNode = new Node(Position, Size);
    for_loop(Index, Simulation::get().Colliders.size())
    {
        RootNode->Insert(Simulation::get().Colliders[Index]);
    }
}

DEBUG_CODE(size_t InsertionWatch = 0;)
void QuadTree::Update()
{
    DEBUG_CODE(InsertionWatch = 0;)

    RootNode->Prune(RootNode);

    for_loop(Index, Simulation::get().Colliders.size())
    {
        ChildAdd(Simulation::get().Colliders[Index]);
    }
}


void QuadTree::ChildAdd(Object_type *_object)
{
    if (_object->is_Alive())
    {
        RootNode->Insert(_object);

        DEBUG_CODE(InsertionWatch++;)
        for (auto &C : _object->Children)
        {
            ChildAdd(C);
        }
    }
}