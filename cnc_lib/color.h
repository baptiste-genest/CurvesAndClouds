#ifndef COLOR_H
#define COLOR_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


#include <QVector4D>

namespace cnc {
namespace graphics {

struct Color
{
    inline Color(float r,float g,float b,float a = 1.f) : values(r,g,b,a) {}
    inline Color(const QVector3D& v) : values(v) {}
    inline Color(const QVector4D& v) : values(v) {}

    QVector4D values;

    inline float red() const{
        return values[0];
    }
    inline float green() const{
        return values[1];
    }
    inline float blue() const{
        return values[2];
    }
    inline float alpha() const{
        return values[3];
    }

    inline Color operator+(const Color& other) const {
        float s = alpha() + other.alpha();
        if (s < 1e-6)
            return Color(QVector4D());
        float wa = alpha()/s;
        float wb = other.alpha()/s;
        return Color(values*wa + other.values*wb);
    }
};

}
}

#endif // COLOR_H
