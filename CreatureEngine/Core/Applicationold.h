#pragma once
/* =============================================================================================================================================================
 APPLICATION
-------------
~ Managers ( singletons )
-------------
	TASK  // Handles the thread pool and asigns processes to each thread
	STATE
	SERVICE
	ENVIRONMENT
	PLATFORM
NOTES: Making these Singletons prevents the duplication of resources to ensure smooth performance
~ Framework
-------------
	GAMELOOP
	---- GetNativeMessages;
	---- Schedule execution
	---- Distribute Changes
	---- Check Execution State
	SCHEDULER
	---- Determine Systems to Execute
	---- Send to Task Manager
	---- Wait for Completion
================================================================================================================================================================ */


#include"Common.h"
//#include"../FrontEnd/Window.h"

//#include"Timer.h"
//#include"EventSystem.h"
//#include"Input.h"
#include"../Profiling/Timing/Timer.h"
 
class Window;
class  CREATURE_API Application
{
public:
	Application();
	~Application();

	iVec2 Position;
	iVec2 Size;

	std::string Title;

	void Init();
	void Run();
	void Pause();
	void End();

	void Update();
	void Render();

	//InputManager *Input;
	//EventSystem  &MessageHandler = EventSystem::Instance();

	Window *ApplicationWindow;
	bool Running = true;

	static Application& get()
	{
		return *GlobalInstance;
	}
	static void set(Application* _app)
	{
		GlobalInstance = _app;
	}

	//specifies the amount of extra class memory to allocate
	uint32_t Style{ 0 };// Specifies the Window Style and HVredraw
	int32_t ExtraClassMemory{ 0 };//int       cbClsExtra;
	int32_t ExtraWindowMemory{ 0 };//int       cbWndExtra;
	void *OSInstance;//HINSTANCE hInstance;

	//WNDPROC   lpfnWndProc;


	/* Future Implementation.  */
	//HICON     hIcon;Icon for Application
	//HCURSOR   hCursor;// Cursor
	//HBRUSH    hbrBackground;
	//LPCSTR    lpszMenuName;
	//LPCSTR    lpszClassName;
	//HICON     hIconSm;



protected:

	virtual void OnCreate();
	virtual void OnStart();
	virtual void OnRun(); /// Differs from start in that it can resume execution; Possibly change to Resume later or something
	virtual void OnPause();
	virtual void OnSleep();
	virtual void OnEnd();

	virtual void OnUpdate();
	virtual void OnRender();

	void CreateApplicationWindow();
	virtual void SetWindowProperties();

	Timing::Timer<> ApplicationTimer;


protected:
	static Application *GlobalInstance;

	void SetHints();
	void SetStereo(bool);
	void sRGBCapable(bool);
	void ForwardCompatible(bool);
	void DebugContext(bool);
	void ResizableWindow(bool);
	void VisibleWindow(bool);
	void DecoratedWindow(bool);
	void SetRedBits(unsigned int);
	void SetGreenBits(unsigned int);
	void SetBlueBits(unsigned int);
	void SetAlphaBits(unsigned int);
	void SetDepthBits(unsigned int);
	void SetStencilBits(unsigned int);
	void SetAccumulatorRedBits(unsigned int);
	void SetAccumulatorGreenBits(unsigned int);
	void SetAccumulatorBlueBits(unsigned int);
	void SetAccumulatorAlphaBits(unsigned int);
	void SetAuxiliaryBits(unsigned int);
	void SetNumberOfSamples(unsigned int);
	void SetRefreshRate(unsigned int);
	void UseOpenGLClient(unsigned int);
	void UseOpenGLESClient(unsigned int);
	void SetMajorVersion(unsigned int);
	void SetMinorVersion(unsigned int);
	void UseOpenGLProfile(unsigned int);


private: // Window Information. Default Properties will be set however user can Override these settings

//	void SetHints(HintsStruct hint);
//	HintsStruct Hints;
};

///                                                                                                   |||
///       Universal scenes and universal objects are responsible for registering all their extensions |||
///   with the state manager so that the extensions will get notified of changes made by other        |||
///   extensions(ie systems). Example would be to receive notifications of position and orientation   |||
///   changes made by the physics extension.                                                          |||
///                                                                           