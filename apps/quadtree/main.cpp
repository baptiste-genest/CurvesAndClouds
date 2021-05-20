#include <curvesandcloud.h>

using namespace cnc;


int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    Plot_window w; w.resize(500,500);

    Plot_tab* T = w.add_tab("Quadtree");
    Plot_frame* F= T->add_frame();
    Plot_layer* layer = F->add_layer();
    scalar HW = 1;
    range R{-HW,HW};

    uint N = 10;

    cloud C = algo::stat::random_var::sample_uniform_in_square(2,HW,N);

    algo::calculus::finite_elements::Quadtree Q(R,R);
    for (uint k = 0;k<N;k++)
        Q.insert(&C[k]);

    auto leaf = Q.find(C[4],1e-6);
    if (leaf != nullptr){
        leaf->print();
        std::cout << "at deep " << leaf->get_deep() << std::endl << "neighbors: \n";
        auto NE = leaf->get_adjacents_cells();
        for (const auto& n : NE)
            n->print();
    }
    else
        std::cout << "query null" << std::endl;

    auto L = Q.get_lines();
    for (const auto& l : L)
        layer->new_2D_curve(l)->fix_plot_range(R,R);
    layer->new_point_cloud(C)->fix_plot_range(R,R);

    w.show();
    return a.exec();
}
