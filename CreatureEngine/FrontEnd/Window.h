#pragma once

/*
#include<unordered_map>
#include<Windows.h>
#include<Windowsx.h>
#include<iostream>
#include<cstdint>    // Type defines 
#include<string>
#include<stack>
#include<queue>

#include"../Renderer/LowLevel/OpenGL/OpenGL.h"
 */



/*
   NATIVE WIN32 Window MODULE
   ==========================
   Location Handling
----------------------------------------------------------
	Vec2   g_Position()
	float  g_PositionX()
	float  g_PositionY()
	Vec2   s_Position(Vec2 _pos)
	Vec2   s_PositionX(float _x)
	Vec2   s_PositionY(float _y)
_________________________________________________________
	Dimensions
---------------------------------------------------------
	Vec2 g_Size() { return Size; }
	int Width()   { return Size.x; }
	int Height()  { return Size.y; }
_________________________________________________________
	HWND g_Handle()    const        Returns the Native Windows Handle (HWND) of the Window
	Window& g_Parent() const        Return the Parent Window if this is a Child
	Window& s_Parent() const        Sets the Parent Window if this is a Child
 Changes with Window size and OpenGL Viewport settings
	void ResizeWindow(uint32_t _x, uint32_t _y);
*/

/// Awesome conversion of a basic Win32 Hello Window to every language imaginable 
/// http://nehe.gamedev.net/tutorial/creating_an_opengl_window_(win32)/13001/

//#include<GL/gl.h>
//#include<GL/glu.h>
//#pragma comment(lib, "shell32.lib")
//#pragma comment(lib, "user32.lib")
//#pragma comment(lib, "gdi32.lib")
//
//#pragma comment(lib, "vcruntime.lib")
//#pragma comment(lib, "msvcrt.lib")
//#pragma comment(lib, "Ws2_32.lib")
//
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "gdiplus.lib")
 

//typedef  MSG  Event;
///================================================================================================================
///============================ INPUT DEVICE STATE ================================================================
///================================================================================================================

/// NOTE: ~ IMPORTANT ~ Code Disabled on 5/7/20 as a more Monolithic Application class was created sometime ago and this I believe is currently all Zombie code
/// Dead but here just waiting to fuck shit up.


/// /
/// struct Input
/// {
/// 	struct _mouse
/// 	{
/// 
/// 		// We are going to likely want to access this a number of ways
/// 		Vec2 Position{ 0,0 };
/// 
/// 		// We are going to likely want to access this a number of ways
/// 		Vec2 Relative{ 0,0 };
/// 
/// 		struct
/// 		{//TODO: Union to Access Buttons as Array might be useful as well
/// 			bool Left, Right, Center, X1, X2;
/// 		}Buttons;
/// 	}static Mouse;
/// 
/// 	struct _keyboard
/// 	{
/// 		bool Keys[256];
/// 	}static Keyboard;
/// };
/// 
/// ///================================================================================================================
/// ///====================== WINDOW SYSTEM  ==========================================================================
/// ///================================================================================================================
/// 
/// namespace Frontend
/// {
/// 	class Window
/// 	{
/// 	public:
/// 		Window(uint32_t _width, uint32_t _height, std::string _name, DWORD _flags);// = RESIZABLE);
/// 		Window(Window *_parent, uint32_t _width, uint32_t _height, std::string _name, DWORD _flags = 0);
/// 
/// 		/* Returns Windows Position */
/// 		Vec2   g_Position() const noexcept { return Position; }
/// 
/// 		/* Returns X location of Window on Screen */
/// 		float  g_PositionX() const noexcept { return Position.x; }
/// 
/// 		/* Returns Y location of Window on Screen */
/// 		float  g_PositionY() const noexcept { return Position.y; }
/// 
/// 		/* Sets the location of a Window */
/// 		Vec2   s_Position(Vec2 _pos)  noexcept { Position = _pos; }
/// 
/// 		/* Sets the X location for a Window */
/// 		Vec2   s_PositionX(float _x) noexcept { Position.x = _x; }
/// 
/// 		/* Sets the Y location for a Window */
/// 		Vec2   s_PositionY(float _y) noexcept { Position.y = _y; }
/// 
/// 		/* return the Dimensions of a Window */
/// 		Vec2   g_Size() const noexcept { return Size; }
/// 
/// 		/* Return the Width dimension of a Window */
/// 		int    Width() const noexcept { return (int)Size.x; }
/// 
/// 		/* Return the Height Dimension of a Window */
/// 		int    Height() const noexcept { return (int)Size.y; }
/// 
/// 		/* Return if the Window is Alive
/// 		Note: A Window can be invisible and still be Alive. When the final Window is no longer Alive our Application will terminate
/// 		*/
/// 		bool   isAlive() const noexcept { return Alive; }
/// 
/// 		/* Let us know if the Window is currently Visible on Screen
/// 		Note: Intentions are to hide all child Windows if Parent becomes invisible*/
/// 		bool isVisible() const { return Visible; }
/// 
/// 		/* Allow us to see if the Window in Question is the Currently Active window on the screen */
/// 		bool isActive() const { return Active; }
/// 
/// 		/* Display the contents of the back buffer to the Screen (*note:future at VSync if Specified) */
/// 		void Sync();
/// 
/// 		/* Clear the Contents of the BackBuffer */
/// 		void CLS();      // Clear The Screen And The Depth Buffer
/// 
/// 		/* Returns the Native Windows Handle (HWND) of the Window */
/// 		const HWND g_Handle() const { return Handle; }
/// 
/// 		/* Return the Parent Window if this is a Child */
/// 		Window& g_Parent() const { return *Parent; }
/// 
/// 		/* Sets the Parent Window if this is a Child */
/// 		Window& s_Parent() const { return *Parent; }
/// 
/// 		/* Changes with Window size and OpenGL Viewport settings */
/// 		void ResizeWindow(uint32_t _x, uint32_t _y);
/// 
/// 		HDC g_DeviceContext() { return DeviceContext; }
/// 		GLuint BasicShader;
/// 
/// 	private:
/// 
/// 		HGLRC GL_Context{ 0 };
/// 
/// 		Window *Parent = nullptr;
/// 		HWND Handle{ 0 };
/// 		HDC DeviceContext{ 0 };
/// 		Vec2 Size{ 0,0 };
/// 		Vec2 Position{ 0,0 };
/// 
/// 		WNDCLASS    WindowProperties = { 0 };
/// 		PIXELFORMATDESCRIPTOR PixelFormatDescriptor{ 0 };
/// 		int PixelFormat{ 0 };
/// 		std::string Title{ "" };
/// 
/// 		bool Active = true;
/// 		bool Alive = true;
/// 		bool Visible = true;
/// 
/// 
/// 
/// 		void CreateDefaultShader();
/// 
/// 		std::string BasicVertexShader = "\
/// #version 330 \n\
/// layout(location = 0) in vec3 VertexPosition;\n\
/// out vec3 VertexColor;\n\
/// out vec4 FragPosition;\n\
/// void main()\n\
/// {\n\
/// FragPosition = vec4(VertexPosition.xyz, 1.0f);\n\
/// VertexColor = vec3(1.0,1.0,1.0);\n\
/// }";
/// 
/// 		std::string BasicFragmentShader = "\n\
/// #version 330            \n\
/// out vec3 color;         \n\
/// in vec3 VertexColor;    \n\
/// in vec4 FragPosition; \n\
/// void main(){            \n\
/// color = vec3(1, 0, 0);  \n\
/// }";
/// 
/// 		std::string VertexShader = " #version 330 core \n\
/// layout(location = 0) in vec3 aPos; 			\n\
/// out vec4 vertexColor; 						\n\
/// void main()									\n\
/// {											\n\
/// 	gl_Position = vec4(aPos, 1.0);			\n\
/// 	vertexColor = vec4(0.5, 0.0, 0.0, 1.0); \n\
/// }";
/// 
/// 
/// 
/// 		std::string FragmentShader = "#version 330 core \n\
/// out vec4 FragColor;									 \n\
/// in vec4 vertexColor;  								 \n\
/// void main()											 \n\
/// {													 \n\
/// 	FragColor = vertexColor;						 \n\
/// }";
/// 
/// 
/// 
/// 	};
/// 
/// 
/// }
/// 
/// 	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
/// 