/**
  * @file figure.h defines image loading/plotting and image editing
  */
#ifndef FIGURE_H
#define FIGURE_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plottable.h"
#include "core/algo/statistics/statistics.h"
#include <QImage>
#include <QString>

namespace cnc {

/**
 * @brief The Figure class ables image loading, editing and saving
 */
class Figure : public Plottable
{
public:
    Figure(const QString& filename);
    Figure(uint w,uint h);
    Figure(uint w,uint h,const algo_GUI::texture_function& tf);

    void set_pixel(uint i,uint j,const QColor&);

    inline void set_edit_mode(){
        if (!edit_mode){
            edit_mode = true;
            edit_img = pix_img.toImage();
        }
    }

    inline void set_display_mode(){
        if (edit_mode){
            edit_mode = false;
            pix_img = QPixmap::fromImage(edit_img);        }
    }

    /**
     * @brief get_rgb_cloud returns the colors of the image seen as a point cloud in RGB space
     * @return rgb space
     */
    cloud get_rgb_cloud();

    inline uint get_width() const {
        return width;
    }

    inline uint get_height() const {
        return height;
    }

    inline  plot_type get_type() const override {
        return figure;
    }

private:

    bool edit_mode;

    uint width;
    uint height;

    QPixmap pix_img;
    QImage edit_img;

    void plot(frame_draw_object&) override;

    void compute_value_range(const frame_info&) override {}
    void compute_values(const frame_info&) override {}

    void call_draw_axis(frame_draw_object&) const override {}

};

}

#endif // FIGURE_H
