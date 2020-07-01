#include<iostream>
#include<Windows.h>
#include<array>
#include<cmath>
#include<string>


/* 

IMGUI ISSUE:   Docking Branch:
    SetWindowPos does not function properly when we have windows docked in one another. The Size nor the Position is properly set.

    Color Picker does not move with Window which has been Docked and can only be seen when Hovering the Window above the Area meaning that the World Vertex Positions do not update when the Window Updates if it is docked in another.

    */



// CMAKE Pluggin
// https://marketplace.visualstudio.com/items?itemName=DaisukeAtaraxiA.VSslnToCMakePlugin

//#pragma optimize( "", off )

//CORONA VIRUS STUDY
//https://jamanetwork.com/journals/jama/fullarticle/2762130

#define _PROFILE_MEMORY

#include"Creature_Engine.h"
// https://www.scmp.com/news/china/society/article/3075567/people-blood-type-may-be-more-vulnerable-coronavirus-china-study?utm_content=article&utm_medium=Social&utm_source=Facebook&fbclid=IwAR1VsuqHUzDgoU6-zaafrHJI89Qniyxbzppif6x_SvsC9LXHGjsfwo7tM6k#Echobox=1584436870

using namespace Core;
using namespace Threading;

#include<stack>
#include<utility>
#include"Core/ECS/ECS.h"
#include"Core/ECS/TestComponents.h"
#include"Core/Math/Easing.h"

std::stack<std::string> CS;


bool TEST_PROFILE_WINDOW();


/* This Camera Situation is all wrong, figure a way to fix this */
Camera2D* WorldCamera{ nullptr };
#define CAMERA_SPEED 4.0f
#define ZOOM_SPEED 1.0

Listener KeyListener([](Event _msg)
{

	DEBUG_CODE(Print("Key Entered: " << _msg.wParam));
	switch (_msg.wParam)
	{
	case 37:
	{// Left Key
		WorldCamera->MoveX(-CAMERA_SPEED);
	}
	break;

	case 38:
	{// Up  Key
		WorldCamera->MoveY(-CAMERA_SPEED);
	}
	break;

	case 39:
	{// Right  Key
		WorldCamera->MoveX(CAMERA_SPEED);
	}
	break;

	case 40:
	{// Down  Key
		WorldCamera->MoveY(CAMERA_SPEED);
	}
    break;

	case 107: 
	{//- Key
        WorldCamera->ZoomOut(ZOOM_SPEED);
	}break;

	case 109:
	{//+ Key
        WorldCamera->ZoomIn(ZOOM_SPEED);
	}break;

	}// End of Switch
});
Listener MouseWheel( [](Event _msg)
{
    Vec2 Mpos = SplitLParam((int)_msg.lParam);

    if ((int16_t)SplitLParam((int)_msg.wParam).y > 0)
    {// Mouse Wheel UP
        WorldCamera->ZoomInto(Mpos, ZOOM_SPEED);
    }
    else
    {// Mouse Wheel DOWN
        WorldCamera->ZoomOutFrom(Mpos, ZOOM_SPEED);
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
            float Max = 10;
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
    void Update()
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
    }

    /* Render the Entire Scene */
    void Render()
    {
        FBO->Bind();
        {
            FBO->Clear();
            RENDERER->Bind();
            {
                Vertices_VAO->Bind();
                {
                    Application::getCamera().Bind();
                    OpenGL::Renderer::drawArray(Vertices.size());
                }
                Vertices_VAO->Unbind();
            }
            RENDERER->Unbind();
        }
        FBO->Unbind();
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

    OpenGL::Renderer2D *MainRenderer{ nullptr };
    OpenGL::RenderPass *test_RenderPass;
    MyScene SCENE;
    Texture *test_Texture;
    size_t  PreviousTime;

    /* Initializes User Variables */
    virtual void OnCreate() override
	{// Initialization
        
        // TEST_ASSERT( TEST_Memory_Pool_Class() , " Memory Pool Class Incomplete contains Errors " , " Memory Pool Class Complete ");
        // TEST_ASSERT( Creatures::TEST_SPRINGS(), " Springs Class Incomplete  contains Errors ", " Springs Class Complete ");
        // TEST_ASSERT( TEST_Ring_Buffer_Class() , " Ring Buffer Class Incomplete  contains Errors " , " Ring Buffer Class Complete ");

        /// Likely add this to the Base Application class to relieve the user of this. Create a setup that only Initializes the desired shaders
        init_DefaultShaders();


        /* Load up the Listeners for the Various Input Events */
        {
            RegisterListener(   WM_KEYDOWN, KeyListener);
            RegisterListener(WM_MOUSEWHEEL, MouseWheel);
        }

        /* Creates a Renderer for our Application */
		MainRenderer = new OpenGL::Renderer2D({SCREEN_X, SCREEN_Y});
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

                    MainRenderer->renderQuad
                    (
                        { x * 8.0f, y * 8.0f },
                        { 7, 7 },
                        OpenGL::Renderer::normalize_RGBA_Color(R, G, B, 255)
                    );
                    MainRenderer->draw_Line(x * 8.0f, y * 8.0f, x * 8.0f + 7, y * 8.0f + 7);
                }
            }
        }

        /* Set the Application window Camera and World Camera to the Renderer */
        {
            getWindow().s_Camera(&MainRenderer->g_Camera());
            WorldCamera = &getCamera();
        }
        Application::setCamera(MainRenderer->g_Camera());
        SCENE.Create();
         
        test_RenderPass = new OpenGL::RenderPass(SCREEN_X, SCREEN_Y, shader_BasicRenderer);
        test_RenderPass->attach(  WorldCamera);;//MainRenderer->g_Camera())&Application::getCamera());//
        test_RenderPass->attach(SCENE.Vertices_VAO);
      
        test_Texture = new Texture("../Test2.bmp");
        DEBUG_CODE(CheckGLERROR());
	}

    /* Renders User Defined Geometry */
    virtual void OnRender() override
    {
        SCENE.Render(); 
        MainRenderer->Render();
        test_RenderPass->Render();

        shader_TextureRenderer->Bind();
        {
            OpenGL::bind_VAO(DebugQuadVAO);
            shader_TextureRenderer->SetUniform("Position", {1,1,100,100 });
            test_Texture->Bind(0);
            OpenGL::Renderer::drawArray(DebugQuadVBO, 6);
        }
        shader_TextureRenderer->Unbind();

        DEBUG_CODE(CheckGLERROR());
	}

    /* Runs on Applications Frame Update */
	virtual void OnUpdate() override
	{// User Genrated Per Frame Update
        SCENE.Update();
		size_t NewTime = Timing::Timer<Milliseconds>::GetTime();
    	size_t Time = NewTime - PreviousTime;
		PreviousTime = NewTime;
	}

    /* Cleans up the Memory of stuff we still have Active */
    virtual void OnEnd() override
    {// Exit of the Application and Clean up
        delete(MainRenderer);
    }

    /* ==========================================================================================
           GUI STUFF, POTENTIALLY MIGHT CHANGE THIS LATER
    /* ========================================================================================== */
    /* Updates the GUI Data: NOTE: Incomplete */
    virtual void OnUpdateGUI() override
    {    }

    /* Renders our ImGui Data */
    virtual void OnRenderGUI() override 
    {

        static float Slide[3] = {-650,-400,-300 };
        Vec2 MainWindowSize = getWindow().g_Size();
        int Boarder = 10;

        /* Start the Camera Widget */
        ImGui::Begin("Camera");
        {
            Vec2 Camera_Position = WorldCamera->g_Position();
            /* Position Slider for the Camera */
            {
                ImGui::SliderFloat3("Position", Slide, -1000, 1000);
                WorldCamera->Translate({ Slide[0], Slide[1] });
                WorldCamera->set_Zoom(Slide[2]);
            }
            ImGui::SetWindowPos(ImVec2(MainWindowSize.x - (MainWindowSize.x * 0.15f), 0), true);
            ImGui::SetWindowSize({ MainWindowSize.x * 0.15f, MainWindowSize.y - ((MainWindowSize.y  * 0.2f) ) });

            /* Position Slider for the Camera */
            {
                ImGui::InputFloat("X", &Camera_Position.x, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Y", &Camera_Position.y, 10.0f, 1.0f, "%.3f");
                WorldCamera->s_Position(Camera_Position);
            }
        
            /* Clickable Position Slider for the Camera */
            {
                float
                    H = (float) WorldCamera->Height(), 
                    W = (float) WorldCamera->Width();
                ImGui::InputFloat("Width", &W, 10.0f, 1.0f, "%.3f");
                ImGui::InputFloat("Height", &H, 10.0f, 1.0f, "%.3f");
                WorldCamera->Size = { W,H };
            }
            /* Dragable Position Slider for the Camera */
            {
                int val = 0, val2 = 100;
                ImGui::DragFloatRange2("Range", &Camera_Position.x, &Camera_Position.y);
                WorldCamera->s_Position(Camera_Position);
            }
        
            /* Color Picker Widget for the Background Color */
            {
                Vec4 c = getWindow().g_ClearColor();
                float Col[3] = { c.x,c.y,c.z };
                ImGui::ColorPicker3("Background Color", Col, 0);
                glClearColor(Col[0], Col[1], Col[2], 1);
                getWindow().s_ClearColor({ Col[0], Col[1], Col[2], 1 });
            }
        
            static float Top_Time = 0;
            if (FrameTimes.back() > Top_Time)
            {
                Top_Time = FrameTimes.back();
            }
            ImGui::PlotLines("Frame Times", (float*)&FrameTimes.front(), FrameTimes.size(), 0, std::string("FPS:" + std::to_string((uint32_t)FrameTimes.back())).c_str(), 0, Top_Time, ImVec2(ImGui::GetWindowWidth(), 50));
        
        }
        ImGui::End();

        /* Displays the FrameBuffers */
        ImGui::Begin("FrameBuffers");
        {
            
            /* Displays the FrameBuffer */
            float WinSize = (float)(ImGui::GetWindowHeight() - (ImGui::GetWindowHeight() * 0.15));
            ImGui::Image((ImTextureID*)((size_t)SCENE.FBO->RenderTarget->g_Handle()), ImVec2(WinSize, WinSize));
            ImGui::SameLine();
            ImGui::Image((ImTextureID*)((size_t)SCENE.FBO->DepthTarget->g_Handle()), ImVec2(WinSize, WinSize));
            ImGui::SameLine();
        
            ImGui::Image((ImTextureID*)((size_t)test_RenderPass->FBO->RenderTarget->g_Handle()), ImVec2(WinSize, WinSize));
            ImGui::SameLine();
            ImGui::Image((ImTextureID*)((size_t)test_Texture->g_Handle()) , ImVec2(WinSize, WinSize));
            ImGui::SameLine();
        
        
            char *Buff = const_cast<char*>(OpenGL_ErrorList.c_str());
            ImGui::InputTextMultiline("OpenGL Errors",Buff, OpenGL_ErrorList.size());

            ImGui::SetWindowPos(ImVec2(0, MainWindowSize.y - (MainWindowSize.y  * 0.2f)), true); 
            ImGui::SetWindowSize({ MainWindowSize.x, (MainWindowSize.y  * 0.2f)  - Boarder});
        }
        ImGui::End();
        ImGui::Begin("Dummy");

        ImGui::ShowDemoWindow();
        ImGui::SetWindowPos(ImVec2(0, 0), true); // 768 - ImGui::GetWindowHeight() - 40
        ImGui::SetWindowSize({ MainWindowSize.x * 0.15f, MainWindowSize.y - (MainWindowSize.y  * 0.2f) });
        ImGui::End();

    } 

};






int main()
{
   // TEST_ASSERT(condition, FAIL_MSG, PASS_MSG);
    DEBUGPrint(CON_Red,"Problem with the Debug print Symbols hConsole and DEBUGMutex in that they have no real home on where they should be. It is possible Common.h should perhaps have a CPP file for Symbol definition");

    TODO(" Setup Mock ups which use the Application class to setup a state in a way that I can test various functionality by switching through different applications. /n Each Module should have its very own Application class. ");
    TODO(" Serious Restructuring needs to take place to the Entire project, it is starting to grow rather large and I am not happy with some of the early design and structure decisions that have begun to slightly conflict. \n\
 It would be wise to reformat and refactor the project before these minor issues become big ones  ");

    App MyApp;
	MyApp.Init();

    /* Get the Viewport Dimensions and the Max view dimensions */
    {
        iVec2 r = OpenGL::get_MaximumViewportDimensions();
        Print("Max Viewport Dimensions: " << r.x << " : " << r.y);
        iVec4 vp = OpenGL::get_Viewport();
        Print("Viewport: " << vp.x << " : " << vp.y << " : " << vp.z << " : " << vp.w);
    }

	MyApp.Run();

	//	Profiling::Memory::TrackDumpBlocks();
	//	Profiling::Memory::TrackListMemoryUsage();
	return 0;
}
  














int LOOP_COUNT{ 1000000 };

#define NUMBER_OF_THREADS 200
#define _TEST_THREADPOOL_SPEED    1

bool THREAD_POOL_TEST()
{
#if 0
	/// TEST_UNIT(TEST_PROFILE_WINDOW());
	DEBUG_CODE(_Trace("Testing Trace Macro", 100000));
	DEBUGPrint(CON_Red, "Testing Print");
	/* */

	while (true)
	{
		TestAsyncSort SortTest(1024); // 4096); // 262144); //
		Print("Running 40");
		ThreadPool::get().Async(TestRecursion, 40);
		Print("Running 30");
		ThreadPool::get().Async(TestRecursion, 30);
		Print("Running 20");
		ThreadPool::get().Async(TestRecursion, 20);
		Print("Running 10");
		ThreadPool::get().Async(TestRecursion, 10);
		{
			Timing::Profiling::Profile_Timer Bench("My Linear Merge Sort");
			// 	SortTest.LinearMergeSort();
		}
		{// Currently freezes if one attempts to recurse to many levels to the point it overwhelms the threadpool as it can never return until it is capable of recursing deeper.
			Timing::Profiling::Profile_Timer Bench("My MT Merge Sort");// Dont use the current Threaded Version its broke.
		//	SortTest.AsyncMergeSort();
		}
		{
			Timing::Profiling::Profile_Timer Bench("std::async Merge Sort");
			//SortTest.StdMergeSort();
		}


#if _TEST_THREADPOOL_SPEED
		Function_Counter = 0;
		//	LOOP_COUNT += 1000;// 22100 is when Threadpool and Linear start to become one.
		Print("/n/n/n/n Loop Counter:" << LOOP_COUNT << " iterations in the Worker Functions/n");
		{
			Timing::Profiling::Profile_Timer Bench("My Threadpool");
			std::vector<std::vector<uint32_t>> Test;

			auto A = ThreadPool::get().Async(TestFunctionE, std::move(LOOP_COUNT));
			auto B = ThreadPool::get().Async(TestFunctionE, std::move(LOOP_COUNT));
			auto C = ThreadPool::get().Async(TestFunctionB, 1431);
			auto D = ThreadPool::get().Async(TestFunctionD, 123.321f, 10);
			auto E = ThreadPool::get().Async(TestFunctionA);
			auto F = ThreadPool::get().Async(TestFunctionC, 3.14159f, 123);
			auto G = ThreadPool::get().Async(TestFunctionF, std::move(LOOP_COUNT));
			auto H = ThreadPool::get().Async(TestFunctionG, std::move(LOOP_COUNT));
			auto I = ThreadPool::get().Async(TestFunctionH, std::move(LOOP_COUNT));
			auto J = ThreadPool::get().Async(TestFunctionI, std::move(LOOP_COUNT));
			auto K = ThreadPool::get().Async(TestFunctionJ, std::move(LOOP_COUNT));

			Print("Thread Pool Cluster");
			std::vector<std::future<float>> Fut;
			//std::vector<Future<float>> Fut;
			for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
			{
				auto F = ThreadPool::get().Async(TestFunctionC, 123.321f, std::move(rand() % NUMBER_OF_THREADS));
				//Fut.push_back(std::forward<std::future<float>>(F));
				///Fut.push_back(std::forward<Future<float>>(F));
			}
			uint64_t result{ 0 };
			uint64_t counter = Fut.size();
			while (counter)
			{
				for (auto& F : Fut)
				{
					if (!is_ready(F))
					{
						continue;
					}
					result += (uint64_t)F.get();
					--counter;
				}
			}
			Print("End Thread Pool Cluster: " << result);

			//while (Function_Counter < 10) {}// SpinLock until every single function called returns as measured via the atomic int Function_Counter.

			Print("Threadpool: " << result);
		}

		Function_Counter = 0;
		{

			Timing::Profiling::Profile_Timer ThreadBM("std::Async");
			auto  TPTest5T = std::async(std::launch::async | std::launch::deferred, TestFunctionE, LOOP_COUNT);
			auto  TPTest1T = std::async(std::launch::async | std::launch::deferred, TestFunctionB, 1431);
			auto  TPTest4T = std::async(std::launch::async | std::launch::deferred, TestFunctionD, 123.321f, 10);
			auto  TPTest3T = std::async(std::launch::async | std::launch::deferred, TestFunctionA);
			auto  TPTest2T = std::async(std::launch::async | std::launch::deferred, TestFunctionC, 3.14159f, 123);
			auto  TPTest6T = std::async(std::launch::async | std::launch::deferred, TestFunctionF, LOOP_COUNT);
			auto  TPTest7T = std::async(std::launch::async | std::launch::deferred, TestFunctionG, LOOP_COUNT);
			auto  TPTest8T = std::async(std::launch::async | std::launch::deferred, TestFunctionH, LOOP_COUNT);
			auto  TPTest9T = std::async(std::launch::async | std::launch::deferred, TestFunctionI, LOOP_COUNT);
			auto TPTest10T = std::async(std::launch::async | std::launch::deferred, TestFunctionJ, LOOP_COUNT);

			Print("Async Cluster");
			std::vector<std::future<float>> Fut;
			for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
			{
				auto TPTest4loop = std::async(std::launch::async, TestFunctionC, 123.321f, rand() % NUMBER_OF_THREADS);
				Fut.push_back(std::move(TPTest4loop));
			}
			uint64_t result{ 0 };
			uint64_t counter = Fut.size();
			while (counter)
			{
				for (auto& F : Fut)
				{
					if (!is_ready(F))
					{// if not ready yet, check the next  
						continue;
					}
					result += (uint64_t)F.get(); // it is ready 
					--counter;
				}
			}

			Print("End Async Cluster");
			Print("Async :" << result);
			while (Function_Counter < 10) {}// SpinLock until every single function called returns as measured via the atomic int Function_Counter. 

			std::vector<std::vector<uint64_t>> Test;

			Test.push_back(TPTest5T.get());
			Test.push_back(TPTest6T.get());
			Test.push_back(TPTest7T.get());
			Test.push_back(TPTest8T.get());
			Test.push_back(TPTest9T.get());
			Test.push_back(TPTest10T.get());
			///Print("Async: " << TPTest4T.get() << " : " << TestCompile(Test));
		}


		Function_Counter = 0;
		{
			Timing::Profiling::Profile_Timer LBench("Linear Benchmark");
			auto Test5 = TestFunctionE(std::move(LOOP_COUNT));// .5ms
			auto Test4 = TestFunctionD(123.321f, 10);
			auto Test1 = TestFunctionB(1431);
			auto Test3 = TestFunctionA();
			auto Test2 = TestFunctionC(3.14159f, 123);
			auto Test6 = TestFunctionF(std::move(LOOP_COUNT));//.3
			auto Test7 = TestFunctionG(std::move(LOOP_COUNT));//3.21
			auto Test8 = TestFunctionH(std::move(LOOP_COUNT));// .32
			auto Test9 = TestFunctionI(std::move(LOOP_COUNT));//2.8
			auto Test10 = TestFunctionJ(std::move(LOOP_COUNT));//2.6


			while (Function_Counter < 10) {}// SpinLock until every single function called returns as measured via the atomic int Function_Counter. 


			uint64_t result{ 0 };
			for (int i{ 0 }; i < NUMBER_OF_THREADS; ++i)
			{
				result += (uint64_t)TestFunctionC(123.321f, rand() % NUMBER_OF_THREADS);
			}
			Print("Linear :" << result);

			std::vector<std::vector<uint64_t>> Test;
			Test.push_back(Test5);
			Test.push_back(Test6);
			Test.push_back(Test7);
			Test.push_back(Test8);
			Test.push_back(Test9);
			Test.push_back(Test10);
			///Print("Linear: " << Test4 << " : " << TestCompile(Test));
		}
		Sleep(1500);
#endif //_TEST_THREADPOOL_SPEED
	}
#endif // IF 0 to turn all this off for now.
	return true;
}
bool TEST_PROFILE_WINDOW()
{
	{
		Profiling::DisplayWindow Test({ 0,0 }, { 10, 150 }, { 30, 450 });
		{	// TEST SET PIXEL

			Test.setPixel(1, 1, Pixel(255,255,255,255));
		}
		{	// TEST UPDATE

			Test.Update(1);
			if (Test.getPixel(1, 2) _NOT_EQUAL_TO_ 0xffffffff)return false;
		}
		{	// REPEATEDLY TEST TO ENSURE UPDATE IS MOVING THE VALUES DOWN

			Test.Update(1);
			if (Test.getPixel(1, 3) _NOT_EQUAL_TO_ 0xffffffff)return false;
			Test.Update(1);
			if (Test.getPixel(1, 4) _NOT_EQUAL_TO_ 0xffffffff)return false;
			Test.Update(1);
			if (Test.getPixel(1, 5) _NOT_EQUAL_TO_ 0xffffffff)return false;
			Test.Update(1);
			if (Test.getPixel(1, 6) _NOT_EQUAL_TO_ 0xffffffff)return false;
			Test.Update(1);
			if (Test.getPixel(1, 7) _NOT_EQUAL_TO_ 0xffffffff)return false;
			Test.Update(1);
			if (Test.getPixel(1, 8) _NOT_EQUAL_TO_ 0xffffffff)return false;
		}
		{	// TEST TO MAKE SURE IT IS NOT LEAVING A VALUE BEHIMD AND IS NOT JUST SETTING NEW VALUE

			if (Test.getPixel(1, 7) _EQUALS_ 0xffffffff)return false;
		}
	}

	return true;
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
/


        Organ *Test1[100];

        for (int i = 0; i < 100; ++i)
        {
            Test1[i] = new Organ(i);
        }

        for (int i = 0; i < 50; ++i)
        {
            delete(Test1[i]);
        }
        for (int i = 0; i < 50; ++i)
        {
            Test1[i] = new Organ(i);
        }
        for (int i = 0; i < 25; ++i)
        {
            delete(Test1[i]);
        }
        for (int i = 0; i < 25; ++i)
        {
            Test1[i] = new Organ(i);
        }

        size_t ElementTest{ 0 };
        for (auto& T : Organ::Pool)
        {// Cycle over ever Block in the Pool
            Print("Range ForLoop: " << T.Value);
            ++ElementTest;
        }
        assert(ElementTest   == Organ::Pool.chunkCount());
        assert(sizeof(Organ) == Organ::Pool.chunkSize());

        for (int i{ 0 }; i < Organ::Pool.size(); ++i)
        {// Cycle over every Byte in the Raw Data
            Print("Data[" << i << "] = " << (int)*Organ::Pool.get_Data(i));
        }

        Print("Pool Size" << Organ::Pool.size());

        Organ::Pool.clear();
        for (int i{ 0 }; i < Organ::Pool.size(); ++i)
        {
            Print("Data[" << i << "] = " << (int)*Organ::Pool.get_Data(i));
        }


        Print("Is It Full : " << Organ::Pool.is_Empty());
        assert(Organ::Pool.is_Full() == true);

        delete(Test1[1]);
        Print("Testing Delete to Free up Space... Is it Still Full : " << Organ::Pool.is_Full());
        assert(Organ::Pool.is_Full() == false);


        Print("Is It Empty : " << Organ::Pool.is_Full());
        assert(Organ::Pool.is_Empty() == false);

        Test1[1] = new Organ(1);
        Print("Allocating Again Is it Full Again: " << Organ::Pool.is_Full());
        assert(Organ::Pool.is_Full() == true);
/

*/















//
//int TestRecursion(int _param)
//{
//	_param--;
//	Print("Recursion " << _param);
//	if(_param > 0)
//	{
//       
//            WARN_ME("This is Currently disabled due to Error cant convert _Ty' to 'int (__cdecl *const )(int &) ------ _Ty=int (__cdecl &)(int) ")
//		//auto E = ThreadPool::get().Async(TestRecursion, (int)_param );
//	//	E.get();
//	}
//	Print("Exit " << _param);
//
//	return 65;
//}
//


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

static const char* fmt_table_int[3][4] =
{
    {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
    { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
    { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
};
static const char* fmt_table_float[3][4] =
{
    {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
    { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
    { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
};


