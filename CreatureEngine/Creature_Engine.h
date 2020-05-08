#pragma once


/* 
    Options can likely be handled in main prior to including this to trigger various changes 
*/

 

#pragma comment (lib, "CreatureEngine.lib")


#include"Core/Common.h"

#include"Core/Application.h"
#include"FrontEnd/Window.h"

#include"Core/Threading/Threadpool.h"


#include"Profiling/SystemInfo.h"
#include"Profiling/MemoryPerf/MemTracker.h"
#include"Profiling/Timing/Benchmark.h"

/* Needs work as the Proper use of Textures and BMP was never incoperated into this class */
#include"Profiling/RenderUtilities.h"



/* Perhaps I should include all the Rendering stuff in the OpenGL header and Dir Tree from there */
#include"Renderer/LowLevel/OpenGL/OpenGL.h" 
#include"Renderer/LowLevel/OpenGL/Renderer/Primitives.h"
#include"Renderer/LowLevel/OpenGL/Renderer/2DRenderer.h"

 

/* Likely not needed */
#include"Core/Threading/TestFunctions.h"
#include"Core/Memory.h"



/* Potentially split off to just become the Soft bodied physics portion of the engine once the proper
 DOD design is laid out and made genaric */

#include"Creatures/Creatures.h"
#include"Creatures/Physics/Springs.h"



//
//
//class Mesh;
//class Application;
//
//Mesh::Mesh();
//Mesh::Mesh(std::string file);
//void Mesh::Bind();
//void Mesh::Render();
//void Mesh::Unbind();
//void Mesh::Update();
//void Mesh::GetPrimativeType(unsigned int input);
//
//Graphics::Bitmap::Bitmap(void * _memory, Vec2 _dimensions) noexcept
//Graphics::Bitmap::Bitmap(std::string _file) noexcept
//void Graphics::Bitmap::FlipYAxis()
//void Graphics::Bitmap::FlipXAxis()
//
// 
//void  Application::Window::Window::InputDevices::_mouse::Hide() { Print("Hide Mouse"); }
//void  Application::Window::Window::InputDevices::_mouse::Show() { Print("Hide Mouse"); }
//void  Application::Window::Window::InputDevices::_mouse::Update(Vec2 _pos, float _wheel);
//
//Application::Window::Window::InputDevices&   Application::Window::getInput();
//
//Application::Window::Window();
//Application::Window::Window(uint32_t _width, uint32_t _height, std::string _name, DWORD _flags) noexcept;
//Application::Window::Window(Window* _parent, uint32_t _width, uint32_t _height, std::string _name, DWORD _flags = 0) noexcept;
//
//        /* Returns Windows Position */
//        Vec2     Application::Window::g_Position() const noexcept;
//        /* Returns X location of Window on Screen */
//        float    Application::Window::g_PositionX() const noexcept;
//        /* Returns Y location of Window on Screen */
//        float    Application::Window::g_PositionY() const noexcept;
//        /* return the Dimensions of a Window */
//        Vec2     Application::Window::g_Size() const noexcept;
//        /* Gets the Name of the Window */
//        std::string   Application::Window::g_Title();
// 
//        /* Return the Parent Window if this is a Child */
//        Window&   Application::Window::g_Parent() const noexcept;
//
//
//        /* Sets the Parent Window if this is a Child */
//        Window&   Application::Window::s_Parent() const noexcept;
//        /* Sets the location of a Window */
//        void     Application::Window::s_Position(Vec2 _pos)  noexcept;
//        /* Sets the X location for a Window */
//        Vec2    Application::Window::s_PositionX(float _x) noexcept;
//        /* Sets the Y location for a Window */
//        Vec2     Application::Window::s_PositionY(float _y) noexcept;
//        /* Sets Window Width */
//        void   Application::Window::s_Width(int _x) const noexcept;
//        /* Sets Window Height*/
//        void   Application::Window::s_Height(int _y) const noexcept;
//        /* Sets Window Title */
//        void   Application::Window::s_Title(std::string _name);
//
//        /* Sets Window Dimensions */
//        void   Application::Window::s_Size(Vec2 _size) noexcept
//
//            /* Sets the Width */
//            Vec2     Application::Window::s_Height() const noexcept;
//
//
//        /* Return the Width dimension of a Window */
//        int   Application::Window::Width()     const noexcept;
//        /* Return the Height Dimension of a Window */
//        int    Application::Window::Height()    const noexcept;
//
//        /* Return if the Window is Alive
//        Note: A Window can be invisible and still be Alive. When the final Window is no longer Alive our Application will terminate*/
//        bool   Application::Window::isAlive()   const noexcept;
//
//        /* Let us know if the Window is currently Visible on Screen
//        Note: Intentions are to hide all child Windows if Parent becomes invisible*/
//        bool   Application::Window::isVisible() const noexcept;
//
//        /* Allow us to see if the Window in Question is the Currently Active window on the screen */
//        bool   Application::Window::isActive()  const noexcept;
//
//        /* Display the contents of the back buffer to the Screen (*note:future at VSync if Specified) */
//        void   Application::Window::Sync();
//
//        /* Clear the Contents of the BackBuffer */
//        void   Application::Window::CLS();      // Clear The Screen And The Depth Buffer
//
//        /* Returns the Native Windows Handle (HWND) of the Window */
//        const HWND   Application::Window::g_Handle() const noexcept;
//
//        /* Changes with Window size and OpenGL Viewport settings */
//        void   Application::Window::ResizeWindow(uint32_t _x, uint32_t _y);
//
//        /* Returns the Windows Device Context */
//        HDC   Application::Window::g_DeviceContext() noexcept;
//
//        /* Return the Most basic and default Shader */
//        uint32_t   Application::Window::defaultShaderHandle() noexcept; 
//
//        /* Gets the default Shader Object */
//        Shader&   Application::Window::defaultShader() noexcept; 
//
//        /* Return the OS Dimensions of the Window */
//        Vec2   Application::Window::get_WindowSize();
//
//        /* returns the default Camera for our Window */
//        Camera2D&   Application::Window::g_Camera() noexcept;
//
//
//            /* Sets the Default Camera for our Window */
//            void   Application::Window::s_Camera(Camera2D* _camera) noexcept;
// 
//        void Application::Window::create_DefaultShader();
// 
//         //  static EventHandler& Messenger() { return EventHandler::get(); } 
//     //   EventHandler& Application::Window::Messenger() noexcept { return Observer; }
// 
//  
//    Application::Application();
//    //Application(int _width, int _height, std::string _name) noexcept;
//
//    Application::~Application() noexcept;
//
//    /* Called when Application is Initialized */
//    void  Application::Init();
//    /* Called when Application is created or comes back from a pause */
//    void  Application::Run();
//    /* Halts Applications execution until run is called again */
//    void  Application::Pause();
//    /* Cleanup Application on Termination */
//    void  Application::End();
//    /* Called Once a Frame to Update Application state */
//    void  Application::Update();
//    /* Called Once a Frame to Render Application */
//    void  Application::Render();
//    /* Kills our Application */
//    void  Application::Terminate();// noexcept { Running = false; }
//
//    /* Returns the Title of the Applications Window */
//    std::string  Application::Name() noexcept;
//    /* Return the Width dimension of the Main Window */
//    int    Application::Width() const noexcept;
//    /* Return the Height Dimension of the Main Window */
//    int    Application::Height() const noexcept;
//    /* Tells us the application is currently running 	NOTE: Perhaps enumerate this to allow more states */
//    bool   Application::isRunning() const noexcept;
//    /* Returns Applications Position */
//    Vec2   Application::g_Position() const noexcept;
//    /* Returns Applications X location on Screen */
//    float  Application::g_PositionX() const noexcept;
//    /* Returns Applications Y location on Screen */
//    float  Application::g_PositionY() const noexcept;
//
//    /* Sets the location of a Window */
//    void   Application::s_Position(Vec2 _pos)  noexcept;
//    /* Sets the X location for a Window */
//    Vec2   Application::s_PositionX(float _x) noexcept;
//    /* Sets the Y location for a Window */
//    Vec2   Application::s_PositionY(float _y) noexcept;
//    /* Sets the Size for the base Window of Application */
//    void   Application::s_Size(Vec2 _size) noexcept;
//    /* Sets the Width for the base Window of Application */
//    void   Application::s_Width(int _x) const noexcept;
//    /* Sets the Height for the base Window of Application */
//    void   Application::s_Height(int _y) const noexcept;
//    /* Sets the Title for the base Window of Application */
//    void   Application::s_Name(std::string _name);
//
//
//    /* Removes an Event from our Listener Queue   */
//    void  Application::RemoveListener(MsgType msg, Listener& handler);
//    /* Registers a Callback for specified Message */
//    void  Application::RegisterListener(MsgType msg, Listener& handler);
//    /* Gather the Events from the OS */
//    void  Application::PollEvents();
//    /* Dispatches Events to their Registered Objects */
//    void  Application::Dispatch(Event  msg);
//    /* Post a Message to Applications */
//    void  Application::PostMSG(Event  msg);
//    /* Non Blocking check of our Message Queues */
//    bool  Application::PeekMSG(Event& msg, unsigned int rangemin, unsigned int rangemax, int handlingflags);
//    /* Basic Nonblocing check of our Message Queue */
//    bool  Application::PeekMSG(Event& msg);
//
//    /* Resize the Application */
//    void  Application::Resize(Vec2 _size);
//protected:
//
//    /* Executed when Application is Initialized */
//    virtual void  Application::OnCreate();
//    /* Executed at Start of Application */
//    virtual void Application::OnStart();
//    /* Executed Application is Started on creation or after a Pause */
//    virtual void  Application::OnRun(); /// Differs from start in that it can resume execution; Possibly change to Resume later or something
//    /* Executed when Application is Paused 	NOTE: Incomplete */
//    virtual void  Application::OnPause();
//    /* Executed when Application goes to sleep from lack of Messages	NOTE: Incomplete */
//    virtual void  Application::OnSleep();
//    /* Called when Application Terminates */
//    virtual void  Application::OnEnd();
//    /* Called once a frame on Application Update */
//    virtual void  Application::OnUpdate();
//    /* Called once a frame when our Application is Rendered */
//    virtual void  Application::OnRender();
//
//    void  Application::CreateApplicationWindow();
//    virtual void  Application::SetWindowProperties();
//
//    static void  Application::set(Application& _app) noexcept;
// 
//
//    static Application&  Application::get() noexcept;
//    static Application::Window&  Application::getWindow() noexcept;
//    static void  Application::setWindow(Application::Window& _window)
//    static Application::Window::InputDevices&  Application::getDevice() noexcept;
//    static Window::InputDevices::_mouse&  Application::getMouse();
//    static Window::InputDevices::_keyboard&  Application::getKeyboard();
//    static void  Application::setCamera(Camera2D& _camera)
//
//    static Camera2D&  Application::getCamera()
// 
//
////Vec2 SplitLParam(int lParam);