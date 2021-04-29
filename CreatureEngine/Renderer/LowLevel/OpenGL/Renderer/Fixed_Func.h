#include"../../../../Core/Common.h"
#include"GLBuffers.h"
#include"../Shader/Shader.h"



#pragma warning(push)

#pragma warning ( disable : 4267)
#pragma warning ( disable : 4244)


/* =====================================================================================================================================
/* MODULE: FIXED FUNCTION.CPP
/* DATE: 7/26/2020
/*
/* THIS LIKELY WILL NOT SURVIVE, JUST EXPERIMENTING WITH MAKING A RENDERING API THAT SIMULATES THE SIMPLICITY OF FIXED FUNCTION 
/*     PIPELINE WHILE OBVIOUSLY ADDING MY OWN SPIN TO MAKE IT MORE POWERFUL
/* ===================================================================================================================================== */

struct GL
{
    struct Bit_Flags
    {
        uint32_t unused1 : 1;
        uint32_t unused2 : 1;
        uint32_t MULTISAMPLE_State : 1;
        uint32_t unused4 : 1;

        uint32_t unused5 : 1;
        uint32_t unused6 : 1;
        uint32_t unused7 : 1;
        uint32_t unused8 : 1;

        uint32_t unused9 : 1;
        uint32_t unused10 : 1;
        uint32_t unused11 : 1;
        uint32_t unused12 : 1;

        uint32_t SCISSOR_State : 1;          //  GL_SCISSOR_TEST flag
        uint32_t TEXTURE_State : 1;	         //  Enable bits for the four texture coordinates
        uint32_t LIST_State : 1;             //  GL_LIST_BASE setting
        uint32_t EVAL_State : 1;

        uint32_t HINT_State : 1;              //  GL_PERSPECTIVE_CORRECTION_HINT setting
        uint32_t COLOR_BUFFE_State : 1;
        uint32_t ENABLE_State : 1;
        uint32_t TRANSFORM_State : 1;        //  Coefficients of the six clipping planes

        uint32_t VIEWPORT_State : 1;         //  Depth range(near and far)
        uint32_t STENCIL_BUFFER_State : 1;
        uint32_t ACCUM_BUFFER_State : 1;
        uint32_t DEPTH_BUFFER_State : 1;

        uint32_t FOG_State : 1;             //	GL_FOG enable bit
        uint32_t LIGHTING_State : 1;        //  GL_COLOR_MATERIAL enable bit
        uint32_t PIXEL_MODE_State : 1;      //	GL_RED_BIAS and GL_RED_SCALE settings
        uint32_t POLYGON_STIPPLE_State : 1; //	Polygon stipple image

        uint32_t POLYGON_State : 1;         //        	//GL_CULL_FACE enable bit
        uint32_t LINE_State : 1;	        //  GL_LINE_SMOOTH flag
        uint32_t POINT_State : 1;           //  GL_POINT_SMOOTH flag
        uint32_t CURRENT_State : 1;         //  GL_CURRENT_BIT      Current RGBA color
    };

    static Bit_Flags State_Flags;
    const uint32_t ALL_ATTRIB_State = 0xFFFFFFFF;

    void EnableClientState(uint32_t _cap)
    {
        switch (_cap)
        {
            case GL_VERTEX_ARRAY          : { pointer_Vertex.Enable();         } break;
            case GL_NORMAL_ARRAY          : { pointer_Normal.Enable();         } break;
            case GL_COLOR_ARRAY           : { pointer_Color.Enable();          } break;
            case GL_TEXTURE_COORD_ARRAY   : { pointer_TexCoord[Active_TextureUnit].Enable();       } break;
            case GL_EDGE_FLAG_ARRAY       : { pointer_EdgeFlag.Enable();       } break;
            case GL_SECONDARY_COLOR_ARRAY : { pointer_SecondaryColor.Enable(); } break;
            case GL_FOG_COORD_ARRAY       : { pointer_FogCoord.Enable();       } break;
        }
    }
    void DisableClientState(uint32_t _cap)
    {
        switch (_cap)
        {
            case GL_VERTEX_ARRAY          : { pointer_Vertex.Disable();         } break;
            case GL_NORMAL_ARRAY          : { pointer_Normal.Disable();         } break;
            case GL_COLOR_ARRAY           : { pointer_Color.Disable();          } break;
            case GL_TEXTURE_COORD_ARRAY   : { pointer_TexCoord[Active_TextureUnit].Disable();       } break;
            case GL_EDGE_FLAG_ARRAY       : { pointer_EdgeFlag.Disable();       } break;
            case GL_SECONDARY_COLOR_ARRAY : { pointer_SecondaryColor.Disable(); } break;
            case GL_FOG_COORD_ARRAY       : { pointer_FogCoord.Disable();       } break;
        }
    }
    void ClientActiveTexture(uint32_t _texture)
    {
        Active_TextureUnit = _texture;
    }

    struct Attribute_Pointer
    {/// I believe I have Stride wrong, it should be 0 but can change for Buffers that are VNCU etc
        Attribute_Pointer() = default;
        Attribute_Pointer(const char *_name) noexcept
            :
            Name{ _name },
            GL_Handle{ OpenGL::new_VBO() }
        {}
        Attribute_Pointer(const char *_name, uint32_t _size) noexcept
            :
            Name{ _name },
            Size{ _size },
            GL_Handle{ OpenGL::new_VBO() }
        {}

        inline void set_Data(uint32_t _stride, void *_ptr)
        {
            set_Data(Name, Size, Type, _stride, _ptr);
        }
        inline void set_Data(uint32_t _type, uint32_t _stride, void *_ptr) noexcept
        {
            set_Data(Name, Size, _type, _stride, _ptr);
        }
        inline void set_Data(uint32_t _size, uint32_t _type, uint32_t _stride, void *_ptr) noexcept
        {
            set_Data(Name, _size, _type, _stride, _ptr);
        }
        inline void set_Data(const char *_name, uint32_t _size, uint32_t _type, uint32_t _stride, void *_ptr) noexcept
        {// Set the Attribute in the Shader and Set our Buffer data 
            assert(GL_Handle != NULL);
            Active_Program->Bind();
            {
                Size = _size;
                Type = _type;
                Name = _name;
                Stride = _stride;
                Pointer = _ptr;

                Bind();//
                OpenGL::set_BufferData(Size, _ptr);
                OpenGL::set_Attribute(Active_Program->g_Handle(), Size, Name);//
            }
            Active_Program->Unbind();

        }

        void Bind() 
        {
            OpenGL::bind_VBO(GL_Handle);
        }   

        void Enable()  { isEnabled = true;  }
        void Disable() { isEnabled = false; }
        void *Pointer{ nullptr };  // Pointer to the Raw Data store
        const char *Name;          // Attribute name when Accessed in the Shader 
        uint32_t Size{ 4 };        // Specifies the number of coordinates per vertex.Must be 2, 3, or 4. The initial value is 4.
        uint32_t Type{ GL_FLOAT }; // Specifies the data type of each coordinate in the array.Symbolic constants GL_SHORT, GL_INT, GL_FLOAT, or GL_DOUBLE are accepted.The initial value is GL_FLOAT.
        uint32_t Stride{ 0 };      // Specifies the byte offset between consecutive vertices.If stride is 0, the vertices are understood to be tightly packed in the array.The initial value is 0.
        uint32_t GL_Handle{ 0 };   // Handle of our OpenGL Vertex Buffer 

        bool isEnabled{ false };   // If true enabled for writing and used during rendering when glArrayElement, glDrawArrays, glDrawElements, glDrawRangeElements glMultiDrawArrays, or glMultiDrawElements is called. 
    };


    static void Initialize(uint32_t _config);

    static void End();
    static void Begin(uint32_t _primativeType);

    static void Rotate(float _angle, float _x, float _y, float _z);
    static void Rotate(double _angle, float _x, float _y, float _z);

    static void Viewport(int32_t _x, int32_t _y, int32_t _w, int32_t _h);
    static void Translate(float _x, float _y, float _z);
    static void MatrixMode(uint32_t _matMode);
    static void LoadIdentity();

    static void Ortho(double left, double right, double bottom, double top, double nearVal, double farVal);
    static void Perspective(double fovy, double aspect, double zNear, double zFar);

    static void Clear();
    static void ClearColor(float _r, float _g, float _b, float _a);
    static void ClearColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a);

    /* The Following I am not sure what I want to do, if point towards a VBO or instead a Client side array */
    static void IndexPointer(uint32_t type, uint32_t stride, void * pointer);
    static void ColorPointer(uint32_t _size, uint32_t _type, uint32_t _stride, void *_pointer);
    static void VertexPointer(uint32_t _size, uint32_t type, uint32_t stride, void * pointer);
    static void NormalPointer(uint32_t type, uint32_t stride, void * pointer);
    static void TexCoordPointer(uint32_t size, uint32_t type, uint32_t stride, void * pointer);
    static void FogCoordPointer(uint32_t type, uint32_t stride, void * pointer);
    static void EdgeFlagPointer(uint32_t stride, void * pointer);
    static void SecondaryColorPointer(uint32_t size, uint32_t type, uint32_t stride, void * pointer);



    /*   DESCRIPTION :  glDrawArrays specifies multiple geometric primitives with very few subroutine call
         Parameters:
         - mode:   Specifies what kind of primitives to render.Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_QUAD_STRIP, GL_QUADS, and GL_POLYGON are accepted.
         - first:  Specifies the starting index in the enabled arrays.
         - count:  Specifies the number of indices to be rendered.*/
    static void DrawArray(uint32_t  mode, int32_t first, uint32_t count);

    /*  DESCRIPTION:   Specifies an index into the enabled vertex data arrays.    */
    static void ArrayElement(uint32_t i);

    /*  DESCRIPTION: Pushes the current Active Matrix to the stack*/
    static void PushMatrix();

    /*  DESCRIPTION: Pops the current Active Matrix to the stack*/
    static void PopMatrix();

    /*  DESCRIPTION: Specify the Indices and type */
    static void Index(uint8_t _indice);
    static void Index(uint16_t _indice);
    static void Index(uint32_t _indice);
    static void Index(float _indice);
    static void Index(double _indice);

    /*    DESCRIPTION: Sets the draw Color for the current Vertex   */
    static void Vertex(int16_t    _x, int16_t   _y);
    static void Vertex(int32_t    _x, int32_t   _y);
    static void Vertex(float      _x, float     _y);
    static void Vertex(double     _x, double    _y);
      
    static void Vertex(int16_t    _x, int16_t   _y, int16_t   _z);
    static void Vertex(int32_t    _x, int32_t   _y, int32_t   _z);
    static void Vertex(float      _x, float     _y, float     _z);
    static void Vertex(double     _x, double    _y, double    _z);
      
    static void Vertex(int16_t    _x, int16_t   _y, int16_t   _z, int16_t  _w);
    static void Vertex(int32_t    _x, int32_t   _y, int32_t   _z, int32_t  _w);
    static void Vertex(float      _x, float     _y, float     _z, float  _w);
    static void Vertex(double     _x, double    _y, double    _z, double _w);

    /*    DESCRIPTION: Sets the draw Color for the current Vertex   */
    static void Color(int8_t     _x, int8_t    _y, int8_t    _z);
    static void Color(int16_t    _x, int16_t   _y, int16_t   _z);
    static void Color(int32_t    _x, int32_t   _y, int32_t   _z);
    static void Color(double     _x, double    _y, double    _z);
    static void Color(float      _x, float     _y, float     _z);
    static void Color(uint8_t   _x, uint8_t   _y, uint8_t   _z);
    static void Color(uint16_t  _x, uint16_t  _y, uint16_t  _z);
    static void Color(uint32_t  _x, uint32_t  _y, uint32_t  _z);
      
    static void Color(int8_t     _x, int8_t    _y, int8_t    _z, int8_t   _w);
    static void Color(int16_t    _x, int16_t   _y, int16_t   _z, int16_t  _w);
    static void Color(int32_t    _x, int32_t   _y, int32_t   _z, int32_t  _w);
    static void Color(double     _x, double    _y, double    _z, double  _w);
    static void Color(float      _x, float     _y, float     _z, float  _w);
    static void Color(uint8_t   _x, uint8_t   _y, uint8_t   _z, uint8_t  _w);
    static void Color(uint16_t  _x, uint16_t  _y, uint16_t  _z, uint16_t  _w);
    static void Color(uint32_t  _x, uint32_t  _y, uint32_t  _z, uint32_t  _w);

    /*   DESCRIPTION:: glNormal — set the current normal vector */
    static void Normal(uint8_t  _x, uint8_t  _y, uint8_t  _z);
    static void Normal(uint16_t _x, uint16_t _y, uint16_t _z);
    static void Normal(uint32_t _x, uint16_t _y, uint32_t _z);
    static void Normal(double   _x, double _y, double _z);
    static void Normal(float    _x, float  _y, float  _z);

    /*    DESCRIPTION: glTexCoord specifies texture coordinates in one, two, three, or four dimensions. */
    static void TexCoord(uint16_t _s);
    static void TexCoord(uint32_t _s);
    static void TexCoord(float    _s);
    static void TexCoord(double   _s);
    static void TexCoord(uint16_t _s, uint16_t _t);
    static void TexCoord(uint32_t _s, uint32_t _t);
    static void TexCoord(float    _s, float    _t);
    static void TexCoord(double   _s, double   _t);
    static void TexCoord(uint16_t _s, uint16_t _t, uint16_t _r);
    static void TexCoord(uint32_t _s, uint32_t _t, uint32_t _r);
    static void TexCoord(float    _s, float    _t, float    _r);
    static void TexCoord(double   _s, double   _t, double   _r);
    static void TexCoord(uint16_t _s, uint16_t _t, uint16_t _r, uint16_t _g);
    static void TexCoord(uint32_t _s, uint32_t _t, uint32_t _r, uint32_t _g);
    static void TexCoord(float    _s, float    _t, float    _r, float    _g);
    static void TexCoord(double   _s, double   _t, double   _r, double   _g);

    /*   DESCRIPTION: glSecondaryColor — set the current secondary color     */
    static void SecondaryColor(int8_t    _r, int8_t  _g, int8_t  _b);
    static void SecondaryColor(int16_t   _r, int16_t _g, int16_t _b);
    static void SecondaryColor(int32_t   _r, int32_t _g, int32_t _b);
    static void SecondaryColor(double     _r, double   _g, double   _b);
    static void SecondaryColor(float      _r, float    _g, float    _b);
    static void SecondaryColor(uint8_t  _r, uint8_t  _g, uint8_t  _b);
    static void SecondaryColor(uint16_t _r, uint16_t _g, uint16_t _b);
    static void SecondaryColor(uint32_t _r, uint32_t _g, uint32_t _b);

    /*  DESCRIPTION: glMultiTexCoord specifies texture coordinates in one, two, three, or four dimensions. */
    static void MultiTexCoord(uint16_t _s);
    static void MultiTexCoord(uint32_t _s);
    static void MultiTexCoord(float    _s);
    static void MultiTexCoord(double   _s);
    static void MultiTexCoord(uint16_t _s, uint16_t _t);
    static void MultiTexCoord(uint32_t _s, uint32_t _t);
    static void MultiTexCoord(float    _s, float    _t);
    static void MultiTexCoord(double   _s, double   _t);
    static void MultiTexCoord(uint16_t _s, uint16_t _t, uint16_t _r);
    static void MultiTexCoord(uint32_t _s, uint32_t _t, uint32_t _r);
    static void MultiTexCoord(float    _s, float    _t, float    _r);
    static void MultiTexCoord(double   _s, double   _t, double   _r);
    static void MultiTexCoord(uint16_t _s, uint16_t _t, uint16_t _r, uint16_t _g);
    static void MultiTexCoord(uint32_t _s, uint32_t _t, uint32_t _r, uint32_t _g);
    static void MultiTexCoord(float    _s, float    _t, float    _r, float    _g);
    static void MultiTexCoord(double   _s, double   _t, double   _r, double   _g);

    /*   DESCRIPTION:: glFogCoord — set the current fog coordinates    */
    static void FogCoord(double coord);
    static void FogCoord(float coord);

    /*   DESCRIPTION:
       - The glVertexAttrib family of entry points allows an application to pass generic vertex attributes in numbered locations.*/
    static void VertexAttrib(uint32_t _index, float _v);  
    static void VertexAttrib(uint32_t _index, uint16_t _v);
    static void VertexAttrib(double _index, double _v);   
    static void VertexAttrib(double _index, int _v); 
    static void VertexAttrib(double _index, uint32_t _v);
      
    static void VertexAttrib(uint32_t _index, float _v1, float _v2);
    static void VertexAttrib(uint32_t _index, uint16_t _v1, uint16_t _v2);
    static void VertexAttrib(uint32_t _index, double _v1, double _v2);
    static void VertexAttrib(uint32_t _index, int _v1, int _v2);
    static void VertexAttrib(uint32_t _index, uint32_t _v1, uint32_t _v2);
      
    static void VertexAttrib(uint32_t _index, float    _v1, float    _v2,    float _v3);
    static void VertexAttrib(uint32_t _index, uint16_t _v1, uint16_t _v2, uint16_t _v3);
    static void VertexAttrib(uint32_t _index, double   _v1, double   _v2,   double _v3);
    static void VertexAttrib(uint32_t _index, int      _v1, int      _v2,      int _v3);
    static void VertexAttrib(uint32_t _index, uint32_t _v1, uint32_t _v2, uint32_t _v3);
      
    static void VertexAttrib(uint32_t _index, float    _v1, float    _v2, float    _v3, float    _v4);
    static void VertexAttrib(uint32_t _index, uint16_t _v1, uint16_t _v2, uint16_t _v3, uint16_t _v4);
    static void VertexAttrib(uint32_t _index, double   _v1, double   _v2, double   _v3, double   _v4);
    static void VertexAttrib(uint32_t _index, int      _v1, int      _v2, int      _v3, int      _v4);
    static void VertexAttrib(uint32_t _index, uint32_t _v1, uint32_t _v2, uint32_t _v3, uint32_t _v4);

    /*  DESCRIPTION: The commands containing N indicate that the arguments will be passed as fixed-point values that are scaled to a normalized range according to the component conversion rules defined by the OpenGL specification.
    Signed values are understood to represent fixed-point values in the range [-1,1], and unsigned values are understood to represent fixed-point values in the range [0,1].*/
    static void VertexAttribNormalized(uint32_t index, int8_t   _v1, int8_t   _v2, int8_t   _v3, int8_t   _v4);
    static void VertexAttribNormalized(uint32_t index, int16_t  _v1, int16_t  _v2, int16_t  _v3, int16_t  _v4);
    static void VertexAttribNormalized(uint32_t index, int32_t  _v1, int32_t  _v2, int32_t  _v3, int32_t  _v4);
    static void VertexAttribNormalized(uint32_t index, uint8_t  _v1, uint8_t   _v2,  uint8_t   _v3, uint8_t   _v4);
    static void VertexAttribNormalized(uint32_t index, uint16_t _v1, uint16_t  _v2, uint16_t   _v3, uint16_t  _v4);
    static void VertexAttribNormalized(uint32_t index, uint32_t _v1, uint32_t  _v2, uint32_t   _v3, uint32_t  _v4);

    /*  DESCRIPTION: evaluates enabled one-dimensional maps at argument u. glEvalCoord2 does the same for two-dimensional maps using two domain values, u and v.
    To define a map, call glMap1 and glMap2; to enable and disable it, call glEnable and glDisable.     */
    static void glEvalCoord(float  _coord);
    static void glEvalCoord(double _coord);
    static void glEvalCoord(float  _cx, float _cy);
    static void glEvalCoord(double _cx, double _cy);

    /*  DESCRIPTION:    Alters the Material being rendered. Perhaps should determine the shader I end up using    */
    static void Material(uint32_t _face, uint32_t _name, float _param);
    static void Material(uint32_t _face, uint32_t _name, int _param);

    /* Set the Edge flag to nonboundary or boundary */
    static void EdgeFlag(bool _flag);

private:
    // static uint32_t active_VBO;
    // static uint32_t active_IBO;
    static std::string V;
    static std::string F;

    static uint32_t VAO;
      
    static uint32_t  Matrix_Mode;
    static uint32_t  Primative_Type;
    static uint32_t  Active_TextureUnit;
    static Shader   *Active_Program;
    static Mat4     *Active_Matrix;
    static Mat4      Active_ModelView;
    static Mat4      Active_Projection;
      
    static std::vector<Mat4> ModelViewMatrix_Stack;
    static std::vector<Mat4> ProjectionMatrix_Stack;
      
    static uint32_t          pointer_Index;
    static Attribute_Pointer pointer_Vertex;
    static Attribute_Pointer pointer_Color;
    static Attribute_Pointer pointer_Normal;
    static Attribute_Pointer pointer_TexCoord[8];
    static Attribute_Pointer pointer_FogCoord;
    static Attribute_Pointer pointer_SecondaryColor;
    static Attribute_Pointer pointer_EdgeFlag;
      
    static std::vector<uint32_t> Indices;
    static std::vector<float>    Vertices;
    static std::vector<float>    Colors;
    static std::vector<float>    Normals;
    static std::vector<float>    TexCoords;
      
    static Vec4 color_Secondary;
      
    static uint32_t state_Error;
    static uint32_t current_Index;// Current Vertex we are working with
    static bool flag_Edge;
    static bool flag_BeginEnd; // Set to tell us we are between a Begin() / End() block
    static bool flag_DrawArrays;
    static bool flag_Error;// For future iterations in which we check the state in Debug mode and react accordingly
    static uint32_t flag_Configuration;


    GL() = default;
};






/* 
SUBJECT: current_Index and ArrayIndex() Function
NOTE: 
 Currently all Elements are placed at the end of their respective Arrays. However ArrayIndex should specify the location of the current Element we wish to either Render or Modify
 As such it would be prudent to use a variable called current_Index which is incremented at every emplace for every element. This however is problematic as this entire experiment
 already has a tons of flaws that resulted in the Fixed Function pipeline being removed in the first place and performance would likely significantly suffer if it was done like this. 

 Perhaps I should totally remove the std::vectors in favor of C style arrays after all this is what OpenGL is built on and performance would likely increase. If specific Memory was 
 Allocated at the start of the program it could all but stop Allocations at run time which hinder performance doing it this way
*/


/*
SUBJECT: Call List and batched rendering
NOTE:
    The Fixed function also provides functionality for Call list which are essentially a bunch of commands that are executed in sequence when called upon. 
    I could perhaps emulate the same system I setup for my Threadpool in which I used a polymorphic Executor class to act as an interface allowing me to make
    queues of various C++ commands. 
    Alternatives would be a huge switch statement and a variable type Tuple to store all the arguments. This solution seems sub par at best.
*/

/*
SUBJECT: Sanity Checks
NOTE:
    If any other GL command is executed between Begin() and End() an error flag should be set and the command is ignored.
    This can be setup so that it only happens in Debug mode allowing the user better information as to what is taking place
    inside the GL state. Currently it is a black box in normal OpenGL however setting up a system such as this, while likely 
    a bit more complex would provide itself many benefits along the way.


*/
//Also, it is acceptable to use glCallList or glCallLists to execute display lists that include only the preceding commands.



/* LINKS: 

 OpenGL Programming/GLStart/Tut3:
 - https://en.wikibooks.org/wiki/OpenGL_Programming/GLStart/Tut3
*/

/*
SUBJECT:  Vulkan Based Game Engine
NOTE:
 - https://github.com/begla/Intrinsic
*/


#pragma warning( pop )










// Attribute_Pointer(const char *_name, uint32_t _type, uint32_t _stride, void *_ptr)
//     :
//     Size{ 0 },
//     Type{ _type },
//     Name{ _name },
//     Pointer{ _ptr },
//     Stride{ _stride }, 
//     GL_Handle{OpenGL::new_VBO()}
// {}
// Attribute_Pointer(const char *_name, uint32_t _size, uint32_t _type, uint32_t _stride, void *_ptr)
//     :
//     Size{ 0 },
//     Type{ _type },
//     Name{ _name },
//     Stride{ _stride },
//     Pointer{ _ptr },
//     GL_Handle{ OpenGL::new_VBO() }
// {}
/* glBindAttribLocation calls can be done before Linking the Shader program and it sets the Attribute Locations in stone */
///   enum Attribute_Locations
///   {// Are we going to use this? Should we use this? 
///       attrib_Vertices,  attrib_Normals,  attrib_Colors,  attrib_TexCoords,  attrib_EdgeFlags,  attrib_SecondaryColors,  attrib_FogCoords
///   };
//struct Texture_Pointer
//{
//    using Coord_t = Vec2;
//    //GL_TEXTURE0
//    void glTexCoordPointer(GLint size,
//        GLenum type,
//        GLsizei stride,
//        const void * pointer);
//
//    uint32_t Slot{ 0 };
//    static float *UV_pointer[];
//};
//0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,
// struct MyVertex
// {
//     float x, y, z;
//     float nx, ny, nz;
//     float s0, t0;
// };
///   SHOULD I HAVE BEHAVIOR FOR THIS DIFFERENT THAN THE OTHER ATTRIBUTES OR IGNORE IT ALTOGETHER
///   case GL_INDEX_ARRAY            : { pointer_Index.Enable();          }break;
///   case GL_INDEX_ARRAY           : { pointer_Index.Disable();          }break;

