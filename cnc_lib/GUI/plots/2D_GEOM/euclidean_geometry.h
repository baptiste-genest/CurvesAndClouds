#ifndef EUCLIDEAN_GEOMETRY_H
#define EUCLIDEAN_GEOMETRY_H

#include "cnc_types.h"
#include "cnc_error.h"
#include "../plottable.h"
#include "core/algo/lin_alg.h"

namespace cnc {

namespace euclid {

using placer = std::function<vec()>;

class EuclideanPrimitive {
public:
    EuclideanPrimitive(){
    }
    virtual void draw(frame_draw_object& p,range x,range y) = 0;
    virtual void draw(frame_draw_object& p){
        draw(p,get_x_range(),get_y_range());
    };
    virtual void update_values() = 0;
    virtual ~EuclideanPrimitive(){}

    friend class EuclieanPlane;

    virtual range get_x_range() const = 0;
    virtual range get_y_range() const = 0;

    virtual bool is_dynamic() const{
        return true;
    }

protected:
    bool dynamic = true;
    static constexpr float range_eps = 0.001;
};

class Point : public EuclideanPrimitive {
public:
    Point(const placer& p,int rad = 2) : pos_updater(p),radius(rad){
    }

    range get_x_range() const override{
        return {pos(0)-range_eps,pos(0)+range_eps};
    }
    range get_y_range() const override{
        return {pos(1)-range_eps,pos(1)+range_eps};
    }

    inline vec get_pos() const {
        return pos;
    }

    virtual void draw(frame_draw_object& p,range rx,range ry) override;
    virtual void update_values() override;

private:

    QColor color = QColorConstants::Red;

    vec pos = vec(2);
    placer pos_updater;
    int radius;

};

class Segment : public EuclideanPrimitive {
public:
    Segment(Point* p1,Point* p2,int w = 2) :
        point1(p1),
        point2(p2),
        width(w){
        dynamic = p1->is_dynamic() || p2->is_dynamic();
        pos1 = vec(2);
        pos2 = vec(2);
    }
    virtual void draw(frame_draw_object& p,range rx,range ry) override;
    virtual void update_values() override;
    range get_x_range() const override{
        range rx = point1->get_x_range();
        algo::extend_range(rx,point2->get_x_range());
        return rx;
    }
    range get_y_range() const override{
        range ry = point1->get_y_range();
        algo::extend_range(ry,point2->get_y_range());
        return ry;
    }
private:
    QColor color = QColorConstants::Blue;
    vec pos1 = vec(2);
    vec pos2 = vec(2);
    Point* point1;
    Point* point2;
    int width;

};

}

}
#endif // EUCLIDEAN_GEOMETRY_H
