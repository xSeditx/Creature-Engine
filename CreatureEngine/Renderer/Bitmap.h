#include"Core\Common.h"
namespace Graphics
{
	class Bitmap
	{


		/* Returns Image Width */
		uint32_t Width() { return Size.x; }
		/* Returns Image Height */
		uint32_t Height() { return Size.y; }

		size_t size() { return Size.x * Size.y * BitsPerPixel; }
	private:
		Vec2 Size{0,0};
		char BitsPerPixel{4};
	};
}