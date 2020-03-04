#include"Bitmap.h"

#include"../Bin/SOIL/src/SOIL.h"

namespace Graphics
{
	Bitmap::Bitmap(void * _memory, Vec2 _dimensions)
		:
		rawData(static_cast<uint8_t*>(_memory)),// Raw Mapped Data 
		Size{ _dimensions }, // A int[2] containing the Width and Height of our Image
		BytesPerPixel{ 4 } // How many bytes each of our pixels contain. 
	{
		if (size() % 3 == 0)
		{
            BytesPerPixel = 3;
		}
		BytesPerPixel = 4;
	}


    /* Creates a Bitmap from a Specified Image */
    Bitmap::Bitmap(std::string _file)
    {
        int32_t 
            Sx{ 0 }, 
            Sy{ 0 };

        int BPP{ 0 };

        rawData = SOIL_load_image(_file.c_str(), &Sx, &Sy, &BPP, SOIL_LOAD_AUTO);

        Size = Vec2(Sx, Sy);
        BytesPerPixel = BPP;
    }


}// NS Graphics
