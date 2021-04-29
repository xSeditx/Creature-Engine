


#include <Windows.h>
#include <array>
#include <cmath>
#include <stack>
#include <string>
#include <utility>
#include <iostream>
 
std::stack<std::string> CS;
 

#include "../CreatureEngine/Creature_Engine.h"
#include "userLayers.h"


bool TEST_PROFILE_WINDOW();

/* 

IMGUI ISSUE:   Docking Branch:
    1) SetWindowPos does not function properly when we have windows docked in one another. 
    2) The Size nor the Position is properly set.
    3) Color Picker does not move with Window which has been Docked and can only be seen when Hovering the Window above the Area meaning 
       that the World Vertex Positions do not update when the Window Updates if it is docked in another.
*/


 
 
//bool Drag_Triggered{ false };

using namespace Core;
using namespace Threading;

struct Scene_Layer
    : Layer
{
    /* Create the Object after OpenGL is Initialized */
    void OnAttach() override
        trace(1)
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
            Vertices_VBO = new VertexBufferObject<Vec2>(Vertices);
            Colors_VBO = new VertexBufferObject<Vec4>(Colors);
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
        Return()
    }

    /* Update the Positions of the Vertices Once Per Frame */
    void OnUpdate() override
        trace(1)
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
    void OnRender() override
        trace(1)
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
    VertexBufferObject<Vec2>  *Vertices_VBO{ nullptr };
    VertexBufferObject<Vec4>    *Colors_VBO{ nullptr };

    std::vector<Vec2> Vertices;
    std::vector<Vec4>  Colors;
};

#include"../CreatureEngine/Physics/Physics.h"
struct Phys_object
{
    void add(Vec3 _vert)
    {
        Vertices.push_back(_vert);
    }
    std::vector<Vec3> Vertices{};
};
class Physics_App
    : public Application
{
public:
    Physics_App(int _x, int _y, std::string _name)
        :Application(_x, _y, _name){}

    Vec2 Screen_size{ 0,0 };
    Camera2D Camera;
    OpenGL::Renderer2D *object_renderer{ nullptr };
    Simulation<Phys_object> *Physics_Simulation;

    Phys_object P_object;
    virtual void OnCreate() override
    {
        std::vector<void *> Vector_test;
        Vec3 d{ 0,1,1 };
        Vector_test.push_back((void*)&d);
        d = Vec3(1, 0, 0);
        Vec3 B = *(Vec3*)Vector_test.back();
        s_Name("Physics_Sim");
        init_DefaultShaders();
        Screen_size = { (float)Application::getWindow().Width(), (float)Application::getWindow().Height() };
        object_renderer = new OpenGL::Renderer2D(Screen_size);
        Camera = Camera2D(Screen_size);
        object_renderer->Attach(&Camera);
        Application::setCamera(Camera);

        P_object.add({ 0, 0, 0 });
        P_object.add({ 0, 100, 0 });
        P_object.add({ 100, 100, 0 });
        
    }
    virtual void OnMouseMove(Window *_window, iVec2 _position, iVec2 _relative, uint32_t _button){}
    virtual void OnKeydown(Window *_window, uint32_t _keyCode, uint32_t _scanCode, bool _repeat)
    {
        Print("Keypressed = " << _keyCode);
        switch (_keyCode)
        {
        case 27:
        {
            Terminate();
        }
        break;
        }
    }
    virtual void OnRender() override
    {

    }
    virtual void OnUpdate() override{}
};

 














/* ============================================================================================
      Main Application class which holds all the functionality and Data for our program 
/* ============================================================================================ */
class App
	: public Application
{
public:
public:
    App(int _x, int _y, std::string _name)
        :Application(_x, _y, _name) {}
    //=================================================================================================================================================================
    //                                                USER VARIABLES
    //=================================================================================================================================================================
    Model_Importer *importer;// ("../Resources/Models/Mech/Drone.fbx");

    Renderer_test *Bucket_Test;
    OpenGL::Renderer2D *test_Renderer2D{ nullptr };
    OpenGL::Renderer3D *test_Renderer3D{ nullptr };
    OpenGL::RenderPass *test_RenderPass;
 //   MyScene SCENE;
    Texture *test_Texture;
    size_t  PreviousTime;
    uint32_t *Pixels;


    Texture *test_Texture2;
    Texture *test_Texture3;
    Texture *White;
    Texture *Black;


    OpenGL::Surface *testSurface{ nullptr };
    Scene_Layer *SCENE;

    float CAMERA_SPEED = 4.0f;
    /* Initializes User Variables */
    virtual void OnCreate() override
    {// Initialization
        // Application::get().Resize({ sz.x, sz.y });//1936-1136
        // TEST_ASSERT( TEST_Memory_Pool_Class() , " Memory Pool Class Incomplete contains Errors " , " Memory Pool Class Complete ");
        // TEST_ASSERT( Creatures::TEST_SPRINGS(), " Springs Class Incomplete  contains Errors ", " Springs Class Complete ");
        // TEST_ASSERT( TEST_Ring_Buffer_Class() , " Ring Buffer Class Incomplete  contains Errors " , " Ring Buffer Class Complete ");
        init_DefaultShaders();        /// Likely add this to the Base Application class to relieve the user of this. Create a setup that only Initializes the desired shaders
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
        Layers.push_back(new guiLayer());
        ((guiLayer*)Layers.back())->Camera_3D = &test_Renderer3D->g_Camera();

        SCENE = new Scene_Layer();
        Layers.push_back(SCENE);


        testSurface = test_RenderPass->new_Surface({ SCREEN_X, SCREEN_Y });
        Bucket_Test = new Renderer_test(&test_Renderer2D->g_Camera());
        test_RenderPass = new OpenGL::RenderPass(SCREEN_X, SCREEN_Y, shader_BasicRenderer); 


        std::vector<Vec2> Verts{};
        std::vector<Vec4> Cols{};

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


        // SETS UP A Render_Test object Note: Later to be 2D render but testing Buckets
        {

            /* Create a Bunch of Quads to Test Render */
            {
                uint8_t R{ 100 }, G{ 0 }, B{ 0 };
                Vec2 Sz{ 10, 10 };
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

            /// TEST TEXTURES BLACK AND WHITE
            White = new Texture("../Resources/White.bmp");
            Black = new Texture("../Resources/Black.bmp");

            test_Texture = new Texture("../Test2.bmp");
            test_Texture2 = new Texture("../Resources/Test.bmp");
            test_Texture3 = new Texture("../Resources/Test2.bmp");
            Texture *ttt  = new Texture("../Resources/Texture.bmp");

            /// TEST MATERIAL- SHADER/TEXTURE PAIRS
            Renderer_test::Material *MAT  = new Renderer_test::Material(shader_BasicRenderer, test_Texture2);
            Renderer_test::Material *MAT2 = new Renderer_test::Material(shader_BasicRenderer, ttt);

            /// VERTICES
            float sz = 1000, sy = 1000;
            std::vector<Vec2> Quad = make_Quad({ 10, 10 }, { sz, sy });
            std::vector<Vec2>UVcoord;
            UVcoord.push_back({ (Quad[0].x / sz + 1) * 0.5f ,   (Quad[0].y / sy + 1) * 0.5f });
            UVcoord.push_back({ (Quad[1].x / sz + 1) * 0.5f ,   (Quad[1].y / sy + 1) * 0.5f });
            UVcoord.push_back({ (Quad[2].x / sz + 1) * 0.5f ,   (Quad[2].y / sy + 1) * 0.5f });
            UVcoord.push_back({ (Quad[3].x / sz + 1) * 0.5f ,   (Quad[3].y / sy + 1) * 0.5f });
            UVcoord.push_back({ (Quad[4].x / sz + 1) * 0.5f ,   (Quad[4].y / sy + 1) * 0.5f });
            UVcoord.push_back({ (Quad[5].x / sz + 1) * 0.5f ,   (Quad[5].y / sy + 1) * 0.5f });

            ///  COLORS
            std::vector<Vec4> C;
            C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });
            C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });
            C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });
            C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });
            C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });
            C.push_back({ RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f),RANDOM(1.0f), });
            std::vector<Vec2>UVcoord2;
            for (auto& C : Verts)
            {
                UVcoord2.push_back({ 1 / C.x, 1 / C.y });
            }

            /// SUBMIT THE MESH/TEXTURE/SHADER CALLS TO THE RENDERER
            for_loop(i, 10)
            {
                Bucket_Test->Submit
                (// SUBMITS MESH/COLORS/TEXTURE COORDS AND MATERIAL TO RANDOM QUADS
                    new Renderer_test::Geometry
                    (// CREATES GEOMETRY USING A VBO
                        new VertexBufferObject<Vec2>
                        (// CREATES A VBO
                            make_Quad
                            (
                                { RANDOM(1300), RANDOM(1300) },
                                { RANDOM(100) , RANDOM(100)  }
                            )
                        ),
                        new VertexBufferObject<Vec4>(C),
                        new VertexBufferObject<Vec2>(UVcoord)
                    ),
                    MAT
                );
            }

            for_loop(i, 10)
            {// SAME AS THE ABOVE WE DO IT FOR ANOTHER 10
                Bucket_Test->Submit
                (
                    new Renderer_test::Geometry(new VertexBufferObject<Vec2>(make_Quad({ RANDOM(3000), RANDOM(3000) }, { RANDOM(100), RANDOM(100) })), new VertexBufferObject<Vec4>(C), new VertexBufferObject<Vec2>(UVcoord)),
                    MAT2
                );
            }
        }/// TEST 2D RENDERER

        /// IF DEBUG
        OpenGL::enable_DebugOutput();

        void init_CPUmonitor();
        // importer = new Model_Importer("../Resources/Models/Mech/Drone.fbx");

        // TEST THE IMMEDIATE MODE EMULATED OPENGL
        {
            GL::Initialize(10);
            DEBUG_CODE(CheckGLERROR());

            Vec3 _center = Vec3(0);
            float _size = 100;

            Vec3 V[] =
            {
                Vec3(_center.x + -_size, _center.y + -_size, _center.z + _size), // FRONT    // 0                                             
                Vec3(_center.x + _size, _center.y + -_size, _center.z + _size),             // 1                                    
                Vec3(_center.x + _size, _center.y + _size, _center.z + _size),             // 2                                    
                Vec3(_center.x + -_size, _center.y + _size, _center.z + _size),             // 3                                    
                Vec3(_center.x + _size, _center.y + _size, _center.z + _size), // RIGHT    // 4    2                                        
                Vec3(_center.x + _size, _center.y + _size, _center.z + -_size),             // 5                                    
                Vec3(_center.x + _size, _center.y + -_size, _center.z + -_size),             // 6                                    
                Vec3(_center.x + _size, _center.y + -_size, _center.z + _size),             // 7    1                                
                Vec3(_center.x + -_size, _center.y + -_size, _center.z + -_size), // BACK     // 8                                            
                Vec3(_center.x + _size, _center.y + -_size, _center.z + -_size),             // 9    6                               
                Vec3(_center.x + _size, _center.y + _size, _center.z + -_size),             // 10   5                                
                Vec3(_center.x + -_size, _center.y + _size, _center.z + -_size),             // 11
                Vec3(_center.x + -_size, _center.y + -_size, _center.z + -_size), // LEFT     // 12   8                                        
                Vec3(_center.x + -_size, _center.y + -_size, _center.z + _size),             // 13   7                              
                Vec3(_center.x + -_size, _center.y + _size, _center.z + _size),             // 14   3                                
                Vec3(_center.x + -_size, _center.y + _size, _center.z + -_size),             // 15  11                                
                Vec3(_center.x + _size, _center.y + _size, _center.z + _size), // TOP      // 16   2                                      
                Vec3(_center.x + -_size, _center.y + _size, _center.z + _size),             // 17   3                                
                Vec3(_center.x + -_size, _center.y + _size, _center.z + -_size),             // 18   11                                
                Vec3(_center.x + _size, _center.y + _size, _center.z + -_size),             // 19   5                                
                Vec3(_center.x + -_size, _center.y + -_size, _center.z + -_size), // BOTTOM   // 20   8                                   
                Vec3(_center.x + _size, _center.y + -_size, _center.z + -_size),             // 21   9                          
                Vec3(_center.x + _size, _center.y + -_size, _center.z + _size),             // 22   1                          
                Vec3(_center.x + -_size, _center.y + -_size, _center.z + _size),             // 23   0                               
            };


            uint32_t Indexdata[] =
            {// 36
                0,1,2,    0,2,3,     // FRONT
                6,5,4,    4,7,6,     // RIGHT 
                10,9,8,   8,11,10,   // BACK
                12,13,14, 12,14,15,  // LEFT
                18,17,16, 16,19,18,  // TOP
                20,21,22, 20,22,23   // BOTTOM
            };
            for_loop(i, 24)
            {
                Vresult.push_back(V[i]);
                Cresult.push_back(Vec4(RANDOM(1), RANDOM(1), RANDOM(1), RANDOM(1)));
            }
            for_loop(i, 36)
            {
                Iresult.push_back(Indexdata[i]);
            }
        }/// IMMEDIATE MODE OPENGL

        return;
    }

    virtual void OnKeydown(Window *_window, uint32_t _keyCode, uint32_t _scanCode, bool _repeat)
    {
 
        switch (_keyCode)
        {
        case 37:
        {// Left Key
            Application::getCamera().MoveX(-CAMERA_SPEED);
            test_Renderer3D->g_Camera().MoveLeft(CAMERA_SPEED);
        }
        break;

        case 38:
        {// Up  Key
            Application::getCamera().MoveY(-CAMERA_SPEED);
            test_Renderer3D->g_Camera().MoveForward(CAMERA_SPEED);
        }
        break;

        case 39:
        {// Right  Key
            Application::getCamera().MoveX(CAMERA_SPEED);
            test_Renderer3D->g_Camera().MoveRight(CAMERA_SPEED);
        }
        break;

        case 40:
        {// Down  Key
            Application::getCamera().MoveY(CAMERA_SPEED);
            test_Renderer3D->g_Camera().MoveBack(CAMERA_SPEED);
        }
        break;
        // was 189 Why the fuck is it 109 now?
        case 109:
        {//- Key
            Application::getCamera().ZoomOut(ZOOM_SPEED);
        }break;
        // Was 187. What the flying fuck
        case 107:
        {//+ Key
            Application::getCamera().ZoomIn(ZOOM_SPEED);
        }break;



        case 188:
        {// <  
            float R = (float)(DEGREES(Application::getCamera().g_Rotation()));
            Application::getCamera().s_Rotation(R - 20);// (float)(RADIANS(Rot)));
            test_Renderer3D->g_Camera().RotateY((float)-RADIANS(CAMERA_SPEED));
        }
        break;

        case 190:
        {// > 
            Application::getCamera().Rotate(2);
            test_Renderer3D->g_Camera().RotateY((float)RADIANS(CAMERA_SPEED));
        }
        break;
        case 27:
        {// Esc
            Terminate();
        }
        }
    }
    virtual void OnMouseMove(Window *_window, iVec2 _position, iVec2 _relative, uint32_t _button)
    {
       // Print("Mouse Position: " << _position);
    }

    std::vector<Vec4> Cresult;
    std::vector<Vec3> Vresult;
    std::vector<uint32_t> Iresult;

    /* Renders User Defined Geometry */
    virtual void OnRender() override 
    {
        //importer->FBO->Bind();
        //{
        //    importer->FBO->Clear();
        //    Camera_3D->Bind();
        //    importer->Render();
        //}
        //importer->FBO->Unbind();
        /// Currently if ANY Camera is bound before this call it trashes the test_Renderer2Ds Camera
        /// It is possibly not the Camera but the Shader Instead. 
        /// Would like to Detach all these systems as seperate entities and make them their own form of DrawCalls.
        {
            trace_scope("test_Renderer2D");
            test_Renderer2D->Render();
        }
        {
            trace_scope("Test RenderPass");
            test_RenderPass->Render();
        }
        
        {
            trace_scope("Blit FrameBuffer");
            testSurface->blit_FrameBuffer(SCENE->FBO, { 1,1, SCREEN_X, SCREEN_Y }, { 1, 1, SCREEN_X * 2, SCREEN_Y * 2 });
        }
        
        {
            trace_scope("Bucket_Test");
            Bucket_Test->Render();
        }
        {
            trace_scope("test_Renderer3D");
        //    test_Renderer3D->Render();
        }
         

        // RENDER_GUI()
        {
            trace_scope("GUI Rendering");
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.25f, 0.25f, 0.25f, 1.0f)); // Set window background to red
        
            static float Slide[3] = { -(SCREEN_X * 0.5f), -(SCREEN_Y * 0.5f), 0 };
        
            // Displays the FrameBuffers 
            ImGui::Begin("FrameBuffers");
            {
                // FPS Times
                {
                    if (FrameTimes.back() > Top_Time)
                    {
                        Top_Time = FrameTimes.back();
                    }
                    ImGui::PlotLines("", (float*)&FrameTimes.front(), FrameTimes.size(), 0, std::string("FPS:" + std::to_string((uint32_t)FrameTimes.back())).c_str(), 0, Top_Time, ImVec2(200, 50));
                }
                ImGui::SameLine();
        
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
                    ImGui::PlotLines("", (float*)&memUsage.front(), memUsage.size(), 0, std::string("Memory:" + std::to_string(25000 + (uint32_t)memUsage.back())).c_str(), 0, 2000, ImVec2(200, 50));
                    ImGui::SameLine();
                    ImGui::PlotLines("", (float*)&CPUUsage.front(), CPUUsage.size(), 0, std::string("CPU%:" + std::to_string((uint32_t)CPUUsage.back())).c_str(), 0, 100, ImVec2(200, 50));
                }
        
                char *Buff = const_cast<char*>(OpenGL_ErrorList.c_str());
                std::string Text = "OpenGL Errors: " + std::to_string(OpenGL_ErrorList.size());
                ImGui::InputTextMultiline(Text.c_str(), Buff, OpenGL_ErrorList.size());
        
                ImGui::Checkbox("Update Geometry", &Update_Geometry);
        
                ImGui::SetWindowPos
                (
                    ImVec2(0, Application::getWindow().Height() - (Application::getWindow().Height() * 0.2f)),
                    true
                );
                ImGui::SetWindowSize
                (
                    {
                        (float)Application::getWindow().Width(),
                        (Application::getWindow().Height()  * 0.2f) - 10 //Border
                    }
                );
            }
            ImGui::End();
            ImGui::PopStyleColor();
        }// GUI Rendering


        OpenGL::unbind_FBO(); // Just ensures we are on the default FrameBuffer
        Ang++;

        // Running Test on Experimental Fixed Function Emulator Renderer
        {
            trace_scope("Fixed Function");
            // Sets our Projection Matrix Placing 0,0 in the middle of the Screen
        
            //GL::IndexPointer(GL_INT, 0, &Iresult[0]);
            GL::ColorPointer(4,  GL_FLOAT , 0, &Cresult[0]);
            GL::VertexPointer(3, GL_FLOAT , 0, &Vresult[0]);
        
            {
                GL::MatrixMode(GL_PROJECTION);
                GL::LoadIdentity();
                GL::Perspective(60.0, SCREEN_X / (float)SCREEN_Y, 0.1, 1000.0);             //   GL.Ortho(-SCREEN_X * .5, SCREEN_X * .5, -SCREEN_Y * .5, SCREEN_Y * .5, .1f, 1000);
                GL::Viewport(0,  0,  SCREEN_X, SCREEN_Y);
            }
        
            // Manipulates the ModeViewMatrix
            {
                GL::MatrixMode(GL_MODELVIEW);
                GL::LoadIdentity();
                GL::Rotate(RADIANS(Ang), 0, 0, 1);
            }
        
            GL::ClearColor(0.0f, 0.0f, 0.2f, 1.0f);
            GL::Begin(GL_TRIANGLES);
            {
                GL::Vertex(10, 0, -100);
                GL::Vertex(0, -10, -100);
                GL::Vertex(0, 0, -100);
                GL::Color(0.0, 0.0, 1.0);
                GL::Color(0.0, 1.0, 0.0);
                GL::Color(1.0, 0.0, 0.0);
            }
            GL::End();
        
            GL::LoadIdentity();
            GL::Rotate(RADIANS(-Ang), 0, 0, 1);
            GL::Begin(GL_TRIANGLES);
            {
                GL::Vertex(10, 10, -100);
                GL::Vertex(-10, -10, -100);
                GL::Vertex(0, 10, -100);
                GL::Color(1.0, 0.0, 1.0);
                GL::Color(1.0, 1.0, 0.0);
                GL::Color(1.0, 0.0, 1.0);
            }
            GL::End();
        }

        DEBUG_CODE(CheckGLERROR());
        return;
	}


    float Ang = 0;
 

    /* Runs on Applications Frame Update */
    virtual void OnUpdate() override 
    {
        test_RenderPass->attach(SCENE->Vertices_VAO);
        
        test_Renderer3D->Main_Camera->s_Rotation( { getMouse().Relative.x , 0, getMouse().Relative.y } );

        test_Renderer3D->Update();
            
        if (Update_Geometry)
        {
           /// trace_scope("UpdateGeometry")
           /// SCENE.Update();
        }

		size_t NewTime = Timing::Timer<Milliseconds>::GetTime();
    	size_t Time = Timing::Timer<Milliseconds>::GetTime() - PreviousTime;
		PreviousTime = NewTime;

        return;
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
    bool  Update_Geometry { true };
    float Top_Time{ 0 };
    ///================================================================================
    std::vector<float> memUsage;
    std::vector<double> CPUUsage;    
};

//#include"assimp/Importer.hpp"
//#include "assimp/scene.h"           // Output data structure
//#include "assimp/postprocess.h"     // Post processing flags
//C:\Users\curti\source\repos\xSeditx\Creature - Engine\Bin\Assimp\include\assimp
int main()
{
   // Physics_App *My_Physics = new Physics_App(960, 600, "Physics");
   // My_Physics->Init();
   // My_Physics->Run();


    App *MyApp = new App(1920, 1200, "TestApp");
    
    
    PROFILE_BEGIN_SESSION("Init", "ProfileInitResults.json");
    {
        MyApp->Init();
    }
    PROFILE_END_SESSION();
    
    
    PROFILE_BEGIN_SESSION("Run", "ProfileRunResults.json");
    {
        MyApp->Run();
    }
    PROFILE_END_SESSION();

	//	Profiling::Memory::TrackDumpBlocks();
	//	Profiling::Memory::TrackListMemoryUsage();
	return 0;


    DEBUGPrint(CON_Red, "Problem with the Debug print Symbols hConsole and DEBUGMutex in that they have no real home on where they should be. It is possible Common.h should perhaps have a CPP file for Symbol definition");
 
    TODO("7/20: Fucked up. It is all fucked up. Cube Instance Renderer3D is not properly rendering a damn thing. Rotate clicker on ImGui on the 2D render is not properly rotating and just spins out of control rapidly. \n ImGui appears to be sticking and this should be one of the first things fixed because it is fucking annoying.");
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



 // https://www.scmp.com/news/china/society/article/3075567/people-blood-type-may-be-more-vulnerable-coronavirus-china-study?utm_content=article&utm_medium=Social&utm_source=Facebook&fbclid=IwAR1VsuqHUzDgoU6-zaafrHJI89Qniyxbzppif6x_SvsC9LXHGjsfwo7tM6k#Echobox=1584436870
 =====================================================================================================================
*/

/* =====================================================================================================================
UNITY Combines meshes that are only used in one scene and not referenced in any script:
Source: https://www.youtube.com/watch?v=LleJbZ3FOPU

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
===================================================================================================================== */

/* =====================================================================================================================
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

======================================================================================================================= */


 /*=====================================================================================================================
 
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
===================================================================================================================== */


 
/* Renders our ImGui Data
virtual void OnRenderGUI() override trace(1)
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.25f, 0.25f, 0.25f, 1.0f)); // Set window background to red

    static float Slide[3] =  {-(SCREEN_X * 0.5f), -(SCREEN_Y * 0.5f), 0 };

    // Displays the FrameBuffers
    ImGui::Begin("FrameBuffers");
    {
        // FPS Times
        {
            if (FrameTimes.back() > Top_Time)
            {
                Top_Time = FrameTimes.back();
            }
            ImGui::PlotLines("", (float*)&FrameTimes.front(), FrameTimes.size(), 0, std::string("FPS:" + std::to_string((uint32_t)FrameTimes.back())).c_str(), 0, Top_Time, ImVec2(200, 50));
        }
        ImGui::SameLine();

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
            ImGui::PlotLines("", (float*)&memUsage.front(), memUsage.size(), 0, std::string("Memory:" + std::to_string(25000 + (uint32_t)memUsage.back())).c_str(), 0, 2000, ImVec2(200, 50));
            ImGui::SameLine();
            ImGui::PlotLines("", (float*)&CPUUsage.front(), CPUUsage.size(), 0, std::string("CPU%:" + std::to_string((uint32_t)CPUUsage.back())).c_str(), 0, 100, ImVec2(200, 50));
        }

        char *Buff = const_cast<char*>(OpenGL_ErrorList.c_str());
        ImGui::InputTextMultiline("OpenGL Errors", Buff, OpenGL_ErrorList.size());

        ImGui::Checkbox("Update Geometry", &Update_Geometry);

        ImGui::SetWindowPos
        (
            ImVec2(0, Application::getWindow().Height() - (Application::getWindow().Height() * 0.2f)),
            true
        );
        ImGui::SetWindowSize
        (
            {
                (float)Application::getWindow().Width(),
                (Application::getWindow().Height()  * 0.2f) - 10//Border
            }
        );
    }
    ImGui::End();
    ImGui::PopStyleColor();
    Return();
}  */
 
//Fixed_Func GL;
//
//GL.MatrixMode(GL_PROJECTION);
//GL.LoadIdentity();
//GL.Ortho(-500, 500, -500, 500, .1f, 1000);
//
//GL.MatrixMode(GL_MODELVIEW);
//GL.Translate(0, 0, 0);
//GL.LoadIdentity();
//
//GL.ClearColor(0.0f, 0.0f, 0.2f, 1.0f);
//GL.Begin(GL_TRIANGLES);
//{
//    GL.Vertex(100, 100, -100);
//    GL.Vertex(-100, -100, -100);
//    GL.Vertex(100, 0, -100);
//    GL.Vertex(100, 100, -100);
//    GL.Vertex(-100, -100, -100);
//    GL.Vertex(0, 100, -100);
//
//    GL.Color(1.0, 0.0, 1.0);
//    GL.Color(1.0, 1.0, 0.0);
//    GL.Color(1.0, 0.0, 1.0);
//    GL.Color(0.0, 1.0, 0.0);
//    GL.Color(1.0, 1.0, 1.0);
//    GL.Color(0.0, 0.0, 0.0);
//}
//GL.End();
//





//   https://en.wikibooks.org/wiki/OpenGL_Programming/GLStart/Tut3
//   http://docs.gl/gl2/glDrawArrays
//   http://opengl.czweb.org/ch14/457-462.html
//   https://www.ics.com/blog/fixed-function-modern-opengl-part-1-4
//   https://nanogui.readthedocs.io/en/latest/api/classnanogui_1_1GLShader.html#classnanogui_1_1GLShader_1a164d25468ea9c4a71aa16f0e91fe4c4e
//   https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/
//   https://docs.microsoft.com/en-us/windows/win32/opengl/glcolor3i

/*
=====================================================================================================================
PlayerLoop
- CameraRender
- UpdateScript
- - Behavbior
- - - AI
- - - Script
- - - Script
- - - Script
*/











/* ============================================================================================
           Rough Sketch of a Scene class to determine what I am going to need
           For the Finalized Version of it. Also gives me a Template to rework the
           VertexArrayObject and VertexBufferObjects
/* ============================================================================================

struct MyScene
{
public:
    using Vec_t = Vec2;

    /* Create the Object after OpenGL is Initialized
    void Create()
    {
        /* Create all the Data we are going to fill the Buffers with
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
        /* Create the FrameBuffer we will be Rendering to
        {
            FBO = new FrameBufferObject(SCREEN_X, SCREEN_Y);
            FBO->Bind();
            {
                OpenGL::set_ClearColor(.2f, .5f, .7f, 1.0f);
            }
            FBO->Unbind();
        }
        /* Create all the Needed Buffers for this class
        {

            RENDERER = shader_BasicRenderer;
            Vertices_VAO = new VertexArrayObject();
            Vertices_VBO = new VertexBufferObject<Vec_t>(Vertices);
            Colors_VBO   = new VertexBufferObject<Vec4>(Colors);
        }

        /* Link all the Buffers into a Single VertexArrayObject
        {
            RENDERER->Bind();
            {
                Vertices_VAO->Attach(BufferTypes::VERTEX, Vertices_VBO);
                Vertices_VAO->Attach(BufferTypes::COLOR, Colors_VBO);
            }
            RENDERER->Unbind();
        }
    }

    /* Update the Positions of the Vertices Once Per Frame
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

        /* Update my Buffer Object
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

    /* Render the Entire Scene
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
*/


