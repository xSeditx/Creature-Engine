#pragma once

#include"../Core/Common.h"
#include"../Renderer/LowLevel/Materials/Image/Texture.h"
#include"../Renderer/LowLevel/OpenGL/Shader/Shader.h"
#include"../Renderer/LowLevel/OpenGL/Camera/Camera2D.h"
#include"../Renderer/LowLevel/OpenGL/Renderer/Renderer.h"
#include"../Renderer/LowLevel/OpenGL/Renderer/Transform.h"
#include"../Profiling/Timing/Timer.h"
#include<atomic>

#define _RGB(r,g,b) (((b << 16)| (g << 8)) | (r))

union Pixel
{// We want to access color of pixel either as RGBA values or 32 bit Color
    Pixel() = default;
    Pixel(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
        : r(_r), g(_g), b(_b), a(_a) {}

    Pixel(uint32_t _col)
        :Color(_col) {}

    struct
    {// Proper RGBA for GL_RGBA values. 
        uint8_t r, g, b, a;
    };
    uint32_t Color;
    operator uint32_t() { return Color; }
};

namespace Profiling
{
    struct CREATURE_API DisplayWindow
    {

        Mat4 ProjectionMatrix;
        Mat4 ViewMatrix;
        Mat4 ModelMatrix;
		Transform Model;


        Mat4 Rotate_Translate(Vec3 _pos, Vec3 _rot)
        {
            return glm::rotate
            ( // ensure unneeded copying as the results from the last transform are immediately used in the next step
                glm::rotate
                (
                    glm::rotate
                    (
                        glm::translate
                        ( // Starts with the Translation here
                            glm::mat4(1.0f),
                            _pos
                        ),
                        glm::radians(_rot.x),
                        Vec3(1.0f, 0.0f, 0.0f)),
                    glm::radians(_rot.y),
                    Vec3(0.0f, 1.0f, 0.0f)),
                glm::radians(_rot.z),
                Vec3(0.0f, 0.0f, 1.0f)
            );
        }
		

        /* X Axis represents Value
           Y Axis represents Time  
           Optional RGB values for additional dimensions 
		   _position:  The Location on the Screen XY for the Window to start
		   _size:      The Size in ScreenXY to Display the Window
		   _dataRange: The Range of the values Displayed*/
        DisplayWindow(Vec2 _position, Vec2 _size, Vec2 _dataRange)
            :
            Size(_size),
            Position(_position),
			DataRange(_dataRange),
            ReadBuffer ( new Pixel[static_cast<size_t>(_dataRange.x) * static_cast<size_t>(_dataRange.y)]),
            WriteBuffer( new Pixel[static_cast<size_t>(_dataRange.x) * static_cast<size_t>(_dataRange.y)])
        {
			High = DataRange.x;

            QuadRenderer = Shader(vRenderer, fRenderer);

            Print("Pixel Size: " << sizeof(Pixel) << " bytes");
			clearRead();
			clearWrite();

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

            // Setup the Matrics for our Quad. 
            {// Possibly rid ourselves of these later
                ProjectionMatrix = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f);  //OrthographicMatrix(_size.x, _size.y);
                ViewMatrix = glm::translate(Mat4(1.0f), Vec3(200, 200, 0));//200, 200, 0))
                ModelMatrix = Mat4(1.0);
                ModelMatrix = Rotate_Translate(Vec3(_position.x, _position.y, 0), Vec3(0, 0, 45));
            }

            // Figure out the Coefficients for the values displayed to the screen
            {
                float xDiff{ 0 }, yDiff{ 0 };

                Low > 0 ?
                    xDiff = High + std::abs(Low) :
                    xDiff = High - Low;

                Low > 0 ?
                    xDiff = High + std::abs(Low) :
                    xDiff = High - Low;

                Xcoeff = DataRange.x / (High + Low);
                Ycoeff = DataRange.y / TimeLength;
            }

            // Creates the display texture
            DisplayTexture = Graphics::Texture(*(new Graphics::Bitmap(ReadBuffer, _dataRange)));// We dereference the Memory Address where the object was constructed than we pass that into Texture for construction and this should give us in place construction of the returned texture???
            
            glUseProgram(QuadRenderer.g_ID());
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            // Create the Vertex Buffer Object containing the Vertices and UV Coords
            {
                Vec4 Vertices[6] =
                {   // positions   // texCoords
                    Vec4(-Size.x  * 0.5f,   Size.y  * 0.5f,    0.0f,  1.0f),
                    Vec4(-Size.x  * 0.5f,  -Size.y  * 0.5f,    0.0f,  0.0f),
                    Vec4( Size.x  * 0.5f,  -Size.y  * 0.5f,    1.0f,  0.0f),
                    Vec4(-Size.x  * 0.5f,   Size.y  * 0.5f,    0.0f,  1.0f),
                    Vec4( Size.x  * 0.5f , -Size.y  * 0.5f,    1.0f,  0.0f),
                    Vec4( Size.x  * 0.5f ,  Size.y  * 0.5f,    1.0f,  1.0f)
                };

                glGenBuffers(1, &VBO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
            }

            // Get the Attribute Location and Check for Errors
            {
                GLuint Location{ 0 };
                Location = glGetAttribLocation(QuadRenderer.g_ID(), "aPos");
                glEnableVertexAttribArray(Location);
                glVertexAttribPointer(Location, 4, GL_FLOAT, GL_FALSE, 0, (char*)NULL);
                CheckGLERROR();
            }
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
        void Update(uint32_t _value)
        {
			TODO("Fix the Y Axis so that the Timer only goes off when it is in the proper range so Y Accurately represents Time ");
            if (Counter++ % 10 == 0)
            {// TODO("This needs to be set after a Timer is added so that if the Sample Limit is passed test the value and display it");
                int DataPoint = static_cast<int>(Xcoeff * _value);

                ClearLine();// Clear the new Line.
                setPixel(val, 0, Pixel(255,0,255,255));   // Sets its value

                rad+=5;// Just temp shit for testing to make a sin wave
                if (rad > 180)rad = 0;
                val = DataRange.x* sin(rad * 3.14159 / 180.0f)  ;
				swapBuffer();
                DisplayTexture.Update((uint8_t*)ReadBuffer);
            }
        }
        void Render()
        {
            QuadRenderer.Enable();
            {
                glBindVertexArray(VAO);
                {
                    glActiveTexture( GL_TEXTURE0 );
                    DisplayTexture.Bind();
                    QuadRenderer.SetUniform("texture1"  ,                0);
                    QuadRenderer.SetUniform("View"      ,       ViewMatrix);
                    QuadRenderer.SetUniform("Model"     ,      ModelMatrix);
                    QuadRenderer.SetUniform("Projection", ProjectionMatrix);

					OpenGL::Renderer::drawArray(VBO, 6);
                }
                glBindVertexArray(0);
            }
            QuadRenderer.Disable();
        }

        size_t   size()                                            {    return (size_t)(DataRange.x * DataRange.y) * sizeof(Pixel);    }
        void     setPixel(uint32_t _x, uint32_t _y, Pixel _color)  {           ReadBuffer[_x + (size_t)DataRange.x * _y] = _color;        }
        uint32_t getPixel(uint32_t _x, uint32_t _y)                {    return ReadBuffer[_x + (size_t)DataRange.x * _y];                 }

    private:
		void ClearLine()
		{
			memset(ReadBuffer, Pixel(100, 100, 100, 100).Color, DataRange.x * sizeof(Pixel));          // Clear the new Line.
		}
		void swapBuffer()
		{
			int LineSize = (sizeof(Pixel) * ((size_t)DataRange.x));
			memcpy((char*)ReadBuffer + (LineSize), (char*)WriteBuffer, size() - (LineSize));
			memcpy((char*)WriteBuffer, (char*)ReadBuffer, size());
		}

		void clearRead() { memset(ReadBuffer, 0, size()); }
		void clearWrite() { memset(WriteBuffer, 0, size()); }






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


        struct 
        {
            bool Border{ false };
            bool Record{ false };
        }Properties;


        GLuint 
            VAO{ 0 }, 
            VBO{ 0 },
            IBO{ 0 };
        Shader QuadRenderer;
        std::string vRenderer =
            "#version 330 core \n\
layout(location = 0) in vec4 aPos;          \n\
uniform mat4 Projection;                    \n\
uniform mat4 View ;                         \n\
uniform mat4 Model;                         \n\
out vec3 vertexColor;                       \n\
out vec2 TexCoords;                         \n\
void main()                                 \n\
{                                           \n\
    mat4 ModelViewMatrix = (View * Model);  \n\
    mat4 ModelViewProjectionMatrix = (Projection * ModelViewMatrix);\n\
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


    };
}