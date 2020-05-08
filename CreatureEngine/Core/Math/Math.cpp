#include"Math.h"



namespace Math
{
    float Point_Direction(float _x1, float _y1, float _x2, float _y2)
    {// Return Angle of two points
        return atan2({_y2 - _y1}, {_x2 - _x1});
    }
    float Point_Direction(Vec2 _a, Vec2 _b)
    {// Return Angle between two Vectors
        return atan2(_b.y - _a.y, _b.x - _a.x);
    }

};













// atan2(vector1.y - vector2.y, vector1.x - vector2.x)
//  Vec2 A{_x1, _y1};
//  Vec2 B{ _x2, _y2 };
//  glm::cross
//  return atan2(glm::normalize(glm::cross(A, B)), glm::dot(A, B))
