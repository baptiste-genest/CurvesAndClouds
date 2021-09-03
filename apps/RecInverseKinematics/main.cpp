#include <curvesandcloud.h>

using namespace cnc;

void constrain_length(const vec* A,vec* B,scalar l){
    (*B) = (*A) + (*B-*A).normalize()*l;
}

void perform_RIK(std::vector<vec>& points,const vec& goal,const std::vector<scalar>& lengths,scalar eps){
    scalar score = 2*eps;
    uint N = points.size();
    vec O = points[0],G = goal;
    std::vector<vec*> points_ref(N+2);
    points_ref[0] = &O;
    points_ref.back() = &G;
    for (uint k = 0;k<N;k++)
        points_ref[k+1] = &points[k];
    uint current_id = N+1;
    int direction = -1;
    uint iter = 0;
    while (score > eps && iter < 100){
        for (uint k = 0;k<N;k++){
            scalar l = (current_id == 0 || current_id == N+1) ? (k*0.01) : 0.2;//lengths[current_id-1];
            std::cout << current_id << ' ' << current_id + direction << ' ' << l << std::endl;
            constrain_length(points_ref[current_id],points_ref[current_id+direction],l);
            current_id += direction;
        }
        current_id += direction;
        direction *= -1;
        score = points[0].distance(O) + points.back().distance(goal);
        std::cout << "SCORE " << score << std::endl;
        iter++;
    }
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    auto G = F->add_plot_group();
    uint N = 5;
    std::vector<vec> points(N,vec(2));
    std::vector<scalar> lengths(N-1,1.f/N);
    for (scalar l : lengths)
        std::cout << l << ' ';
    std::cout << std::endl;
    for (uint k = 0;k<N;k++)
        points[k](0) = k/(N-1);
    perform_RIK(points,vec({0.5,0.5}),lengths,1e-2);
    for (uint k = 0;k<points.size()-1;k++)
        std::cout << points[k].distance(points[k+1]) << ' ';
    std::cout << std::endl;
    std::cout << points[0] << std::endl;
    std::cout << points.back() << std::endl;

    cloud C;C.points = points;

    G->new_2D_curve(points);
    G->new_point_cloud(C);
    w.show();
    return App.exec();
}
