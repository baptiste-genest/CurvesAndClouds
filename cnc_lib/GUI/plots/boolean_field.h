#ifndef BOOLEAN_FIELD_H
#define BOOLEAN_FIELD_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "field.h"
#include "core/algo/calculus/calculus.h"
#include <QPixmap>

namespace cnc {

class BooleanField : public Field
{
public:
    BooleanField(const cnc::algo::calculus::property_2D&,range x,range y);

    inline plot_type get_type() const override
    {
        return boolean_field;
    }
private:
    void call_draw_axis(frame_draw_object&) const override {}
    algo::calculus::property_2D P;
    QPixmap img;
    bool thick_border = false;

    void compute_values(const frame_info&) override;
    void compute_value_range(const frame_info& ) override {}


    inline void plot (frame_draw_object& fdo) override {
        fdo.painter.drawPixmap(0,0,fdo.fi.width,fdo.fi.height,img);
    }

    /*
    static const uint MAX_WIDTH = 200;
    static const uint MAX_HEIGHT = 200;
    */

};

}

#endif // BOOLEAN_FIELD_H
