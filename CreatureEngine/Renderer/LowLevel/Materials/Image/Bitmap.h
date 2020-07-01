#pragma

#include"../../../../Core/Common.h"

#if _DEBUG
//#    pragma comment(lib, "debugSOIL.lib")
//#    pragma comment(lib, "SOIL.lib")
#else
//#    pragma comment(lib, "SOIL.lib")
#endif

//namespace Graphics
//{
	class CREATURE_API Bitmap
	{
	public:
		/* Default because we are creating custom constructors */
		Bitmap() = default;

		/* Create a Bitmap from a Memory  block */
		Bitmap(void* _memory, Vec2 _dimensions) noexcept;

        /* Creates a Bitmap from a Specified Image */
        Bitmap(std::string _file) noexcept;

		/* Returns Image Width */
		uint32_t Width() noexcept { return Size.x; }

		/* Returns Image Height */
		uint32_t Height() noexcept { return Size.y; }

		/* Return the total size in Bytes for our entire Image */
		size_t size() noexcept { return  ((size_t)Size.x * (size_t)Size.y * (size_t)BytesPerPixel); }

		/* Return pointer to Raw data */
		uint8_t* Data() noexcept { return rawData; }

		/* Return pointer to specific byte offset in Raw BMP data */
		uint8_t* Data(size_t _pos) noexcept { return rawData + _pos; }

		/* Return how many channels in the form of RGBA our image contains RGBA being 4*/
		uint8_t Channels() { return BytesPerPixel;	}

		/* Flip Image on Y axis to be inline with OpenGL */
		void FlipYAxis();

		/* Flip Image on Y axis to be inline with OpenGL */
		void FlipXAxis();

        iVec2 Dimensions() { return Size; }
	private:
        uint8_t* rawData{ nullptr };// Raw Mapped Data 
		iVec2     Size{0,0}; // A int[2] containing the Width and Height of our Image
		char BytesPerPixel{4}; // How many bytes each of our pixels contain. 
		/// Likely later incorperate a RedBitsCount, BlueBitCount, GreenBitCount etc... for nonstandard bitstructures in the image
	};
//}