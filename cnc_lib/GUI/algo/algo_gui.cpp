#include "algo_gui.h"

using namespace cnc::algo_GUI;

cnc::algo_GUI::mapping cnc::algo_GUI::gen_mapping_value_to_frame(const cnc::range &x_val_range, const cnc::range &y_val_range, const cnc::frame_info& F)
{
    using namespace cnc::algo::calculus;
    scalar_function_1D ix = build_range_mapper(x_val_range,{0.f,F.width});
    scalar_function_1D iy = build_range_mapper(y_val_range,{0.f,F.height});
    //scalar_function_1D iy = build_1D_linear_interpolator({y_val_range.first,y_val_range.second},{0.f,(float)F.height});
    int h = F.height;
    return [ix,iy,h] (const QPointF& p){
        return QPoint((int)ix(p.x()),h - (int)iy(p.y()));
    };
}


cnc::algo_GUI::Qcloud cnc::algo_GUI::convert_cloud_to_qcloud(const cnc::cloud & C)
{
    Qcloud QC(C.size());
    for (uint i = 0;i<C.size();i++)
        QC[i] = QPointF(C.points[i](0),C.points[i](1));
    return QC;
}

std::pair<cnc::range, cnc::range> cnc::algo_GUI::compute_cloud_bounds(const Qcloud & qc)
{ 
    cnc::range X{MAXFLOAT,-MAXFLOAT};
    cnc::range Y{MAXFLOAT,-MAXFLOAT};

    for (const QPointF& p : qc){
        if (p.x() < X.first)
            X.first = p.x();
        if (p.x() > X.second)
            X.second = p.x();
        if (p.y() < Y.first)
            Y.first = p.y();
        if (p.y() > Y.second)
            Y.second = p.y();
    }
    return {X,Y};
}

QString cnc::algo_GUI::to_qstring(float number, int precision)
{
    std::stringstream tmp;
    tmp << std::fixed << std::setprecision(precision) << number;

    QString str(tmp.str().c_str());
    return (str);
}

double cnc::algo_GUI::point_to_pixel(double points)
{
    double dpi = QGuiApplication::primaryScreen()->physicalDotsPerInch();
    return (points*dpi / 72);
}

QColor cnc::algo_GUI::vec_to_color(const cnc::vec &v)
{
    auto clamp = [] (float x){
        if (x < 0)
            return 0;
        if (x > 255)
            return 255;
        return (int)x;
    };
    return QColor::fromRgb(clamp(v(0)),clamp(v(1)),clamp(v(2)));
}

QPointF cnc::algo_GUI::vec_to_QPointF(const cnc::vec& x){
    return QPointF(x(0),x(1));
}
