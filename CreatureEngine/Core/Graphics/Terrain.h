#include"../Common.h"
#include"../../Renderer/LowLevel/Materials/Image/Texture.h"
#include"../../Renderer/LowLevel/OpenGL/Renderer/GLBuffers.h"


#define MAXWIDTH 10000

struct Terrain_Chunk
{
    Terrain_Chunk(Vec2 _pos, Vec2 _size, uint32_t _subDivisions);
    void Render() {}
    void Update() {}
};




struct Terrain 
{
public:
    Terrain();
    Terrain(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    Terrain(Vec3 position, int width, int height, Texture *groundtex);



    ~Terrain();

    Vec3 Position, Rotation;
    Mat4 Transform;
    VertexArrayObject *VAO;
private:
    uint32_t MapID;

    uint32_t
        X,
        Y,
        Height,
        Width;

    iVec2 Size;
    iVec2 GridSize;

    float Max_Height = 0;

    float *HeightMap{ nullptr };
    float SampleTerrain(float x, float z);

    int
        VertexCount,
        ColorsCount,
        IndexCount,
        NormalCount;
    std::vector<Vec3> VecVertices;
    std::vector<Vec3> VecNormals;
 
    void Update();


    int Get_Vertex(int x, int z) { return (x + Width * z); }
    Vec3 GetNormal(Vec3 v1, Vec3 v2, Vec3 v3);
// =====---------- STATICS ----------=====
    static float OneDimension[MAXWIDTH];
    static float Gradient[1000][1000][2]; /// Compiler loses heap space due to this, Need new Perline noise Generator, this one sucks


    static float PerlinNoise1D();
    static float PerlinNoise2D(float x, float y);
    static float PerlinNoise3D();
    static float Gradiant(float p);
    static float Fade(float t);
    static float Noise(float p);
    static float Lerp(float a0, float a1, float w);
    static float DotGridGradient(int ix, int iy, float x, float y);

    std::vector<Terrain_Chunk> Chunks;
};

 