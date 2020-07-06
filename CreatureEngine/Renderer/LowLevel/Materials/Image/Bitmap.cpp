#include"Bitmap.h"

#include"../Bin/SOIL/src/SOIL.h"

//namespace Graphics
//{
	Bitmap::Bitmap(void * _memory, Vec2 _dimensions) noexcept
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
    Bitmap::Bitmap(std::string _file) noexcept
    {
        int32_t 
            Sx{ 0 }, 
            Sy{ 0 };

        int BPP{ 0 };

        rawData = SOIL_load_image(_file.c_str(), &Sx, &Sy, &BPP, SOIL_LOAD_AUTO);
        if (!rawData)
        {
            DEBUGPrint(CON_Red, "Failed to load texture: " << _file);
            return;
        }

        Size = Vec2(Sx, Sy);
        BytesPerPixel = BPP;

       // FlipXAxis();
    }



    void Bitmap::FlipYAxis()
    { // FLIP THE Image TO PLAY NICE WITH OPENGL
        size_t RowSize = Width() * sizeof(unsigned char) * Channels();
        size_t ImageSize = RowSize * Height();

        unsigned char* TempImage = new unsigned char[ImageSize] {0};

        for (auto I{ 0 }; I < Height(); ++I)
        {
            memcpy(TempImage + ((size_t)I * RowSize), (rawData + (ImageSize - (((size_t)I + 1) * RowSize))), RowSize);// Copy bottom row into top of Temp buffer
        }

        memcpy(rawData, TempImage, ImageSize);
        delete[](TempImage);
    }


    void Bitmap::FlipXAxis()
    { // FLIP THE Image TO PLAY NICE WITH OPENGL
         size_t RowSize = Width() * sizeof(unsigned char) * Channels();
         size_t ImageSize = RowSize * Height();

         uint8_t* TempImage = new uint8_t[ImageSize] {0};

         for (int Y{ 0 }; Y < Height() ; ++Y)
         {
             for (int X = Width() ; X > 0; --X)
             {
                 uint8_t* Source = rawData;
                 int
                     Src = (((X ) * Channels()) - Channels()),
                     Dest = ((Width() - X) * Channels()) * Y;
                 memcpy(TempImage + Dest, Source + Src, Channels());
             }
         }
         
         memcpy(rawData, TempImage, ImageSize);
         delete[](TempImage);
    }
//}// NS Graphics
 