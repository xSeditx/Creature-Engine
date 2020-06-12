#include<iostream>
#include<Windows.h>
#include<array>
#include<cmath>
#include<string>

//#define CacheLineFlush(Address) _mm_clflush(Address)
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


std::stack<std::string> CS;


bool TEST_PROFILE_WINDOW();
#define CAMERA_SPEED 4.0f
#define ZOOM_SPEED 1.0
Camera2D* WorldCamera{ nullptr };
Listener KeyListener(
	[](Event _msg)
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
Listener MouseWheel(
    [](Event _msg)
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


EntityComponentSystem *TestECS;
SystemList MainSystems;

COMPONENT(MovementComponent)
{
    Vec3 Velocity;
    Vec3 Acceleration;
};
COMPONENT(PositionComponent)
{
    Vec3 Position;
};
struct MovementSystem
    :
    public BaseSystem
{
    MovementSystem()
        :
        BaseSystem()
    {
        AddComponentType(PositionComponent::ID);
        AddComponentType(MovementComponent::ID);

        DEBUG_CODE(SystemName = (typeid(this).name()));
    }

    virtual void UpdateComponents(float _delta, BaseComponent** _components)
    {
        PositionComponent* Pos = (PositionComponent*)_components[0];
        MovementComponent *Movement = (MovementComponent*)_components[1];

        Movement->Velocity += Movement->Acceleration;
        Pos->Position += Movement->Velocity;
        Movement->Velocity *= .9;
    };
};
MovementSystem movementSys;
PositionComponent PosComponent;
MovementComponent TestMovementComponent;


#include"Core/Math/Easing.h"

class App
	: public Application
{
    //=================================================================================================================================================================
    //                                                USER VARIABLES
    //=================================================================================================================================================================

    EntityComponentSystem *ECS{ nullptr };

	Vec2 Vertices[3] = { {200.0, 200.0 },  { 400.0, 200.0 },  { 0.0, 400.0 } };
	Vec2 UVcoords[3] = { {  0.0,   0.0 },  {   1.0,   0.0 },  { 1.0,   1.0 } };

	GLuint Indices[3] = { 0, 1, 2 };
    GLuint VAO{ 0 }, VBO{ 0 };


    Mesh *TestMesh{ nullptr };
    Shader *TextureShader{ nullptr };

    FrameBufferObject *FBO{ nullptr };
    Graphics::Texture *TestTexture{ nullptr };
    Graphics::Texture *TestTexture2{ nullptr };

	OpenGL::Renderer2D *MainRenderer{ nullptr };
	Profiling::DisplayWindow *ProfilerTest{ nullptr };

	Transform ModelMatrix = Transform(Vec3(0), Vec3(0), "ModelMatrix");

    /// This needs to go
    std::string VTextureRenderer =
               "#version 330 core                                                                                                                        \n\
                layout(location = 0) in vec2 aPos;                                                                                                       \n\
                layout(location = 1) in vec4 Position;                                                                                                   \n\
                uniform mat4 ProjectionMatrix;                                                                                                           \n\
                uniform mat4 ViewMatrix;                                                                                                                 \n\
                out  vec2 TexCoords;                                                                                                                     \n\
                void main()                                                                                                                              \n\
                {                                                                                                                                        \n\
                    mat4 ModelViewMatrix = (ViewMatrix * mat4(1.0));                                                                                     \n\
                    mat4 ModelViewProjectionMatrix = (ProjectionMatrix * ModelViewMatrix);                                                               \n\
                    gl_Position = ModelViewProjectionMatrix * vec4( (aPos.x * Position.z) + Position.x, (aPos.y * Position.w) +  Position.y, -1.0, 1.0); \n\
                }";

    std::string FTextureRenderer =
              "#version 330 core                                                     \n\
               uniform sampler2D DiffuseTexture;                                     \n\
               in  vec2 TexCoords;                                                   \n\
               out vec4 FragColor;                                                   \n\
               void main()                                                           \n\
               {                                                                     \n\
                   FragColor = vec4(texture(DiffuseTexture,TexCoords.xy).xyz, 1.0);  \n\
               }";

    //=================================================================================================================================================================
    //=================================================================================================================================================================
  
	size_t PreviousTime;
    
    virtual void OnCreate() override
	{// Initialization
        
        // TEST_ASSERT( TEST_Memory_Pool_Class() , " Memory Pool Class Complete " , " Memory Pool Class Complete ");
        // TEST_ASSERT( Creatures::TEST_SPRINGS(), " Springs Class Complete ", " Springs Class Complete ");
        // TEST_ASSERT( TEST_Ring_Buffer_Class() , " Ring Buffer Class Complete " , " Ring Buffer Class Complete ");

         int *A = new int[1000];

        /* Load up the Listeners for the Various Input Events */
        {
            RegisterListener(   WM_KEYDOWN, KeyListener);
            RegisterListener(WM_MOUSEWHEEL, MouseWheel);
        }

        /* Creates a Renderer for our Application */
		MainRenderer = new OpenGL::Renderer2D({SCREEN_X, SCREEN_Y});

        /* Set the Application window Camera and World Camera to the Renderer */
        {
            getWindow().s_Camera(&MainRenderer->g_Camera());
            WorldCamera = &getCamera();
        }

        /* Create a Triangle to Test with */
		getWindow().defaultShader().Bind();
        {
            VAO = OpenGL::new_VAO();
            OpenGL::bind_VAO(VAO);

            VBO = OpenGL::new_VBO();
            OpenGL::bind_VBO(VBO);
            OpenGL::set_BufferData(sizeof(Vertices), &Vertices);
            OpenGL::set_Attribute(getWindow().defaultShader().g_Handle(), 2, "aPos");
        }

 		/*   Make a Profiler Window which Displays the FPS as a graph */
        {
            float Aspect = getCamera().AspectRatio;
            float Size = 100;
            ProfilerTest = new Profiling::DisplayWindow
            (
                { SCREEN_X / 2  , SCREEN_Y / 2 },
                { std::floor((Size)* Aspect), std::floor((Size * 2)) },
                { std::floor(Size*.35 * Aspect), std::floor((Size)) }
            );
            ProfilerTest->Update(1);
        }

        /// What is being rendered to the screen is a Texture, that is capped at 32k x 32 k duh. 
        /// Idk how the fuck to fix that just yet it means my post processing efforts might be limited or that I need to not scale the texture but need to texture the Images
        TODO("Find out why instance count is being capped. It is likely being capped at 64,000 roughly if I had to guess. Check to see if unsigned integer is used");
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

                   MainRenderer->renderQuad({ x * 8.0f, y * 8.0f }, { 7,7 }, OpenGL::Renderer::normalize_RGBA_Color(R, G, B, 255));
                   MainRenderer->draw_Line(x * 8.0f, y * 8.0f ,x * 8.0f + 7 ,y * 8.0f + 7);

                }
            }
  		    DEBUG_CODE(CheckGLERROR());
        }
 
        /* Create FBO to Test with */
        {
            FBO = new FrameBufferObject(SCREEN_X, SCREEN_Y);
            FBO->Bind();
        }
  
        Init_DefaultShaders();

        /* Load Test Textures to Test With using two different Texture Constructors */
        {
            Graphics::Bitmap *Bmp = new Graphics::Bitmap("../Resources/Test.bmp");
            TestTexture = new Graphics::Texture(*Bmp);
            TestTexture2 = new Graphics::Texture("../Resources/Sheep.bmp");
        }

        TextureShader = new  Shader(VTextureRenderer, FTextureRenderer); ;

        ///Entity Component System
        {
            //    ECS = new EntityComponentSystem();
            //    SystemList mainSystems;
            //    ECStest::TransformComponent transformComp;
            //    ECS->AddComponent(transformComp);
            //    ECStest::InitECS();
            //    TestECS = new EntityComponentSystem();
            /// Create Entities
            //    EntityPTR Entity = TestECS->MakeEntity(PosComponent, TestMovementComponent);
            /// Create Systems
            //    MainSystems.AddSystem(movementSys);
            //    TestECS->AddComponent(Entity, &PosComponent);
            //    TestECS->AddComponent(Entity, &TestMovementComponent);
        }


        DEBUG_CODE(CheckGLERROR());
	}
    virtual void OnRender() override
    {
        FBO->Bind();
        {// Bind the Framebuffer Object

            FBO->Clear();
            OpenGL::bind_VAO(VAO);

            getWindow().defaultShader().Bind();
            {// Bind the Shader and the Uniforms

                ModelMatrix.Bind();
                getCamera().Bind();
                OpenGL::Renderer::drawArray(VBO, 3);
            }
            getWindow().defaultShader().Unbind();

            MainRenderer->Render();
            DEBUG_CODE(CheckGLERROR());

            ProfilerTest->Render();

            TestTexture->g_Handle();
             
           // MainRenderer->renderImage({ 100, 100 }, { 100, 300 }, TestTexture);
          //  MainRenderer->renderImage({ 400, 300 }, { 300, 300 }, TestTexture2);
        }
        FBO->Unbind();
        // Renders the Frame Buffer Object we Rendered all the Images to
        MainRenderer->renderImage({ 0, 0 }, { SCREEN_X, SCREEN_Y }, FBO->RenderTarget);

        DEBUG_CODE(CheckGLERROR());
	}
	virtual void OnUpdate() override
	{// User Genrated Per Frame Update

		size_t NewTime = Timing::Timer<Milliseconds>::GetTime();
    	size_t Time = NewTime - PreviousTime;
		PreviousTime = NewTime;
	 	ProfilerTest->Update((uint32_t)(Time));

         //  MainRenderer->Submit(*TextureShader, *TestTexture, *TestMesh);
        //  TestECS->UpdateSystems(MainSystems, (float)(Time / 1000.0f));
	}
    virtual void OnEnd() override
    {// Exit of the Application and Clean up

        delete(TestTexture);
        delete(TestTexture2);
        delete(FBO);
        delete(MainRenderer);
        delete(ProfilerTest);
        delete(ECS);

    }

};





struct Dummy
{
    Dummy(int _a, int _b) : A(_a), B(_b)
    {}

    int Add()
    {
        return A + B;
    }
    int B;
    int A;
};


int main()
{
    DEBUGPrint(CON_Red,"Problem with the Debug print Symbols hConsole and DEBUGMutex in that they have no real home on where they should be. It is possible Common.h should perhaps have a CPP file for Symbol definition");

    TODO(" Setup Mock ups which use the Application class to setup a state in a way that I can test various functionality by switching through different applications. \n Each Module should have its very own Application class. ");
    TODO(" Serious Restructuring needs to take place to the Entire project, it is starting to grow rather large and I am not happy with some of the early design and structure decisions that have begun to slightly conflict. \n\
 It would be wise to reformat and refactor the project before these minor issues become big ones  ");

    Dummy Dum(10, 10);



  //FUKKKKKKKKKKKKFKFKFKFKFKFK      ThreadPool::get().Async(Core::Threading::ThreadPool::mem_fn<int,Dummy>(int(&Dum(10,10)), 40);


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

#define MY_WRAPPER
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
		Print("\n\n\n\n Loop Counter:" << LOOP_COUNT << " iterations in the Worker Functions\n");
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
\


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
\

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