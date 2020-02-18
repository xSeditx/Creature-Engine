#pragma once

#include"../Core/Common.h"
#include"../Renderer/LowLevel/Materials/Image/Texture.h"
#include"../Renderer/LowLevel/OpenGL/Shader/Shader.h"
#include"../Renderer/LowLevel/OpenGL/Camera/Camera2D.h"
#include"../Renderer/LowLevel/OpenGL/Renderer/Renderer.h"
#include"../Renderer/LowLevel/OpenGL/Renderer/Transform.h"
#include"../Profiling/Timing/Timer.h"
#include<atomic>

/*===================================================================*/
/* Union to specify an RGBA value and convert a DWORD into
/* RGBA and visa/versa */
/* ~* WARNING : Currently not working properly due to Alpha value *~ */
union Pixel
{// We want to access color of pixel either as RGBA values or 32 bit Color
    Pixel() = default;
    Pixel(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
        : r(_r), g(_g), b(_b), a(_a) {}

    Pixel(uint32_t _col)
        :Color(_col)
    {}

    struct
    {// Proper RGBA for GL_RGBA values. 
        uint8_t r, g, b, a;
    };
    uint32_t Color;
    operator uint32_t() { return Color; }
};
/*===================================================================*/

#include<cmath>

namespace Profiling
{

    /*===================================================================*/
    /* Window for Displaying various Profiling data Graphically          */
    /*===================================================================*/
    struct CREATURE_API DisplayWindow
    {
		Camera2D Camera;
		Transform Model;

        /* X Axis represents Value
           Y Axis represents Time  
           Optional RGB values for additional dimensions 
		   _position:  The Location on the Screen XY for the Window to start
		   _size:      The Size in ScreenXY to Display the Window
		   _dataRange: The Range of the values Displayed*/
        DisplayWindow(Vec2 _position, Vec2 _size, Vec2 _dataRange)
            :
            Size       (_size),
            Position   (_position),
            DataRange({ std::floor(_dataRange.x) ,std::floor(_dataRange.y) }),
            ReadBuffer ( new Pixel[static_cast<size_t>(_dataRange.x) * static_cast<size_t>(_dataRange.y)]),
            WriteBuffer( new Pixel[static_cast<size_t>(_dataRange.x) * static_cast<size_t>(_dataRange.y)]),
			Model      ({ Vec3(0.0f), Vec3(0.0f), "Model" }),
			Camera     (Camera2D(Vec2(640, 480 )))
        {
            TODO("Still much work to do here. Perhaps just store a list of values and make a line drawing routine to draw lines from one data point to the other.");

            High = Size.x;
			Xcoeff = 1.0f / _dataRange.x;
            QuadRenderer = Shader(vRenderer, fRenderer);
            QuadRenderer.Bind();       // Bind our Shader
			//Model.Rotate_Translate(Vec3(_position.x, _position.y, 0), Vec3(0, 0, 45));
			Model.Translate({ _position.x,_position.y, 0.0f });
			Model.RotateZ(-90.0f);


			clearRead();
			clearWrite();
            /* TODO: This should be a changable value so 
            /* multiple colors can be used */
            uint8_t R{ 100 }, G{ 0 }, B{ 0 };
            for_loop(y, _dataRange.y)
            {
                for_loop(x, _dataRange.x)
                {
                    WriteBuffer[x + (int)_dataRange.x * y] = Pixel(255, 255, 255, 100);
                    R += 5;
                    if (R >= 255) { G += 5; R = 0; }
                    if (G >= 255) { B += 5; G = 0; }

                }
            }


			// Figure out the Coefficients for the values displayed to the screen
            {
				Xcoeff = DataRange.x / High;//Fix this later to allow Negative Values + Low);
                Ycoeff = DataRange.y / TimeLength;
            }

            // Creates the display texture
            DisplayTexture = Graphics::Texture(*(new Graphics::Bitmap(ReadBuffer, _dataRange)));// We dereference the Memory Address where the object was constructed than we pass that into Texture for construction and this should give us in place construction of the returned texture???
            VAO = OpenGL::create_VAO();// Create an ID for our VAO
            OpenGL::bind_VAO(VAO);     // Enable our VAO

            // Create the Vertex Buffer Object containing the Vertices and UV Coords
            {
                Vec4 Vertices[6] =
                {   // positions   // texCoords
                    Vec4(0 ,         Size.y   ,    0.0f,  1.0f),
                    Vec4(0 ,        -Size.y   ,    0.0f,  0.0f),
                    Vec4( Size.x ,  -Size.y   ,    1.0f,  0.0f),
                    Vec4(0 ,         Size.y   ,    0.0f,  1.0f),
                    Vec4( Size.x ,  -Size.y   ,    1.0f,  0.0f),
                    Vec4( Size.x ,   Size.y   ,    1.0f,  1.0f)
                };

                VBO = OpenGL::create_VBO();
                OpenGL::bind_VBO(VBO);
                OpenGL::set_BufferData(sizeof(Vertices), &Vertices);
            }

            // Get the Attribute Location and Check for Errors
            {
                OpenGL::set_Attribute(QuadRenderer.g_ID(), 4, "aPos");
            }
        }

        /* Creates a Coefficient so that our Window has a Scale that is designated for it */
		void generateXCoeff()
		{
            TODO("Work on the XCoefficient on the Profiler Window ");
		    //    float xDiff{ 0 }, yDiff{ 0 };
		    //    Low > 0 ?
		    //    xDiff = High + std::abs(Low) :
		    //    xDiff = High - Low;
		}

        ~DisplayWindow()
        {
            delete(ReadBuffer);
            delete(WriteBuffer);
        }

        int // DEBUG VALUES
            Counter{ 0 },
            val = 0,
            rad = 0;
		size_t PreviousTime{ 0 };
		uint32_t PreviousX{ 0 };


        /*~===============================================~*/
        /*           Updates the Profiler Window           */
        /*       NEEDS TO HANDLE DELTATIME IN FUTURE       */
        void Update(uint32_t _value)
        {

            size_t NewTime = Timing::Timer<Milliseconds>::GetTime();
			size_t Time = NewTime - PreviousTime;
			if (Time > 100)
			{
				PreviousTime = NewTime; //Print("PreviousTime: " << Time);
                int DataPoint = static_cast<int>(_value);

                ClearLine();// Clear the new Line.
				
				int start = val < (int)PreviousX ? val : (int)PreviousX;
				int end = val > (int)PreviousX ? val : (int)PreviousX;

                uint32_t _color = Pixel(255, 255, 255, 155);

                if (val < (size_t)DataRange.x && val >= 0)
                {
                    memset(&ReadBuffer[(int)(Size.x/4)], _color, val+1);
                }
                setPixel(start , 0 , Pixel(255, 0, 255, 155));
			    PreviousX = val;

                float Y = (_value * (_value / DataRange.x)) * (1.0 / Size.x);
                val = std::log(_value) * Size.x / std::log(DataRange.x);
				swapBuffer();
                DisplayTexture.Update((uint8_t*)ReadBuffer);
            }
        }
        void Render()
        {
            QuadRenderer.Bind();
            {
                OpenGL::bind_VAO(VAO);
                {
                    OpenGL::ActivateTexture(0);
                    DisplayTexture.Bind();
                    QuadRenderer.SetUniform("texture1" , 0);
					Model.Bind();
					Camera.Bind();
					OpenGL::Renderer::drawArray(VBO, 6);
                }
                OpenGL::unbind_VAO();
            }
            QuadRenderer.Unbind();
        }
        /*~===============================================~*/


        /*~===============================================~*/
        /*    Returns size in bytes of Profiler Texture    */
        size_t size()
        {
            return (size_t)(DataRange.x * DataRange.y) * sizeof(Pixel);  
        }
        /* Sets a pixel in the Texture to the desired color */
		__forceinline void setPixel(uint32_t _x, uint32_t _y, Pixel _color) 
		{  
            if (_x > DataRange.x)
            {// If larger than the texture return
                return;
            }
			ReadBuffer[_x + (size_t)DataRange.x * _y] = _color;
		}
        /* Gets the Pixelat a specific location in the Texture */
        uint32_t getPixel(uint32_t _x, uint32_t _y) 
        {
            return ReadBuffer[_x + (size_t)DataRange.x * _y];         
        }

    private:

        /*~===============================================~*/
        /*  Data Members for our Profiler Display Window   */

        Graphics::Texture DisplayTexture;

        Pixel
			DefaultColor{ 255,255,255, 255 },
			* WriteBuffer{ nullptr },
			* ReadBuffer{ nullptr };

		Vec2
			Size{ 0,0 },
			Position{ 0,0 },
			DataRange{0,0};

        float 
            TimeLength{ 1.0f },
            Xcoeff, Ycoeff,
            Low{ 0 },
            High{ 1000 };
        /*~===============================================~*/




        /*~===============================================~*/
        /* A single unit of recorded data for the Profiler */
		template<typename _Ty>
		struct dataPoint
		{
			dataPoint(_Ty _val, size_t _time)
				:
				Value(_val),
				Time(_time)
			{}
			_Ty Value;
			size_t Time;
		};
		std::vector<dataPoint<uint32_t>> Record;
        Protected_Value<uint32_t> CurrentValue;
        /*~===============================================~*/



        /*~===============================================~*/
        /* State information for our Profiler window telling
        /* which data to record                            */
        struct
        {
            bool Border{ false };
            bool Record{ false };
        }Properties;
        /*~===============================================~*/

        GLuint 
            VAO{ 0 }, 
            VBO{ 0 },
            IBO{ 0 };
        Shader QuadRenderer;


        /*~===============================================~*/
        /*  Private Functions for Profiler Display Window  */

        /* Sets the First line to Zero clearing it out*/
        void ClearLine()
        {
            memset(ReadBuffer, Pixel(100, 100, 100, 100).Color, (size_t)(DataRange.x * sizeof(Pixel)));          // Clear the new Line.
        }
        /* Swaps the front and back buffer for the texture */
        void swapBuffer()
        {
            int LineSize = (sizeof(Pixel) * ((size_t)DataRange.x));
            memcpy((char*)ReadBuffer + (LineSize), (char*)WriteBuffer, size() - (LineSize));
            memcpy((char*)WriteBuffer, (char*)ReadBuffer, size());
        }
        /* Clears the Read Buffer */
        void clearRead()
        {
            memset(ReadBuffer, 0, size());
        }
        /* Clears the Write Buffer */
        void clearWrite()
        {
            memset(WriteBuffer, 0, size());
        }

        /*~===============================================~*/




        /*~===============================================~*/
        /* The Shaders used to render the Profiler Window  */
        /*~~~~* REPLACE THIS LATER WITH PROPER SHADERS *~~~*/
        /*~===============================================~*/
        std::string vRenderer =
            "#version 330 core \n\
layout(location = 0) in vec4 aPos;          \n\
uniform mat4 ProjectionMatrix;              \n\
uniform mat4 ViewMatrix;                    \n\
uniform mat4 Model;                         \n\
out vec3 vertexColor;                       \n\
out vec2 TexCoords;                         \n\
void main()                                 \n\
{                                           \n\
    mat4 ModelViewMatrix = (ViewMatrix * Model);  \n\
    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
    TexCoords = aPos.zw;                          \n\
    gl_Position = ModelViewProjectionMatrix * vec4(aPos.x, aPos.y, -1.0, 1.0); \n\
}";

        std::string fRenderer =
            "#version 330 core \n\
in vec3 vertexColor;                              \n\
uniform sampler2D texture1;                       \n\
out vec4 FragColor;                               \n\
in  vec2 TexCoords;                               \n\
void main()                                       \n\
{                                                 \n\
    FragColor = vec4(texture(texture1,TexCoords.xy).xyzw);  \n\
}";
        /*~===============================================~*/


    };// class DISPLAY WINDOW
    /*===================================================================*/


}//NS Profiler







/*=======================================================================================================================================================
/*                                               TRASH
/*=======================================================================================================================================================
 

                // GLuint Location{ 0 };
               //uint32_t Location = glGetAttribLocation(QuadRenderer.g_ID(), "aPos");
               //glEnableVertexAttribArray(Location);
               //glVertexAttribPointer(Location, 4, GL_FLOAT, GL_FALSE, 0, (char*)NULL);
               //




//Camera.ProjectionMatrix = ProjectionMatrix;
//Camera.ViewMatrix = ViewMatrix;
// A Camera is just a projection and View Matrix. Make it so...
 //Shader::get().SetUniform("ViewMatrix", ViewMatrix);
 //Shader::get().SetUniform("ProjectionMatrix", ProjectionMatrix);




                 // rad+=5;// Just temp shit for testing to make a sin wave
                //if (rad > 180) rad = 0;
                //val = (int)DataPoint;//(DataRange.x * sin(rad * 3.14159 / 180.0f))  ;
                //float J = _value * (Size.x / DataRange.x);// *(DataRange.x / Size.x);// (DataRange.x / Size.x);
                // Cx = Size.x / DataRange.x;
                // Cx2 = DataRange.x / Size.x;
                //float coe = (_value / DataRange.x)* (1 / (DataRange.x / _value));
                //val = coe;// (J * DataRange.x);
                //log(InputValue) * Pixelheight/ log(dataRange.x)

/*=======================================================================================================================================================*/



