#include "ColorWheel.h"

QColor cnc::get_RGB_by_angle(complex_scalar z,bool fade_by_mod){
    scalar th = std::arg(z);
    if (th < 0)
        th += 2*M_PI;
    uint i = th/(2*M_PI)*NB_SEG;
    auto color = WHEEL[i];
    scalar mod = std::abs(z);
    constexpr scalar LOW_VAL_TRESH = 8e-2;
    scalar fade = (fade_by_mod ? (mod < LOW_VAL_TRESH ? mod/LOW_VAL_TRESH : 1.) : 1.);
    constexpr scalar HIGH_VAL_TRESH = 1e1,MAX_VAL = 5e1;
    if (mod > HIGH_VAL_TRESH && fade_by_mod){
        auto cl = [] (scalar x) {
            return cnc::algo::clamp<scalar>(x,0.,255.);
        };
        fade = algo::clamp<scalar>(mod-HIGH_VAL_TRESH,0,MAX_VAL)/MAX_VAL;
        scalar sat = fade*255;
        return QColor::fromRgb(cl(color[0]+sat),
                cl(color[1]+sat),
                cl(color[2]+sat)
                );
    }
    return QColor::fromRgb(color[0]*fade,color[1]*fade,color[2]*fade);
}
