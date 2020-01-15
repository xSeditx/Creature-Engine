#include"Bitmap.h"
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
	}
}