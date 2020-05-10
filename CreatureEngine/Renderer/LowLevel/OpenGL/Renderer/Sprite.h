
#include"Core/Common.h"
#include"LowLevel/Materials/Image/Texture.h"

#include<string>

class Sprite
{
    enum Update_t
    {
        Frames_Per_Second,
        Frames_Per_GameTick
    };

    /* Sets the Update type: Frames_Per_Second or Frame_Per_GameTick Allowed */
    void set_Speed_Type(Update_t _type) { Frame_Update_Type = Frames_Per_Second; }

    /* Returns the Type of Delta being used for the Sprite */
    Update_t get_Speed_Type(){ return Frame_Update_Type; }

    /* Returns the Delta to wait before Updating Sprite */
    float get_Speed() { return Image_Speed; }

    Sprite() 
    { 
        TODO("Far from complete and just laying out rough Guidelines for functionality desired in the Sprite Class ");
        Sprite_Renderer = new Shader(V_Renderer, F_Renderer);
        Sprite_Renderer->Bind();
        {
            OpenGL::set_Attribute(2, "VertexPosition");
            OpenGL::set_Attribute(2, "TextureCoord");
        }
        Sprite_Renderer->Unbind();
    }
    ~Sprite();

    ///================= Dimensions and Offsets ===================

    /* Return the Display Width of the Sprite */
    float Width()  { return (float)Image_Width * image_Scale_x; }

    /* Return the Display Height of the Sprite */
    float Height() { return (float)Image_Height * image_Scale_y;  }

    /* Sets the Offset of the Origin */
    void set_Origin(Vec2 _offset) { Origin = _offset; }
    /* Sets the Offset of the Origin using XY position */
    void set_Origin(float _x, float _y) { Origin = { _x, _y }; }

    /* Returns the X component of the Origin */
    float get_Offset_X() { return Origin.x; }

    /* Returns the Y component of the Origin */
    float get_Offset_y() { return Origin.y; }

    Vec2 Origin {0, 0};
    ///============================================================

    /* Returns the amount of Subimages the sprite image has */
    uint32_t SubImage_count() { return Subimages; }
    uint32_t Subimages{ 0 }; /// This needs to be the amount of sub images. Perhaps that should be a Vector and we return the Size in Subimage_count


    /* Returns the Sprites Object Name */
    std::string get_Name() { return Object_Name; }

    ///====== This will likely be part of a 2D GameObject and inherited  ======
    /// That will come from a Collision Mask object Applies to 2D GameObjects 
    enum Mask_t
    {
        Automatic,
        Full, 
        Manual
    } Mask_Mode;
    /* Sets the Current Mask mode */
    void set_Mask_Mode(Mask_t _mode) { Mask_Mode = _mode; }
    /* Sets Bounding Box from Vec4 */
    void set_BBOX(Vec4 _rect) { Bounding_Box = _rect; }
    /* Sets Bounding Box from Offset and Size */
    void set_BBOX(Vec2 _topleft, Vec2 _size) { Bounding_Box = { _topleft.x, _topleft.y, _topleft.x + _size.x, _topleft.y + _size.y }; }
    /* Sets Bounding Box from floating point position and dimensions */
    void set_BBOX(float _x, float _y, float _w, float _h) { Bounding_Box = { _x,_y,_x + _w,_y + _h }; }

    /* Returns the Y component of the Bounding Box Relative to the Image */
    float get_bbox_Top()    {return Bounding_Box.y;}
    /* Returns the X component of the Bounding Box Relative to the Image */
    float get_bbox_Left()   {return Bounding_Box.x;}
    /* Returns the Width component of the Bounding Box */
    float get_bbox_Right()  {return Bounding_Box.z;}
    /* Returns the Height component of the Bounding Box */
    float get_bbox_Bottom() {return Bounding_Box.w;}
    /* Returns the way the Collision mask was Created */
    Mask_t get_Mask_Mode()  { return Mask_Mode; }
    Vec4 Bounding_Box { 0, 0, 0, 0  };// The Bounding Box of the Sprite
    ///========================================================================

    /* Returns a pointer to the Texture Object in use by the Sprite */
    Graphics::Texture& get_Texture() { return *Image_Texture; }
    /// This will return the Subimage of the Texture that this sprite uses for when single Texture pages contain multiple Image Objects 
    Graphics::Texture& get_Texture(uint32_t _subimage) { TODO("Single Texture Pages will contain multiple Sprites and will be Indexed. This will return the specific image from the Input Subimage."); return *Image_Texture; }

    /// For when proper Animation handling is created 
    void sprite_get_uvs()
    {// Description: This function returns an array with the UV coordinates and other data for the texture of the sprite sub - image on the texture page.The function returns an array with the following 8 elements:
        //  [0] = left
        //  [1] = top
        //  [2] = right
        //  [3] = bottom
        //  [4] = amount of pixels the asset compiler has trimmed from the sprites left side
        //  [5] = amount of pixels the asset compiler has trimmed from the sprites top side
        //  [6] = normalised percentage of pixel data from the original sprites width that has been saved to the texture page
        //  [7] = normalised percentage of pixel data from the original sprites height that has been saved to the texture page
    }
private:
    Graphics::Texture *Image_Texture{ nullptr };
    uint32_t Image_Width{ 0 };
    uint32_t Image_Height{ 0 };

    float image_Scale_x{ 1.0f };// Scale of the X Axis On Screen Image
    float image_Scale_y{ 1.0f };// Scale of the Y Axis On Screen Image

    Update_t Frame_Update_Type{ Frames_Per_Second };
    float Image_Speed{ 0 };

    /// All of this will come from a Parent GameObject class soon enough
    std::string Object_Name;
    uint32_t Object_ID; 

    float Image_Alpha{ 1.0f };
    float Image_Angle{ 0.0f };
    Vec4  Image_Blend{ 1.0,1.0,1.0,1.0 };



    void Render()
    {
        Sprite_Renderer->Bind();
        {
            Sprite_Renderer->SetUniform("Image_Angle", Image_Angle);
            Sprite_Renderer->SetUniform("Image_Blend", Image_Blend);
            Sprite_Renderer->SetUniform("Image_Alpha", Image_Alpha);
            Sprite_Renderer->SetUniform("Origin",Origin);

        }
        Sprite_Renderer->Unbind();
    }
    uint32_t Sprite_VBO{ 0 };
    uint32_t Sprite_VAO{ 0 };

    Shader *Sprite_Renderer{ nullptr };
    std::string V_Renderer = "                    \n\
    #version 430                                  \n\
    layout(location = 0) in vec2 VertexPosition;  \n\
    layout(location = 1) in vec2 TextureCoord;    \n\
    uniform mat4 ProjectionMatrix;                \n\
    uniform mat4 ViewMatrix;                      \n\
    uniform mat4 ModelMatrix;                     \n\
    uniform float Image_Angle;                    \n\
    uniform vec2 Origin;                          \n\
    out vec2 TCoords;                             \n\
    void main()                                   \n\
    {                                             \n\
        TCoords = TextureCoord;                   \n\
        vec2 Pos = vec2(VertexPosition.x + cos(Image_Angle) * Origin.x, VertexPosition.y + sin(Image_Angle) * Origin.y); \n\
        gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Pos.xy, 0.0, 1.0); \n\
    }";

    std::string F_Renderer= "                   \n\
    #version 430                                \n\
    #extension GL_NV_bindless_texture : enable  \n\
    #extension GL_NV_gpu_shader5 : enable       \n\
    in vec2 TCoords;                            \n\
    uniform uint64_t DiffuseTexture;            \n\
    uniform Vec4 Image_Blend;                   \n\
    uniform Vec4 Image_Alpha;                   \n\
    out vec4 color;                             \n\
    void main()                                 \n\
    {                                           \n\
        color = texture(sampler2D(DiffuseTexture), TCoords); \n\
    }";

};
    //uniform sampler2D DiffuseTexture;




/// Sprite Manipulation Functions
// _exists
// _add
// _replace
// _duplicate
// _assign
// _merge
// _create_from_surface
// _add_from_surface
// _collision_mask
// _delete
// _set_alpha_from_sprite
// _set_cache_size
// _set_cache_size_ext
// _save
// _save_strip
// _flush
// _flush_multi
// _prefetch
// _prefetch_multi







/// Sprite Instance Variables
//Description: These variables are built in to all GameMaker Studio 2 objects and are all related to the sprite assigned to the instance in the room and can be used to change what is drawn and how:
// _blend /// Tints the Given object a specific RGBA color 

/// Image stuff
// _index
// _number
// _speed

/// Idk if I want to feed these variables into the Shader and perform Scaling there or if I wish to predefine the positioning of the Vertices for the bottom Right corner prior to sending that data to the Shader
// _xscale
// _yscale