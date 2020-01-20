___
# CAMERA 
___

___
### Data Members
___
  
___
  PUBLIC:
___

#### Floating points

   Data Members   |  Description
------------------|----------------------------------------
  FOV             |   Field of view in Degrees
  AspectRatio     |   Aspect Ratio of the Screen
  Near            |   Near clipping range
  Far;            |   Far clipping range




  #### Matrix

  Data Members |  Mat4 Matrix
---------------|---------------
ProjectionMatrix;   |Current Projection Matrix
ViewMatrix;    | Current ViewMatrix

#### std::stack\<Matrix\>

  Data Member |  Description
--------------|----------------------
ViewMatrixStack;       |  - Current View Matrix Stack.
ProjectionMatrixStack; |  - Current Projection Matrix Stack.


 Data Member |   Viewport
-------------|-------------------
  static Viewport *Camera; |  Static Currently Active Camera Instance

___						
  PRIVATE:									
___

#### Vec3
 Data Memeber | Description
-----------------------------|-------
  Position,    |    Current Position state
  Rotation,    |    Current Rotation state
  Forward,     |    Current Forward Vector the direction the Camera is facing
  Right,       |    Current Right facing Vector
  Up;          |    Current Up Vector


#### Matrix

 Data Members |  Description
--------------|-------------
  Matrix Identity = Matrix(1.0f);  |    Returns an Identity Matrix


<hr>


____
### Methods:
____

____
   Viewport(Vec3 positon, Vec3 Rotation);   Constructor
____


Methods                  |   Description
-------------------------|-------------------------------------------------------------------
void PushProjectionMatrix(); | Pushes the Current Camera Projection Matrix to the Stack
void PushViewMatrix();       | Pushes the Current Camera View Matrix to the Stack
void PopProjectionMatrix();  | Pop the Current Camera Projection Matrix to the Stack setting the previous Matrix as the current
void PopViewMatrix();        | Pop the Current Camera View Matrix to the Stack setting the previous Matrix as the current
 

____
#### FUNCTIONS FOR THE CAMERA CLASS
____


 
   Public Function                        |        Description
------------------------------------------|-------------------------------
Matrix GetViewMatrix();                   |    Return Current Active ViewMatrix
void Update();				              |    Updates the Transforms for Camera Location
void Bind();					          |    Sets the Uniforms of the Currently Active Shader for the Projection and Model Matrices
void Unbind();				              |    Currently Nothing
void Render();                            |    FUTURE: Render The Scene Associate with the Camera here, possibly shadow mapping but that might get contained in the Light class
void Rotate(float pitch, float yaw);      |    Set the current Rotation of the Camera
void MoveForward(float speed);            |    Move the current Position Forward at @param Speed
void MoveBack(float speed);               |    Move the current Position back at @param Speed
void MoveLeft(float speed);               |    Move the current Position Left at @param Speed
void MoveRight(float speed);	          |    Move the current Position Right at @param Speed
void RotateX(GLfloat Angle);	          |    Apply X Axis Rotation
void RotateY(GLfloat Angle);	          |    Apply Y Axis Rotation
void RotateZ(GLfloat Angle);	          |    Apply Z Axis Rotation
void Translate(Vec3 pos);	              |    Apply Transform for the Current Position
void Set_ViewMatrix(Matrix mat);          |    Set the Current view Matrix via @param Matrix mat
void Set_ProjectionMatrix(Matrix mat);    |    Set the Current Projection Matrix via @param Matrix mat
