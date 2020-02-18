#pragma once

class Shader;

class GameObject
{
public:
	GameObject()
		:
		Scale(0),
		Position(0),
		Rotation(0),
		Transform(Mat4(0)),
		Handle(ObjectCount++)
	{
		Transform = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
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

	virtual void Update() = pure_virtual;
	virtual void Unbind() = pure_virtual;

	virtual void Bind()   = pure_virtual;
	virtual void Render() = pure_virtual;

	//TagID GetID() { return Object_ID; }


	void s_Position(Vec3 _position) { Position = _position; }
	void s_Rotation(Vec3 _rotation) { Rotation = _rotation; }
	void s_Scale(Vec3 _scale) { Scale = _scale; }
	void s_Scale(float _scale) { Scale = Vec3(_scale); }
	void s_Transform(Mat4 _transform) { Transform = _transform; }

	void UpdateTransform()
	{

		Transform = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
		Transform = glm::translate(Transform, Position);
		Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
		Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
	}


 

    /* Compares if two objects refer to a different object. */
	bool operator !=(GameObject& _other)
	{
		return Handle != _other.Handle;
	}

	/* Compares two object references to see if they refer to the same object.*/
	bool operator ==(GameObject& _other)
	{
		return Handle == _other.Handle;
	}


	Mat4 Transform;
	Vec3 Position, Rotation, Scale;
	static int ObjectCount;
private:

	uint32_t Handle;
};


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