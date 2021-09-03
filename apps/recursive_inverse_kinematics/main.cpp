#include <curvesandcloud.h>

using namespace cnc;

void constrain_length(const vec& a,vec& b,scalar l){
    b = a + (b-a).normalize()*l;
}

void perform_forward_backward_inverse_kinemtic(std::vector<vec>& points,const vec& goal,scalar eps){
    vec origin = points[0];
    const vec& head = points.back();
    int direction = 1;//1 goes forward & -1 goes backward
    uint nb_points = points.size();
    scalar score = 2*eps;
    std::vector<scalar> lengths(nb_points-1);
    for (uint k = 0;k<nb_points-1;k++)
        lengths[k] = points[k].distance(points[k+1]);
    while (score > eps){
        uint id = (direction == 1) ? 0 : nb_points-1;
        if (id == 0)
            points[0] = origin;
        else
            points.back() = goal;
        for (int k = 0;k<nb_points;k++){
        }
        score = origin.distance(points[0]) + head.distance(goal);
        direction *= -1;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("Recursive Inverse Kinematics");
    auto L = T->add_frame()->add_layer();

    const uint nb_points = 5;

    std::vector<vec> control_points(nb_points,vec(2));



    w.show();
    return a.exec();
}
