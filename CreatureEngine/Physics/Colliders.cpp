#include<algorithm>
#include"Quadtree.h"

#include"Core/Common.h"
//#include"SDL_defines.h"
//#include"Sprite.h"
//#include"Renderer.h"
#include"../Renderer/LowLevel/OpenGL/Camera/Camera.h"
#include"Collider.h"
#include"Gameobject.h"


void Default_Response(GameObject&, GameObject& _other)
{
	//Vec2 Point1 = RigidBody.Position;
	Print("Default Collision response");
}
void Default_Update(Collider* _object, float _deltaTime)
{
	//_object->Velocity *= .95;//#PERF: Warm Cache, create Reference object for _object and operate on that
	_object->Position += _object->Velocity * _deltaTime;
	TODO("I could now move the Position and Velocity back to the GameObject class if I want to. It is HACKED for now");

	// Forward Euler Vt + dt = Vt + m-1 FORCE t dt
	// Implicit Euler Vt + dt = Vt + m -1 FORCE t + dtDT

	//_deltaTime = 1.0 / _deltaTime;
//Collider &O = *_object;
//O.Velocity = O.Velocity * .95f; // Perform adhoc Friction to slow object down
//
//Vec2 Last_Acceleration = O.Acceleration;
//O.Position += O.Velocity * _deltaTime + (Last_Acceleration * 0.5f * Squared(_deltaTime));
//O.Acceleration = O.Force / O.Weight;
//
//O.Force = Vec2(0.0f);
//
//Vec2 Avg_Acceleration = (Last_Acceleration + O.Acceleration) * 0.5f;
//O.Velocity += Avg_Acceleration * _deltaTime;
}


///__________________________________________________________________________________________________________________________
///============================================ COLLIDER BASE ===============================================================


Collider::Collider(GameObject* _parent, UserCollisionResponse _response, float _weight)
	:
	Parent(_parent),
	Weight(_weight),
	Velocity(NULL),
	Acceleration(NULL),
	Force(NULL)
{
	Rotation = 0;
	assert(Parent != nullptr);
}
void Collider::Respond(GameObject& _other)
{
	Response(this->g_Parent(), _other);
}

SphereCollider::SphereCollider(Sprite* _sprite)
	:
	SphereCollider
	(
		NULL,
		_sprite->Position,
		(float)min((int)((float)_sprite->Size.x * 0.35f), // What... The fuck did I do here and why??? Oh Size was iVec2
		(int)((float)_sprite->Size.y * 0.35f))
	)
{
	__debugbreak();
}
SphereCollider::SphereCollider(GameObject* _parent)
	:
	SphereCollider
	(
		_parent,
		_parent->g_Sprite().Position,
		(float)min((int)((float)_parent->g_Sprite().Size.x * 0.35f), // What... The fuck did I do here and why??? Oh Size was iVec2
		(int)((float)_parent->g_Sprite().Size.y * 0.35f))
	)
{
}
SphereCollider::SphereCollider(GameObject* _parent, Vec2 _position, float _radius)
	:
	Collider(_parent, Default_Response, 10)/// Fix default weight thing here
{
	Size = Vec2(_radius);
	Position = _position,
		Type = SPHERE;
}

AABBCollider::AABBCollider(GameObject* _parent)
	:
	AABBCollider(_parent, _parent->g_Sprite().Size, _parent->g_Sprite().Size)  // (Vec2)_parent->g_Sprite().Position - ((Vec2)_parent->g_Sprite().Size * 0.5f), _parent->g_Sprite().Size)
{
}
AABBCollider::AABBCollider(GameObject* _parent, Vec2 _position, Vec2 _size)
	:
	Collider(_parent, Default_Response, 10)
{
	Size = _size;
	Position = _position;
	Type = AABB;
}


void Collider::s_UserPointer(void* _ptr)
{
	User_Ptr = _ptr;
}
void Collider::s_Parent(GameObject* _parent)
{
	Parent = _parent;
}
void Collider::s_Group(PropertiesEnum _group)
{
	Group = _group;
}
void Collider::s_Weight(float _weight)
{
	Weight = _weight;
}
void Collider::s_CollisionResponse(UserCollisionResponse _func)
{
	Response = _func;
}
void Collider::s_UpdateFunction(UserUpdateFunc _func)
{
	UserUpdate = _func;
}



bool Collider::isCollision(Collider* _other)
{
	return _other->isCollision(this);
}
bool Collider::isCollision(Vec2 _point)
{
	__debugbreak();
	return false;
}
bool Collider::isCollision(AABBCollider* _other)
{
	return _other->isCollision(this);
}
bool Collider::isCollision(SphereCollider* _other)
{
	return _other->isCollision(this);
}


void Collider::StaticResolve(Vec2 _point)
{
	// Make point collider...
}
void Collider::StaticResolve(Collider* _other)
{
	_other->StaticResolve(this);
}
void Collider::StaticResolve(AABBCollider* _other)
{
	_other->StaticResolve(this);
}
void Collider::StaticResolve(SphereCollider* _other)
{
	_other->StaticResolve(this);
}
extern size_t CollisionTestCounter;
bool Collider::Sweep(std::vector<GameObject*>& _test)
{/// We should return the Collision Pair in the future but this is good enough for now
	if (Alive _EQUALS_ false)
	{// if it is not even Alive get the hell out of here
		return {};
	}

	for (auto& Other : _test)
	{// Test the Collider against all other Colliders in _test
		if (Other->hasCollider() _EQUALS_ false)
		{
			continue;
		}


		if (g_Group() _NOT_EQUAL_TO_ Other->g_Collider().g_Group() _AND_ Other->Alive() _EQUALS_ true)
		{// If they are not in the same Collision Group AND As long as Other Collider is Active Test them
			if (isCollision(&Other->g_Collider()))
			{// If they Collide Call the User defined Response Function. In a perfect world we would detect all
				Respond(*Other); //(g_Parent(), 
				//return{};
			}
			if (hasChildren())
			{
				Other->g_Collider().Sweep(g_Parent().Children);
			}
			if (Other->Children.size())
			{
				Sweep(Other->Children);// If this Collider has Children Pass them to Sweep and Recurse
			}
#ifdef _DEBUG
			CollisionTestCounter++;
#endif
		}
	}
	return {};
}

//for (auto &Child : g_Parent().Children)
//{
//	if(isCollision(//Sweep(_test);// If this Collider has Children Pass them to Sweep and Recurse
//}





bool Collider::hasChildren()
{
	return g_Parent().hasChildren();
}

void Collider::Render()
{
	DEBUG_TRACE("Default Collider Render");
}
///__________________________________________________________________________________________________________________________
///============================================ AABB COLLIDER ===============================================================

bool AABBCollider::isCollision(Collider* _other)
{// Polymorphic Collision Test
	return _other->isCollision(this);
}
bool AABBCollider::isCollision(Vec2 _point)
{// Checks to see if a point is within the bounds of the AABB axes

	float
		x1 = (Position.x - (Size.x * 0.5f)),
		x2 = (Position.x - (Size.x * 0.5f)) + this->Size.x,
		y1 = (Position.y - (Size.y * 0.5f)),
		y2 = (Position.y - (Size.y * 0.5f)) + this->Size.y;

	return
		(_point.x >= min(x1, x2) && _point.x <= max(x1, x2)) &&
		(_point.y >= min(y1, y2) && _point.y <= max(y1, y2));

}
bool AABBCollider::isCollision(AABBCollider* _other)
{// Check the Axes to see if they intersect

	Vec2 Topleft = Position - (Size * 0.5f);
	Vec2 OtherTopleft = _other->Position - (_other->Size * 0.5f);

	//Overlap(_other);
	return
		(Topleft.x < (OtherTopleft.x + _other->Size.x)) &&
		((Topleft.x + Size.x) > OtherTopleft.x) &&
		(Topleft.y < (OtherTopleft.y + _other->Size.y)) &&
		((Topleft.y + Size.y) > OtherTopleft.y);
}
bool AABBCollider::isCollision(SphereCollider* _other) // sphere, box
{// Check to see if the Closest point on an AABB is less than Radius of the Sphere
	Vec2 Topleft = (Position - (Size * 0.5f));
	Vec2 Min = Vec2(min(Topleft.x, (Topleft.x + Size.x)), min(Topleft.y, (Topleft.y + Size.y)));
	Vec2 Max = Vec2(max(Topleft.x, (Topleft.x + Size.x)), max(Topleft.y, (Topleft.y + Size.y)));

	float x = max(Min.x, min(_other->Position.x, Max.x));
	float y = max(Min.y, min(_other->Position.y, Max.y));

	float distance = sqrt
	(// Could replace with the Sphere point test in the future
		(x - _other->Position.x) * (x - _other->Position.x) +
		(y - _other->Position.y) * (y - _other->Position.y)
	);
	return distance < _other->Size.x;// Nearest point is less than the Spheres Radius
}

Vec4 Collider::Overlap(Collider* _other)
{
	Vec2
		Min1 = Position - (Size * 0.5f),
		Min2 = _other->Position - (_other->Size * 0.5f);
	Vec2
		Max1 = Min1 + Size,
		Max2 = Min2 + _other->Size;

	Vec2 Min = maxVec2(Min1, Min2);
	Vec2 Max = minVec2(Max1, Max2);

	Vec2 O = Camera::get().g_Center();

	//float Distance = std::sqrt(Squared(Position.x - _other->Position.x) + Squared(Position.y - _other->Position.y));
	//Vec2 Normals = glm::normalize(Position, _other->Position);

	Vec2 Sz = { Max.x - Min.x, Max.y - Min.y };
	return { Min, Vec2(Sz.x, Sz.y) };

}
void AABBCollider::StaticResolve(Vec2 _point) { Print("Incomplete"); }
void AABBCollider::StaticResolve(Collider* _other)
{
	_other->StaticResolve(this);
}
void AABBCollider::StaticResolve(AABBCollider* _other)
{
	//	AABBCollider O = Overlap(_other);
	//
	////if (O.Position.x > _other->Position.x)
	//{
	//	this->Position.x += O.Size.x;// *0.5f;
	//}
	//else
	//{
	////	this->Position.x -= O.Size.x;// *0.5f;
	//	if (O.Position.y > _other->Position.y)
	//	{
	//		this->Position.y += O.Size.y;// *0.5f;
	//	}
	//	else
	//	{
	//		this->Position.y -= O.Size.y;// *0.5f;
	//	}
	//
	//}
	//
	//
	//////if(O.Position.x <)
	//////this->Position.x   -= O.Size.x*0.5f; 
	//////_other->Position.x += O.Size.x*0.5f;
	////
	// if (O.Size.x <= O.Size.y)
	// {
	// 	this->Position.x -= O.Size.x;// *0.5f;
	// 	//_other->Position.x += O.Size.x*0.5f;
	// 
	// }
	// else
	// {
	// 	this->Position.y -= O.Size.y;// *0.5f;
	// 	//_other->Position.y += O.Size.y*0.5f;
	// }
}
void AABBCollider::StaticResolve(SphereCollider* _other)
{
	//float Distance = sqrt(Squared(Position.x - _other->Position.x) + Squared(Position.y - _other->Position.y));
	//float Overlap = (Distance - _other->Size.x - Size.x) * .5f;

	//Vec2 Difference = (_other->Position - Position) / Distance;

	//Position.x += Difference.x;
	//_other->Position -= Overlap * Difference;
}
Vec2 Collider::minVec2(Vec2 _first, Vec2 _second)
{
	return { min(_first.x,_second.x), min(_first.y, _second.y) };
}
Vec2 Collider::maxVec2(Vec2 _first, Vec2 _second)
{
	return { max(_first.x,_second.x), max(_first.y, _second.y) };
}
void AABBCollider::Render()
{// Renders a red box
	Vec2 Offset = Camera::get().g_Center();
	if (Alive)
	{
		SDL_SetRenderDrawColor(Renderer::get().g_Context(), 255, 0, 0, 10);
	}
	else
	{
		SDL_SetRenderDrawColor(Renderer::get().g_Context(), 0, 75, 255, 100);
	}
	SDL_RenderDrawRect(Renderer::get().g_Context(), &Renderer::make_Rect((Position - (Size * 0.5f)) + Offset, Size));
	SDL_SetRenderDrawColor(Renderer::get().g_Context(), DEFAULT_DRAW_COLOR);
}


///__________________________________________________________________________________________________________________________
///=========================================== SPHERE COLLIDER ==============================================================

bool SphereCollider::isCollision(Collider* _other)
{// Polymorphic Collision test for Spheres
	return _other->isCollision(this);
}
bool SphereCollider::isCollision(Vec2 _point)
{// Check to see if the Distance of a point from the center of a Sphere is closer than Spheres Radius
	// we are using multiplications because is faster than calling Math.pow
	return std::sqrt
	(// if the point is closer that the Radius of the Sphere return true
		(_point.x - this->Position.x) * (_point.x - this->Position.x) +
		(_point.y - this->Position.y) * (_point.y - this->Position.y)
	) < this->Size.x;
}
bool SphereCollider::isCollision(AABBCollider* other)
{// Perhaps redefine for better performance instead of calling another objects method
	return other->isCollision(this); // Use the Already defined AABB vs Sphere test
}
bool SphereCollider::isCollision(SphereCollider* other)
{// Checks to see if the distance between two spheres is smaller than their Radii
	return
		std::sqrt
		(
			Squared(this->Position.x - other->Position.x) +
			Squared(this->Position.y - other->Position.y)
		) < (this->Size.x + other->Size.x);
}

void SphereCollider::StaticResolve(Vec2 _point) { Print("Incomplete"); }
void SphereCollider::StaticResolve(Collider* _other)
{
	_other->StaticResolve(this);
}
void SphereCollider::StaticResolve(AABBCollider* other) {}
void SphereCollider::StaticResolve(SphereCollider* _other)
{
	float Distance =
		sqrt
		(
			Squared(Position.x - _other->Position.x) +
			Squared(Position.y - _other->Position.y)
		);

	float Overlap = (Distance - _other->Size.x - Size.x) * .5f;
	Vec2 Difference = (_other->Position - Position) / Distance;

	Position += Overlap * Difference;
	_other->Position -= Overlap * Difference;

}
void SphereCollider::Render()
{ // Below draws a rough Circle from a bunch of line segments
	Vec2 Offset = Camera::get().g_Center();
	SDL_SetRenderDrawColor(Renderer::get().g_Context(), 255, 0, 0, 130);
	int
		x1 = (int)(Position.x),
		y1 = (int)(Position.y),
		x2 = (int)(Position.x + (float)(cos(RADIANS(0)) * Size.x)),
		y2 = (int)(Position.y + (float)(sin(RADIANS(0)) * Size.x));

	Vec2 Center = Camera::toScreenSpace({ x1,y1 });
	Vec2 Test = Position + Offset;
	//	assert(Center _EQUALS_ Test);
	for (int i = 0; i < 360; i += (360 / 20))
	{
		x1 = (int)(Position.x + (float)cos(RADIANS(i)) * Size.x);
		y1 = (int)(Position.y + (float)sin(RADIANS(i)) * Size.x);
		SDL_RenderDrawLine(Renderer::get().g_Context(), x1 + Offset.x, y1 + Offset.y, x2 + Offset.x, y2 + Offset.y);
		x2 = x1;
		y2 = y1;
	}
	SDL_SetRenderDrawColor(Renderer::get().g_Context(), DEFAULT_DRAW_COLOR);
}



///=============================================================================================================================================================================================================================================











//Vec4 SphereCollider::Overlap(Collider *_other)
//{
//
//}
//Vec4 Collider::Overlap(Collider *_other) {}
//
//
//AABBCollider AABBCollider::Overlap(AABBCollider *_other)
//{
//	Vec2
//		Min1 = Position - (Size * 0.5f),
//		Min2 = _other->Position - (_other->Size * 0.5f);
//	Vec2
//		Max1 = Min1 + Size,
//		Max2 = Min2 + _other->Size;
//
//	Vec2 Min = maxVec2(Min1, Min2); 
//	Vec2 Max = minVec2(Max1, Max2); 
//
//	Vec2 O = Camera::get().g_Center();
//
//	//float Distance = std::sqrt(Squared(Position.x - _other->Position.x) + Squared(Position.y - _other->Position.y));
//	//Vec2 Normals = glm::normalize(Position, _other->Position);
//#if 0
///// Draws a Line from the Center of the Object to the Other AABB
//	SDL_SetRenderDrawColor(Renderer::get().g_Context(), 255, 255, 255, 155);
//	Vec2 Displacement = { _other->Position.x - Position.x ,  _other->Position.y - Position.y };
//	SDL_RenderDrawLine(Renderer::get().g_Context(), Position.x + O.x, Position.y + O.y, Position.x + Displacement.x + O.x, Position.y + Displacement.y + O.y);
//
//	/// Draws a Rect between the Overlap
//	SDL_SetRenderDrawColor(Renderer::get().g_Context(), 0, 0, 255, 255);
//	Vec2 Sz = { Max.x - Min.x, Max.y - Min.y };
//	SDL_Rect r = { Min.x + O.x, Min.y + O.y, Sz.x, Sz.y };
//	SDL_RenderDrawRect(Renderer::get().g_Context(), &r);
/////
//#endif
//	Vec2 Sz = { Max.x - Min.x, Max.y - Min.y };
//	return { Min, Vec2(Sz.x, Sz.y) };//Max };
//}
/*
Turning this into an Instant reaction instead of creating collision pairs. It is not a true physics simulation so this might now be much of a problem
///CollisionPair Collider::Sweep(std::vector<Collider *> _test) ///
bool Collider::Sweep(std::vector<Collider *> _test)
{
	if (this->Alive _EQUALS_ false)
	{
		return {};
	}
	for (auto &C : _test)
	{
		if (
			this->CollisionGroup _NOT_EQUAL_TO_ C->CollisionGroup
			_AND_ C->Alive _EQUALS_ true
			)
		{
			if (isCollision(C))
			{// Here we are bailing early at the first detected Collision. In a perfect world we would detect all
				///return { this, C };
				Response(this, C);
			}
			for (auto& Child : C->Children)
			{ //#IMROVE Children Test. This needs to be done Differently
				if (isCollision(Child))
				{// Here we are bailing early at the first detected Collision. In a perfect world we would detect all
					Response(this, Child);
					///return { this, Child };
				}
			}
		}
	}
	*/
	//  
	//  bool Collider::Sweep(std::vector<GameObject*> &_test)
	//  {/// We should return the Collision Pair in the future but this is good enough for now
	//  	if (this->Alive _EQUALS_ false)
	//  	{// if it is not even Alive get the hell out of here
	//  		return {};
	//  	}
	//  
	//  	for (auto &C : _test)
	//  	{
	//  		if (C->hasCollider() _EQUALS_ false)
	//  		{
	//  			continue;
	//  		}
	//  		if( this->g_Group() _NOT_EQUAL_TO_ C->g_Collider().g_Group() _AND_ C->Active _EQUALS_ true )
	//  		{// Test the Collider against all other Colliders in _test
	//  			if (isCollision(C->Hitbox))
	//  			{// Here we are bailing early at the first detected Collision. In a perfect world we would detect all
	//  				Response(this->g_Parent(), *C);
	//  			}
	//  			TODO("GameObject has no member Get Group... GO should be the one with that, Hacked for now....");
	//  			for(auto& Child: C->Children)
	//  			{ //#IMPROVE Children Test. This needs to be done Differently
	//  				if ( this->g_Group() _NOT_EQUAL_TO_ Child->g_Collider().g_Group() _AND_ Child->Active _EQUALS_ true) /// The Child is almost always same group as Parent do we need this really?
	//  				{// Test All Children Colliders as well
	//  					if (isCollision(Child->Hitbox))
	//  					{// Here we are bailing early at the first detected Collision. In a perfect world we would detect all
	//  						Response(this->g_Parent(), *Child);
	//  					}
	//  				}
	//  			}
	//  		}
	//  
	//  	}
	//  }
//bool AABBCollider::isCollision(Vec2 _point)
//{
//	Vec2 SZ = Size * 0.5f;
//	return
//		((_point.x > Position.x - SZ.x) _AND_
//		 (_point.x < Position.x + SZ.x) _AND_
//		 (_point.y > Position.y - SZ.y) _AND_
//		 (_point.y < Position.y + SZ.y));
//
//}
//bool SphereCollider::isCollision(Vec2 _point)
//{
//	return (Distance(Position, _point) < Size.x);
//}
//
				//TODO("GameObject has no member Get Group... GO should be the one with that, Hacked for now....");
				//for (auto& Child : C->Children)
				//{ //#IMPROVE Children Test. This needs to be done Differently
				//	if (this->g_Group() _NOT_EQUAL_TO_ Child->g_Collider().g_Group() _AND_ Child->Active _EQUALS_ true) /// The Child is almost always same group as Parent do we need this really?
				//	{// Test All Children Colliders as well
				//		if (isCollision(Child->Hitbox))
				//		{// Here we are bailing early at the first detected Collision. In a perfect world we would detect all
				//			Response(this->g_Parent(), *Child);
				//		}
				//	}
				//}