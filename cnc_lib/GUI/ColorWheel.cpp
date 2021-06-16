#include "ColorWheel.h"

QColor cnc::get_RGB_by_angle(complex_scalar z,bool fade_by_mod){
    scalar th = std::arg(z);
    if (th < 0)
        th += 2*M_PI;
    uint i = th/(2*M_PI)*NB_SEG;
    auto color = WHEEL[i];
    scalar mod = std::abs(z);
    scalar fade = (fade_by_mod ? (mod < 1e-3 ? mod/1e-3 : 1.) : 1.);
    return QColor::fromRgb(color[0]*fade,color[1]*fade,color[2]*fade);
}
