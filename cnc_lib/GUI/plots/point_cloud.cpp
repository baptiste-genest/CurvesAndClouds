#include "point_cloud.h"
using namespace cnc;

Point_cloud::Point_cloud(const cloud& c, const uint radius)
{
    if (c.size() == 0)
        throw Cnc_error("Can't plot void point cloud");
    uint dimension = c.points[0].rowNum();
    for (const vec& x : coords.points)
        if (x.rowNum() != dimension)
            throw Cnc_error("To display a cloud each point must be in the same dimension");
    pap = square_max_range;
    uniform_radius = true;
    point_radius.push_back(radius);
    coords = algo::stat::project_into_2D(c);
    projected_2d_coords = algo_GUI::convert_cloud_to_qcloud(coords);
    size = projected_2d_coords.size();
}

Point_cloud::Point_cloud(const cloud& c, const std::vector<uint>& radius){
    pap = square_max_range;
    if (radius.size() != c.points.size()){
        throw(Cnc_error("You need to have the same size for the vectors c and radius."));
    }
    uniform_radius = false;
    uint dimension = c.points[0].rowNum();
    for (const vec& x : coords.points)
        if (x.rowNum() != dimension)
            throw Cnc_error("To display a cloud each point must be in the same dimension");
    point_radius = radius;
    coords = algo::stat::project_into_2D(c);
    projected_2d_coords = algo_GUI::convert_cloud_to_qcloud(coords);

    size = projected_2d_coords.size();
}

void Point_cloud::call_draw_axis(frame_draw_object &fdo) const
{
    fdo.painter.setPen(QPen(Qt::black, 1));
    draw_axis(fdo, {get_x_range(),get_y_range()});
}

void Point_cloud::plot(frame_draw_object& fdo)
{
    range gx_range = get_x_range();
    range gy_range = get_y_range();
    if (coords[0].size() == 1 && pap != given_range)
        throw Cnc_error("Can't plot single point as cloud if value range isn't explicitely fixed");

    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(gx_range,gy_range, fdo.fi);

    fdo.painter.setBrush(plot_color);
    for (uint i = 0; i < size; i++){
        if (uniform_radius){
            fdo.painter.drawEllipse(M(projected_2d_coords[i]), point_radius[0], point_radius[0]);
        } else {
            fdo.painter.drawEllipse(M(projected_2d_coords[i]), point_radius[i], point_radius[i]);
        }
    }
}

void Point_cloud::compute_value_range(const frame_info &)
{
    if (pap == given_range)
        return;

    std::pair<range, range> B = algo_GUI::compute_cloud_bounds(projected_2d_coords);
    range MR;
    switch (pap) {
        case min_max_range:
            x_range = B.first;
            y_range = B.second;
        break;
        case same_than_x_range:
            x_range = B.first;
            y_range = B.first;
        break;
        case same_than_y_range:
            x_range = B.second;
            y_range = B.second;
        break;
        case square_max_range:
            MR = {std::min(B.first.first,B.second.first),std::max(B.first.second,B.second.second)};
            x_range = MR;
            y_range = MR;
        break;
        default:
            x_range = B.first;
            y_range = B.second;
    }
    algo::scale_range(x_range,zoom_factor);
    algo::scale_range(y_range,zoom_factor);
}
