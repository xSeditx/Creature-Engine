#pragma once

#include"Core/Common.h"

#include "Renderer.h"
#include"../Camera/Camera2D.h"

namespace OpenGL
{
	class Renderer2D
		:
		public Renderer
	{
        Camera2D mainCamera{Vec2(640,480)};
        

        void drawQuad(Vec2 _topLeft, Vec2 _bottomRight);
        void drawLine(Vec2 _start, Vec2 _end);
        void drawCircle(Vec2 _pos, float _radius);

        std::vector<Vec3> LineData;
        std::vector<Vec3> CircleData;
        std::vector<Vec3> QuadData;
	};
}