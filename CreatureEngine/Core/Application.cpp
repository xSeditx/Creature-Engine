#include"Application.h"
#include<iostream>
#include<cassert>
/*=====================================================================================================
_________________________________ APPLICATION CLASS ___________________________________________________
=======================================================================================================
  Application class Abstracts away functionality of the application itself. It handles some important
  Messages such as Keyboard or Mouse Events. The User can define a handler for all Keyboard or Mouse
  Events however if they chose not to they can instead define specific Key or Mouse events such as
  a Handler for only the Left Mouse click or one for Only the Key Repeat.
  If the User does not define a specific Generic handler one has been supplied which calls the
  appropritate sub functionality for the input device
=========================================================================================================
USAGE:
=========================================================================================================
*/


#include<string>
 Application::Application()
 { 
	 Instance = GetModuleHandle(NULL);
	 set(*this); 
 }
 Application::~Application()
 { }

 //=======================================================================================================
 //_______________________________ STATE and FLOW HANDLING _______________________________________________
 //=======================================================================================================

 void Application::Init()
 {
	 CreateApplicationWindow();
 
	 //--------Gathering information about OpenGL state and Display it -----------------------------------------------
	 //  int NumberOfExtensions = 0;
	 //  _GL(glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions));
	 //  
	 //  for (int i = 0; i < NumberOfExtensions; i++) {
	 //  	const GLubyte *ccc = glGetStringi(GL_EXTENSIONS, i);
	 //  	Print(ccc);
	 //  }

	 //const GLubyte *extensions = glGetString(GL_EXTENSIONS);
	 //Print(extensions);
	 Print("");
	 Print("Vendor: " << OpenGL::get_Vendor());
	 Print("Renderer: " << OpenGL::get_Renderer());
	 Print("OpenGL Version: " << OpenGL::get_Version());
	 Print("GLSL Version: " << OpenGL::get_GLSL_version());

	// Print("Current Context: "; Print(glGetCurrentContext()));

	 //-------------------------------------------------------------------------------------------------------------
	 OnCreate();
 }

 void Application::Pause() {}
 void Application::End()
 {
	 OnEnd();
 }
 void Application::Run()
 {
	 MSG msg;

	while (isRunning())
	{
	 Application::PollEvents();
	 while(Application::PeekMSG(msg))
	 {
		 TranslateMessage(&msg);
		 Application::Dispatch(msg);
		 Print(msg.message);
		 if (msg.message == WM_DESTROY)
		 {
			 Print("Exit");
			 
		 }
	 }
	 Update();
	 Render();
	}
    End();
 }

 void Application::Window::EventHandler::PollEvents()
 {
	 MSG msg;
	 while (PeekMessage(&msg, Application::getWindow().g_Handle(), 0, 0, PM_REMOVE))
	 {
		 Application::Window::EventHandler::get().PostMSG(msg);
		 DispatchMessage(&msg);
	 }
 }
 void Application::Window::EventHandler::Dispatch(Event msg)
 {

	 for (auto& Callback : ListenerMap[msg.message])
	 {
		 Callback->Handler(msg);
	 }
 } 
 void Application::Update()
 { // Calls User defined Application Update function
	 OnUpdate();
 }
 void Application::Render()
 { // Calls User define Application Render function
	 OnRender();
	 mainWindow.Sync();
	// mainWindow.CLS(); // 100, 150, 255); // Do not like the duel Dereference every single frame Feels unneeded at best
 }
 void Application::OnStart()
 {
	 CreateApplicationWindow();
 }
 /// IDK ABOUT ANY OF THESE
 void Application::OnCreate() {  }
 void Application::OnPause()  {  }
 void Application::OnSleep()  {  }
 void Application::OnEnd()    {  }
 void Application::OnRun()    {  }
 void Application::OnUpdate() {  }
 void Application::OnRender() {  }



 //=================================================================================================
 //________________________________ WINDOW CREATION PROPERTIES _____________________________________
 //=================================================================================================
 void Application::SetWindowProperties()
 {
	 s_Position ({ 0,0 });// iVec2(0);
	 s_Size ( { 640, 480 });//iVec2(640, 480);
	 mainWindow.s_Title("Default Mystic Application Window");
 }
 void Application::CreateApplicationWindow()
 {
	 SetWindowProperties();
	 if (Width() == 0 || Height() == 0)
	 {
		 Print(" ERROR: Window Settings have not been set or have been set to NULL.");
		 Print(" Define the virtual function with the specifications for your applications Window before calling CreateWindow() ");
		 __debugbreak();
	 }
	 mainWindow = Window( 640,480, "TestWIndow",0);
 }

///==================================================================================================================
///====================== WINDOW SYSTEM  ============================================================================
///==================================================================================================================

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Application* Application::AppInstance{ nullptr };

Application::Window::Window(uint32_t _width, uint32_t _height, std::string _name, DWORD _flags)
	:
	Parent(nullptr),
	Size({ (float)_width, (float)_height }),
	Title(_name)
{
	/// Create Window Handle and Device Context
	{
		assert(&Application::get() != nullptr);
		WindowProperties.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		WindowProperties.lpfnWndProc = (WNDPROC)WindowProc;
		WindowProperties.cbClsExtra = 0;
		WindowProperties.cbWndExtra = 0;
		WindowProperties.hInstance = Application::get().Instance;
		WindowProperties.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		WindowProperties.hCursor = LoadCursor(NULL, IDC_ARROW);
		WindowProperties.hbrBackground = NULL;
		WindowProperties.lpszMenuName = NULL;
		WindowProperties.lpszClassName = "OpenGL";

		if (!RegisterClass(&WindowProperties))
		{
			MessageBox(NULL, "RegisterClass() failed:  "
				"Cannot register window class.", "Error", MB_OK);
			__debugbreak();
		}

		if (!(Handle = CreateWindow
		(
			"OpenGL",
			Title.c_str(),
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			(int)g_PositionX(), (int)g_PositionY(), (int)Width(), (int)Height(),
			NULL,
			NULL,
			Application::get().Instance,
			NULL
		)))
		{
			MessageBox(NULL, "CreateWindow() failed:  Cannot create a window. Error",
				"Error", MB_OK);
			__debugbreak();
		}
		DeviceContext = GetDC(Handle);
	}

	/// Set Pixel Format
	{// Creating and Setting Pixel Format Scope
		/* 	NOTE:  A good pixel format to choose for the dummy context is a simple 32-bit RGBA color buffer, with a 24-bit depth buffer and 8-bit stencil, as we did in the above sample PFD. This will usually get a hardware accelerated pixel format. */
		memset(&PixelFormatDescriptor, 0, sizeof(PixelFormatDescriptor));
		PixelFormatDescriptor.nSize = sizeof(PixelFormatDescriptor);
		PixelFormatDescriptor.nVersion = 1;
		PixelFormatDescriptor.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | _flags;
		PixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
		PixelFormatDescriptor.cColorBits = 32;

		PixelFormat =
			ChoosePixelFormat
			(
				DeviceContext,
				&PixelFormatDescriptor
			);

		if (PixelFormat == NULL)
		{
			MessageBox
			(
				NULL,
				"ChoosePixelFormat() failed:  "
				"Cannot find a suitable pixel format.",
				"Error",
				MB_OK
			);
			__debugbreak();
		}

		if (SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDescriptor) == FALSE)
		{
			MessageBox
			(
				NULL,
				"SetPixelFormat() failed:  "
				"Cannot set format specified.",
				"Error",
				MB_OK
			);
			__debugbreak();
		}

		DescribePixelFormat
		(
			DeviceContext,
			PixelFormat,
			sizeof(PIXELFORMATDESCRIPTOR),
			&PixelFormatDescriptor
		);
	}

	/// Create OpenGL Rendering Context
	GL_Context = OpenGL::create_OpenGLContext(DeviceContext);
	s_Title(std::string("OPENGL VERSION ") + std::string((char*)glGetString(GL_VERSION)));

	/// Set Window State
	{
		SetForegroundWindow(Handle);                           // Slightly Higher Priority
		SetFocus(Handle);
		ShowWindow(Handle, SW_SHOW);
		UpdateWindow(Handle);
	}

	/// Set OpenGL State
	{
		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);                  // Black Background
		glClearDepth(1.0f);                                    // Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);                               // Enables Depth Testing
		glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Test To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);     // Really Nice Perspective Calculations
    	glViewport(0, 0, _width, _height);

	}

	/// Set FullScreen Mode : Define _FULL_SCREEN_MODE to activate
	{
#ifdef _FULL_SCREEN_MODE

		DEVMODE dmScreenSettings;                   // Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));       // Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);       // Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;            // Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;           // Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;             // Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
#endif
	}
	create_DefaultShader();
}
Application::Window::Window(Window* _parent, uint32_t _width, uint32_t _height, std::string _name, DWORD _flags)
	:
	Window(_width, _height, _name, _flags)
{
	Parent = _parent;
}

void Application::Window::Sync()
{//Display the contents of the back buffer to the Screen (*note:future at VSync if Specified) 
	SwapBuffers(DeviceContext);
	CLS();
}
void Application::Window::CLS()
{// Clear the Contents of the BackBuffer 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}        
void Application::Window::ResizeWindow(uint32_t _width, uint32_t _height)             // Resize And Initialize The GL Window
{/// NOTE :https://stackoverflow.com/questions/692742/how-do-you-programmatically-resize-and-move-windows-with-the-windows-api
	if (!this)
	{
		return;
	}
	if (_height <= 0)                                 
	{// Prevent A Divide By Zero By
		_height = 1;                                   
	}

	Size = { (float)_width,(float)_height };

	glViewport(0, 0, _width, _height);               // Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);                     // Select The Projection Matrix
	glLoadIdentity();                                // Reset The Projection Matrix
	
	//// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)_width / (GLfloat)_height, 0.1f, 100.0f);
	
	glMatrixMode(GL_MODELVIEW);                      // Select The Modelview Matrix
	glLoadIdentity();                                // Reset The Modelview Matrix

	MoveWindow
	(
		Handle,
		(int)Position.x,
		(int)Position.y,
		_width,
		_height,
		true
	);
}
void Application::Window::s_Title(std::string _name)
{
	Title = _name;
	SetWindowTextA
	(
		Handle,
		Title.c_str()
	);
}

void Application::Window::create_DefaultShader()
{
	WindowShader = Shader(VertexShader, FragmentShader);
}

///==================================================================================================================

void Application::Window::EventHandler::PostMSG(Event msg)
{
	Messages.push(msg);
}
void Application::Window::EventHandler::RegisterListener(MsgType msg, Listener& handler)
{
	ListenerMap[msg].push_back(&handler);
}
void Application::Window::EventHandler::RemoveListener(MsgType msg, Listener& handler)
{
	ListenerMap[msg].erase(std::find(ListenerMap[msg].begin(), ListenerMap[msg].end(), &handler));
}
bool Application::Window::EventHandler::PeekMSG(Event& msg, unsigned int rangemin, unsigned int rangemax, int handlingflags)
{
	if (Messages.size())
	{
		msg = Event(Messages.front());
		if (msg.message > rangemin && msg.message < rangemax)
		{
			switch (handlingflags)
			{
			case 0:
			{// ONLY USE THIS FOR NOW
				Messages.pop();
				return true;
			}break;
			case 2:
			{
				__debugbreak();
			}break;
			case 3:
			{/// THIS WILL CURRENTLY BE A PROBLEM DO NOT USE IT
				Messages.pop();
				PostMSG(msg);
				__debugbreak();
			}break;

			}
		}
	}
	return false;//Messages.size() > 0;
}
bool Application::Window::EventHandler::PeekMSG(Event& msg)
{
	
	if (Messages.size() > 0) 
	{
		msg = Event(Messages.front());
		Messages.pop();
		return true;
	}
	return false;
}

void Application::RemoveListener  (MsgType _msg, Listener& _handler)
{
	getWindow().Messenger().RemoveListener(_msg, _handler);
}
void Application::RegisterListener(MsgType _msg, Listener& _handler)
{
	getWindow().Messenger().RegisterListener(_msg, _handler);
}
void Application::PollEvents()
{
	getWindow().Messenger().PollEvents();
}
void Application::PostMSG ( Event _msg) 
{
	getWindow().Messenger().PostMSG(_msg);
}
void Application::Dispatch( Event _msg) 
{
	getWindow().Messenger().Dispatch(_msg);
} ///This is a test of my Trace macro
bool Application::PeekMSG ( Event& _msg) trace(1)
{
	Return( getWindow().Messenger().PeekMSG(_msg));
}
bool Application::PeekMSG ( Event& _msg, unsigned int _rangemin, unsigned int _rangemax, int _handlingflags)
{
	return getWindow().Messenger().PeekMSG(  _msg, _rangemin,  _rangemax,   _handlingflags);
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) UNLIKELY
	{
		Application::get().Terminate();
	}
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


_static Application::Window::InputDevices::_mouse    Application::Window::InputDevices::Mouse;
_static Application::Window::InputDevices::_keyboard Application::Window::InputDevices::Keyboard;
_static Application::Window::EventHandler& Application::Window::EventHandler::get()
{
	static Application::Window::EventHandler instance;
	return instance;
}




/* =============================================================================================================================================
/*                                          NOTES       
/* ============================================================================================================================================= 


 Windows Messages:
 https://wiki.winehq.org/List_Of_Windows_Messages

 
 https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee418748(v%3Dvs.85)

*/


//std::ostream& operator <<(std::ostream& os, ErrorMessage& _msg)
//{
//	os << "Error: " << _msg.ErrorNumber << "\n At: " << _msg.Time << "\n ";
//	return os;
//}
//std::ostream& operator<<(std::ostream& stream, const Vec2& vector)
//{
//	stream << "X:[" << vector.x << "] - Y:[" << vector.y << "]";
//	return stream;






std::string BasicVertexShader = "#version 330 \n\
layout(location = 0) in vec3 VertexPosition;\n\
out vec3 VertexColor;\n\
out vec4 FragPosition;\n\
void main()\n\
{\n\
FragPosition = vec4(VertexPosition.xyz, 1.0f);\n\
VertexColor = vec3(1.0,1.0,1.0);\n\
}";

std::string BasicFragmentShader = "#version 330 \n\
out vec4 color;         \n\
in vec3 VertexColor;    \n\
in vec4 FragPosition;   \n\
void main(){            \n\
color = vec3(1, 1, 0, 1);  \n\
}";

std::string VertexShader = " #version 330 core \n\
layout(location = 0) in vec3 aPos; 			\n\
out vec4 vertexColor; 						\n\
void main()									\n\
{											\n\
	gl_Position = vec4(aPos, 1.0);			\n\
	vertexColor = vec4(0.5, 1.0, 0.0, 1.0); \n\
}";

std::string FragmentShader = "#version 330 core \n\
out vec4 FragColor;                             \n\
in vec4 vertexColor;                            \n\
void main()                                     \n\
{                                               \n\
	FragColor = vertexColor;                    \n\
}";