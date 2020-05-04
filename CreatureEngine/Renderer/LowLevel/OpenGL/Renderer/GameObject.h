#pragma once

class Shader;
#include"../../../../Physics/Colliders.h"

// struct Collider
// {
//     void Update(float _dt){}
//     void Sweep(std::vector<Collider*> _potentialPairs) {}// *&SpatialTree->RootNode->QueryRange(C->g_Position(), { 100,100 }));
// };


class GameObject
{
public:
	GameObject()
		:
		Scale(0),
		Position(0),
		Rotation(0),
		Transform(Mat4(1.0f)),
		Handle(ObjectCount++)
	{
        WARN_ME("Pure Virtual Nature of this Class has been Removed. All derived classes must use Override");
		Transform = glm::translate(Transform, Position);
		Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	}

	GameObject(Vec3 _pos, Vec3 _rot, Vec3 _scale)
		:
		Position(_pos),
		Rotation(_rot),
		Scale(_scale),
		Handle(ObjectCount++)
	{
		Transform = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
		Transform = glm::translate(Transform, _pos);
		Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	}

    virtual void Update()= pure_virtual; // { DEBUGPrint(CON_Red,"Pure Virtual Removed from GameObject Class, Update your Derived class to include Override Statement"); }           //
    virtual void Unbind() = pure_virtual; // { DEBUGPrint(CON_Red,"Pure Virtual Removed from GameObject Class, Update your Derived class to include Override Statement"); }           //
    virtual void Bind() = pure_virtual; // { DEBUGPrint(CON_Red,"Pure Virtual Removed from GameObject Class, Update your Derived class to include Override Statement"); }           //
    virtual void Render() = pure_virtual; // { DEBUGPrint(CON_Red,"Pure Virtual Removed from GameObject Class, Update your Derived class to include Override Statement"); }           //


	inline void s_Scale(Vec3 _scale) { Scale = _scale; }
	inline void s_Scale(float _scale) { Scale = Vec3(_scale); }
	inline void s_Position(Vec3 _position) { Position = _position; }
	inline void s_Rotation(Vec3 _rotation) { Rotation = _rotation; }
	inline void s_Transform(Mat4 _transform) { Transform = _transform; }
 
    inline Vec3 g_Scale()      { return Scale;     }
    inline Vec3 g_Position()   { return Position;  }
    inline Vec3 g_Rotation()   { return Rotation;  }
    inline Mat4 g_Transform()  { return Transform; }

    inline float g_PositionX() { return Position.x; }
    inline float g_PositionY() { return Position.y; }
    inline float g_PositionZ() { return Position.z; }

    inline float g_RotationX() { return Rotation.x; }
    inline float g_RotationY() { return Rotation.y; }
    inline float g_RotationZ() { return Rotation.z; }


    inline Collider& g_Collider()
    {
        WARN_ME("g_Collider() only returns a Temp object for now be sure to correct this in the future");  
        return *(new SphereCollider(this));                //        *(new Collider());
    }

  




	void UpdateTransform()
	{
		Transform = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
		Transform = glm::translate(Transform, Position);
		Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	}


    /* Gets the Handle of the Object that the Engine uses */
    const uint32_t g_Handle() const
    {
        return Handle;
    }

	/* Gets the OpenGL VAO handle of the Mesh */
	const uint32_t g_GLHandle() const
	{
		return GL_Handle;
	}

    /* Adds a Child object to this Objects Children List */
    void add_Child(GameObject *_child)
    {
        Children.emplace_back(_child);
    }

    /* Erases a Child object from the List of Children */
    void remove_Child(GameObject* _child)
    {
        REFACTOR("This stupid thing keeps asking me for a comparison == operator that takes [ vector<GameObject*>::iterator == const GameObject ] which can not exist because GameObject is pure_virtual");
        Children.erase(std::find(Children.begin(), Children.end(), _child));
    }

    /* Removes a Child object from its list and returns that GameObject */
    //GameObject& pop_Child(GameObject& _child)
    //{ 
    //   //WARN_ME("I have concerns that I am deleting the object before dereferencing it and attempting to return it \n Since it is a Virtual Object I can not instantiate a physical copy of GameObject ");
    //   //auto Obj = std::find(Children.begin(), Children.end(), _child);
    //   //GameObject *results{ *Obj };
    //   //Children.erase(Obj);
    //    return *results;
    //}
    //
    /* Test to see if the Object is alive and should be Updated */
    bool is_Alive() { return Active; }

    /* Kills an Object to prevent it from being Updated, Rendered or Collided with
    /* NOTE: Does not delete object and can be Resurrected at a later time */
    void Kill() { Active = false; }

    /* Reactivates an Object that was previously disabled */
    bool Resurrect() { if (Active _EQUALS_ true) return false; Active = true; return Active; }


    /* Checks if this was created After _other */
     bool operator >(const GameObject& _other)
     {
         return Handle > _other.Handle;
     }
 
     /* Checks if this was created before _other */
     bool operator <(const GameObject& _other)
     {
         return Handle < _other.Handle;
     }
 
     /* Compares if two objects refer to a different object. */
     bool operator !=(const GameObject& _other)
     {
         return Handle != _other.Handle;
     }
 
     /* Compares two object references to see if they refer to the same object.*/
     bool operator ==(const GameObject& _other)
     {
         return Handle == _other.Handle;
     }

     bool operator ==(GameObject** _other)
     {
         auto Test = *_other;  // GameObject *Test;
         auto Test2 = &_other; // GameObject ***Test
         //auto Test3 = **_other;// GameObject Test
         auto Test4 = _other;  // GameObject **Test
         return this->Handle == (*_other)->Handle;
     }

    //template<typename _Ty>
    //bool operator ==(const _Ty& _other)
    //{
    //    return this->Handle == _other;
    //}



    Mat4 Transform{ 1.0f };
    Vec3 Position{ 0.0f }, Rotation{ 0 }, Scale{ 1.0f };

    std::vector<GameObject*> Children;

    static int ObjectCount;

    friend std::ostream& operator <<(std::ostream& _str, GameObject& _object);

private:
	uint32_t GL_Handle{ 0 };
	uint32_t Handle{ 0 };
    bool Active{ true };
};

std::ostream& operator <<(std::ostream& _stream, GameObject& _object);



static std::ostream& operator <<(std::ostream& _stream, GameObject& _object)
{

    _stream << " Transform: " << _object.Transform  << "\n";
    _stream << " Position: " << _object.Position    << "\n";
    _stream << " Rotation: " << _object.Rotation    << "\n";
    _stream << " Scale: " << _object.Scale          << "\n";
    _stream << " Active: " << PBool(_object.Active) << "\n";
    _stream << " VAO Handle: " << _object.GL_Handle << "\n";
    _stream << " Object ID: " << _object.Handle     << "\n";
    _stream << " Number of Children: " << _object.Children.size() << "\n";

    return _stream;
}

/* Unity Mono behavior 

Start()
Update()
FixedUpdate()
LateUpdate()
OnGUI()
OnDisable()
OnEnable()


 Public Methods
----------------
BroadcastMessage        Calls the method named methodName on every MonoBehaviour in this game object or any of its children.
CompareTag              Is this game object tagged with tag ?
GetComponent            Returns the component of Type type if the game object has one attached, null if it doesn't.
GetComponentInChildren  Returns the component of Type type in the GameObject or any of its children using depth first search.
GetComponentInParent    Returns the component of Type type in the GameObject or any of its parents.
GetComponents           Returns all components of Type type in the GameObject.
GetComponentsInChildren Returns all components of Type type in the GameObject or any of its children.
GetComponentsInParent   Returns all components of Type type in the GameObject or any of its parents.
SendMessage             Calls the method named methodName on every MonoBehaviour in this game object.
SendMessageUpwards      Calls the method named methodName on every MonoBehaviour in this game object and on every ancestor of the behaviour.
TryGetComponent         Gets the component of the specified type, if it exists.
GetInstanceID           Returns the instance id of the object.
ToString                Returns the name of the object.



 Static Methods
----------------
Destroy                 Removes a GameObject, component or asset.
DestroyImmediate        Destroys the object obj immediately. You are strongly recommended to use Destroy instead.
DontDestroyOnLoad       Do not destroy the target Object when loading a new Scene.
FindObjectOfType        Returns the first active loaded object of Type type.
FindObjectsOfType       Returns a list of all active loaded objects of Type type.
Instantiate             Clones the object original and returns the clone.


 Operators
-----------
bool                    Does the object exist?
operator  !=            Compares if two objects refer to a different object.
operator  ==            Compares two object references to see if they refer to the same object.
*/














/*===============================================================================================================================================
                                                               TRASH
  ===============================================================================================================================================
//class GameObject;
//bool operator == (std::vector<GameObject *>::iterator &lhv, const GameObject& rhv);
bool operator == (std::vector<GameObject *>::iterator &lhv, const GameObject& rhv)
{
    return (*lhv)->g_Handle() == rhv.g_Handle();
}




//
//static bool operator==(const GameObject& lhv, const GameObject& rhv)
//{
//    return lhv.g_Handle() == rhv.g_Handle();
//}

//template<typename _Ty>
//bool operator==(const _Ty& lhv, const GameObject& rhv)
//{
//    return *lhv == rhv.g_Handle();
//}
    /*

    template<class _InIt,
    class _Ty> inline
    _InIt _Find_unchecked1(_InIt _First, const _InIt _Last, const _Ty& _Val, false_type)
    {	// find first matching _Val
    for (; _First != _Last; ++_First)
        if (*_First == _Val)
            break;
    return (_First);
    }


*/
