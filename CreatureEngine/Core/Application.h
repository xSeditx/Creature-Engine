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
#include"../Renderer/LowLevel/OpenGL/Shader/Shader.h"
#include"../Renderer/LowLevel/OpenGL/Camera/Camera2D.h"
#include"../Profiling/Timing/Timer.h"

extern std::string BasicVertexShader;
extern std::string BasicFragmentShader;
extern std::string VertexShader;
extern std::string FragmentShader;

using MsgType = uint32_t;
using Event = MSG;

Event& make_msg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* Creates a Listener Object to Handle whatever message is assigned to it
Syntax:   Listener(void(*func)(Event))   */
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
        int  Width()     const noexcept { return (int)Size.x; }
        /* Return the Height Dimension of a Window */
        int  Height()    const noexcept { return (int)Size.y; }

        /* Return if the Window is Alive
        Note: A Window can be invisible and still be Alive. When the final Window is no longer Alive our Application will terminate*/
        bool isAlive()   const noexcept { return Alive; }

        /* Let us know if the Window is currently Visible on Screen
        Note: Intentions are to hide all child Windows if Parent becomes invisible*/
        bool isVisible() const { return Visible; }

        /* Allow us to see if the Window in Question is the Currently Active window on the screen */
        bool isActive()  const { return Active; }

        /* Display the contents of the back buffer to the Screen (*note:future at VSync if Specified) */
        void Sync();

        /* Clear the Contents of the BackBuffer */
        void CLS();      // Clear The Screen And The Depth Buffer

        /* Returns the Native Windows Handle (HWND) of the Window */
        const HWND g_Handle() const { return Handle; }

        /* Changes with Window size and OpenGL Viewport settings */
        void ResizeWindow(uint32_t _x, uint32_t _y);

        /* Returns the Windows Device Context */
        HDC g_DeviceContext() { return DeviceContext; }

        /* Return the Most basic and default Shader */
        uint32_t defaultShaderHandle() { return WindowShader->g_Handle(); }

        /* Gets the default Shader Object */
        Shader& defaultShader() { return *WindowShader; }
 
        /* returns the default Camera for our Window */
        Camera2D& g_Camera()
        {
            return *defaultCamera;
        }
        /* Sets the Default Camera for our Window */
        void s_Camera(Camera2D* _camera)
        {
            defaultCamera = _camera;
        }
    protected:
        Shader* WindowShader{ nullptr };
        Camera2D* defaultCamera{ nullptr };

    private:

        Window* Parent = nullptr;


        HGLRC GL_Context{ 0 };

        HWND Handle{ 0 };
        HDC  DeviceContext{ 0 };
        Vec2 Size{ 0,0 };
        Vec2 Position{ 0,0 };
        std::string Name;

        /* NOTE: This and PFD both need to be sent to Application class */
        WNDCLASS WindowProperties{ 0 };
        PIXELFORMATDESCRIPTOR PixelFormatDescriptor{ 0 }; // This is where our Hints are going to be coming from.

        int PixelFormat{ 0 };
        std::string Title{ "" };

        bool Active{ true };
        bool Alive{ true };
        bool Visible{ true };

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

 public:
     size_t FPS{ 0 };
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

    /* Returns the Title of the Applications Window */
    std::string Name() { return mainWindow.g_Title(); }
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

    /* Resize the Application */
    void Resize(Vec2 _size);
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

    static Application& get() { return *AppInstance; }
    static Application::Window& getWindow() { return AppInstance->mainWindow; }
    static void setWindow(Application::Window& _window) { AppInstance->mainWindow = _window; }

    static Application::Window::InputDevices& getDevice() { return AppInstance->mainWindow.Input; }
    static Window::InputDevices::_mouse& getMouse() { return AppInstance->getDevice().Mouse; }
    static Window::InputDevices::_keyboard& getKeyboard() { return AppInstance->getDevice().Keyboard; }
    static Camera2D& getCamera() 
    {
        return AppInstance->mainWindow.g_Camera();
    }
    static void setCamera(Camera2D& _camera)
    {
        AppInstance->mainWindow.s_Camera(&_camera);
    }
};// Application




/* =============================================================================================================================================
/*                                                TRASH
/* =============================================================================================================================================







/* =============================================================================================================================================*/
