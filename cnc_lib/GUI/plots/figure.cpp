#include "figure.h"


cnc::Figure::Figure(const QString &filename) : edit_mode(false)
{
   if (!pix_img.load(filename))
       throw Cnc_error("couldn't load given image");
    width = pix_img.width();
    height = pix_img.height();
}

cnc::Figure::Figure(uint w, uint h) : edit_mode(true),edit_img(w,h,QImage::Format_RGBA64)
{
    width = w;
    height = h;

}

cnc::Figure::Figure(uint w, uint h, const cnc::algo_GUI::texture_function &tf)
{
    edit_mode = true;
    edit_img = QImage(w,h,QImage::Format_RGBA64);

    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<w;i++)
            edit_img.setPixelColor(i,j,tf(i,j));
}

void cnc::Figure::set_pixel(uint i, uint j, const QColor &C)
{
    edit_img.setPixelColor(i,j,C);

}

void cnc::Figure::plot(cnc::frame_draw_object &fdo)
{
    set_display_mode();
    uint d = std::min(fdo.fi.width,fdo.fi.height);
    fdo.painter.drawPixmap(0,0,d,d,pix_img);
}

cnc::cloud cnc::Figure::get_rgb_cloud()
{
    set_edit_mode();
    cnc::cloud R = algo::stat::init_empty_cloud(width*height,3);
    for (uint j = 0;j<height;j++){
        for (uint i = 0;i<width;i++){
            auto C = edit_img.pixelColor(i,j);
            R.points[j*width + i](0) = C.red();
            R.points[j*width + i](1) = C.green();
            R.points[j*width + i](2) = C.blue();
        }
    }

    return R;
}
