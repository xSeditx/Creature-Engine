#include"Core/Common.h"
#include"../Shader/Shader.h"

namespace OpenGL
{

    struct Geometry
    {
        Geometry() = default;
        /* Create Geometry using Data of specified size */
        Geometry(char *_data, uint32_t _size)
            :
            Data(_data),
            Length(_size)
        {}    
        /* Allocate Memory for Empty Geometry*/
        Geometry(uint32_t _size)
            :
            Length(_size)
        {
            Data = malloc(_size);
        }
         

        uint32_t size() { return Length; }
        char* get() { return Data; }

    private:
        uint32_t Start{ 0 };
        uint32_t Length{ 0 };
        char *Data;

    public:
        /* Returns Geometry object containing Quad data */
        static Geometry& new_Quad(Vec2 _position, Vec2 _size)
        {
            Geometry result = Geometry(sizeof(Vec2) * 6);

            Vec2 Offset = Vec2(_size.x / 2.0f, _size.y / 2.0f);
            Vec2 tempData[6] =
            {
                  Vec2(_position.x - Offset.x, _position.y + Offset.y),
                  Vec2(_position.x - Offset.x, _position.y - Offset.y),
                  Vec2(_position.x + Offset.x, _position.y - Offset.y),
                  Vec2(_position.x - Offset.x, _position.y + Offset.y),
                  Vec2(_position.x + Offset.x, _position.y - Offset.y),
                  Vec2(_position.x + Offset.x, _position.y - Offset.y)
            };
            memcpy((void*)result.Data, (void*)tempData, sizeof(Vec2) * 6);
            return result;
        }
        /* Returns Geometry object containing Triangle data */
        static Geometry& new_Triangle(Vec2 _p1, Vec2 _p2, Vec2 _p3)
        {
            Geometry result = Geometry(sizeof(Vec2) * 3);
            Vec2 tempData[3] = { _p1, _p2,_p3 };
            memcpy((void*)result.Data, (void*)tempData, sizeof(Vec2) * 3);
            return result;
        }
    };



    struct Pipeline
    {
 
    };

}


/* 
void initVulkan()
{
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    void createRenderPass()
    {
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    }
    createGraphicsPipeline();
}
*/