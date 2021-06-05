#include "vector_field.h"
using namespace cnc;

VectorField::VectorField(const algo::calculus::vector_function_2D &f, const range &x, const range &y, uint nb_vector) : value(f),nb_vector_per_line(nb_vector)
{
    Plottable::x_range = x;
    Plottable::y_range = y;
    VectorField::compute_values(frame_info());
}

void VectorField::plot(frame_draw_object &fdo)
{
    float xs = 0.5f*(fdo.fi.width)/nb_vector_per_line;
    float ys = 0.5f*(fdo.fi.height)/nb_vector_per_line;
    auto Mx = cnc::algo::calculus::build_range_mapper({0,nb_vector_per_line},x_range);
    auto My = cnc::algo::calculus::build_range_mapper({0,nb_vector_per_line},y_range);

    auto M = cnc::algo_GUI::gen_mapping_value_to_frame(x_range,y_range,fdo.fi);

    float mid_val = (norm_range.first + norm_range.second)*0.5f;

    auto R = algo::calculus::build_range_mapper({mid_val,norm_range.second},{0,255});
    auto B = algo::calculus::build_range_mapper({norm_range.first,mid_val},{255,0});
    float vanishing_treshold = 0.2f;
    float vanish_below = norm_range.first*(1-vanishing_treshold) + norm_range.second*vanishing_treshold;
    auto S = algo::calculus::build_range_mapper({norm_range.first,vanish_below},{0,1.f});

    for (uint j = 0;j<nb_vector_per_line;j++){
        for (uint i = 0;i<nb_vector_per_line;i++){
            QPointF p(Mx(i),My(j));
            const cnc::vec& val = samples[j*nb_vector_per_line+i];
            float l = val.norm();
            QPointF v = cnc::algo_GUI::vec_to_QPointF(val.normalize());
            v.rx() *= xs*S(l);
            v.ry() *= -ys*S(l);
            QPoint gp = M(p);
            QPen C = QColor::fromRgb(R(l),0,B(l));
            C.setWidth(2);
            fdo.painter.setPen(C);
            fdo.painter.drawLine(gp,gp+v);
            fdo.painter.drawEllipse(gp+v,1,1);
        }
    }
}

void VectorField::compute_values(const frame_info &)
{
    const uint sample_size = nb_vector_per_line;
    auto Mx = cnc::algo::calculus::build_range_mapper({0,sample_size},x_range);
    auto My = cnc::algo::calculus::build_range_mapper({0,sample_size},y_range);

    std::vector<scalar> norm_samples(sample_size*sample_size);
    samples.resize(sample_size*sample_size);
    for (uint j = 0;j<sample_size;j++)
        for (uint i = 0;i<sample_size;i++){
            samples[j*sample_size + i] = value(Mx(i),My(j));
            norm_samples[j*sample_size+i] = samples[j*sample_size+i].norm();
        }
    norm_range = algo::get_min_max_range(norm_samples);
}
