#include"Fixed_Func.h"
#pragma warning(push)

#pragma warning ( disable : 4267)
#pragma warning ( disable : 4244)

// _static uint32_t GL::active_VBO{ 0 };
// _static uint32_t GL::active_IBO{ 0 };
_static uint32_t               GL::VAO                         { 0 };
_static Shader                *GL::Active_Program           { nullptr };
_static uint32_t               GL::Matrix_Mode                 { 0 };
_static uint32_t               GL::Active_TextureUnit          { 0 };
_static uint32_t               GL::Primative_Type         { GL_TRIANGLES };
_static Mat4                   GL::Active_ModelView            { 0 };
_static Mat4                   GL::Active_Projection           { 0 };
_static Mat4                  *GL::Active_Matrix            { nullptr };
                              
_static std::vector<Mat4>      GL::ModelViewMatrix_Stack;
_static std::vector<Mat4>      GL::ProjectionMatrix_Stack;


_static uint32_t               GL::pointer_Index;
_static GL::Attribute_Pointer  GL::pointer_Color;
_static GL::Attribute_Pointer  GL::pointer_Vertex;
_static GL::Attribute_Pointer  GL::pointer_Normal;
_static GL::Attribute_Pointer  GL::pointer_FogCoord;
_static GL::Attribute_Pointer  GL::pointer_TexCoord[8];
_static GL::Attribute_Pointer  GL::pointer_SecondaryColor;
_static GL::Attribute_Pointer  GL::pointer_EdgeFlag;
                               
                               
_static std::vector<float>     GL::Normals                      {};
_static std::vector<float>     GL::Colors                       {};
_static std::vector<float>     GL::Vertices                     {};
_static std::vector<float>     GL::TexCoords                    {};
_static std::vector<uint32_t>  GL::Indices                      {};

_static Vec4                   GL::color_Secondary  { 1.0f , 0.0f, 0.0, 1.0f };
_static uint32_t               GL::current_Index               { 0 };// Current Vertex we are working with

_static bool                   GL::flag_Edge                 { false };
_static bool                   GL::flag_BeginEnd             { false }; // Set to tell us we are between a Begin() / End() block
_static bool                   GL::flag_DrawArrays           { false };
_static bool                   GL::flag_Error                { false };// For future iterations in which we check the state in Debug mode and react accordingly
                                                           
_static uint32_t               GL::state_Error                 { 0 };
_static uint32_t               GL::flag_Configuration          { 0 };
_static GL::Bit_Flags          GL::State_Flags                 { 0 };


_static std::string GL::V =
"#version 330 core                                  \n\
    layout(location = 0) in vec3 VertexAttrib;      \n\
    layout(location = 1) in vec4 ColorAttrib;       \n\
    layout(location = 2) in vec3 NormalAttrib;      \n\
    layout(location = 3) in vec2 TexCoordAttrib;    \n\
    uniform  int EdgeFlagAttrib;                    \n\
    uniform mat4 ModelViewProjectionMatrix;         \n\
    out vec4 FColor;                                \n\
    void main()                                     \n\
    {                                               \n\
        FColor = ColorAttrib;                       \n\
        gl_Position = ModelViewProjectionMatrix* vec4( VertexAttrib.x, VertexAttrib.y, VertexAttrib.z, 1.0); \n\
    }";

_static std::string GL::F =
"#version 330 core                      \n\
        in vec4 FColor;                 \n\
        out vec4 FragColor;             \n\
        void main()                     \n\
        {                               \n\
            FragColor = FColor;         \n\
        }";

// layout(location = 4) in vec3 FogCoordAttrib;        \n\
// layout(location = 5) in vec4 SecondaryColorAttrib;  \n\

//  "ColorAttrib"
//  "VertexAttrib"
//  "NormalAttrib"
//  "FogCoordAttrib"
//  "TexCoordAttrib"
//  "EdgeFlagAttrib"
//  "SecondaryColorAttrib"
/// Color_VBO = OpenGL::new_VBO();
/// Normal_VBO = OpenGL::new_VBO();
/// TexCoord_VBO = OpenGL::new_VBO();
/// Vertices_VBO = OpenGL::new_VBO();




#include<stdio.h>
std::ostream& operator<<(std::ostream& _s, const GL::Bit_Flags& _in)
{

    _s << _in.unused1;
    _s << _in.unused2;
    _s << _in.MULTISAMPLE_State;
    _s << _in.unused4;
    _s << _in.unused5;
    _s << _in.unused6;
    _s << _in.unused7;
    _s << _in.unused8;
    _s << _in.unused9;
    _s << _in.unused10;
    _s << _in.unused11;
    _s << _in.unused12;
    _s << _in.SCISSOR_State;
    _s << _in.TEXTURE_State;
    _s << _in.LIST_State;
    _s << _in.EVAL_State;
    _s << _in.HINT_State;
    _s << _in.COLOR_BUFFE_State;
    _s << _in.ENABLE_State;
    _s << _in.TRANSFORM_State;
    _s << _in.VIEWPORT_State;

    return _s;
}




_static void GL::Initialize(uint32_t _config)
{
    Print("Flags Size:  " << sizeof(State_Flags) << " bytes");
    //uint32_t A = (uint32_t)State_Flags;
   // Print("Flag Value: " << _);
   // State_Flags.
  //  Print("Flag Value: " << State_Flags);
    Print("State Flags: " << State_Flags);

    /* Create the Default Matrices */
    ModelViewMatrix_Stack.push_back(Mat4(1.0f));
    ProjectionMatrix_Stack.push_back(Mat4(1.0f));

    Active_ModelView = ModelViewMatrix_Stack.back();
    Active_Projection = ProjectionMatrix_Stack.back();

    /* Create Default Shader: NOTE: Materials will allow changing of this */
    Active_Program = new Shader(V, F);

    /* Create our Vertex Array Layout */
    VAO = OpenGL::new_VAO();
    OpenGL::bind_VAO(VAO);

    // We are required not to have data in Vector to set Buffer
    Vertices.push_back(1); Colors.push_back(1);

    Active_Program->Bind();
    {
//OpenGL::bind_VBO(Vertices_VBO);
//OpenGL::set_BufferData(Vertices);
//OpenGL::set_Attribute(3, "VertexAttrib");
//
//OpenGL::bind_VBO(Color_VBO);
//OpenGL::set_BufferData(Colors);
//OpenGL::set_Attribute(4, "ColorAttrib");
        pointer_Vertex = Attribute_Pointer("VertexAttrib", 3);
        pointer_Vertex.set_Data(0, Vertices.data());

        pointer_Color = Attribute_Pointer("ColorAttrib", 4);
        pointer_Color.set_Data(0, Colors.data());
    }
    Active_Program->Unbind();
    Vertices.clear(); Colors.clear();



//    pointer_Index = OpenGL::new_IBO();
//pointer_Normal          = Attribute_Pointer("NormalAttrib",3);
//pointer_FogCoord        = Attribute_Pointer("FogCoordAttrib",3);
//pointer_TexCoord        = Attribute_Pointer("TexCoordAttrib",2);
//pointer_EdgeFlag        = Attribute_Pointer("EdgeFlagAttrib");
//pointer_SecondaryColor  = Attribute_Pointer("SecondaryColorAttrib");


}






void GL::Begin(uint32_t _primativeType)
{
    Vertices.clear();
    Colors.clear();

    flag_BeginEnd = true;
    Primative_Type = _primativeType;

    OpenGL::bind_VAO(VAO);
//    OpenGL::bind_VBO(Vertices_VBO);
    DEBUG_CODE(CheckGLERROR());
}
void GL::End()
{
    Active_Program->Bind();
    {
        OpenGL::bind_VAO(VAO);
        {
            OpenGL::bind_VBO(pointer_Vertex.GL_Handle);
            OpenGL::set_BufferData(Vertices);

            OpenGL::bind_VBO(pointer_Color.GL_Handle);
            OpenGL::set_BufferData(Colors);

            Active_Program->SetUniform("ModelViewProjectionMatrix", Active_Projection * Active_ModelView);
            glDrawArrays(Primative_Type, 0, Vertices.size() / 3);
            DEBUG_CODE(CheckGLERROR());
            glFlush();

            flag_BeginEnd = false;
        }
        OpenGL::unbind_VAO();
    }
    Active_Program->Unbind();
}

/*   DESCRIPTION :  glDrawArrays specifies multiple geometric primitives with very few subroutine call
     - Parameters:
     --  mode:   Specifies what kind of primitives to render.Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_QUAD_STRIP, GL_QUADS, and GL_POLYGON are accepted.
     --  first:  Specifies the starting index in the enabled arrays.
     --  count:  Specifies the number of indices to be rendered.*/
void GL::DrawArray(uint32_t  _mode, int32_t _first, uint32_t _count)
{
    __debugbreak();
    OpenGL::set_Shader(Active_Program->g_Handle());
//    OpenGL::bind_VBO(VBOs[active_VBO++]);
    glDrawArrays(_mode, _first, _count);
}

/*  DESCRIPTION:  ArrayElement commands are used within Begin()/End() pairs to specify vertex and attribute data for point, line, and polygon primitives.
    - If GL_VERTEX_ARRAY is enabled when glArrayElement is called, a single vertex is drawn, using vertex and attribute data taken from location i of the enabled arrays.
    - If GL_VERTEX_ARRAY is not enabled, no drawing occurs but the attributes corresponding to the enabled arrays are modified.
*/
void GL::ArrayElement(uint32_t _index)
{
    current_Index = _index;
}

void GL::Rotate(float _angle, float _x, float _y, float _z)
{    /// Man, Comparison IFs would potentially be faster than all these damn dereferences every split second. All Cache misses unless Compiler is doing some slick shit

    *Active_Matrix = glm::rotate(*Active_Matrix, (float)_angle, Vec3(_x, _y, _z));
}
void GL::Rotate(double _angle, float _x, float _y, float _z) 
{
   *Active_Matrix = glm::rotate(*Active_Matrix, (float)_angle, Vec3(_x, _y, _z));
}
void GL::Translate(float _x, float _y, float _z)
{
   *Active_Matrix = glm::translate(*Active_Matrix, Vec3(_x, _y, _z));
}
void GL::MatrixMode(uint32_t _matMode)
{
    /// Assert that it is GL_PROJECTION, MODELVIEW, COLOR or Texture here
    Matrix_Mode = _matMode;
    if (Matrix_Mode == GL_PROJECTION)
    {
        Active_Matrix = &Active_Projection;
    }
    if (Matrix_Mode == GL_MODELVIEW)
    {
        Active_Matrix = &Active_ModelView;
    }
}
void GL::LoadIdentity()
{
    *Active_Matrix = Mat4(1.0f);
}

void GL::Ortho(double left, double right, double bottom, double top, double nearVal, double farVal)
{
    Mat4 Orth = glm::ortho(left, right, bottom, top, nearVal, farVal);
    *Active_Matrix = *Active_Matrix * Orth;
}
void GL::Perspective(double fovy, double aspect, double zNear, double zFar)
{
    Active_Projection = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
}








/*  DESCRIPTION: Pushes the current Active Matrix to the stack*/
void GL::PushMatrix()
{
    if (Matrix_Mode = GL_PROJECTION)
    {
        ProjectionMatrix_Stack.emplace_back(*Active_Matrix);
    }
    if (Matrix_Mode == GL_MODELVIEW)
    {
        ModelViewMatrix_Stack.emplace_back(*Active_Matrix);
    }
}
/*  DESCRIPTION: Pops the current Active Matrix to the stack*/
void GL::PopMatrix()
{
    if (Matrix_Mode == GL_PROJECTION)
    {
        *Active_Matrix = ProjectionMatrix_Stack.back();
        ProjectionMatrix_Stack.pop_back();
    }
    if (Matrix_Mode == GL_MODELVIEW)
    {
        *Active_Matrix = ModelViewMatrix_Stack.back();
        ModelViewMatrix_Stack.pop_back();
    }
}


void GL::Viewport(int32_t _x, int32_t _y, int32_t _w, int32_t _h)
{
    glViewport(_x, _y, (GLsizei)_w, (GLsizei)_h);
}
void GL::ClearColor(float _r, float _g, float _b, float _a)
{
    glClearColor(_r, _g, _b, _a);
}
void GL::ClearColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
{
    glClearColor((float)(_r / 255), (float)(_g / 255), (float)(_b / 255), (float)(_a / 255));
}
void GL::Clear()
{ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}  


/*  DESCRIPTION: Specify the Indices and type */
void  GL::Index(uint8_t _indice)  { Indices.emplace_back(_indice); }
void  GL::Index(uint16_t _indice) { Indices.emplace_back(_indice); }
void  GL::Index(uint32_t _indice) { Indices.emplace_back(_indice); }
void  GL::Index(float _indice)    { Indices.emplace_back(_indice); }
void  GL::Index(double _indice)   { Indices.emplace_back(_indice); }

/*    DESCRIPTION: Sets the draw Color for the current Vertex   */
void  GL::Vertex(int16_t    _x, int16_t   _y) {Vertices.emplace_back(_x);  Vertices.emplace_back(_y);  }
void  GL::Vertex(int32_t    _x, int32_t   _y) {Vertices.emplace_back(_x);  Vertices.emplace_back(_y);  }
void  GL::Vertex(float      _x, float     _y) {Vertices.emplace_back(_x);  Vertices.emplace_back(_y);  }
void  GL::Vertex(double     _x, double    _y) {Vertices.emplace_back(_x);  Vertices.emplace_back(_y);  }

void  GL::Vertex(int16_t    _x, int16_t   _y, int16_t   _z) { Vertices.emplace_back(_x);  Vertices.emplace_back(_y); Vertices.emplace_back(_z);  }
void  GL::Vertex(int32_t    _x, int32_t   _y, int32_t   _z) { Vertices.emplace_back(_x);  Vertices.emplace_back(_y); Vertices.emplace_back(_z);  }
void  GL::Vertex(float      _x, float     _y, float     _z) { Vertices.emplace_back(_x);  Vertices.emplace_back(_y); Vertices.emplace_back(_z);  }
void  GL::Vertex(double     _x, double    _y, double    _z) { Vertices.emplace_back(_x);  Vertices.emplace_back(_y); Vertices.emplace_back(_z);  }

void  GL::Vertex(int16_t    _x, int16_t   _y, int16_t   _z, int16_t  _w) { Vertices.emplace_back(_x);  Vertices.emplace_back(_y); Vertices.emplace_back(_z); Vertices.emplace_back(_w);   }
void  GL::Vertex(int32_t    _x, int32_t   _y, int32_t   _z, int32_t  _w) { Vertices.emplace_back(_x);  Vertices.emplace_back(_y); Vertices.emplace_back(_z); Vertices.emplace_back(_w);   }
void  GL::Vertex(float      _x, float     _y, float     _z, float  _w)   { Vertices.emplace_back(_x);  Vertices.emplace_back(_y); Vertices.emplace_back(_z); Vertices.emplace_back(_w);   }
void  GL::Vertex(double     _x, double    _y, double    _z, double _w)   { Vertices.emplace_back(_x);  Vertices.emplace_back(_y); Vertices.emplace_back(_z); Vertices.emplace_back(_w);   }



/*    DESCRIPTION: Sets the draw Color for the current Vertex   */
void  GL::Color(int8_t     _r, int8_t    _g, int8_t    _b)               { Colors.emplace_back(_r / SCHAR_MAX);  Colors.emplace_back(_g/ SCHAR_MAX); Colors.emplace_back(_b/ SCHAR_MAX); Colors.emplace_back(1.0f);}
void  GL::Color(int16_t    _r, int16_t   _g, int16_t   _b)               { Colors.emplace_back(_r / SHRT_MAX);  Colors.emplace_back(_g / SHRT_MAX); Colors.emplace_back(_b / SHRT_MAX); Colors.emplace_back(1.0f);}
void  GL::Color(int32_t    _r, int32_t   _g, int32_t   _b)               { Colors.emplace_back(_r / INT_MAX);  Colors.emplace_back(_g / INT_MAX); Colors.emplace_back(_b / INT_MAX); Colors.emplace_back(1.0f);}
void  GL::Color(double     _r, double    _g, double    _b)               { Colors.emplace_back((float)_r);  Colors.emplace_back((float)_g); Colors.emplace_back((float)_b); Colors.emplace_back(1.0f);}
void  GL::Color(float      _r, float     _g, float     _b)               { Colors.emplace_back(_r);  Colors.emplace_back(_g); Colors.emplace_back(_b); Colors.emplace_back(1.0f);}
void  GL::Color(uint8_t    _r, uint8_t   _g, uint8_t   _b)               { Colors.emplace_back(_r / 255);  Colors.emplace_back(_g / 255); Colors.emplace_back(_b / 255); Colors.emplace_back(1.0f);}
void  GL::Color(uint16_t   _r, uint16_t  _g, uint16_t  _b)               { Colors.emplace_back(_r / USHRT_MAX);  Colors.emplace_back(_g / USHRT_MAX); Colors.emplace_back(_b / USHRT_MAX); Colors.emplace_back(1.0f);}
void  GL::Color(uint32_t   _r, uint32_t  _g, uint32_t  _b)               { Colors.emplace_back(_r / UINT_MAX);  Colors.emplace_back(_g / UINT_MAX); Colors.emplace_back(_b / UINT_MAX); Colors.emplace_back(1.0f);}
void  GL::Color(int8_t     _r, int8_t    _g, int8_t    _b, int8_t   _w)  { Colors.emplace_back(_r / SCHAR_MAX);  Colors.emplace_back(_g / SCHAR_MAX); Colors.emplace_back(_b / SCHAR_MAX); Colors.emplace_back(_w / SCHAR_MAX);}
void  GL::Color(int16_t    _r, int16_t   _g, int16_t   _b, int16_t  _w)  { Colors.emplace_back(_r / SHRT_MAX);  Colors.emplace_back(_g / SHRT_MAX); Colors.emplace_back(_b / SHRT_MAX); Colors.emplace_back(_w / SHRT_MAX);}
void  GL::Color(int32_t    _r, int32_t   _g, int32_t   _b, int32_t  _w)  { Colors.emplace_back(_r / INT_MAX);  Colors.emplace_back(_g / INT_MAX); Colors.emplace_back(_b / INT_MAX); Colors.emplace_back(_w / INT_MAX);}
void  GL::Color(double     _r, double    _g, double    _b, double   _w)  { Colors.emplace_back(_r);  Colors.emplace_back((float)_g); Colors.emplace_back((float)_b); Colors.emplace_back((float)_w);}
void  GL::Color(float      _r, float     _g, float     _b, float    _w)  { Colors.emplace_back(_r);  Colors.emplace_back(_g); Colors.emplace_back(_b); Colors.emplace_back(_w);}
void  GL::Color(uint8_t    _r, uint8_t   _g, uint8_t   _b, uint8_t  _w)  { Colors.emplace_back(_r / 255);  Colors.emplace_back(_g / 255); Colors.emplace_back(_b / 255); Colors.emplace_back(_w / 255);}
void  GL::Color(uint16_t   _r, uint16_t  _g, uint16_t  _b, uint16_t _w)  { Colors.emplace_back(_r / USHRT_MAX);  Colors.emplace_back(_g / USHRT_MAX); Colors.emplace_back(_b / USHRT_MAX); Colors.emplace_back(_w / USHRT_MAX);}
void  GL::Color(uint32_t   _r, uint32_t  _g, uint32_t  _b, uint32_t  _w) { Colors.emplace_back(_r / UINT_MAX);  Colors.emplace_back(_g / UINT_MAX); Colors.emplace_back(_b / UINT_MAX); Colors.emplace_back(_w / UINT_MAX);}


/*   DESCRIPTION:: glNormal — set the current normal vector */
void  GL::Normal(uint8_t  _x, uint8_t  _y, uint8_t  _z) {  Normals.emplace_back(_x); Normals.emplace_back(_y);Normals.emplace_back(_z); }
void  GL::Normal(uint16_t _x, uint16_t _y, uint16_t _z) {  Normals.emplace_back(_x); Normals.emplace_back(_y);Normals.emplace_back(_z); }
void  GL::Normal(uint32_t _x, uint16_t _y, uint32_t _z) {  Normals.emplace_back(_x); Normals.emplace_back(_y);Normals.emplace_back(_z); }
void  GL::Normal(double   _x, double _y, double _z)     {  Normals.emplace_back(_x); Normals.emplace_back(_y);Normals.emplace_back(_z); }
void  GL::Normal(float    _x, float  _y, float  _z)     {  Normals.emplace_back(_x); Normals.emplace_back(_y);Normals.emplace_back(_z); }

/*    DESCRIPTION: glTexCoord specifies texture coordinates in one, two, three, or four dimensions. */
void  GL::TexCoord(uint16_t _s) {TexCoords.emplace_back(_s);  }
void  GL::TexCoord(uint32_t _s) {TexCoords.emplace_back(_s);  }
void  GL::TexCoord(float    _s) {TexCoords.emplace_back(_s);  }
void  GL::TexCoord(double   _s) {TexCoords.emplace_back(_s);  }
void  GL::TexCoord(uint16_t _s, uint16_t _t) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  }
void  GL::TexCoord(uint32_t _s, uint32_t _t) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  }
void  GL::TexCoord(float    _s, float    _t) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  }
void  GL::TexCoord(double   _s, double   _t) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  }
void  GL::TexCoord(uint16_t _s, uint16_t _t, uint16_t _r) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  TexCoords.emplace_back(_r);  }
void  GL::TexCoord(uint32_t _s, uint32_t _t, uint32_t _r) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  TexCoords.emplace_back(_r);  }
void  GL::TexCoord(float    _s, float    _t, float    _r) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  TexCoords.emplace_back(_r);  }
void  GL::TexCoord(double   _s, double   _t, double   _r) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  TexCoords.emplace_back(_r);  }
void  GL::TexCoord(uint16_t _s, uint16_t _t, uint16_t _r, uint16_t _q) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  TexCoords.emplace_back(_r);  TexCoords.emplace_back(_q);   }
void  GL::TexCoord(uint32_t _s, uint32_t _t, uint32_t _r, uint32_t _q) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  TexCoords.emplace_back(_r);  TexCoords.emplace_back(_q);   }
void  GL::TexCoord(float    _s, float    _t, float    _r, float    _q) {TexCoords.emplace_back(_s);  TexCoords.emplace_back(_t);  TexCoords.emplace_back(_r);  TexCoords.emplace_back(_q);   }
void  GL::TexCoord(double   _s, double   _t, double   _r, double   _q) { TexCoords.emplace_back(_s); TexCoords.emplace_back(_t);  TexCoords.emplace_back(_r);  TexCoords.emplace_back(_q);   }

/*   DESCRIPTION: glSecondaryColor — set the current secondary color     */
void  GL::SecondaryColor(int8_t    _r, int8_t  _g, int8_t  _b) {}
void  GL::SecondaryColor(int16_t   _r, int16_t _g, int16_t _b) {}
void  GL::SecondaryColor(int32_t   _r, int32_t _g, int32_t _b) {}
void  GL::SecondaryColor(double     _r, double   _g, double   _b) {}
void  GL::SecondaryColor(float      _r, float    _g, float    _b) {}
void  GL::SecondaryColor(uint8_t  _r, uint8_t  _g, uint8_t  _b) {}
void  GL::SecondaryColor(uint16_t _r, uint16_t _g, uint16_t _b) {}
void  GL::SecondaryColor(uint32_t _r, uint32_t _g, uint32_t _b) {}


/*  DESCRIPTION: glMultiTexCoord specifies texture coordinates in one, two, three, or four dimensions. */
void  GL::MultiTexCoord(uint16_t _s) {}
void  GL::MultiTexCoord(uint32_t _s) {}
void  GL::MultiTexCoord(float    _s) {}
void  GL::MultiTexCoord(double   _s) {}
void  GL::MultiTexCoord(uint16_t _s, uint16_t _t) {}
void  GL::MultiTexCoord(uint32_t _s, uint32_t _t) {}
void  GL::MultiTexCoord(float    _s, float    _t) {}
void  GL::MultiTexCoord(double   _s, double   _t) {}
void  GL::MultiTexCoord(uint16_t _s, uint16_t _t, uint16_t _r) {}
void  GL::MultiTexCoord(uint32_t _s, uint32_t _t, uint32_t _r) {}
void  GL::MultiTexCoord(float    _s, float    _t, float    _r) {}
void  GL::MultiTexCoord(double   _s, double   _t, double   _r) {}
void  GL::MultiTexCoord(uint16_t _s, uint16_t _t, uint16_t _r, uint16_t _g) {}
void  GL::MultiTexCoord(uint32_t _s, uint32_t _t, uint32_t _r, uint32_t _g) {}
void  GL::MultiTexCoord(float    _s, float    _t, float    _r, float    _g) {}
void  GL::MultiTexCoord(double   _s, double   _t, double   _r, double   _g) {}


/*   DESCRIPTION:: glFogCoord — set the current fog coordinates    */
void  GL::FogCoord(double coord) {}
void  GL::FogCoord(float coord) {}


/*   DESCRIPTION:
   - The glVertexAttrib family of entry points allows an application to pass generic vertex attributes in numbered locations.*/
void  GL::VertexAttrib(uint32_t _index, float _v) {}
void  GL::VertexAttrib(uint32_t _index, uint16_t _v) {}
void  GL::VertexAttrib(double _index, double _v) {}
void  GL::VertexAttrib(double _index, int _v) {}
void  GL::VertexAttrib(double _index, uint32_t _v) {}

void  GL::VertexAttrib(uint32_t _index, float _v1, float _v2) {}
void  GL::VertexAttrib(uint32_t _index, uint16_t _v1, uint16_t _v2) {}
void  GL::VertexAttrib(uint32_t _index, double _v1, double _v2) {}
void  GL::VertexAttrib(uint32_t _index, int _v1, int _v2) {}
void  GL::VertexAttrib(uint32_t _index, uint32_t _v1, uint32_t _v2) {}

void  GL::VertexAttrib(uint32_t _index, float    _v1, float    _v2, float _v3) {}
void  GL::VertexAttrib(uint32_t _index, uint16_t _v1, uint16_t _v2, uint16_t _v3) {}
void  GL::VertexAttrib(uint32_t _index, double   _v1, double   _v2, double _v3) {}
void  GL::VertexAttrib(uint32_t _index, int      _v1, int      _v2, int _v3) {}
void  GL::VertexAttrib(uint32_t _index, uint32_t _v1, uint32_t _v2, uint32_t _v3) {}

void  GL::VertexAttrib(uint32_t _index, float    _v1, float    _v2, float    _v3, float    _v4) {}
void  GL::VertexAttrib(uint32_t _index, uint16_t _v1, uint16_t _v2, uint16_t _v3, uint16_t _v4) {}
void  GL::VertexAttrib(uint32_t _index, double   _v1, double   _v2, double   _v3, double   _v4) {}
void  GL::VertexAttrib(uint32_t _index, int      _v1, int      _v2, int      _v3, int      _v4) {}
void  GL::VertexAttrib(uint32_t _index, uint32_t _v1, uint32_t _v2, uint32_t _v3, uint32_t _v4) {}


/*  DESCRIPTION: The commands containing N indicate that the arguments will be passed as fixed-point values that are scaled to a normalized range according to the component conversion rules defined by the OpenGL specification.
Signed values are understood to represent fixed-point values in the range [-1,1], and unsigned values are understood to represent fixed-point values in the range [0,1].*/
void  GL::VertexAttribNormalized(uint32_t index, int8_t   _v1, int8_t   _v2, int8_t   _v3, int8_t   _v4) {}
void  GL::VertexAttribNormalized(uint32_t index, int16_t  _v1, int16_t  _v2, int16_t  _v3, int16_t  _v4) {}
void  GL::VertexAttribNormalized(uint32_t index, int32_t  _v1, int32_t  _v2, int32_t  _v3, int32_t  _v4) {}
void  GL::VertexAttribNormalized(uint32_t index, uint8_t  _v1, uint8_t   _v2, uint8_t   _v3, uint8_t   _v4) {}
void  GL::VertexAttribNormalized(uint32_t index, uint16_t _v1, uint16_t  _v2, uint16_t   _v3, uint16_t  _v4) {}
void  GL::VertexAttribNormalized(uint32_t index, uint32_t _v1, uint32_t  _v2, uint32_t   _v3, uint32_t  _v4) {}


/*  DESCRIPTION: evaluates enabled one-dimensional maps at argument u. glEvalCoord2 does the same for two-dimensional maps using two domain values, u and v.
To define a map, call glMap1 and glMap2; to enable and disable it, call glEnable and glDisable.     */
void  GL::glEvalCoord(float  _coord) {}
void  GL::glEvalCoord(double _coord) {}
void  GL::glEvalCoord(float  _cx, float _cy) {}
void  GL::glEvalCoord(double _cx, double _cy) {}

void  GL::Material(uint32_t _face, uint32_t _name, float _param)
{// Alters the Material being rendered. 
}
void  GL::Material(uint32_t _face, uint32_t _name, int _param)
{// Alters the Material being rendered.  
}

void  GL::EdgeFlag(bool _flag)
{// Set the Edge flag to nonboundary or boundary 
    flag_Edge = _flag;
}

void GL::ColorPointer(uint32_t _size, uint32_t _type, uint32_t _stride, void *_pointer)
{// void glColorPointer(GLint size,    GLenum type,    GLsizei stride,    const void * pointer);
//Type =  GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, and GL_DOUBLE 
    Active_Program->Bind();
    OpenGL::bind_VAO(VAO);
    pointer_Color.set_Data(_size, _type, _stride, _pointer);
    Active_Program->Unbind();

}
void GL::IndexPointer(uint32_t _type, uint32_t _stride, void *_pointer)
{
    OpenGL::bind_VAO(VAO);
    OpenGL::bind_IBO(pointer_Index);
}
void GL::VertexPointer(uint32_t _size, uint32_t _type, uint32_t _stride,  void *_pointer)
{
    Active_Program->Bind();
    OpenGL::bind_VAO(VAO);
    pointer_Vertex.set_Data(_size, _type, _stride, _pointer);
    Active_Program->Unbind();
}
void GL::NormalPointer(uint32_t _type, uint32_t _stride, void *_pointer)
{
    Active_Program->Bind();
    OpenGL::bind_VAO(VAO);
    pointer_Normal.set_Data(_type, _stride, _pointer);
    Active_Program->Unbind();
}
void GL::FogCoordPointer(uint32_t _type, uint32_t _stride, void *_pointer)
{
    Active_Program->Bind();
    OpenGL::bind_VAO(VAO);
    pointer_FogCoord.set_Data(_type, _stride, _pointer);
    Active_Program->Unbind();
}
void GL::TexCoordPointer(uint32_t _size, uint32_t _type, uint32_t _stride, void *_pointer)
{
    Active_Program->Bind();
    OpenGL::bind_VAO(VAO);
    pointer_TexCoord[Active_TextureUnit].set_Data(_size, _type, _stride, _pointer);
    Active_Program->Unbind();
}
void GL::SecondaryColorPointer(uint32_t _size, uint32_t _type, uint32_t _stride, void *_pointer)
{
    Active_Program->Bind();
    OpenGL::bind_VAO(VAO);
    pointer_SecondaryColor.set_Data(_size, _type, _stride, _pointer);
    Active_Program->Unbind();
}

void GL::EdgeFlagPointer(uint32_t _stride, void * pointer)
{
    Active_Program->Bind();
    OpenGL::bind_VAO(VAO);
    pointer_EdgeFlag.set_Data(0, 0);
    Active_Program->Unbind();
}



#pragma warning(pop)