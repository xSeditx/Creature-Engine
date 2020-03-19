#pragma once
#include"Core/Common.h"
 
#include<optional>

#define DEFAULT_WEIGHT 10.0f


enum PropertiesEnum
{
	SHIVA,
	ENEMY,
	PLAYER,
	GOODGUY_PROJECTILE,
	BADGUY_PROJECTILE,
	SHIELD,
	UFO,
	CITY,
	PROJECTILE,
	HEALTH_POWERUP,
	MONEY_POWERUP,
	STRENGTH_POWERUP
};




struct Collider;
struct Sprite;
struct AABBCollider;
struct SphereCollider;
class GameObject;

typedef void(*UserCollisionResponse)(GameObject& _first, GameObject& _other);
typedef void(*UserUpdateFunc)(Collider* _object, float _deltaTime);
//UserCollisionResponse Default_Response;

void Default_Response(GameObject& _first, GameObject& _other);
void Default_Update(Collider* _object, float _deltaTime);

typedef std::pair<GameObject&, GameObject&> CollisionPair;

enum Collider_t
{
	UNDEFINED,
	SPHERE,
	AABB
};

struct Collider
{
	//TODO(Colliders need a Custom Function Pointer(with defaults of course) so that its Update can be precisely controlled.
	//TODO("Make sure you set UserPointer to point to the Gameobject that this is attached to. At least until I figure out a better way and I very well might not have time for all that");
//Collider()
//	:
//	Weight(DEFAULT_WEIGHT),
//	Response(Default_Response),
//	UserUpdate(Default_Update),
//	Position(NULL),
//	Velocity(NULL),
//	Acceleration(NULL),
//	Force(NULL),
//	Rotation(0)
//{
//	//SOLVED: TODO("Should I do the same thing with the Response that I did with Update? If might allow better implementation down the line if this can just be passed");
//	/// GOOD IDEA: HIGH FIVE! FUCK YEAH!
//}

	Collider() = delete;
	Collider(GameObject* _parent, UserCollisionResponse _response, float _weight = DEFAULT_WEIGHT);

	///=============================================================================================================================================================================================================================================
	/// I have so many concerns about this right now but I need to access the Colliding objects by what Gameobject they are not what collider type.
	/// Perhaps I will figure a solution but currently my structure does not go well with what I need for this.
	
	void* User_Ptr;
	void s_UserPointer(void* _ptr);
	template<typename _Ty> inline _Ty& g_UserPointer();

	///=============================================================================================================================================================================================================================================

	GameObject* Parent;
	inline GameObject& g_Parent();    void s_Parent(GameObject* _parent);

	///==============================================================================================================================================================================================================================
	/// This should likely go inside of the GameObject class since they are connected now
	/// It should also be replaced by a Bitfield so that all uses of the Engine can define their own properties

	PropertiesEnum Group;
	inline PropertiesEnum g_Group();    void s_Group(PropertiesEnum _group);

	///==============================================================================================================================================================================================================================

	Vec2
		Position,
		Velocity,
		Acceleration,
		Force;

	float
		Rotation,
		Weight;

	Collider_t Type; // debug information, Irrelevent in Release builds.

	Vec2 Size;

	bool Alive = true;

	virtual Vec4 Overlap(Collider* _other);
	Vec2 minVec2(Vec2 first, Vec2 _second);
	Vec2 maxVec2(Vec2 first, Vec2 _second);
	//AABBCollider Overlap(AABBCollider *_other);

	virtual bool isCollision(Vec2 _point);
	virtual bool isCollision(Collider* other);
	virtual bool isCollision(AABBCollider* other);
	virtual bool isCollision(SphereCollider* other);

	virtual void StaticResolve(Vec2 _point);
	virtual void StaticResolve(Collider* other);
	virtual void StaticResolve(AABBCollider* other);
	virtual void StaticResolve(SphereCollider* other);

	virtual void s_Collider(Vec2  _pos, Vec2 _size) { Print("This should never be called directly"); }

	virtual void Render();

	void s_Weight(float _weight);

	inline UserUpdateFunc        g_UpdateFunction();       void s_UpdateFunction(UserUpdateFunc _func);
	inline UserCollisionResponse g_CollisionResponse();    void s_CollisionResponse(UserCollisionResponse _func);

	inline void Update(float deltaTime);

	bool Sweep(std::vector<GameObject*>& _test);///CollisionPair Sweep(std::vector<Collider *> _test);/// Should return a Vector of Collision Pairs but I am going to bail early on the first collision detected

	void Respond(GameObject& _other);

	//std::vector<GameObject *> Children;
	inline bool hasChildren();

protected:
	UserUpdateFunc UserUpdate = Default_Update; // For now we unprotect this
	UserCollisionResponse Response = Default_Response;
};

///===============================================================================================================
///
///        AABB Collider class
///
///===============================================================================================================

struct AABBCollider
	:
	public Collider
{
	AABBCollider() = delete;
	AABBCollider(GameObject* _parent, Vec2 _topleft, Vec2 _size);
	AABBCollider(GameObject* _parent);

	void s_Collider(Vec2 _topleft, Vec2 _size)
	{
		Position = _topleft + (Size * .5f);
		Size = _size;
	}

	void Resize();

	//bool isCollision(Vec2 _point);

	virtual bool isCollision(Vec2 _point);
	virtual bool isCollision(Collider* other);
	virtual bool isCollision(AABBCollider* other);
	virtual bool isCollision(SphereCollider* other);

	virtual void StaticResolve(Vec2 _point);
	virtual void StaticResolve(Collider* other);
	virtual void StaticResolve(AABBCollider* other);
	virtual void StaticResolve(SphereCollider* other);

	virtual void Render();
};
struct SphereCollider : public Collider
{
	SphereCollider(GameObject* _parent, Vec2 _position, float _radius);
	SphereCollider(GameObject* _parent);
	SphereCollider(Sprite* _Sprite);

	void s_Collider(Vec2 _position, float _radius)
	{
		Position = _position;  Size = Vec2(_radius);
	}
	virtual bool isCollision(Vec2 _point);
	virtual bool isCollision(Collider* other);
	virtual bool isCollision(AABBCollider* other);
	virtual bool isCollision(SphereCollider* other);

	virtual void StaticResolve(Vec2 _point);
	virtual void StaticResolve(Collider* other);
	virtual void StaticResolve(AABBCollider* other);
	virtual void StaticResolve(SphereCollider* other);

	virtual void Render();
};















///__________________________________________________________________________________________________________________________
///============================================ Inlined Member Functions ===============================================================


inline void Collider::Update(float deltaTime)
{ // There, now nothing has to change and I venture to say... this is much much better, hmmm
	UserUpdate(this, deltaTime);
}


inline GameObject& Collider::g_Parent()
{
	return *Parent;
}

inline UserUpdateFunc Collider::g_UpdateFunction()
{
	return UserUpdate;
}
inline UserCollisionResponse Collider::g_CollisionResponse()
{
	return Response;
}

template<typename _Ty>
inline _Ty& Collider::g_UserPointer()
{
	return (_Ty*)&User_Ptr;
}

inline PropertiesEnum Collider::g_Group() { return Group; }