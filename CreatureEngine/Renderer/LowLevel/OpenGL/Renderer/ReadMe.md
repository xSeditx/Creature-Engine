# Highly Incomplete

___
## Sprite Class
___


___
### Enumerations
___

Update_t::Frames_Per_Second,
Update_t::Frames_Per_GameTick

Mask_t::Automatic,
Mask_t::Full, 
Mask_t::Manual

<hr>

___
#### Methods 
___

Function Name | Description
--------------|----------------
Sprite() | Creates a Sprite Object
~Sprite() | Destroys a Sprite Object
void set_Speed_Type(Update_t _type) | Sets the Update type: Frames_Per_Second or Frame_Per_GameTick Allowed 
Update_t get_Speed_Type() | Returns the Type of Delta being used for the Sprite 
float get_Speed() | Returns the Delta to wait before Updating Sprite 
float Width() | Return the Display Width of the Sprite 
float Height() | Return the Display Height of the Sprite 
void set_Origin(Vec2 _offset) | Sets the Offset of the Origin 
void set_Origin(float _x, float _y) | Sets the Offset of the Origin using XY position 
float get_Offset_X(); | Returns the X component of the Origin 
float get_Offset_y(); | Returns the Y component of the Origin 
uint32_t SubImage_count() | Returns the amount of Subimages the sprite image has 
std::string get_Name() | Returns the Sprites Object Name 
void set_Mask_Mode(Mask_t _mode) | Sets the Current Mask mode
void set_BBOX(Vec4 _rect) | Sets Bounding Box from Vec4
void set_BBOX(Vec2 _topleft, Vec2 _size) | Sets Bounding Box from Offset and Size 
void set_BBOX(float _x, float _y, float _w, float _h) | Sets Bounding Box from floating point position and dimensions
float get_bbox_Top() | Returns the Y component of the Bounding Box Relative to the Image 
float get_bbox_Left() | Returns the X component of the Bounding Box Relative to the Image 
float get_bbox_Right() | Returns the Width component of the Bounding Box 
float get_bbox_Bottom() | Returns the Height component of the Bounding Box 
Mask_t get_Mask_Mode() | Returns the way the Collision mask was Created 
Graphics::Texture& get_Texture() | Returns a pointer to the Texture Object in use by the Sprite 
void Render(); | Renders the Sprite in its current state


<hr>

___
### Variables
___


Data Members|  Description
------------|-----------
Vec4 Bounding_Box  | The Bounding Box of the Sprite
uint32_t Subimages | This needs to be the amount of sub images. Perhaps that should be a Vector and we return the Size in Subimage_count
Vec2 Origin | Center of the Sprite
Graphics::Texture *Image_Texture |
uint32_t Image_Width | Height of the Image on Screen
uint32_t Image_Height | Width of Image on Screen
float image_Scale_x | Scale of the X Axis On Screen Image 
float image_Scale_y | Scale of the Y Axis On Screen Image
Update_t Frame_Update_Type | Type of Update, Per Tick or Per Second
float Image_Speed | Speed to Update the Sprite
std::string Object_Name | Name of the Object
uint32_t Object_ID | ID of the Object
float Image_Alpha | Alpha on screen
float Image_Angle | Rotation of Sprite
Vec4  Image_Blend | Blends the Object with this Color
uint32_t Sprite_VBO | Object GL Vertex Quad
uint32_t Sprite_VAO | The Vertex Array Object
Shader *Sprite_Renderer | Default Shader for the Sprites
std::string V_Renderer | Default Vertex Shader
std::string F_Renderer | Default Fragment Shader
Mask_t Mask_Mode | The Mode of the Collision Mask for the Object


___
## TODO
___
* This will return the Subimage of the Texture that this sprite uses for when single Texture pages contain multiple Image Objects 
* Graphics::Texture& get_Texture(uint32_t _subimage);
* void sprite_get_uvs()  Description: This function returns an array with the UV coordinates and other data for the texture of the sprite sub - image on the texture page.The function returns an array with the following 8 elements:










___
## DISCUSSION AND NOTES
___


A batch breaks in other Game Engines like GMS2 for instance can occur when you update one of the engines draw settings. <br>  
These include things such as changing color/alpha, font, blendmode, as well as others.                                  <br> 
                                                                                                                        <br> 
Here are some common buckets in GMS2: |                                                                                 <br> 
-----------|----------------------------------------------------------------                                                                                                                         
Primitives | draw_rectangle, draw_circle, draw_primitive_begin, etc.                                                     
Surfaces| surface_set_target, surface_reset_target                                                                     
Shaders | shader_set_target, shader_reset_target                                                                         
Draw settings |  draw_set_font, draw_set_colour, etc.                                                                   
GPU settings | gpu_set_blendmode, gpu_set_alphaenable, etc.                                                             
                                                                                                                        <br> 
                                                                                                                        <br> 
This falls inline with the structure I am shooting for although having a bit of trouble realizing which is the following<br> 
and is what Driver writers suggest one does inorder to get the lowest possible overhead from Modern OpenGL Drivers      <br>

```
foreach(render target)     // framebuffer
foreach(pass)              // depth, blending, etc... states
foreach(material)          // shaders
foreach(material instance) // textures, normals, diffuse,
foreach(vertex format)     // vertex buffers
foreach(object)            //
{
    WriteUniformData(object);
    glDrawElementBaseVertex
    (
        GL_TRIANGLES,
        object->indexCount,
        GL_UNSIGNED_SHORT,
        object->indexDataOffset,
        object->baseVertex
    );
}
```

