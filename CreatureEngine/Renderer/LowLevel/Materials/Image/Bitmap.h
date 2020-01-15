#include"Core\Common.h"
namespace Graphics
{
	class CREATURE_API Bitmap
	{
	public:
		/* Default because we are creating custom constructors */
		Bitmap() = default;

		/* Create a Bitmap from a Memory  block */
		Bitmap(void* _memory, Vec2 _dimensions);

		/* Returns Image Width */
		uint32_t Width() { return Size.x; }

		/* Returns Image Height */
		uint32_t Height() { return Size.y; }

		/* Return the total size in Bytes for our entire Image */
		size_t size() { return static_cast<int64_t>(Size.x * Size.y * BytesPerPixel); }

		/* Return pointer to Raw data */
		uint8_t* Data()            { return rawData; }

		/* Return pointer to specific byte offset in Raw BMP data */
		uint8_t* Data(size_t _pos) { return rawData + _pos; }

		/* Return how many channels in the form of RGBA our image contains RGBA being 4*/
		uint8_t Channels() { return BytesPerPixel;	}

	private:
		uint8_t* rawData;// Raw Mapped Data 
		iVec2     Size{0,0}; // A int[2] containing the Width and Height of our Image
		char BytesPerPixel{4}; // How many bytes each of our pixels contain. 
		/// Likely later incorperate a RedBitsCount, BlueBitCount, GreenBitCount etc... for nonstandard bitstructures in the image
	};
}