#pragma once


/* 
    Options can likely be handled in main prior to including this to trigger various changes 
*/


#pragma comment (lib, "CreatureEngine.lib")



#include"Core/Application.h"
#include"FrontEnd/Window.h"

#include"Core/Threading/Threadpool.h"


#include"Profiling/SystemInfo.h"
#include"Profiling/MemoryPerf/MemTracker.h"
#include"Profiling/Timing/Benchmark.h"

/* Perhaps I should include all the Rendering stuff in the OpenGL header and Dir Tree from there */
#include"Renderer/LowLevel/OpenGL/OpenGL.h" 
#include"Renderer/LowLevel/OpenGL/Renderer/Primitives.h"

#include"Renderer/LowLevel/OpenGL/Renderer/Renderer.h"
#include"Renderer/LowLevel/OpenGL/Renderer/2DRenderer.h"
#include"Renderer/LowLevel/OpenGL/Renderer/3DRenderer.h"


/* Likely not needed */
#include"Core/Threading/TestFunctions.h"
#include"Core/Memory.h"


/* Potentially split off to just become the Soft bodied physics portion of the engine once the proper
 DOD design is laid out and made genaric */


#include"Creatures/Creatures.h"
#include"Creatures/Physics/Springs.h"



#include"../CreatureEngine/Core/ECS/ECS.h"
#include"../CreatureEngine/Core/ECS/TestComponents.h"
#include"../CreatureEngine/Core/Math/Easing.h"



#include"Renderer/LowLevel/OpenGL/Camera/Camera.h"
#include"Renderer/LowLevel/OpenGL/Camera/Camera3D.h"
#include"Renderer/LowLevel/OpenGL/Camera/Camera2D.h"


#include"../CreatureEngine/Core/Graphics/Model_importer.h"
#include"../../Creature-Engine/CreatureEngine/Renderer/LowLevel/OpenGL/Renderer/Fixed_Func.h"


#include"Core/Common.h"
