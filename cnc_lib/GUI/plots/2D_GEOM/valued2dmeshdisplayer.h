#ifndef VALUED2DMESHDISPLAYER_H
#define VALUED2DMESHDISPLAYER_H

#include "mesh2ddisplayer.h"
#include <memory>

namespace cnc {
using meshref = std::shared_ptr<algo::geometry::Mesh2>;

class Valued2DMeshDisplayer : public Mesh2DDisplayer
{
public:
    Valued2DMeshDisplayer(meshref m,const vec& v);

private:
    virtual void plot(frame_draw_object& fdo) override;
    virtual void compute_values(const frame_info&) override;

    /*
    algo::calculus::scalar_function_1D R;
    algo::calculus::scalar_function_1D B;
    */
    std::vector<QColor> vertex_color;
    std::map<algo::topology::face,QColor,algo::topology::faceComp> face_color;
    range vrange;
    vec values;
};

}
#endif // VALUED2DMESHDISPLAYER_H
