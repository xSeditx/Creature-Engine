#pragma once


#include"../Core/Common.h"
#include"../Renderer/LowLevel/Materials/Image/Texture.h"
#include"../Renderer/LowLevel/OpenGL/Shader/Shader.h"

#include<atomic>

#define _RGB(r,g,b) (((b << 16)| (g << 8)) | (r))
namespace Profiling
{
	template<typename _Ty>
	struct CREATURE_API Protected_Value
	{
		void Store(_Ty _value)
		{
			while (Flag.test_and_set(std::memory_order_acquire))
			{// maybe an atomic test wait?
			}  
			Value = _value;
			Flag.clear(std::memory_order_release);
		}
		_Ty Load()
		{
			return Value;
		}
	private:
		std::atomic_flag Flag{ ATOMIC_FLAG_INIT };
		_Ty Value{ 0 };
	};

	struct CREATURE_API DisplayWindow
	{
		using Data_t = uint32_t;
		/* X Axis represents Value
		   Y Axis represents Time 
		   Optional RGB values for additional dimensions */
		DisplayWindow(Vec2 _position, Vec2 _size)
			:
			Size(_size),
			Position(_position),
			ReadBuffer (new Pixel[static_cast<size_t>(_size.x) * static_cast<size_t>(_size.y)]),
			WriteBuffer(new Pixel[static_cast<size_t>(_size.x) * static_cast<size_t>(_size.y)])
		{
			 QuadRenderer = Shader(vRenderer, fRenderer);


			 memset(ReadBuffer, 0, size());
			 memset(WriteBuffer, 0, size());


			//memset( ReadBuffer, 255, size());
			//memset(WriteBuffer, 255, size());

		    uint8_t R{ 100 }, G{ 0 }, B{ 0 };
		    for_loop(y, Size.y)
		    {
		    	for_loop(x, Size.x)
		    	{
					//ReadBuffer[x + (int)Size.x * y].r = R;
					//ReadBuffer[x + (int)Size.x * y].g = G;
					//ReadBuffer[x + (int)Size.x * y].b = B;
					ReadBuffer[x + (int)Size.x * y] = Pixel(R, G, B);;

		    		R+=5;
		    		if (R >= 255) { G+= 5; R = 0; }
		    		if (G >= 255) { B+= 5; G = 0; }
		    
		    	}
		    }


			float xDiff{ 0 }, yDiff{ 0 };

			Low > 0 ?
				xDiff = High + std::abs(Low) :
				xDiff = High - Low;

			Low > 0 ?
				xDiff = High + std::abs(Low) :
				xDiff = High - Low;

			Xcoeff = Size.x / (High + Low);
			Ycoeff = Size.y / TimeLength;
			
			TODO("Probably not... Test this with a Special Test");// Wish to construct than Immediately move into place
			glActiveTexture(GL_TEXTURE0);
			DisplayTexture = Graphics::Texture(*(new Graphics::Bitmap(ReadBuffer, _size)));// We dereference the Memory Address where the object was constructed than we pass that into Texture for construction and this should give us in place construction of the returned texture???

			glUseProgram(QuadRenderer.g_ID());
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);

			GLuint Location{ 0 };

			Location = glGetAttribLocation(QuadRenderer.g_ID(), "aPos");
			glEnableVertexAttribArray(Location);
			glVertexAttribPointer(Location, 4, GL_FLOAT, GL_FALSE, 0, (char*)NULL);

 

			CheckGLERROR();
		}


		~DisplayWindow()
		{
			delete(ReadBuffer);
			delete(WriteBuffer);
		}

		void Update(Data_t _value)
		{
			TODO("This needs to be set after a Timer is added so that if the Sample Limit is passed test the value and display it");
			if (int Amount_of_time_has_passed = 1) 
			{
				int DataPoint = static_cast<int>(Xcoeff * _value);
				CurrentValue.Store(_value);

				int LineSize = (sizeof(Pixel) * (size_t)Size.x);
				memcpy((char*)ReadBuffer + LineSize, (char*)WriteBuffer, size() - LineSize);
				//ReadBuffer[DataPoint] = DefaultColor;
				memcpy((char*)WriteBuffer, (char*)ReadBuffer, size());
			 	DisplayTexture.Update((uint8_t*)ReadBuffer);
			}
		}
		Data_t getPixel(uint32_t _x, uint32_t _y)
		{
			return ReadBuffer[_x + (size_t)Size.x * _y].Color;
		}
		void setPixel(uint32_t _x, uint32_t _y, uint32_t _color)
		{
			WriteBuffer[_x + (size_t)Size.x * _y].Color = _color;
		}
		void Render()
		{
			QuadRenderer.Enable();
			{
				glBindVertexArray(VAO);
				{
					glActiveTexture(GL_TEXTURE0 );
					DisplayTexture.Bind();
				//glBindTexture(GL_TEXTURE_2D,0);
					glUniform1i(glGetUniformLocation(QuadRenderer.g_ID(), "texture1"), 0);

					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					{
						glDrawArrays(GL_TRIANGLES, 0, 6);
					}
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				glBindVertexArray(0);
			}
			QuadRenderer.Disable();
			//setPixel(1, 1, 0xffffffff);

		}

		size_t size()
		{
			return ((size_t)Size.x * (size_t)Size.y) * sizeof(uint32_t);
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

		std::vector<dataPoint<Data_t>> Record;

	    union Pixel
		{// We want to access color of pixel either as RGBA values or 32 bit Color
			Pixel() = default;
			Pixel(uint8_t _r, uint8_t _g, uint8_t _b)//, uint8_t _a)
				:
				r(_r), g(_g), b(_b)//, a(_a)
			{ }

			struct
			{
				uint8_t r, g, b;// , a;
			};	
			uint32_t Color;
			operator Data_t() { return Color; }

		};

		Pixel
			DefaultColor{ 255,255,255}, //, 255
	
			 *WriteBuffer{ nullptr },
			 *ReadBuffer{ nullptr };

		Vec2 Size{ 0,0 };
		Vec2 Position{ 0,0 };

		float TimeLength{ 1.0f };
		float Xcoeff, Ycoeff;

		float
			Low{ 0 },
			High{ 1000 };
 
		Protected_Value<Data_t> CurrentValue;
		struct 
		{
			bool Border{ false };
			bool Record{ false };
		}Properties;

		Shader QuadRenderer;

//Vec4 Vertices[4] =
//{
//	{ -1.0, -1.0,             0.0, 0.0 },
//	{  0.0, -1.0,             1.0, 0.0 },
//	{  0.0,  0.0,             1.0, 1.0 },
//	{ -1.0,  0.0,             0.0, 1.0 }
//};
		Vec4 Vertices[6] =
		{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			Vec4(-1.0f,  1.0f,  0.0f, 1.0f),
			Vec4(-1.0f, -1.0f,  0.0f, 0.0f),
			Vec4(1.0f, -1.0f,  1.0f, 0.0f),

			Vec4(-1.0f,  1.0f,  0.0f, 1.0f),
			Vec4(1.0f, -1.0f,  1.0f, 0.0f),
			Vec4(1.0f,  1.0f,  1.0f, 1.0f)
		};
		Vec2 UVcoords[4] =
		{
			{  },
			{  },
			{  },
			{  }
		};

 
		GLuint VAO{ 0 }, VBO{ 0 }, IBO{ 0 };

		std::string vRenderer = 
"#version 330 core \n\
layout(location = 0) in vec4 aPos;          \n\
uniform sampler2D texture1;                 \n\
out vec3 vertexColor;                       \n\
out vec2 TexCoords;                         \n\
void main()                                 \n\
{                                           \n\
	gl_Position = vec4(aPos.x, aPos.y, 1.0, 1.0);          \n\
	vertexColor = texture(texture1, aPos.zw).xyz; \n\
}";

		std::string fRenderer = 
"#version 330 core \n\
in vec3 vertexColor;                            \n\
out vec4 FragColor;                             \n\
void main()                                     \n\
{                                               \n\
	FragColor = vec4(vertexColor.xyz, 1.0);     \n\
}";


	};



}


