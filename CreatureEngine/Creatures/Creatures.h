#pragma once
#include<string>
#include<vector>
#include"Core/Common.h"
#include"Core/ECS/ECS.h"
#include"../Renderer/LowLevel/OpenGL/Shader/Shader.h"

#include"Physics/Springs.h"

#include<stack>

namespace Creatures
{




 extern     EntityComponentSystem CreatureECS;
    class Creature
    {
        static bool initCreatures()
        {
            return true;
        }
        public:
            std::vector<Spring> Springs;

        static Shader *CreatureShader;
    };
}



//get_BlockAddress(get_FreeBlock());index_from_MemoryLocation(_item)



// data_type *get_BlockAddress(uint32_t _id)
// {
//     return static_cast<data_type*>(&Data[_id]);
// }
//
//  uint32_t index_from_MemoryLocation(void *_memory)
//  {
//      return ((char *)_memory - (char*)Data) / sizeof(data_type);
//  }
//
// uint32_t get_FreeBlock()
// {
//     uint32_t result = freeIDs.top();
//     freeIDs.pop();
//     return result;
// }