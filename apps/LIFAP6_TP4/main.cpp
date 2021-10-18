#include <curvesandcloud.h>
#include "skiplist.h"

using namespace cnc;
using namespace cnc::timeHandling;
using namespace cnc::algo::calculus;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);
    auto T = w.add_tab("benchmarking");

    int NB_SKIPLIST = 6;
    int max_val = 30000;

    auto ps = get_lin_space(0.f,0.9f,NB_SKIPLIST);

    std::vector<float> time_per_p(NB_SKIPLIST);

    std::vector<int> nb_elements = algo::get_index_lin_space(1000,20000,1000);


    //PLOT TIME TO EXECUTE PER PROBA
    if (false)
    {
        auto F = T->add_frame();
        auto G = F->add_plot_group();
        for (auto nb : nb_elements){
            std::vector<int> elements_to_insert(nb);
            for (int k = 0;k<nb;k++)
                elements_to_insert[k] = rand()%max_val;

            std::vector<vec> curves;
            for (auto p : ps){
                curves.push_back(vec({p,0.f}));
                SkipList S(p);
                auto start_time = std::chrono::high_resolution_clock::now();
                for (auto e : elements_to_insert)
                    S.insert(e);
                auto end_time = std::chrono::high_resolution_clock::now();
                curves.back()(1) = float(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count())*0.001;
            }
            G->new_2D_curve(curves);
        }
    }
    {
        auto F = T->add_frame();
        auto G = F->add_grid_layer({0,nb_elements.back()+1000},{0,50},false);
        //auto G = F->add_layer();
        std::vector<std::vector<int>> elements_to_insert;
        for (auto nb : nb_elements){
            elements_to_insert.push_back(std::vector<int>(nb));
            for (int k = 0;k<nb;k++)
                elements_to_insert.back()[k] = rand()%max_val;
        }
        for (auto p : ps){
            std::vector<vec> curves;
            uint i = 0;
            for (auto nb : nb_elements){
                curves.push_back(vec({float(nb),0.f}));
                SkipList S(p);
                auto start_time = std::chrono::high_resolution_clock::now();
                for (auto e : elements_to_insert[i])
                     S.insert(e);
                auto end_time = std::chrono::high_resolution_clock::now();
                curves.back()(1) = float(3000*std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count())/float(nb);
                i++;
            }
            G->new_2D_curve(curves);
        }
    }


    w.show();
    return App.exec();
}
