#pragma once

#include"../Core/Common.h"
#include"../Renderer/LowLevel/Materials/Image/Texture.h"
#include"../Renderer/LowLevel/OpenGL/Shader/Shader.h"
#include"../Renderer/LowLevel/OpenGL/Camera/Camera2D.h"
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
		   Optional RGB values for additional dimensions */
		DisplayWindow(Vec2 _position, Vec2 _size)
			:
			Size(_size),
			Position(_position),
			ReadBuffer(new Pixel[static_cast<size_t>(_size.x)* static_cast<size_t>(_size.y)]),
			WriteBuffer(new Pixel[static_cast<size_t>(_size.x)* static_cast<size_t>(_size.y)])
		{

			QuadRenderer = Shader(vRenderer, fRenderer);

			Print("Pixel Size: " << sizeof(Pixel) << " bytes");
			memset(ReadBuffer, 0, size());
			memset(WriteBuffer, 0, size());

			uint8_t R{ 100 }, G{ 0 }, B{ 0 };
			for_loop(y, Size.y)
			{
				for_loop(x, Size.x)
				{
					WriteBuffer[x + (int)Size.x * y] = Pixel(255, 255, 255, 100);
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
				ModelMatrix = Rotate_Translate(Vec3(_position.x, _position.y, 0), Vec3(0, 0, 90));
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

				Xcoeff = Size.x / (High + Low);
				Ycoeff = Size.y / TimeLength;
			}

			// Creates the display texture
			DisplayTexture = Graphics::Texture(*(new Graphics::Bitmap(ReadBuffer, _size)));// We dereference the Memory Address where the object was constructed than we pass that into Texture for construction and this should give us in place construction of the returned texture???
			
			glUseProgram(QuadRenderer.g_ID());
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

            // Create the Vertex Buffer Object containing the Vertices and UV Coords
			{
				Vec4 Vertices[6] =
				{   // positions   // texCoords
					Vec4(-Size.x,  Size.y,    0.0f,  1.0f),
					Vec4(-Size.x, -Size.y,    0.0f,  0.0f),
					Vec4(Size.x, -Size.y,    1.0f,  0.0f),
					Vec4(-Size.x,  Size.y,    0.0f,  1.0f),
					Vec4(Size.x, -Size.y,    1.0f,  0.0f),
					Vec4(Size.x,  Size.y,    1.0f,  1.0f)
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

		int Counter{ 0 }; 
		int val = 0;
		int rad = 0;
		void Update(uint32_t _value)
		{
			if (Counter++ % 100 == 0) 
			{// TODO("This needs to be set after a Timer is added so that if the Sample Limit is passed test the value and display it");
				int DataPoint = static_cast<int>(Xcoeff * _value);
				int LineSize = (sizeof(Pixel) * ((size_t)Size.x)) ;
				memset(ReadBuffer, 0, LineSize);          // Clear the new Line.
				setPixel(val, 0, Pixel(255,0,255,255));   // Sets its value

				rad+=5;// Just temp shit for testing to make a sin wave
				if (rad > 180)rad = 0;
				val = Size.x* sin(rad * 3.14159 / 180.0f)  ;  

				memcpy((char*)ReadBuffer + (LineSize), (char*)WriteBuffer, size() - (LineSize ));
				memcpy((char*)WriteBuffer, (char*)ReadBuffer, size());
			 	DisplayTexture.Update((uint8_t*)ReadBuffer);
			}
		}

		void Render()
		{
			QuadRenderer.Enable();
			{
				glBindVertexArray(VAO);
				{
					glActiveTexture(GL_TEXTURE0 );
					DisplayTexture.Bind();
 					QuadRenderer.SetUniform("texture1", 0);
					QuadRenderer.SetUniform("View"      , ViewMatrix);
					QuadRenderer.SetUniform("Model"     , ModelMatrix);
					QuadRenderer.SetUniform("Projection", ProjectionMatrix);

					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					{
						glDrawArrays(GL_TRIANGLES, 0, 6);
					}
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				glBindVertexArray(0);
			}
			QuadRenderer.Disable();
		}
		size_t size()
		{
			return ((size_t)Size.x * (size_t)Size.y) * sizeof(Pixel);
		}
		uint32_t getPixel(uint32_t _x, uint32_t _y)
		{
			return ReadBuffer[_x + (size_t)Size.x * _y];// .Color;
		}
		void setPixel(uint32_t _x, uint32_t _y, Pixel _color)
		{
			ReadBuffer[_x + (size_t)Size.x * _y] = _color;
		}

	private:
		Graphics::Texture DisplayTexture;
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


		Pixel
			DefaultColor{ 255,255,255, 255 };
		Pixel
			 *WriteBuffer{ nullptr },
			 *ReadBuffer{ nullptr };

		Vec2 Size{ 0,0 };
		Vec2 Position{ 0,0 };

		float TimeLength{ 1.0f };
		float Xcoeff, Ycoeff;

		float
			Low{ 0 },
			High{ 1000 };
 
		Protected_Value<uint32_t> CurrentValue;
		struct 
		{
			bool Border{ false };
			bool Record{ false };
		}Properties;

		Shader QuadRenderer;

		GLuint VAO{ 0 }, VBO{ 0 }, IBO{ 0 };

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