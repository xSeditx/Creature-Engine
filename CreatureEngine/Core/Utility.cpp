#include"Utility.h"
#include"Common.h"
#include<iomanip>



std::ostream& operator<<(std::ostream& _stream, const Vec2& _vector)
{
    _stream << " [ " << _vector.x << " ][ " << _vector.y << " ]";
    return _stream;
}
std::ostream& operator<<(std::ostream& _stream, const Vec3& _vector)
{
    _stream << " [ " << _vector.x << " ][ " << _vector.y << " ][ " << _vector.z << " ]";
    return _stream;
}
std::ostream& operator<<(std::ostream& _stream, const Vec4& _vector)
{
    _stream << " [ " << _vector.x << " ][ " << _vector.y << " ][ " << _vector.z << " ][ " << _vector.w << " ]";
    return _stream;
}
std::ostream& operator<<(std::ostream& _stream, const Mat4& _matrix)
{
    int Precision = 10;
    _stream <<  std::fixed << std::setprecision(Precision - 3);
    _stream << " [ " << std::setw(Precision) << _matrix[0].x << " ][ " << std::setw(Precision) << _matrix[0].y << " ][ " << std::setw(Precision) << _matrix[0].z << " ][ " << std::setw(Precision) << _matrix[0].w << " ] \n";
    _stream << " [ " << std::setw(Precision) << _matrix[1].x << " ][ " << std::setw(Precision) << _matrix[1].y << " ][ " << std::setw(Precision) << _matrix[1].z << " ][ " << std::setw(Precision) << _matrix[1].w << " ] \n";
    _stream << " [ " << std::setw(Precision) << _matrix[2].x << " ][ " << std::setw(Precision) << _matrix[2].y << " ][ " << std::setw(Precision) << _matrix[2].z << " ][ " << std::setw(Precision) << _matrix[2].w << " ] \n";
    _stream << " [ " << std::setw(Precision) << _matrix[3].x << " ][ " << std::setw(Precision) << _matrix[3].y << " ][ " << std::setw(Precision) << _matrix[3].z << " ][ " << std::setw(Precision) << _matrix[3].w << " ] \n";

    return _stream;
}


 




/* =============================================================================================================================================
/*                                          NOTES
/* =============================================================================================================================================

 Formatting Numbers with C++ Output Streams
 http://umich.edu/~eecs381/handouts/formatting.pdf

 IO Manipulators reference
 http://www.cplusplus.com/reference/iomanip/
 
*/

