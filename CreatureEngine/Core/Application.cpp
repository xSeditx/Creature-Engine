#include"Application.h"
#include"../Profiling/Timing/Timer.h"
#include<iostream>
#include<cassert>
#include<string>


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
#include"../Profiling/Timing/Benchmark.h"


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
std::mutex DEBUGMutex;


Application::Application(int _width, int _height, std::string _name) noexcept
    :
    Application_Name(_name),
    Size({ _width, _height })
{
     Instance = GetModuleHandle(nullptr);
     set(*this);
}
 

 //=======================================================================================================
 //_______________________________ STATE and FLOW HANDLING _______________________________________________
 //=======================================================================================================

 void Application::Init() trace(1)
 {
	 set(*this);
	 CreateApplicationWindow();
	 DEBUG_CODE(CheckGLERROR());

	 //--------Gathering information about OpenGL state and Display it -----------------------------------------------
	 //  int NumberOfExtensions = 0;
	 //  _GL(glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions));
	 //  
	 //  for (int i = 0; i < NumberOfExtensions; i++) {
	 //  	const GLubyte *ccc = glGetStringi(GL_EXTENSIONS, i);
	 //  	Print(ccc);
	 //  }

	 const GLubyte *extensions = glGetString(GL_EXTENSIONS);
	 Print(extensions);
	 Print("");
	 Print("Vendor: " << OpenGL::get_Vendor());
	 Print("Renderer: " << OpenGL::get_Renderer());
	 Print("OpenGL Version: " << OpenGL::get_Version());
	 Print("GLSL Version: " << OpenGL::get_GLSL_version());

	// Print("Current Context: "; Print(glGetCurrentContext()));
     //-------------------------------------------------------------------------------------------------------------
     //-------------------------------------------------------------------------------------------------------------
     //-------------------------------------------------------------------------------------------------------------

     //ImGui::CreateContext();
  // GUI_io.ConfigDockingWithShift = true;
   //GUI_io.ConfigResizeWindowsFromEdges = true;
   //GUI_io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
   //GUI_io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

 //1    

//2  Init OpenGL Imgui Implementation
//   GL 3.0 + GLSL 130
//   const char* glsl_version = "#version 130";
//3  ImGui::ImGui_ImplWin32_InitPlatformInterface(); 
// ImGui_ImplWin32_InitPlatformInterface();     //-------------------------------------------------------------------------------------------------------------
     //-------------------------------------------------------------------------------------------------------------
     ImGui_ImplWin32_EnableDpiAwareness();


     // Setup Dear ImGui context
     IMGUI_CHECKVERSION();
     ImGUI_Context =  ImGui::CreateContext();
     ::SetCapture(Application::getWindow().g_Handle()); /// Dont believe this is needed as I do this in the Window Creation Function I think
     ImGuiIO&  GUI_io = ImGui::GetIO(); (void)GUI_io;

     GUI_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
     GUI_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
     GUI_io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
     GUI_io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
     GUI_io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can set io.MouseHoveredViewport correctly (optional, not easy)
     GUI_io.BackendPlatformName = "imgui_impl_win32";
     // GUI_io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;    // We can create multi-viewports on the Platform side (optional)
     // GUI_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
     // GUI_io.Platform_RenderWindow = ImGui_ImplWin32_RenderWindow;
     // GUI_io.Platform_SwapBuffers = ImGui_ImplWin32_SwapBuffers;
 
     ImGui::StyleColorsDark();

     //ImGuiViewportP* viewport = g.Viewports[i];
     ImGuiStyle& style = ImGui::GetStyle();
     if (GUI_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
     {
       //  style.WindowRounding = 0.0;
        // style.Colors[ImGuiCol_WindowBg].w = 1.0f;
     }


    // GUI_io.WantCaptureMouse = true;
    // GUI_io.WantCaptureKeyboard = true;
     Port = ImGui::GetMainViewport();
 
     OnCreate();
     for (auto& L : Layers)
     {
         L->OnAttach();
     }
     ImGui_ImplWin32_Init(getWindow().g_Handle());// ImGui_ImplWin32_InitPlatformInterface();//
     ImGui_ImplOpenGL3_Init("#version 130");

     Return();
 }

 // Helper structure we store in the void* RenderUserData field of each ImGuiViewport to easily retrieve our backend data.
 struct ImGuiViewportDataOGL
 {
     HWND Window;
     HDC hDC;
     HGLRC hGL_DC;
     bool        WindowOwned;
     int         IgnoreWindowPosEventFrame;
     int         IgnoreWindowSizeEventFrame;

     ImGuiViewportDataOGL() { Window = NULL; WindowOwned = false; IgnoreWindowSizeEventFrame = IgnoreWindowPosEventFrame = -1; }
     ~ImGuiViewportDataOGL() { IM_ASSERT(Window == NULL); }
 };


 void Application::Pause() {}
 void Application::End()
 {
     ImGui_ImplWin32_Shutdown();
     getWindow().Close();
	 OnEnd();
     
 }
 void Application::Run()
 {
	 Event msg;
	 size_t PreviousTime{ 0 };
     std::string Name = getWindow().g_Title()  + " FPS: ";
     FrameTimes.emplace_back(120.0f);
 

	 while (isRunning())
	 {
		 size_t NewTime = Timing::Timer<Milliseconds>::GetTime();
		 ++FPS;
		 Update();
		 Render();

         if ((NewTime - PreviousTime) > 1000)
		 {
             getWindow().s_Title(Name + std::to_string((int)FPS));
             FrameTimes.emplace_back((float)FPS);
			 FPS = 0;
			 PreviousTime = NewTime;
		 }

		 Application::PollEvents();
		 while (Application::PeekMSG(msg))
		 {
			 TranslateMessage(&msg);
			 Application::Dispatch(msg);
             if (msg.message _EQUALS_ WM_DESTROY)
             {// We check this in far to many places. this is Indicative of poor structure for the program or Shitty API to start with
                 Print("Exit");
             }
		 }
	 }
	 End();
 }


 void Application::Update()
 { // Calls User defined Application Update function
	 OnUpdate();
    //for (auto& L : Layers)
    //{
    //    L->OnUpdate();
    //}
 }
 void Application::Render()
 { // Calls User define Application Render function

     ImGuiIO&  GUI_io = ImGui::GetIO();
      
     ImGui_ImplOpenGL3_NewFrame();
     {
         ImGui_ImplWin32_NewFrame();
         {
             ImGui::NewFrame();
             {
                 OnRender();
                 //for (auto& L : Layers)
                 //{
                 // //   L->OnRender();
                 //}
             }
             ImGui::EndFrame();
         }
         ImGui::Render();
     }
     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

     if (GUI_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
     {
         //ImGui::UpdatePlatformWindows();
         //ImGui::RenderPlatformWindowsDefault();
     }// ImGuiViewportFlags_NoInputs
	 mainWindow.Sync(); //	SwapBuffers(DeviceContext); CLS();
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

// HGLRC  backup = wglGetCurrentContext();
// wglMakeCurrent(getWindow().g_DeviceContext(), backup);


 // WGL_EXT_swap_control

 //=================================================================================================
 //________________________________ WINDOW CREATION PROPERTIES _____________________________________
 //=================================================================================================
 void Application::SetWindowProperties()
 {
	 s_Position ({ 0,0 });// iVec2(0);
	 s_Size ( Size);//iVec2(640, 480);
	 mainWindow.s_Title("Default Application Window");
 }
 void Application::CreateApplicationWindow()
 {
	// wglSwapIntervalEXT
	// wg
	 SetWindowProperties();
	 if (Width() == 0 || Height() == 0)
	 {
		 Print(" ERROR: Window Settings have not been set or have been set to NULL.");
		 Print(" Define the virtual function with the specifications for your applications Window before calling CreateWindow() ");
		 __debugbreak();
	 }
	 mainWindow = Window( Size.x, Size.y, Application_Name,0);
 }

///==================================================================================================================
///====================== WINDOW SYSTEM  ============================================================================
///==================================================================================================================

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Application* Application::AppInstance{ nullptr };






Application::Window::Window(uint32_t _width, uint32_t _height, std::string _name, DWORD _flags) noexcept
	:
	Parent(nullptr),
	Size({ (float)_width, (float)_height })
{
    s_Title(_name);
	/// Create Window Handle and Device Context
	{
        LPCSTR classname = _name.c_str();
        trace_scope("CreateWindow")
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
        WindowProperties.lpszClassName = classname;/// _name.c_str();//"OpenGL";

		if (!RegisterClass(&WindowProperties))
		{
			MessageBox(NULL, "RegisterClass() failed:  "
				"Cannot register window class.", "Error", MB_OK);
			__debugbreak();
		} 
		if (!(Handle = CreateWindow
		(
            classname,
			Title.c_str(),
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			(int)g_PositionX(), (int)g_PositionY(), (int)Width() , (int)Height() ,
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
        trace_scope("SetPixelFormat");

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
    Loading_GL_Context = OpenGL::create_OpenGLContext(DeviceContext);

    /// wglShareLists causes a few things to be shared such as textures, display lists, VBO/IBO, shaders.
    if (wglShareLists(GL_Context, Loading_GL_Context) == FALSE)
    {// Checks to make sure the Loading thread and the GL render thread are in proper communication
        DWORD errorCode = GetLastError();
        LPVOID lpMsgBuf;
        FormatMessage
        (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, 
            NULL
        );
        MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION);
        LocalFree(lpMsgBuf);
        wglDeleteContext(Loading_GL_Context);// Destroy if not Applicable
    }
    OpenGL::make_Context_Current(DeviceContext, GL_Context);



	s_Title(std::string("OPENGL VERSION ") + std::string( (char*)glGetString(GL_VERSION)) );
	s_Camera(new Camera2D());
	Application::setWindow(*this);

	/// Set Window State
	{
		SetForegroundWindow(Handle);                           // Slightly Higher Priority
		SetFocus(Handle);
        ShowWindow(Handle, SW_MAXIMIZE);// SW_SHOW);
		UpdateWindow(Handle);
	}

    RECT rect;
    GetWindowRect(Handle, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    ResizeWindow(width, height);

    s_Title(std::string("OPENGL VERSION ") + std::string( (char*)glGetString(GL_VERSION)) );


	/// Set OpenGL State for Loading thread Context
	{
        OpenGL::make_Context_Current(DeviceContext, Loading_GL_Context);
		glShadeModel(GL_SMOOTH);
        OpenGL::set_ClearColor(0.0f, 0.0f, 0.25f, 1.0f);       // Black Background
		glClearDepth(1.0f);                                    // Depth Buffer Setup
		OpenGL::enable_DepthTest();                            // Enables Depth Testing
        OpenGL::set_DepthFunction(GL_LEQUAL);                                // The Type Of Depth Test To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);     // Really Nice Perspective Calculations
    	OpenGL::set_Viewport(0, 0, _width, _height);

        OpenGL::enable_Blending();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
 

    /// Set OpenGL State  
    {
        OpenGL::make_Context_Current(DeviceContext, GL_Context);
        glShadeModel(GL_SMOOTH);
        OpenGL::set_ClearColor(0.0f, 0.0f, 0.25f, 1.0f);       // Black Background
        glClearDepth(1.0f);                                    // Depth Buffer Setup
        OpenGL::enable_DepthTest();                            // Enables Depth Testing
        OpenGL::set_DepthFunction(GL_LEQUAL);                                // The Type Of Depth Test To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);     // Really Nice Perspective Calculations
        OpenGL::set_Viewport(0, 0, _width, _height);
        OpenGL::enable_Blending();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
Application::Window::Window(Window* _parent, uint32_t _width, uint32_t _height, std::string _name, DWORD _flags) noexcept
	:
	Window(_width, _height, _name, _flags)
{
	Parent = _parent;
}



//
//
//// Thread_1:
//wglMakeCurrent(NULL, NULL);
//WaitForThread2(); OrDoSomeCPUJob();
//wglMakeCurrent(dc, glrc);
//// Thread_2:
//wglMakeCurrent(dc, glrc);
//DoSome_GL_Work();
//wglMakeCurrent(NULL, NULL);
//TerminateThread2_And_GiveControlToThread1();
//
// 




void Application::Window::Sync()
{//Display the contents of the back buffer to the Screen (*note:future at VSync if Specified) 
	SwapBuffers(DeviceContext);
	CLS();
}
void Application::Window::CLS()
{// Clear the Contents of the BackBuffer 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}        
void Application::Window::ResizeWindow(uint32_t _width, uint32_t _height) // Resize And Initialize The GL Window
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

    OpenGL::set_Viewport(0, 0, _width, _height);     // Reset The Current Viewport

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
	WindowShader = new Shader(VertexShader, FragmentShader);
	WindowShader->Bind();
}

Vec2 Application::Window::get_WindowSize()
{
    RECT rect;
    if (GetWindowRect(Handle, &rect))
    {
        Size =
        {
            rect.right - rect.left,
            rect.bottom - rect.top
        };
        return Size;
    }
    return Vec2(0);
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
}

bool Application::PeekMSG(Event& _msg)
trace(1)
{
    Return(getWindow().Messenger().PeekMSG(_msg));
}
bool Application::PeekMSG ( Event& _msg, unsigned int _rangemin, unsigned int _rangemax, int _handlingflags)
{
	return getWindow().Messenger().PeekMSG(  _msg, _rangemin,  _rangemax,   _handlingflags);
}


/* 
Listener ResizeListener(
	[](Event _msg)
{
	float W = GET_X_LPARAM(_msg.lParam);
	float H = GET_Y_LPARAM(_msg.lParam);
	WorldCamera->Resize({ W, H });
});
*/


Vec2 SplitLParam(int lParam)
{
	return Vec2((int)(lParam) & 0xFFFF, ((int)(lParam) >> 16) & 0xFFFF);
}

Vec2 getMouseCursor()
{
	POINT p;
	GetCursorPos(&p);
	return { p.x, p.y };
}

void Application::Resize(Vec2 _size)
{
    mainWindow.s_Size(_size);
	mainWindow.g_Camera().Resize(_size);
}

//
//void Application::
//void Application::OnMouseDown(Window *_window, iVec2 _position, uint32_t _button) {}
//void Application::OnMouseUp(Window *_window, iVec2 _position, uint32_t _button) {}
//void Application::OnKeydown(Window *_window, uint32_t _keyCode, uint32_t _scanCode, bool _repeat) {}
//void Application::OnKeyup(Window *_window, uint32_t _modification) {}
//MK_CONTROL  0x0008  The CTRL key is down.
//MK_LBUTTON  0x0001  The left mouse button is down.
//MK_MBUTTON  0x0010  The middle mouse button is down.
//MK_RBUTTON  0x0002  The right mouse button is down. 
//MK_SHIFT    0x0004  The SHIFT key is down.
//MK_XBUTTON1 0x0020  The first X button is down.
//MK_XBUTTON2 0x0040  The second X button is down.

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) trace(1)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
    {
        return true;
    }

    Application::Window *Active_Window = &Application::get().getWindow();
    switch (static_cast<uint32_t>(uMsg))
    {
    case NULL: /* If nothing exit */break;

    case WM_MOUSEWHEEL:
    {
        float Wheel = (int16_t)SplitLParam((int)wParam).y / 120.0f;
        Vec2  Mpos = SplitLParam((int)lParam);
        Application::getMouse().Update(Mpos, Wheel);
    }break;

    case WM_MOUSEMOVE:
    {
        Application::getMouse().Update(SplitLParam((int)lParam), 0);
        Application::get().OnMouseMove(Active_Window, { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, Application::getMouse().Relative, (uint32_t)wParam);

    }break;

    case WM_KEYDOWN:
    {
        Application::get().OnKeydown(Active_Window, (uint32_t)wParam, lParam, BIT(30));
    }break;


    case WM_KEYUP:
    { 
        Application::get().OnKeyup(Active_Window, (uint32_t)wParam);
    }break;
     

    case WM_SIZE:
    {
        Vec2 sz = SplitLParam((int)lParam);
        sz.x += 16; // Did not have the .x and .y here? Why?
        sz.y += 39;
        Application::get().Resize({ sz.x, sz.y });
    }break;

    case WM_DESTROY:
    {
        ImGui::Shutdown(Application::get().ImGUI_Context);
        Application::get().Terminate();
    }break;

    };
 
    Return( DefWindowProc(hwnd, uMsg, wParam, lParam));
}


_static Application::Window::InputDevices::_mouse    Application::Window::InputDevices::Mouse;
_static Application::Window::InputDevices::_keyboard Application::Window::InputDevices::Keyboard;


std::string VertexShader =
"#version 330 core \n\
layout(location = 0) in vec2 aPos;          \n\
uniform mat4 ProjectionMatrix;              \n\
uniform mat4 ViewMatrix;                    \n\
uniform mat4 ModelMatrix;                   \n\
void main()                                 \n\
{                                           \n\
    mat4 ModelViewMatrix = (ViewMatrix * ModelMatrix);  \n\
    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);\n\
    gl_Position = ModelViewProjectionMatrix * vec4(aPos.x, aPos.y, -1.0, 1.0); \n\
}";


std::string FragmentShader =
"#version 330 core  \n\
out vec4 FragColor; \n\
void main()         \n\
{                   \n\
    FragColor = vec4(0.5, 1.0, 0.0, 1.0);  \n\
}";



void Application::OnMouseMove(Window *_window, iVec2 _position, iVec2 _relative, uint32_t _button) {}
void Application::OnMouseDown(Window *_window, iVec2 _position, uint32_t _button) {}
void Application::OnMouseUp(Window *_window, iVec2 _position, uint32_t _button) {}
void Application::OnKeydown(Window *_window, uint32_t _keyCode, uint32_t _scanCode, bool _repeat) {}
void Application::OnKeyup(Window *_window, uint32_t _modification) {}








//https://www.worldometers.info/coronavirus/country/south-korea/

/* =============================================================================================================================================
/*                                          NOTES       
/* ============================================================================================================================================= 


 Windows Messages:
 https://wiki.winehq.org/List_Of_Windows_Messages

 
 https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ee418748(v%3Dvs.85)


Odds of Winning Powerball : 1 in 292,201,338


US Population:  327,200,000 Million
Infection Rate: Double every 4 Days

Currently Infected [3/12/2020]: 202

Mortality:
 
 [Note]: I do not wish to use the current percentage for mortality in the
    USA because it's initial infection started at an anomaly in a Nursing
    home meaning the data will be skewed until we have more information 
    Currently it is extremely high mortality rate.
     
     
     
      ___________________________________________________________________
     |                           MORTALITY RATE                          |    
     |___________________________________________________________________|
     |   Worst Case (World Average)    | Best Case (South Korea Numbers) |
     |_________________________________|_________________________________|
     |             3.4%                |           0.7%                  |
     |_________________________________|_________________________________|
     | 11,124,800 Theoretical max      |    2,290,440 Theoretical max    |      
     |_________________________________|_________________________________|
     
     
       1 in 49.432 Will contract it over the next 60 days
     
      ______________________________________________________
     | DAY   |  INFECTIONS |      World Avg    |  SK Avg    |
     |_______|_____________|___________________|____________|
     |  4    |    404      |                   |            |
     |  8    |    808      |                   |            |
     |  12   |    1616     |                   |            |
     |  16   |    3232     |                   |            |
     |  20   |    6464     |                   |            |
     |  24   |    12928    |                   |            |
     |  28   |    25856    |                   |            |
     |  32   |    51712    |                   |            |
     |  26   |    103424   |                   |            |
     |  40   |    206848   |                   |            |
     |  44   |    413696   |                   |            |
     |  48   |    827392   |                   |            |
     |  52   |    1654784  |                   |            |
     |  56   |    3309568  |                   |            |
     |  60   |    6619136  |   225,050.264     |   46,333   |
     |_____________________|___________________|____________|
     
                       Population / Deaths
     ________________________________________________________
      World Mortality Avg:                 1  in  1453
      South Korea Mortality Avg:           1  in  7061
     ________________________________________________________
      The above is your odds of dying in the next 60 days
      Take note this is if you are even currently healthy 
      and not infected at the moment. 
     ________________________________________________________
     
     

|
|
|                                 |                                 |
|                                 |                                 |
|                                 |                                 |
|                                 |                                 |
|



 */



/// 7/21/2020
///    REFACTOR("I do not want this for the Application, I wantLayers for the Renderer, I already have Event System and this is redundent");
// for (auto it = Layers.end(); it != Layers.begin();)
// {
//     if ((*--it)->OnEvent(_msg))
//     {
//         break;
//     }
// }

//_static Application::Window::EventHandler& Application::Window::EventHandler::get()
//{
//	static Application::Window::EventHandler instance;
//	return instance;
//}
//

