#include"Window.h"

_static Input::_mouse Input::Mouse;
_static Input::_keyboard Input::Keyboard;


std::ostream& operator<<(std::ostream& stream, const Vec2& vector)
{
	stream << "X:[" << vector.x << "] - Y:[" << vector.y << "]";
	return stream;
}



extern HINSTANCE AppInstance;


#include"../Core/Application.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Print("Callback:" << uMsg);
	switch (uMsg)
	{

	case WM_ACTIVATE:                       // Watch For Window Activate Message
	{
		if (!HIWORD(wParam))                    // Check Minimization State
		{
			Print("Active");                    // Program Is Active
		}
		else
		{
			Print("Inactive");              // Program Is No Longer Active
		}

		break;                              // Return To The Message Loop
	}

	case WM_SYSCOMMAND:                     // Intercept System Commands
	{
		switch (wParam)                     // Check System Calls
		{
		case SC_SCREENSAVE:                 // Screensaver Trying To Start?
		case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
			return 0;                       // Prevent From Happening
		}
		break;                              // Exit
	}

	case WM_CLOSE:                          // Did We Receive A Close Message?
	{
		Print("Close");
		PostQuitMessage(0);                 // Send A Quit Message
		break;    // Jump Back
	}

	case WM_KEYDOWN:                        // Is A Key Being Held Down?
	{
		Input::Keyboard.Keys[wParam] = true;                    // If So, Mark It As TRUE
		break;                      // Jump Back
	}

	case WM_KEYUP:                        // Is A Key Being Held Down?
	{
		Input::Keyboard.Keys[wParam] = false;                    // If So, Mark It As TRUE
		break;                      // Jump Back
	}

	case WM_MOUSEMOVE:
	{
		Input::Mouse.Relative = Input::Mouse.Position;
		Input::Mouse.Position = { (float)GET_X_LPARAM(lParam),(float)GET_Y_LPARAM(lParam) };
		Input::Mouse.Relative = Input::Mouse.Position - Input::Mouse.Relative;

		Print("MouseMove:" << Input::Mouse.Relative.x << " : " << Input::Mouse.Relative.y);
		Print("Position:" << Input::Mouse.Position.x << " : " << Input::Mouse.Position.y);

	}

	case WM_LBUTTONDOWN:
	{
		Print("MouseDown");
		break;
	}

	case WM_LBUTTONUP:
	{
		Print("Left Button Up");
		break;
	}

	case WM_LBUTTONDBLCLK:
	{
		Print("L DClick ");
		break;
	}

	case WM_RBUTTONDOWN:
	{
		Print(" RButtonDown");
		break;
	}

	case WM_RBUTTONUP:
	{
		Print(" RButUP");
		break;
	}

	case WM_RBUTTONDBLCLK:
	{
		Print(" R Button DClick");
		break;
	}

	case WM_MBUTTONDOWN:
	{
		Print(" MButtonDown");
		break;
	}

	case WM_MBUTTONUP:
	{
		Print("M BUtton UP ");
		break;
	}

	case WM_MBUTTONDBLCLK:
	{
		Print(" MButton DClick");
		break;
	}

	case WM_XBUTTONDOWN:
	{
		Print(" XButtonDown");
		break;
	}
	case WM_XBUTTONUP:
	{
		Print(" X Up");
		break;
	}

	case WM_XBUTTONDBLCLK:
	{
		Print(" X DClick");
		break;
	}

	case WM_SIZE:
	{
		//Application::ActiveWindow().ResizeWindow(HIWORD(wParam), LOWORD(wParam));
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



namespace Frontend {

	///==================================================================================================================
	///====================== WINDOW SYSTEM  ============================================================================
	///==================================================================================================================
	// Multithreading: https://aturon.github.io/blog/2015/08/27/epoch/
	//http://www.rastertek.com/gl40tut03.html
	//Fast Hashmap: https://martin.ankerl.com/2016/09/21/very-fast-hashmap-in-c-part-3/

	Window::Window(uint32_t _width, uint32_t _height, std::string _name, DWORD _flags)
		:
		Parent(nullptr),
		Size({ (float)_width, (float)_height }),
		Title(_name)
	{
		// Create Window Handle and Device Context
		{
        if (!AppInstance)
        {// TODO: Just do this  in the Application class as that should be responsible for registering its various Class styles
			AppInstance = GetModuleHandle(NULL);
        	WindowProperties.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        	WindowProperties.lpfnWndProc = (WNDPROC)WindowProc;
        	WindowProperties.cbClsExtra = 0;
        	WindowProperties.cbWndExtra = 0;
			WindowProperties.hInstance = AppInstance;
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
        }
		else
		{
			Application::get().Style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			WindowProperties.style = Application::get().Style;

			WindowProperties.lpfnWndProc = (WNDPROC)WindowProc;// Fix this later so App can have custom handler

			WindowProperties.cbClsExtra = Application::get().ExtraClassMemory;
			WindowProperties.cbWndExtra = Application::get().ExtraWindowMemory;

			WindowProperties.hInstance = (HINSTANCE)Application::get().OSInstance;
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
		}






			if (!(Handle = CreateWindow
			(
				"OpenGL",
				Title.c_str(),
				WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				(int)Position.x, (int)Position.y, (int)Size.x, (int)Size.y,
				NULL,
				NULL,
				AppInstance,//Application::Instance,
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

			if (PixelFormat _EQUALS_ NULL)
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

			if (SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDescriptor) _EQUALS_ FALSE)
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
		{// OpenGL Rendering Context Scope




			//  wglCreateContext	    Creates a new rendering context.
			//  WglMakeCurrent	        Sets a thread's current rendering context.
			//  WglGetCurrentContext	Obtains a handle to a thread's current rendering context.
			//  WglGetCurrentDC      	Obtains a handle to the device context associated with a thread's current rendering context.
			//  WglDeleteContext	    Deletes a rendering context.





			GL_Context = wglCreateContext(DeviceContext);
			if (!wglMakeCurrent(DeviceContext, GL_Context))
			{
				Print("Making Current Rendering Context Failed");
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

			Title = std::string("OPENGL VERSION ") + std::string((char*)glGetString(GL_VERSION));
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
		}

		/// Set Window State
		{
			SetForegroundWindow(Handle);                      // Slightly Higher Priority
			SetFocus(Handle);
			ShowWindow(Handle, SW_SHOW);
			UpdateWindow(Handle);
		}

		/// Set OpenGL State
		{
			glShadeModel(GL_SMOOTH);
			glClearColor(0.0f, 0.0f, 1.0f, 0.0f);                   // Black Background
			glClearDepth(1.0f);                         // Depth Buffer Setup
			glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
			glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
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
		CreateDefaultShader();
		trace_OUT("");
	}
	Window::Window(Window *_parent, uint32_t _width, uint32_t _height, std::string _name, DWORD _flags)
		:
		Window(_width, _height, _name, _flags)
	{
		Parent = _parent;
	}
	/* Display the contents of the back buffer to the Screen (*note:future at VSync if Specified) */
	void Window::Sync()
	{
		SwapBuffers(DeviceContext);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/* Clear the Contents of the BackBuffer */
	void Window::CLS()
	{
	}       // Clear The Screen And The Depth Buffer


	void Window::ResizeWindow(uint32_t _width, uint32_t _height)             // Resize And Initialize The GL Window
	{/// NOTE :https://stackoverflow.com/questions/692742/how-do-you-programmatically-resize-and-move-windows-with-the-windows-api
		if (!this)return;
		if (_height <= 0)                              // Prevent A Divide By Zero By
		{
			_height = 1;                           // Making Height Equal One
		}

		Size = { (float)_width,(float)_height };

		glViewport(0, 0, _width, _height);                    // Reset The Current Viewport
		glMatrixMode(GL_PROJECTION);
		glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
		glLoadIdentity();                           // Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		//gluPerspective(45.0f, (GLfloat)_width / (GLfloat)_height, 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
		glLoadIdentity();                           // Reset The Modelview Matrix

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










	void Window::CreateDefaultShader()
	{
		// Create the shaders
		uint32_t ERR = 0;
		if ((ERR = glGetError()))
		{
			Print("Error" << ERR);
			__debugbreak();
		}

		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode = VertexShader;
		std::string FragmentShaderCode = FragmentShader;


		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		//printf("Compiling shader : %s\n", vertex_file_path);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}

		// Compile Fragment Shader
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}

		// Link the program
		printf("Linking program\n");
		BasicShader = glCreateProgram();
		glAttachShader(BasicShader, VertexShaderID);
		glAttachShader(BasicShader, FragmentShaderID);
		glLinkProgram(BasicShader);

		// Check the program
		glGetProgramiv(BasicShader, GL_LINK_STATUS, &Result);
		glGetProgramiv(BasicShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(BasicShader, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}

		glDetachShader(BasicShader, VertexShaderID);
		glDetachShader(BasicShader, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		ERR = 0;
		if ((ERR = glGetError()))
		{
			Print("Error" << ERR);
			__debugbreak();
		}
	}
}



//std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
//if (VertexShaderStream.is_open()) {
//	std::stringstream sstr;
//	sstr << VertexShaderStream.rdbuf();
//	VertexShaderCode = sstr.str();
//	VertexShaderStream.close();
//}
//else {
//	printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
//	getchar();
//	return 0;
//}
//std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
//if (FragmentShaderStream.is_open()) {
//	std::stringstream sstr;
//	sstr << FragmentShaderStream.rdbuf();
//	FragmentShaderCode = sstr.str();
//	FragmentShaderStream.close();
//}
	// Read the Fragment Shader code from the file

/*
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define CLASS_NAME "window_class"
#define WND_NAME "CHIP-8"
#define WND_WIDTH 500
#define WND_HEIGHT 300
#define MSGBOX_ERROR(t) MessageBox(NULL, t, "Error", MB_ICONERROR | MB_OK);
void msgbox_error(const char *text) {
	char *error = NULL;
	if (GetLastError() != 0) {
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char *)&error,
			0,
			NULL);
	}
	if (error) {
		char *error_text = new char[strlen(error) + strlen(text) + 2];
		sprintf(error_text, "%s\n%s", text, error);
		MSGBOX_ERROR(error_text);
		LocalFree(error);
		free(error_text);
	}
	else {
		MSGBOX_ERROR(text);
	}
	exit(0);
}
LRESULT CALLBACK wnd_proc(HWND wnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
	default:
		return DefWindowProc(wnd, msg, w_param, l_param);
	}
	return 0;
}
void *GetGLFunctionAddress(const char *name) {
	void *p = (void *)wglGetProcAddress(name);
	if (p == 0 || (p == (void *)0x1) || (p == (void *)0x2) || (p == (void *)0x3)
		|| (p == (void *)-1)) {
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void *)GetProcAddress(module, name);
	}
	return p;
}
int WINAPI WinMain(HINSTANCE instance,
	HINSTANCE prev_instance,
	LPSTR cmd_line,
	int cmd_show) {
	WNDCLASSEX wnd_class;
	wnd_class.cbSize = sizeof(WNDCLASSEX);
	wnd_class.style = CS_OWNDC;
	wnd_class.lpfnWndProc = wnd_proc;
	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 0;
	wnd_class.hInstance = instance;
	wnd_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wnd_class.lpszMenuName = NULL;
	wnd_class.lpszClassName = CLASS_NAME;
	wnd_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wnd_class)) {
		msgbox_error("Failed to register window.");
	}
	HWND wnd = CreateWindowEx(WS_EX_CLIENTEDGE,
		CLASS_NAME,
		WND_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WND_WIDTH,
		WND_HEIGHT,
		NULL,
		NULL,
		instance,
		NULL);
	if (!wnd) {
		msgbox_error("Failed to create window.");
	}
	HDC dc = GetDC(wnd);
	PIXELFORMATDESCRIPTOR pfd
		= { sizeof(PIXELFORMATDESCRIPTOR),
		   1,
		   PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
		   PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
		   32,            // Colordepth of the framebuffer.
		   0,
		   0,
		   0,
		   0,
		   0,
		   0,
		   0,
		   0,
		   0,
		   0,
		   0,
		   0,
		   0,
		   24, // Number of bits for the depthbuffer
		   8,  // Number of bits for the stencilbuffer
		   0,  // Number of Aux buffers in the framebuffer.
		   PFD_MAIN_PLANE,
		   0,
		   0,
		   0,
		   0 };
	int pf = ChoosePixelFormat(dc, &pfd);
	if (pf == 0) {
		msgbox_error("Failed to choose pixel format.");
	}
	if (!SetPixelFormat(dc, pf, &pfd)) {
		msgbox_error("Failed to set pixel format.");
	}
	HGLRC glrc = wglCreateContext(dc);
	if (!glrc) {
		msgbox_error("Failed to create OpenGL context.");
	}
	if (!wglMakeCurrent(dc, glrc)) {
		msgbox_error("Failed to set OpenGL context.");
	}
	gladLoadGL();
	//if (!gladLoadGLLoader((GLADloadproc)GetAnyGLFuncAddress("wglGetProcAddress"))) {
	//    msgbox_error("Failed to initialise GLAD.");
	//}
	int done = 0;
	MSG msg;
	ShowWindow(wnd, cmd_show);
	UpdateWindow(wnd);
	while (!done) {
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = 1;
				break;
			}
			DispatchMessage(&msg);
		}
		// glClear(GL_COLOR_BUFFER_BIT);
		// glColor3f(1, 0, 0);
		// glBegin(GL_TRIANGLES);
		// glVertex3f(-1, -1, 0);
		// glVertex3f(1, -1, 0);
		// glVertex3f(0, 1, 0);
		// glEnd();
		SwapBuffers(dc);
	}
	ExitProcess(0);
}
*/


/*
HMODULE libEGL = LoadLibraryW(L"libEGL.dll");
	if (libEGL != NULL) {
		void* result = GetProcAddress(libEGL, "eglGetProcAddress");
		GLADloadproc proc = NULL;
		if (result != NULL) {
			proc = (GLADloadproc)result;
		} else
			proc = (GLADloadproc)eglGetProcAddress;
		if (!gladLoadGLES2Loader(proc)) {
				g_logger.fatal(stdext::format("Couldn't initialize GLES2Loader ver %d.%d", GLVersion.major, GLVersion.minor));
		}
		//close lib
		if (libEGL != NULL) {
			FreeLibrary(libEGL);
			libEGL = NULL;
		}
	}
*/