#include <curvesandcloud.h>

using namespace cnc;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("valued mesh");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    auto X = algo::stat::random_var::sample_uniform_in_square(2,2,20);
    auto M = std::make_shared<algo::geometry::Mesh2>(algo::geometry::mesh_generation::BowyerWatson(X));
    auto V = algo::stat::random_var::random_vec(-1,1,20);

    L->addPlot<Valued2DMeshDisplayer>(M,V);

    w.show();
    return App.exec();
}
