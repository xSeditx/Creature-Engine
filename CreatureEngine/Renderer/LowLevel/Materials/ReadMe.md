___
## Material Module
___
 Handles Texture Mapping, Shader Instances and Image Loading. Everything responsible for the surfacing of a Polygon
___



___
namespace Graphics
___

___
# Bitmap Class
___

Function Name | Description
--------------|----------------
 uint32_t Width()| Returns Image Width  
 uint32_t Height()| Returns Image Height  
 size_t size() | Returns the Memory footprint in Bytes of The Loaded Bitmap

 Data Members | Description
 -------------|---------------
private:|
Vec2 Size;| Dimensions of given Bitmap
char BytesPerPixel;| Number of Bits Each pixel contains



___
# Texture Class
___
 Image that has been Uploaded to our GPU for use Texturing an set of Polygons
___
		NO_COPY_OR_ASSIGNMENT(Texture);
___

Constructor | Description
------------|--------------
Texture(std::string file);| Load Texture from Image file
Texture(Bitmap &image);| Create a Texture from a Bitmap

Method Names| Description
------------|----------------
void SetTarget(unsigned int param);| Sets the Target type for a given Texture
void SetFiltering(unsigned int param);| Sets the Filtering mode for the Min and Max Filtering for Texture
void SetMagFiltering(unsigned int param);| Sets Specific Filtering mode for Magnification setting
void SetMinFiltering(unsigned int param);| Sets Specific Filtering mode for Minification setting
void SetWrap(unsigned int param);| Sets the Wrap mode for the X and Y wrap of the Texture
void SetWrapX(unsigned int param);|Sets the Wrap mode for the X wrap of the Texture
void SetWrapY(unsigned int param);|Sets the Wrap mode for the Y wrap of the Texture
inline void Bind();| Bind the Texture to OpenGL so it is currently active in the Active Texture Slot
inline void Unbind();| Releases the Texture so it is not Current in OpenGL
inline void MipmapOn();| Enables Mipmapping for the Texture
inline void MipmapOff();| Disables Mipmapping for the Texture

void Render(int _x, int _y, int _w, int _h);| Renders a Quad with the Specific texture at the XY coords specified

Data Members|  Description
------------|-----------
Bitmap *Picture;| Pointer to a Bitmap Object which contains the memory for our Texture Image
uint32_t GL_Handle;| OpenGL naming Handle for identifying which texture this is
uint32_t Target;|  Target format for the type of Texture we have default being set to  GL_TEXTURE_2D

uint32_t Type;| One of the following GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32 and GL_DEPTH_COMPONENT32F.
uint32_t Format;| Byte Format of Texture once Uploaded to the GPU default being GL_RGB,
uint32_t WrapMode;| Wrap mode for our Texture
uint32_t Filtering;| Min/Max Filter mode for our Texture
uint32_t InternalFormat;| Internal storage structure of our Byte array default being GL_RGB;

GPUptr Handle = 0;| Bindless 64 bit pointer to resident GPU memory for Accessing our texture
private:|
bool MipmapComplete;| Mipmapping was been properly defined when true
bool ImageFormatComplete;| Becomes True when the texture is ready to be uploaded to the GPU

