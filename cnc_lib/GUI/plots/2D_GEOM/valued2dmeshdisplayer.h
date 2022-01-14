#ifndef VALUED2DMESHDISPLAYER_H
#define VALUED2DMESHDISPLAYER_H

#include "mesh2ddisplayer.h"
#include "../../../core/algo/calculus/finite_elements/vector_finite_elements.h"
#include <memory>

namespace cnc {

using valueUpdater = std::function<vec()>;

class Valued2DMeshDisplayer : public Mesh2DDisplayer
{
public:
    Valued2DMeshDisplayer(cnc::algo::geometry::MeshRef m,const vec& v);
    Valued2DMeshDisplayer(cnc::algo::geometry::MeshRef m,const vec& v,range rv);
    Valued2DMeshDisplayer(cnc::algo::geometry::MeshRef m,const valueUpdater& U,range rv);

private:
    virtual void plot(frame_draw_object& fdo) override;
    virtual void compute_values(const frame_info&) override;

    bool fixed_value_range = false;
    std::vector<QColor> vertex_color;
    std::map<algo::topology::face,QColor,algo::topology::faceComp> face_color;
    range vrange;
    valueUpdater up;
};

class VectorValued2DMeshDisplayer : public Mesh2DDisplayer
{
public:
    VectorValued2DMeshDisplayer(cnc::algo::geometry::MeshRef m,const algo::FEM::VectorSample& V);
private:
    virtual void plot(frame_draw_object& fdo) override;
    virtual void compute_values(const frame_info&) override;
    const algo::FEM::VectorSample& V;
};


}
#endif // VALUED2DMESHDISPLAYER_H
