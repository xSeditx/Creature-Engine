#pragma once

#include<queue>
#include<string>
#include<cstdint>
#include<iostream>
#include<Windows.h>
#include<windowsx.h>
#include<unordered_map>

#include"../Core/Common.h"
#include"../Renderer/LowLevel/OpenGL/OpenGL.h"

extern std::string BasicVertexShader;
extern std::string BasicFragmentShader;
extern std::string VertexShader;
extern std::string FragmentShader;

using MsgType = uint32_t;
using Event = MSG;


struct Listener
{
public:
	Listener(void(*func)(Event))
		:
		_handler(func)
	{}
	void SetHandler(void(*func)(Event))
	{
		_handler = func;
	}
	inline void Handler(Event msg)
	{
		_handler(msg);
	};
private:
	void(*_handler)(Event msg);
};
class Application
{
private:
	bool Running{ true };
	HINSTANCE Instance{ nullptr };
	struct Window
	{
		struct InputDevices
		{
			struct _mouse
			{

				// We are going to likely want to access this a number of ways
				Vec2 Position{ 0,0 };

				// We are going to likely want to access this a number of ways
				Vec2 Relative{ 0,0 };

				struct
				{//TODO: Union to Access Buttons as Array might be useful as well
					bool 
						Left{ false },
						Right{ false },
						Center{ false }, 
						X1{ false }, 
						X2{ false };
				}Buttons;

				void Hide() { Print("Hide Mouse"); }
				void Show() { Print("Hide Mouse"); }
			}static Mouse;
			struct _keyboard
			{
				bool Keys[256];
			}static Keyboard;
		}Input;

	public:
		InputDevices& getInput()
		{
			return Input;
		}

		Window() = default;
		Window(uint32_t _width, uint32_t _height, std::string _name, DWORD _flags);// = RESIZABLE);
		Window(Window* _parent, uint32_t _width, uint32_t _height, std::string _name, DWORD _flags = 0);

		/* Returns Windows Position */
		Vec2   g_Position() const noexcept { return Position; }
		/* Returns X location of Window on Screen */
		float  g_PositionX() const noexcept { return Position.x; }
		/* Returns Y location of Window on Screen */
		float  g_PositionY() const noexcept { return Position.y; }
		/* return the Dimensions of a Window */
		Vec2   g_Size() const noexcept { return Size; }
		/* Gets the Name of the Window */
		std::string g_Title() { return Name; }
		/* Return the Parent Window if this is a Child */
		Window& g_Parent() const { return *Parent; }


		/* Sets the Parent Window if this is a Child */
		Window& s_Parent() const { return *Parent; }
		/* Sets the location of a Window */
		void   s_Position(Vec2 _pos)  noexcept { Position = _pos; }
		/* Sets the X location for a Window */
		Vec2   s_PositionX(float _x) noexcept { Position.x = _x; }
		/* Sets the Y location for a Window */
		Vec2   s_PositionY(float _y) noexcept { Position.y = _y; }
		/* Sets Window Width */
		void s_Width(int _x) const noexcept { Size.x; }
		/* Sets Window Height*/
		void s_Height(int _y) const noexcept { Size.y; }
		/* Sets Window Title */
		void s_Title(std::string _name);

		/* Sets Window Dimensions */
		void s_Size(Vec2 _size) noexcept
		{
			Size.x = _size.x;
			Size.y = _size.y;
		}
		/* Sets the Width */
		Vec2   s_Height() const noexcept { return Size; }


		/* Return the Width dimension of a Window */
		int  Width()     const noexcept  { return (int)Size.x; }
		/* Return the Height Dimension of a Window */
		int  Height()    const noexcept  { return (int)Size.y; }

		/* Return if the Window is Alive
		Note: A Window can be invisible and still be Alive. When the final Window is no longer Alive our Application will terminate*/
		bool isAlive()   const noexcept  { return Alive; }

		/* Let us know if the Window is currently Visible on Screen
		Note: Intentions are to hide all child Windows if Parent becomes invisible*/
		bool isVisible() const           { return Visible; }

		/* Allow us to see if the Window in Question is the Currently Active window on the screen */
		bool isActive()  const           { return Active; }

		/* Display the contents of the back buffer to the Screen (*note:future at VSync if Specified) */
		void Sync();

		/* Clear the Contents of the BackBuffer */
		void CLS();      // Clear The Screen And The Depth Buffer

		/* Returns the Native Windows Handle (HWND) of the Window */
		const HWND g_Handle() const { return Handle; }

		/* Changes with Window size and OpenGL Viewport settings */
		void ResizeWindow(uint32_t _x, uint32_t _y);

		HDC g_DeviceContext() { return DeviceContext; }

 	private:
		uint32_t BasicShader;

		Window* Parent = nullptr;

		HGLRC GL_Context{ 0 };

		HWND Handle{ 0 };
		HDC  DeviceContext{ 0 };
		Vec2 Size{ 0,0 };
		Vec2 Position{ 0,0 };
		std::string Name;

		/* NOTE: This and PFD both need to be sent to Application class */
		WNDCLASS WindowProperties = { 0 };
		PIXELFORMATDESCRIPTOR PixelFormatDescriptor{ 0 }; // This is where our Hints are going to be coming from.

		int PixelFormat{ 0 };
		std::string Title{ "" };

		bool Active  = true;
		bool Alive   = true;
		bool Visible = true;


 	private:

		void create_DefaultShader();

		struct EventHandler
		{
			EventHandler() = default;
		public:
			static EventHandler& get();                 // Initializes EventSystem when called first time


			void PollEvents();
			void PostMSG(Event msg);
			bool PeekMSG(Event& msg, unsigned int rangemin, unsigned int rangemax, int handlingflags);
			bool PeekMSG(Event& msg);

			void Dispatch(Event msg);

			void RegisterListener(MsgType msg, Listener& handler);
			void RemoveListener(MsgType msg, Listener& handler);

			EventHandler(EventHandler const&) = delete; // and prevents Copies from being made
			void operator = (EventHandler const&) = delete;

		private:
			std::queue<Event> Messages;
			std::unordered_map<MsgType, std::vector<Listener*>> ListenerMap;
		};

	public:
		static EventHandler& Messenger() { return EventHandler::get(); }
	}  mainWindow;// Window

	std::string Name() { return mainWindow.g_Title(); }
	//Timer ApplicationTimer;

public:
	Application();
	Application(int _width, int _height, std::string _name);
	~Application();

	/* Called when Application is Initialized */
	void Init();
	/* Called when Application is created or comes back from a pause */
	void Run();
	/* Halts Applications execution until run is called again */
	void Pause();
	/* Cleanup Application on Termination */
	void End();
	/* Called Once a Frame to Update Application state */
	void Update();
	/* Called Once a Frame to Render Application */
	void Render();
	/* Kills our Application */
	void Terminate() { Running = false; }

	/* Return the Width dimension of the Main Window */
	int   Width() const noexcept { return (int)mainWindow.Width(); }
	/* Return the Height Dimension of the Main Window */
	int   Height() const noexcept { return (int)mainWindow.Height(); }
	/* Tells us the application is currently running 	NOTE: Perhaps enumerate this to allow more states */
	bool  isRunning() { return Running; }
	/* Returns Applications Position */
	Vec2  g_Position() const noexcept { return mainWindow.g_Position(); }
	/* Returns Applications X location on Screen */
	float g_PositionX() const noexcept { return mainWindow.g_PositionX(); }
	/* Returns Applications Y location on Screen */
	float g_PositionY() const noexcept { return mainWindow.g_PositionY(); }

	/* Sets the location of a Window */
	void  s_Position(Vec2 _pos)  noexcept { mainWindow.s_Position(_pos); }
	/* Sets the X location for a Window */
	Vec2  s_PositionX(float _x) noexcept { mainWindow.s_PositionX(_x); }
	/* Sets the Y location for a Window */
	Vec2  s_PositionY(float _y) noexcept { mainWindow.s_PositionY(_y); }
	/* Sets the Size for the base Window of Application */
	void  s_Size(Vec2 _size) noexcept { mainWindow.s_Size(_size); }
	/* Sets the Width for the base Window of Application */
	void  s_Width(int _x) const noexcept { mainWindow.s_Width(_x); }
	/* Sets the Height for the base Window of Application */
	void  s_Height(int _y) const noexcept { mainWindow.s_Height(_y); }
	/* Sets the Title for the base Window of Application */
	void  s_Name(std::string _name) { mainWindow.s_Title(_name); }
	

	/* Removes an Event from our Listener Queue   */
	void RemoveListener(MsgType msg, Listener& handler);
	/* Registers a Callback for specified Message */
	void RegisterListener(MsgType msg, Listener& handler);
	/* Gather the Events from the OS */
	void PollEvents();
	/* Dispatches Events to their Registered Objects */
	void Dispatch(Event  msg);
	/* Post a Message to Applications */
	void PostMSG(Event  msg);
	/* Non Blocking check of our Message Queues */
	bool PeekMSG(Event& msg, unsigned int rangemin, unsigned int rangemax, int handlingflags);
	/* Basic Nonblocing check of our Message Queue */
	bool PeekMSG(Event& msg);

protected:

	/* Executed when Application is Initialized */
	virtual void OnCreate();
	/* Executed at Start of Application */
	virtual void OnStart();
	/* Executed Application is Started on creation or after a Pause */
	virtual void OnRun(); /// Differs from start in that it can resume execution; Possibly change to Resume later or something
	/* Executed when Application is Paused 	NOTE: Incomplete */
	virtual void OnPause();
	/* Executed when Application goes to sleep from lack of Messages	NOTE: Incomplete */
	virtual void OnSleep();
	/* Called when Application Terminates */
	virtual void OnEnd();
	/* Called once a frame on Application Update */
	virtual void OnUpdate();
	/* Called once a frame when our Application is Rendered */
	virtual void OnRender();

	void CreateApplicationWindow();
	virtual void SetWindowProperties();

	static void set(Application& _app) { AppInstance = &_app; }
public:
	static Application* AppInstance;

	static Application&                       get()         { return *AppInstance;                      }
	static Application::Window&               getWindow()   { return AppInstance->mainWindow;           }
	static Application::Window::InputDevices& getDevice()   { return AppInstance->mainWindow.Input;     }
	static Window::InputDevices::_mouse&      getMouse()    { return AppInstance->getDevice().Mouse;    }
	static Window::InputDevices::_keyboard&   getKeyboard() { return AppInstance->getDevice().Keyboard; }

};// Application














/* =============================================================================================================================================
/*                                                TRASH
/* =============================================================================================================================================

/*


	//Print("Callback:" << uMsg);
	///switch (uMsg)
	///{
	///
	///case WM_ACTIVATE:                       // Watch For Window Activate Message
	///{
	///	if (!HIWORD(wParam))                    // Check Minimization State
	///	{
	///		Print("Active");                    // Program Is Active
	///	}
	///	else
	///	{
	///		Print("Inactive");              // Program Is No Longer Active
	///	}
	///
	///	break;                              // Return To The Message Loop
	///}
	///
	///case WM_SYSCOMMAND:                     // Intercept System Commands
	///{
	///	switch (wParam)                     // Check System Calls
	///	{
	///	case SC_SCREENSAVE:                 // Screensaver Trying To Start?
	///	case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
	///		return 0;                       // Prevent From Happening
	///	}
	///	break;                              // Exit
	///}
	///
	///case WM_CLOSE:                          // Did We Receive A Close Message?
	///{
	///	Print("Close");
	///	PostQuitMessage(0);                 // Send A Quit Message
	///	break;    // Jump Back
	///}
	///
	///case WM_KEYDOWN:                        // Is A Key Being Held Down?
	///{
	///	Application::getDevice().Keyboard.Keys[wParam] = true;                    // If So, Mark It As TRUE
	///	break;                      // Jump Back
	///}
	///
	///case WM_KEYUP:                        // Is A Key Being Held Down?
	///{
	///	Application::getDevice().Keyboard.Keys[wParam] = false;                    // If So, Mark It As TRUE
	///	break;                      // Jump Back
	///}
	///
	///case WM_MOUSEMOVE:
	///{
	///	 Application::getDevice().Mouse.Relative = Application::getDevice().Mouse.Position;
	///	 Application::getDevice().Mouse.Position = { (float)GET_X_LPARAM(lParam),(float)GET_Y_LPARAM(lParam) };
	///
	///	 Application::getDevice().Mouse.Relative.x = Application::getDevice().Mouse.Position.x - Application::getDevice().Mouse.Relative.x;
	///	 Application::getDevice().Mouse.Relative.y = Application::getDevice().Mouse.Position.y - Application::getDevice().Mouse.Relative.y;
	///
	///	Print("MouseMove:" <<  Application::getDevice().Mouse.Relative.x << " : " <<  Application::getDevice().Mouse.Relative.y);
	///	Print("Position:" <<   Application::getDevice().Mouse.Position.x << " : " <<  Application::getDevice().Mouse.Position.y);
	///
	///}
	///
	///case WM_LBUTTONDOWN:
	///{
	///	Print("MouseDown");
	///	break;
	///}
	///
	///case WM_LBUTTONUP:
	///{
	///	Print("Left Button Up");
	///	break;
	///}
	///
	///case WM_LBUTTONDBLCLK:
	///{
	///	Print("L DClick ");
	///	break;
	///}
	///
	///case WM_RBUTTONDOWN:
	///{
	///	Print(" RButtonDown");
	///	break;
	///}
	///
	///case WM_RBUTTONUP:
	///{
	///	Print(" RButUP");
	///	break;
	///}
	///
	///case WM_RBUTTONDBLCLK:
	///{
	///	Print(" R Button DClick");
	///	break;
	///}
	///
	///case WM_MBUTTONDOWN:
	///{
	///	Print(" MButtonDown");
	///	break;
	///}
	///
	///case WM_MBUTTONUP:
	///{
	///	Print("M BUtton UP ");
	///	break;
	///}
	///
	///case WM_MBUTTONDBLCLK:
	///{
	///	Print(" MButton DClick");
	///	break;
	///}
	///
	///case WM_XBUTTONDOWN:
	///{
	///	Print(" XButtonDown");
	///	break;
	///}
	///case WM_XBUTTONUP:
	///{
	///	Print(" X Up");
	///	break;
	///}
	///
	///case WM_XBUTTONDBLCLK:
	///{
	///	Print(" X DClick");
	///	break;
	///}
	///
	///case WM_SIZE:
	///{
	///	Application::getWindow().ResizeWindow(HIWORD(wParam), LOWORD(wParam));
	///	break;
	///}
	///default:
	///	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	///}



#if 0
while (true)
{
	MSG msg;
	BOOL bRet;
	while (PeekMessage(&msg, Application::getWindow().g_Handle(), 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Update();
	Render();
}
#else


Application::Window::Event::Event()
	:
	ID(0),
	///WinID(0),
	Time(0)
{
	memset(&data, 0, 16);
}
Application::Window::Event::Event(MsgType msg)
	:
	ID(msg),
	///WinID(0),
	Time(0)
{
	memset(&data, 0, 16);
}
*/
//struct Application
//{
//	struct Window
//	{
//		struct InputDevices
//		{
//			struct {}Mouse;
//			struct {}Keyboard;
//		}Input;
//		struct EventHandler
//		{}EventSystem;
//	}mainWindow;
//};
		/*
		struct Event
		{// Event Union Defining an Event the Engine can produce
			Event();
			Event(MsgType msg);

			MsgType ID;
			///	WindowID WinID;
			unsigned long Time;
			Vec2 Location{ 0, 0 };
#pragma pack(push, 1)
			union msgUnion
			{// All messages are 128 bits
				msgUnion() = default;

				msgUnion& operator =(const msgUnion& other)
				{
					memcpy(this, &other, sizeof(other)); // Should be 16 bytes, check this if any changes get made
					return *this;
				}

				struct msgstruct
				{
					uint32_t wordParam;
					uint64_t longParam;
					uint32_t Padding;
				}MSG; // 128 bits
				struct kbstruct
				{
					uint32_t KeyCode;
					uint32_t ScanCode;
					uint32_t Action;
					uint32_t Mods;
				}Keyboard; // 128
				struct mousestruct
				{
					uint32_t Button;
					uint32_t x;
					uint32_t y;
					int16_t relX;
					int16_t relY;
				}Mouse; // 128
				struct buttonstruct
				{
					struct buttons
					{
						uint8_t
							Left,
							Right,
							Middle,
							Wheel;
					}Buttons; // 32
					uint32_t Action;
					uint32_t Modification;
					uint32_t Padding;
				}MouseButton; // 128
				struct textstruct {}TextInput;
			}data;
#pragma pack(pop)
			Event(MSG _msg)
			{
				Location.x = _msg.pt.x;
				Location.y = _msg.pt.y;

				ID = _msg.message;
				this->data.MSG.longParam = _msg.lParam;
				this->data.MSG.wordParam = _msg.wParam;
				this->data.MSG.Padding = 0;
				this->Time = _msg.time;
			}
			bool Handled() { return (ID == 0); }
		};
*/

/*	{// OpenGL Rendering Context Scope
		//  wglCreateContext	    Creates a new rendering context.
		//  WglMakeCurrent	        Sets a thread's current rendering context.
		//  WglGetCurrentContext	Obtains a handle to a thread's current rendering context.
		//  WglGetCurrentDC      	Obtains a handle to the device context associated with a thread's current rendering context.
		//  WglDeleteContext	    Deletes a rendering context.

		GL_Context = wglCreateContext(DeviceContext);
		if (!wglMakeCurrent(DeviceContext, GL_Context))
		{
			std::cout << "Making Current Rendering Context Failed" << "\n";
		}
		if (!GL_Context)
		{
			MessageBox
			(/// Turn all this into a Macro for Clearity
				NULL,
				"GL Context Creation Failed  "
				"Cannot Create Renderer",
				"Error",
				MB_OK
			);
		}

		if (!gladLoadGL())
		{// If the Loading of OpenGL functions fails report it and exit
			int error_code = glad_glGetError();
			std::cout << "Failed to initialize GLAD" << error_code << std::endl;
			__debugbreak();
		};

		Title = std::string("OPENGL VERSION ") + std::string((char*) glGetString(GL_VERSION));
		SetWindowTextA
		(
			Handle,
			Title.c_str()
		);

		//  wglCreateContext	    Creates a new rendering context.
		//  WglMakeCurrent	        Sets a thread's current rendering context.
		//  WglGetCurrentContext	Obtains a handle to a thread's current rendering context.
		//  WglGetCurrentDC      	Obtains a handle to the device context associated with a thread's current rendering context.
		//  WglDeleteContext	    Deletes a rendering context.
	}*/

