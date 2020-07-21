#include<iostream>
#include<Windows.h>
#include<array>
#include<cmath>
#include<string>

#include"Creature_Engine.h"

#include<stack>
#include<utility>

#include"Renderer/LowLevel/OpenGL/Renderer/3DRenderer.h"
#include"Renderer/LowLevel/OpenGL/Camera/Camera3D.h"
#include"Profiling/Timing/Benchmark.h"
std::stack<std::string> CS;


bool TEST_PROFILE_WINDOW();

/* 

IMGUI ISSUE:   Docking Branch:
    1) SetWindowPos does not function properly when we have windows docked in one another. 
    2) The Size nor the Position is properly set.
    3) Color Picker does not move with Window which has been Docked and can only be seen when Hovering the Window above the Area meaning 
       that the World Vertex Positions do not update when the Window Updates if it is docked in another.

*/


Camera3D *Camera_3D{ nullptr };

static ULARGE_INTEGER
        lastCPU,
        lastSysCPU, 
        lastUserCPU;

static int numProcessors;
static HANDLE self;

#include "TCHAR.h"
#include "pdh.h"
static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

#pragma comment(lib, "Pdh.lib")
void init_CPUmonitor() 
{
    PdhOpenQuery(NULL, NULL, &cpuQuery);
    // You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
    PdhAddEnglishCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
}

double getCurrentValueCPU()
{
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;
}


#define _PROFILE_MEMORY
// https://www.scmp.com/news/china/society/article/3075567/people-blood-type-may-be-more-vulnerable-coronavirus-china-study?utm_content=article&utm_medium=Social&utm_source=Facebook&fbclid=IwAR1VsuqHUzDgoU6-zaafrHJI89Qniyxbzppif6x_SvsC9LXHGjsfwo7tM6k#Echobox=1584436870

using namespace Core;
using namespace Threading;


/* This Camera Situation is all wrong, figure a way to fix this */

#define CAMERA_SPEED 4.0f
#define ZOOM_SPEED 1.0

Listener KeyListener([](Event _msg)
{

	DEBUG_CODE(Print("Key Entered: " << _msg.wParam));
	switch (_msg.wParam)
	{
	case 37:
	{// Left Key
        Application::getCamera().MoveX(-CAMERA_SPEED);
        Camera_3D->MoveLeft(CAMERA_SPEED);
    }
	break;

	case 38:
	{// Up  Key
        Application::getCamera().MoveY(-CAMERA_SPEED);
        Camera_3D->MoveForward(CAMERA_SPEED);

	}
	break;

	case 39:
	{// Right  Key
        Application::getCamera().MoveX(CAMERA_SPEED);
        Camera_3D->MoveRight(CAMERA_SPEED);

	}
	break;

	case 40:
	{// Down  Key
        Application::getCamera().MoveY(CAMERA_SPEED);
        Camera_3D->MoveBack(CAMERA_SPEED);
	}
    break;

	case 189: 
	{//- Key
         Application::getCamera().ZoomOut(ZOOM_SPEED);
	}break;

    case 187:
	{//+ Key
         Application::getCamera().ZoomIn(ZOOM_SPEED);
	}break;



    case 188:
    {// <  
       // static float keyRotate = Application::getCamera().g_Rotation();

        float R =(float)(DEGREES(Application::getCamera().g_Rotation()));
        Application::getCamera().s_Rotation(R - 20);// (float)(RADIANS(Rot)));
        Camera_3D->RotateY((float)-RADIANS(CAMERA_SPEED));

       // Application::getCamera().Rotate(-1);
    }
    break;

    case 190:
    {// > 
        Application::getCamera().Rotate(2);
        Camera_3D->RotateY((float)RADIANS(CAMERA_SPEED));
    }
    break;

    


	}// End of Switch

});
Listener MouseWheel( [](Event _msg)
{
    Vec2 Mpos = SplitLParam((int)_msg.lParam);

    if ((int16_t)SplitLParam((int)_msg.wParam).y > 0)
    {// Mouse Wheel UP
        Application::getCamera().ZoomInto(Mpos, ZOOM_SPEED);
        Camera_3D->MoveForward(CAMERA_SPEED);

    }
    else
    {// Mouse Wheel DOWN
        Application::getCamera().ZoomOutFrom(Mpos, ZOOM_SPEED);
        Camera_3D->MoveBack(CAMERA_SPEED);

    }
});


iVec2 GLOBAL_OldMouse{ 0 };
iVec2 GLOBAL_Delta_Mouse{ 0 };
bool Drag_Triggered{ false };



Listener Mouse_Click
(
    [](Event _msg)
{
    iVec2 NewMouse = SplitLParam((int)_msg.lParam);

    POINT Mouse_Position = { NewMouse.x, NewMouse.y};
    if (DragDetect(Application::getWindow().g_Handle(), Mouse_Position))
    {// We are Dragging the Screen, Get the Normal and the Velocity and apply to the Rotation of the 3D Camera
        GLOBAL_Delta_Mouse = NewMouse - GLOBAL_OldMouse;
        GLOBAL_OldMouse = NewMouse;

        Drag_Triggered = true;
       // Print( " Dragging the Mouse :" << NewMouse << " : " << GLOBAL_Delta_Mouse);
    }
});

 

/* ============================================================================================
           Rough Sketch of a Scene class to determine what I am going to need
           For the Finalized Version of it. Also gives me a Template to rework the 
           VertexArrayObject and VertexBufferObjects
/* ============================================================================================ */
#include"Renderer/LowLevel/OpenGL/Renderer/Renderer.h"
struct MyScene
{
public:
    using Vec_t = Vec2;

    /* Create the Object after OpenGL is Initialized */
    void Create()
    {
        /* Create all the Data we are going to fill the Buffers with */
        {
            float Max = 0;
            for_loop(i, 1000 * 3)
            {
                Vertices.push_back
                (
                    {
                        RANDOM(Max), RANDOM(Max)
                    }
                );
                Colors.push_back
                (
                    OpenGL::Renderer::normalize_RGBA_Color((int)RANDOM(255), (int)RANDOM(255), (int)RANDOM(255), (int)RANDOM(255))
                );
            }
        } 
        /* Create the FrameBuffer we will be Rendering to */
        {
            FBO = new FrameBufferObject(SCREEN_X, SCREEN_Y);
            FBO->Bind();
            {
                OpenGL::set_ClearColor(.2f, .5f, .7f, 1.0f);
            }
            FBO->Unbind();
        }
        /* Create all the Needed Buffers for this class */
        {

            RENDERER = shader_BasicRenderer;
            Vertices_VAO = new VertexArrayObject();
            Vertices_VBO = new VertexBufferObject<Vec_t>(Vertices);
            Colors_VBO   = new VertexBufferObject<Vec4>(Colors);
        }

        /* Link all the Buffers into a Single VertexArrayObject */
        {
            RENDERER->Bind();
            {
                Vertices_VAO->Attach(BufferTypes::VERTEX, Vertices_VBO);
                Vertices_VAO->Attach(BufferTypes::COLOR, Colors_VBO);
            }
            RENDERER->Unbind();
        }
    }

    /* Update the Positions of the Vertices Once Per Frame */
    void Update() trace(1)
    {
        float Amount = 1;

        for_loop(i, 1000 * 3)
        {
            Vertices[i] = Vec2
            (
                Vertices[i].x + RANDOM_RANGE(Amount),
                Vertices[i].y + RANDOM_RANGE(Amount)
            );
        }

        /* Update my Buffer Object */
        Vertices_VAO->Bind();
        {
            Vertices_VBO->Bind();
            {
                OpenGL::set_Subbuffer_Data(Vertices.size() * sizeof(Vec2), Vertices.data(), 0);
            }
            Vertices_VBO->Unbind();

        }
        Vertices_VAO->Unbind();
        Return();
    }

    /* Render the Entire Scene */
    void Render() trace(1)
    {
        FBO->Bind();
        {
            trace_scope("FBO_bind");
            FBO->Clear();
            RENDERER->Bind();
            {
                trace_scope("Shader_Bind");
                Vertices_VAO->Bind();
                {
                    trace_scope("VAO_Bind");
                    Application::getCamera().Bind();
                    OpenGL::Renderer::drawArray(Vertices.size());
                }
                Vertices_VAO->Unbind();
            }
            RENDERER->Unbind();
        }
        FBO->Unbind();
        Return();
    }

    Shader                        *RENDERER{ nullptr };
    FrameBufferObject                  *FBO{ nullptr };
    VertexArrayObject         *Vertices_VAO{ nullptr };
    VertexBufferObject<Vec_t> *Vertices_VBO{ nullptr };
    VertexBufferObject<Vec4>    *Colors_VBO{ nullptr };

    std::vector<Vec_t> Vertices;          
    std::vector<Vec4>  Colors;

};



/* ============================================================================================
      Main Application class which holds all the functionality and Data for our program 
/* ============================================================================================ */
class App
	: public Application
{
    //=================================================================================================================================================================
    //                                                USER VARIABLES
    //=================================================================================================================================================================

    Renderer_test *Bucket_Test;
    OpenGL::Renderer2D *test_Renderer2D{ nullptr };
   
    OpenGL::Renderer3D *test_Renderer3D{ nullptr };

    OpenGL::RenderPass *test_RenderPass;
    MyScene SCENE;
    Texture *test_Texture;
    size_t  PreviousTime;
    uint32_t *Pixels;


    Texture *test_Texture2;
    Texture *test_Texture3;
    Texture *White;
    Texture *Black;

    OpenGL::Surface *testSurface{ nullptr };

    /* Initializes User Variables */
    virtual void OnCreate() override trace(1)
    {// Initialization

        // TEST_ASSERT( TEST_Memory_Pool_Class() , " Memory Pool Class Incomplete contains Errors " , " Memory Pool Class Complete ");
        // TEST_ASSERT( Creatures::TEST_SPRINGS(), " Springs Class Incomplete  contains Errors ", " Springs Class Complete ");
        // TEST_ASSERT( TEST_Ring_Buffer_Class() , " Ring Buffer Class Incomplete  contains Errors " , " Ring Buffer Class Complete ");

        init_DefaultShaders();        /// Likely add this to the Base Application class to relieve the user of this. Create a setup that only Initializes the desired shaders

        /* Load up the Listeners for the Various Input Events */
        {
            RegisterListener(WM_KEYDOWN, KeyListener);
            RegisterListener(WM_MOUSEWHEEL, MouseWheel);
            RegisterListener(WM_MOUSEMOVE, Mouse_Click);
        }

        /* Creates Different Test Renderers for our Application to try out */
        test_Renderer2D = new OpenGL::Renderer2D({ SCREEN_X, SCREEN_Y });
        test_Renderer3D = new OpenGL::Renderer3D({ SCREEN_X, SCREEN_Y });

        test_Renderer2D->Attach
        (
            new Camera2D
            (
            (float)Application::getWindow().Width(),
                (float)Application::getWindow().Height()
            )
        );
        Application::setCamera(test_Renderer2D->g_Camera());


        Camera_3D = &test_Renderer3D->g_Camera();
        SCENE.Create();

        testSurface = test_RenderPass->new_Surface({ SCREEN_X, SCREEN_Y });
        test_RenderPass = new OpenGL::RenderPass(SCREEN_X, SCREEN_Y, shader_BasicRenderer);// SCENE.RENDERER);
        Bucket_Test = new Renderer_test(&test_Renderer2D->g_Camera());


        std::vector<Vec2> Verts;
        std::vector<Vec4> Cols;

        for_loop(z, 10)
        {
            for_loop(y, 10)
            {
                for_loop(x, 10)
                {
                    test_Renderer3D->renderCube({ x,y,z }, RANDOM(30));
                }
            }
        }
    
        /* Create a Bunch of Quads to Test Render */
        {
            uint8_t R{ 100 }, G{ 0 }, B{ 0 };
            Vec2 Sz{ 100, 100 };
            for_loop(y, Sz.y)
            {
                for_loop(x, Sz.x)
                {
                    R += 5;
                    if (R >= 255) { G += 5; R = 0; }
                    if (G >= 255) { B += 5; G = 0; }

                    test_Renderer2D->renderQuad
                    (
                        { x * 8.0f, y * 8.0f },
                        { 7, 7 },
                        OpenGL::Renderer::normalize_RGBA_Color(R, G, B, 255)
                    );
                    test_Renderer2D->draw_Line(x * 8.0f, y * 8.0f, x * 8.0f + 7, y * 8.0f + 7);

                    Verts.push_back({ x * 8.0f, y * 8.0f });
                    Verts.push_back({ x * 8.0f + 100, y * 8.0f });
                    Verts.push_back({ x * 8.0f, y * 8.0f + 100 });
                    Verts.push_back({ x * 8.0f + 100, y * 8.0f + 100 });
                    Verts.push_back({ x * 8.0f, y * 8.0f + 100 });
                    Verts.push_back({ x * 8.0f + 100, y * 8.0f });
                    Cols.push_back(OpenGL::Renderer::normalize_RGBA_Color(R, G, B, 255));
                    Cols.push_back(OpenGL::Renderer::normalize_RGBA_Color(R, G, B, 255));
                    Cols.push_back(OpenGL::Renderer::normalize_RGBA_Color(R, G, B, 255));
                    Cols.push_back(OpenGL::Renderer::normalize_RGBA_Color(R, G, B, 255));
                    Cols.push_back(OpenGL::Renderer::normalize_RGBA_Color(R, G, B, 255));
                    Cols.push_back(OpenGL::Renderer::normalize_RGBA_Color(R, G, B, 255));
                }
            }
        }

        test_RenderPass->attach(&Application::getCamera());;//test_Renderer2D->g_Camera())&Application::getCamera());//
        test_RenderPass->attach(SCENE.Vertices_VAO);

        //   [ ] FIX THE GOD DAMN CAMERA ALREADY WILL YOU. 
        //   [X] MAKE SURE IT UPDATES PROPERLY, 
        //   [X] COVERS PROPER AREA, 
        //   [X] ATTACHES PROPER UNIFORMS 
        //   [ ] FUCKING MAKE IT PROPER GOD DAMNIT

        White = new Texture("../Resources/White.bmp");
        Black = new Texture("../Resources/Black.bmp");
        test_Texture = new Texture("../Test2.bmp");
        test_Texture2 = new Texture("../Resources/Test.bmp");
        test_Texture3 = new Texture("../Resources/Test2.bmp");
        Texture *ttt = new Texture("../Resources/Texture.bmp");
        Renderer_test::Material *MAT = new Renderer_test::Material(shader_BasicRenderer, test_Texture2);
        Renderer_test::Material *MAT2 = new Renderer_test::Material(shader_BasicRenderer, ttt);

        float sz = 1000, sy = 1000;
        std::vector<Vec2> Quad = make_Quad({ 10, 10 }, { sz, sy });
        std::vector<Vec2>UVcoord;
        UVcoord.push_back({  (Quad[0].x / sz + 1) * 0.5f ,   (Quad[0].y / sy + 1) * 0.5f  });
        UVcoord.push_back({  (Quad[1].x / sz + 1) * 0.5f ,   (Quad[1].y / sy + 1) * 0.5f  });
        UVcoord.push_back({  (Quad[2].x / sz + 1) * 0.5f ,   (Quad[2].y / sy + 1) * 0.5f  });
        UVcoord.push_back({  (Quad[3].x / sz + 1) * 0.5f ,   (Quad[3].y / sy + 1) * 0.5f  });
        UVcoord.push_back({  (Quad[4].x / sz + 1) * 0.5f ,   (Quad[4].y / sy + 1) * 0.5f  });
        UVcoord.push_back({  (Quad[5].x / sz + 1) * 0.5f ,   (Quad[5].y / sy + 1) * 0.5f  });
      
        std::vector<Vec4> C;
        C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });        C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });        C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });        C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });        C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });        C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });
        std::vector<Vec2>UVcoord2;
        for (auto& C : Verts)
        {
            UVcoord2.push_back({ 1 / C.x, 1 / C.y });
        }

        for_loop(i, 10)
        {
            Bucket_Test->Submit
            (
                new Renderer_test::Geometry(new VertexBufferObject<Vec2>(make_Quad({ RANDOM(1300), RANDOM(1300) }, { RANDOM(100), RANDOM(100) })), new VertexBufferObject<Vec4>(C), new VertexBufferObject<Vec2>(UVcoord)),
                MAT
            );
        }


        for_loop(i, 10)
        {
            Bucket_Test->Submit
            (
                new Renderer_test::Geometry(new VertexBufferObject<Vec2>(make_Quad({ RANDOM(3000), RANDOM(3000) }, { RANDOM(100), RANDOM(100) })), new VertexBufferObject<Vec4>(C), new VertexBufferObject<Vec2>(UVcoord)),
                MAT2
            );
        }

       /// IF DEBUG
        OpenGL::enable_DebugOutput();

        void init_CPUmonitor();

        DEBUG_CODE(CheckGLERROR());
        Return();
    }

    /* Renders User Defined Geometry */
    virtual void OnRender() override trace(1)
    {
        /// Currently if ANY Camera is bound before this call it trashes the test_Renderer2Ds Camera
        /// It is possibly not the Camera but the Shader Instead. 
        /// Would like to Detach all these systems as seperate entities and make them their own form of DrawCalls.
        {
            trace_scope("test_Renderer2D");
            test_Renderer2D->Render();
        }
        {
            trace_scope("SceneRender");
            SCENE.Render();
        }

        {
            trace_scope("Test RenderPass");
            test_RenderPass->Render();
        }

        {
            trace_scope("Blit FrameBuffer");
            testSurface->blit_FrameBuffer(SCENE.FBO, { 1,1, SCREEN_X, SCREEN_Y }, { 1, 1, SCREEN_X * 2, SCREEN_Y * 2 });
        }

        {
            trace_scope("Bucket_Test");
            Bucket_Test->Render();
        }
        {
            trace_scope("test_Renderer3D");
            test_Renderer3D->Render();
        }

        DEBUG_CODE(CheckGLERROR());
        Return();
	}

    /* Runs on Applications Frame Update */
    virtual void OnUpdate() override trace(1)
    {
        test_Renderer3D->Main_Camera->s_Rotation
        (
            {
                RADIANS(GLOBAL_Delta_Mouse.x),
                0,
                RADIANS(GLOBAL_Delta_Mouse.y)
            }
        );

        test_Renderer3D->Update();
            
        if (Update_Geometry)
        {
            trace_scope("UpdateGeometry")
            SCENE.Update();
        }

		size_t NewTime = Timing::Timer<Milliseconds>::GetTime();
    	size_t Time = Timing::Timer<Milliseconds>::GetTime() - PreviousTime;
		PreviousTime = NewTime;

        Return();
	}

    /* Cleans up the Memory of stuff we still have Active */
    virtual void OnEnd() override
    {
        delete(test_Renderer2D);
        delete(test_Texture);
    }

    /* ==========================================================================================
           GUI STUFF, POTENTIALLY MIGHT CHANGE THIS LATER
    /* ========================================================================================== */

    int   Update_Interval { 60 };
    int   Update_counter  { Update_Interval - 1 };
    Vec2  Camera_Position { 0,0 }; 
    bool  Update_Geometry { true };
    float Rot             { 0 };

    float Top_Time{ 0 };

    std::vector<float> memUsage;
    std::vector<double> CPUUsage;

    /* Renders our ImGui Data */
    
    virtual void OnRenderGUI() override trace(1)
    {

        static float Slide[3] =  {-(SCREEN_X * 0.5f), -(SCREEN_Y * 0.5f), 0 };

        Vec2 MainWindowSize = getWindow().g_Size();
        int Boarder = 10;
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.25f, 0.25f, 0.25f, 1.0f)); // Set window background to red
        // Start the Camera Widget 
        ImGui::Begin("Camera");
        {

            ImGui::SetWindowPos(ImVec2(MainWindowSize.x - (MainWindowSize.x * 0.15f), 0), true);
            ImGui::SetWindowSize({ MainWindowSize.x * 0.15f, MainWindowSize.y - ((MainWindowSize.y  * 0.2f) ) });

            float
                H = (float)Application::getCamera().Height(),
                W = (float)Application::getCamera().Width();

            float Z = (float)Application::getCamera().g_Zoom();


            // Position Slider for the Camera 
            {
                ImGui::InputFloat("X", &Camera_Position.x, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Y", &Camera_Position.y, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Zoom", &Z, .1f, 1.0f, "%.3f");
            }


            // Rotation Slider for Camera
            {
                Application::getCamera().Translate(Camera_Position);
                Application::getCamera().s_Zoom(Z);
            }

            // Set the Rotation of the Default Framebuffer
            {
               
                
                ImGui::InputFloat("Rotation", &Rot, 1.0f, 1.0f, "%.3f");
                float R = (float)(DEGREES(Application::getCamera().g_Rotation())); 
                Application::getCamera().Rotate(Rot - R);// (float)(RADIANS(Rot)));
            }
            

            // Clickable Position Slider for the Camera 
            {
                ImGui::InputFloat("Width", &W, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Height", &H, 10.0f, 1.0f, "%.3f");
                Application::getCamera().Resize( { W,H });
            }

            // Camera for Test 3D Renderer
            {
                ImGui::InputFloat("3D Px", &Camera_3D->Target_Position.x, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("3D Py", &Camera_3D->Target_Position.y, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("3D Pz", &Camera_3D->Target_Position.z, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Rotation X", &Camera_3D->Rotation.x, .1f, 1.0f, "%.3f");
                ImGui::InputFloat("Rotation Y", &Camera_3D->Rotation.y, .1f, 1.0f, "%.3f");
                ImGui::InputFloat("Rotation Z", &Camera_3D->Rotation.z, .1f, 1.0f, "%.3f");
            }

            // Color Picker Widget for the Background Color 
            {
                Vec4 c = getWindow().g_ClearColor();
                float Col[3] = { c.x,c.y,c.z };
                ImGui::ColorPicker3("Background Color", Col, 0);
                glClearColor(Col[0], Col[1], Col[2], 1);
                getWindow().s_ClearColor({ Col[0], Col[1], Col[2], 1 });
            }


            // FPS Times
            {
                if (FrameTimes.back() > Top_Time)
                {
                    Top_Time = FrameTimes.back();
                }
                ImGui::PlotLines("Frame Times", (float*)&FrameTimes.front(), FrameTimes.size(), 0, std::string("FPS:" + std::to_string((uint32_t)FrameTimes.back())).c_str(), 0, Top_Time, ImVec2(ImGui::GetWindowWidth(), 50));
            }

            // Gets the Memory and the CPU Usage 
            {
                if (++Update_counter % Update_Interval == 0)
                {

                    MEMORYSTATUSEX memInfo;
                    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
                    GlobalMemoryStatusEx(&memInfo);

                    GlobalMemoryStatusEx(&memInfo);
                    memUsage.push_back(25000 - ((float)memInfo.ullAvailPhys / 1024 / 1024));
                    CPUUsage.push_back(getCurrentValueCPU());
                }
                ImGui::PlotLines("Memory Usage", (float*)&memUsage.front(), memUsage.size(), 0, std::string("Memory:" + std::to_string(25000 + (uint32_t)memUsage.back())).c_str(), 0, 2000, ImVec2(ImGui::GetWindowWidth(), 50));
                ImGui::PlotLines("CPU Usage", (float*)&CPUUsage.front(), CPUUsage.size(), 0, std::string("CPU%:" + std::to_string((uint32_t)CPUUsage.back())).c_str(), 0, 100, ImVec2(ImGui::GetWindowWidth(), 50));
            }

            ImGui::Checkbox("Update Geometry", &Update_Geometry);
        }
        ImGui::End();



        // Displays the FrameBuffers 
        ImGui::Begin("FrameBuffers");
        {
            
            // Displays the FrameBuffer 
            float WinSize = (float)(ImGui::GetWindowHeight() - (ImGui::GetWindowHeight() * 0.15));
            ImGui::Image((ImTextureID*)((size_t)SCENE.FBO->RenderTarget->g_Handle()), ImVec2(WinSize, WinSize));
            ImGui::SameLine();
            ImGui::Image((ImTextureID*)((size_t)SCENE.FBO->DepthTarget->g_Handle()), ImVec2(WinSize, WinSize));
            ImGui::SameLine();
        

            ImGui::Image((ImTextureID*)((size_t)test_RenderPass->FBO->RenderTarget->g_Handle()), ImVec2(WinSize, WinSize));
            ImGui::SameLine();


            ImGui::Image((ImTextureID*)((size_t)testSurface->FBO->RenderTarget->g_Handle()) , ImVec2(WinSize, WinSize));
            ImGui::SameLine();
        
        
            char *Buff = const_cast<char*>(OpenGL_ErrorList.c_str());
            ImGui::InputTextMultiline("OpenGL Errors",Buff, OpenGL_ErrorList.size());

            ImGui::SetWindowPos(ImVec2(0, MainWindowSize.y - (MainWindowSize.y  * 0.2f)), true); 
            ImGui::SetWindowSize({ MainWindowSize.x, (MainWindowSize.y  * 0.2f)  - Boarder});
        }
        ImGui::End();



        ImGui::Begin("Textures");
        {// Displays ALL the created Textures
            
            float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
            float WinSize = 200; 
            ImGuiStyle& style = ImGui::GetStyle();
            uint32_t n = 0;
            for (auto& T : Texture::Texture_Resources)
            {
                // Displays the FrameBuffer  
                ImGui::Image((ImTextureID*)((size_t)T->g_Handle()), ImVec2(WinSize, WinSize));
                float last_button_x2 = ImGui::GetItemRectMax().x;
                float next_button_x2 = last_button_x2 + style.ItemSpacing.x + WinSize; // Expected position if next button was on same line
                if (n + 1 < Texture::Texture_Resources.size() && next_button_x2 < window_visible_x2)
                {
                    ImGui::SameLine();
                }
                ++n;

            }

            ImGui::ShowDemoWindow();
            ImGui::SetWindowPos(ImVec2(0, 0), true); // 768 - ImGui::GetWindowHeight() - 40MainWindowSize.x * 0.15f
            ImGui::SetWindowSize({ ImGui::GetWindowWidth(), MainWindowSize.y - (MainWindowSize.y  * 0.2f) });
        }
        ImGui::End();
        ImGui::PopStyleColor();
        Return();
    } 
    
};


int main()
{
    App MyApp;

    PROFILE_BEGIN_SESSION("Init", "ProfileInitResults.json");
    {
        MyApp.Init();
    }
    PROFILE_END_SESSION();


    PROFILE_BEGIN_SESSION("Run", "ProfileRunResults.json");
    {
        MyApp.Run();
    }
    PROFILE_END_SESSION();

	//	Profiling::Memory::TrackDumpBlocks();
	//	Profiling::Memory::TrackListMemoryUsage();
	return 0;


    DEBUGPrint(CON_Red, "Problem with the Debug print Symbols hConsole and DEBUGMutex in that they have no real home on where they should be. It is possible Common.h should perhaps have a CPP file for Symbol definition");
 
    TODO("Fucked up. It is all fucked up. Cube Instance Renderer3D is not properly rendering a damn thing. Rotate clicker on ImGui on the 2D render is not properly rotating and just spins out of control rapidly. \n ImGui appears to be sticking and this should be one of the first things fixed because it is fucking annoying.");
    TODO(" Setup Mock ups which use the Application class to setup a state in a way that I can test various functionality by switching through different applications. /n Each Module should have its very own Application class. ");
    TODO(" Serious Restructuring needs to take place to the Entire project, it is starting to grow rather large and I am not happy with some of the early design and structure decisions that have begun to slightly conflict. \n\
 It would be wise to reformat and refactor the project before these minor issues become big ones | HA!! Dumbass! You never fuckin listen! ");

}
  



/*
=====================================================================================================================
									  NOTES:
=====================================================================================================================

 Found 100
 Cancer found it 5pm 3/4/2020 Moon in Cancer Sun in Pisces 

 Lost Art of Structure Packing:
 http://www.catb.org/esr/structure-packing/

 Nice
 http://eel.is/c++draft/meta.trans.other

 Getting the Stack frame caller
 https://social.msdn.microsoft.com/Forums/vstudio/en-US/da4e85c5-407e-4a57-87e7-14bf994504d9/error-asm-keyword-not-supported-on-this-architecture?forum=vcgeneral
 
 pragma message (__FILE__ "[" STRING(__LINE__) "]: test")
 https://docs.microsoft.com/en-us/cpp/preprocessor/pragma-directives-and-the-pragma-keyword?view=vs-2019

 OpenGL & Window System Integration
 https://www.cs.montana.edu/courses/fall2004/425/course24.pdf

 Pragmas for C++ Compilers, Good resource
 https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.cbclx01/prag_ishome.htm

 Open STD:
 http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3857.pdf

 Actor based system
 http://www.actor-framework.org/pdf/manual.pdf

 Physics Solver:
 https://www.gdcvault.com/play/1013359/High-Performance-Physics-Solver-Design

 Advanced Programming with Microsoft Quick C
 https://books.google.com/books?id=IjCjBQAAQBAJ&pg=PA41&lpg=PA41&dq=%23pragma+check_stack(+%5B%7B+on+%7C+off+%7D%5D+)+what+does+it+do&source=bl&ots=44JLKNArHT&sig=ACfU3U3eSXG1JpiNDv9IgKbrUv3ByJrJqw&hl=en&sa=X&ved=2ahUKEwj3rrKygeTmAhVxkuAKHbx8C1YQ6AEwAXoECAkQAQ#v=onepage&q=%23pragma%20check_stack(%20%5B%7B%20on%20%7C%20off%20%7D%5D%20)%20what%20does%20it%20do&f=false
=====================================================================================================================
*/

/*

    https://en.cppreference.com/w/cpp/compiler_support



             Great List of Compiler Directives.
    Likely can eliminate verything else in this Comment because of this
    https://assets.ctfassets.net/9pcn2syx7zns/61QggiXm0CGYmsJEUfwjld/682ad1c09e795402d3c5f10c009985ad/preprocessor.pdf
    
    
    int _inp(ushort _port);                          Reads a Port
    int _outp(unsigned short port, int databyte)    Writes to a Port
    
    #pragma auto_inline( [ { on | off } ] )
    
    #pragma loop( hint_parallel( n ) )
    #pragma loop( no_vector )
    #pragma loop( ivdep )
    
    #pragma omp [directive]
    			 parallel      Defines a parallel region, which is code that will be executed by multiple threads in parallel.
    			 for           Causes the work done in a for loop inside a parallel region to be divided among threads.
    			 sections      Identifies code sections to be divided among all threads.
    			 single        Lets you specify that a section of code should be executed on a single thread, not necessarily the master thread.
    
    			 For master and synchronization:
    			 Directive     Description
    
    			 master        Specifies that only the master thread should execute a section of the program.
    			 critical      Specifies that code is only executed on one thread at a time.
    			 barrier       Synchronizes all threads in a team; all threads pause at the barrier, until all threads execute the barrier.
    			 atomic        Specifies that a memory location that will be updated atomically.
    			 flush         Specifies that all threads have the same view of memory for all shared objects.
    			 ordered       Specifies that code under a parallelized for loop should be executed like a sequential loop.
    
    			 threadprivate Specifies that a variable is private to a thread.
*/

/*
                  FASTEST RELEASE SETTINGS IN MSVC
                 ==================================
Disable Vtable Init         	__declspec (novtable)	Stops compiler from generating code to initialize the vfptr in the constructor. Apply to pure interface classes.
No Throw	                    __declspec (nothrow)	Stops compiler from tracking unwindable objects. Apply to functions that don’t throw exceptions. Recommend using the Std C exception specification throw() instead.
Pentium Pro	                    /G6	Optimize for PentiumPro and above (program might not run on Pentium)
Windows	                        /GA	Optimize for Windows
Fastcall	                    /Gr	Fastest calling convention
String pooling RO	            /GF	Merge duplicate strings into one read-only memory location
Disable RTTI	                /GR-	Turn off run time type information.
Stack probes off	            /Gs	Turn off stack checking
Exception handling	            /GX-	Turns off exception handling (assumes program isn’t using excptn handling)
Func-level linking	            /Gy	Only include functions that are referenced
Assume no aliasing	            /Oa	Assume no aliasing occurs within functions
Inline any or inline expansion	/Ob2 or /Ob1	Inline any function deemed appropriate by compiler or turn inlining on. Alternates: inline, __forceinline
Global opts	                    /Og	Full loop, common subexpression and register optimizations
Intrinsic functions	            /Oi	Replaces specific functions with inline versions (memcpy, strcpy, etc.)
Fast code	                    /Ot	Favor code speed over size
Omit frame pointer	            /Oy	Omit frame pointer
Ctor displacement	            /vd0	Disable constructor displacement.
Best case ptrs	                /vmb	Use best case “pointer to class member” representation

*/


 /*
 
 add_executable (${PROJECT_NAME} ../Bin/glad/src/glad.c    Core/Application.cpp    Core/ECS/ECS.cpp   Core/ECS/ECScomponent.cpp    Core/ECS/ECSsystem.cpp    Core/ECS/TestComponents.cpp    Core/EventSystem.cpp    Core/Math/Easing.cpp  
 Core/Math/Math.cpp    Core/Memory.cpp    Core/Observer.cpp    Core/Threading/TestFunctions.cpp    Core/Threading/Threadpool.cpp    Core/Utility.cpp    Creatures/AI/Evolution.cpp    Creatures/AI/NeuralNetwork.cpp    Creatures/Creatures.cpp    Creatures/Physics/Springs.cpp    Physics/Colliders.cpp    Physics/Physics.cpp    Physics/Quadtree.cpp    Profiling/MemoryPerf/MemTracker.cpp    Profiling/SystemInfo.cpp    Profiling/Timing/Profiling.cpp  
 Renderer/Layer.cpp    Renderer/LowLevel/Materials/Image/Bitmap.cpp    Renderer/LowLevel/Materials/Image/Textures.cpp    Renderer/LowLevel/OpenGL/Camera/Camera.cpp    Renderer/LowLevel/OpenGL/Camera/Camera2D.cpp    enderer/LowLevel/OpenGL/Camera/Camera3D.cpp    Renderer/LowLevel/OpenGL/OpenGL.cpp    Renderer/LowLevel/OpenGL/Renderer/2DRenderer.cpp    Renderer/LowLevel/OpenGL/Renderer/GLBuffers.cpp    Renderer/LowLevel/OpenGL/Renderer/Mesh.cpp   Renderer/LowLevel/OpenGL/Renderer/Pipeline.cpp    Renderer/LowLevel/OpenGL/Renderer/Sprite.cpp    Renderer/LowLevel/OpenGL/Renderer/Transform.cpp    Renderer/LowLevel/OpenGL/Shader/Builtin_Shaders.cpp    Renderer/LowLevel/OpenGL/Shader/Shader.cpp    Renderer/LowLevel/OpenGL/UniformBuffer.cpp   
)
 
 cmake_minimum_required(VERSION 3.10)

project(CreatureLibrary)

add_executable (${PROJECT_NAME} ../Bin/glad/src/glad.c    Core/Application.cpp    Core/ECS/ECS.cpp   Core/ECS/ECScomponent.cpp    Core/ECS/ECSsystem.cpp    Core/ECS/TestComponents.cpp    Core/EventSystem.cpp    Core/Math/Easing.cpp  
 Core/Math/Math.cpp    Core/Memory.cpp    Core/Observer.cpp    Core/Threading/TestFunctions.cpp    Core/Threading/Threadpool.cpp    Core/Utility.cpp    Creatures/AI/Evolution.cpp    Creatures/AI/NeuralNetwork.cpp    Creatures/Creatures.cpp    Creatures/Physics/Springs.cpp    Physics/Colliders.cpp    Physics/Physics.cpp    Physics/Quadtree.cpp    Profiling/MemoryPerf/MemTracker.cpp    Profiling/SystemInfo.cpp    Profiling/Timing/Profiling.cpp  
 Renderer/Layer.cpp    Renderer/LowLevel/Materials/Image/Bitmap.cpp    Renderer/LowLevel/Materials/Image/Textures.cpp    Renderer/LowLevel/OpenGL/Camera/Camera.cpp    Renderer/LowLevel/OpenGL/Camera/Camera2D.cpp    enderer/LowLevel/OpenGL/Camera/Camera3D.cpp    Renderer/LowLevel/OpenGL/OpenGL.cpp    Renderer/LowLevel/OpenGL/Renderer/2DRenderer.cpp    Renderer/LowLevel/OpenGL/Renderer/GLBuffers.cpp    Renderer/LowLevel/OpenGL/Renderer/Mesh.cpp   Renderer/LowLevel/OpenGL/Renderer/Pipeline.cpp    Renderer/LowLevel/OpenGL/Renderer/Sprite.cpp    Renderer/LowLevel/OpenGL/Renderer/Transform.cpp    Renderer/LowLevel/OpenGL/Shader/Builtin_Shaders.cpp    Renderer/LowLevel/OpenGL/Shader/Shader.cpp    Renderer/LowLevel/OpenGL/UniformBuffer.cpp   
)

#add_library(A STATIC SOIL.lib)
target_link_libraries(LINK_PUBLIC SOIL.lib)
                   
include_directories(${PROJECT_SOURCE_DIR}  ../Bin/ )

Add any required libraries to the target_link_libraries line. For example:

target_link_libraries (${PROJECT_NAME} applibs pthread gcc_s c mycustomlibrary)


$(VC_LibraryPath_x64); $(WindowsSDK_LibraryPath_x64);$(NETFXKitsDir)Lib/um/x64;$(Solution)../Bin/Soil/Debug/

/OUT:"C:\Users\curti\Source\Repos\xSeditx\Creature-Engine\x64\Debug\CreatureEngine.lib" "SOIL.lib" /MACHINE:X64 /NOLOGO 


add_library(B STATIC b.c)
target_link_libraries(A B)
target_link_libraries(B A)
add_executable(main main.c)
target_link_libraries(main A)
   
   HEADERS
  =========
    Core/Application.h   
    Core/Common.h   
    Core/Defines.h   
    Core/ECS/ECS.h   
    Core/ECS/ECScomponent.h   
    Core/ECS/ECSsystem.h   
    Core/ECS/TestComponents.h   
    Core/EventSystem.h   
    Core/Math/Easing.h   
    Core/Math/Math.h   
    Core/Memory.h   
    Core/Observer.h   
    Core/Threading/Future.h   
    Core/Threading/TestFunctions.h   
    Core/Threading/Threadpool.h   
    Core/Utility.h   
    Creatures/AI/Evolution.h   
    Creatures/AI/NeuralNetwork.h   
    Creatures/Creatures.h   
    Creatures/Physics/Springs.h   
    Physics/Colliders.h   
    Physics/Physics.h   
    Physics/Quadtree.h   
    Profiling/MemoryPerf/MemTracker.h   
    Profiling/RenderUtilities.h   
    Profiling/SystemInfo.h   
    Profiling/Timing/Benchmark.h   
    Profiling/Timing/Timer.h   
    Renderer/Layer.h   
    Renderer/LowLevel/Materials/Image/Bitmap.h   
    Renderer/LowLevel/Materials/Image/Texture.h   
    Renderer/LowLevel/OpenGL/Camera/Camera.h   
    Renderer/LowLevel/OpenGL/Camera/Camera2D.h   
    Renderer/LowLevel/OpenGL/Camera/Camera3D.h   
    Renderer/LowLevel/OpenGL/OpenGL.h   
    Renderer/LowLevel/OpenGL/Renderer/2DRenderer.h   
    Renderer/LowLevel/OpenGL/Renderer/GameObject.h   
    Renderer/LowLevel/OpenGL/Renderer/GLBuffers.h   
    Renderer/LowLevel/OpenGL/Renderer/Mesh.h   
    Renderer/LowLevel/OpenGL/Renderer/Pipeline.h   
    Renderer/LowLevel/OpenGL/Renderer/Primitives.h   
    Renderer/LowLevel/OpenGL/Renderer/Renderer.h   
    Renderer/LowLevel/OpenGL/Renderer/Sprite.h   
    Renderer/LowLevel/OpenGL/Renderer/Transform.h   
    Renderer/LowLevel/OpenGL/Shader/Shader.h   
    Renderer/LowLevel/OpenGL/UniformBuffer.h   
*/