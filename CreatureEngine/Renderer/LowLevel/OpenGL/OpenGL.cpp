#include"OpenGL.h"



#include<utility>//std::pair
#include<vector>//std::vector
/*
*/
namespace OpenGL
{
	/*
	Experimental nonsense more than likely
	========================================================*/
	struct Mesh{};
	struct Shader {};
	struct Texture{};


	enum  Surface_t{ Normals, Albedo, Metallic};
	using SurfaceFragment = std::pair<Surface_t, Texture>;
	using Surface = std::vector<SurfaceFragment>;
	using Material = std::pair<Surface, Shader>;
	using RenderPair = std::pair<Material, Mesh>;
	//======================================================
	void InitOpenGL()
	{



	

		Print(gladLoadGL());
		RenderPair TestObject =
		{
			Material
			(
				{
					{
						{
							Normals, Texture()
						},
				        {
				        	Albedo, Texture()
				        },
				        {
				        	Metallic, Texture()
				        }
					},
					Shader()
				}
			),
			Mesh()
		};
	}
}


/*
Mathmatics for Lighting in OpenGL
https://www.tjhsst.edu/~dhyatt/supercomp/n310.html

Vertexbanana Basic-OpenGL-Tutorials
https://github.com/vertexbanana/Basic-OpenGL-Tutorials/blob/master/BasicTutorials/DEPENDENCIES/include/GL/GL.H
*/
