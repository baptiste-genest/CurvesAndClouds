#include <curvesandcloud.h>
#include "hash_table.h"

using namespace cnc;

void display_as_histogram(PlotLayer* L,const histogram& H){
    for (uint k = 0;k<H.size();k++){
        if (H[k])
            L->new_2D_curve({vec({float(k),0.}),vec({float(k),1.})})->set_color(QColorConstants::Blue);
    }
}

template<class T>
void display_benchmark(PlotLayer* L, T& obj, const std::function<void(uint,T&)>& test_function,const std::vector<int>& test_sizes)
{
    uint N = test_sizes.size();
    using namespace timeHandling;
    auto nlin = algo::calculus::get_lin_space(0,N-1,1.0f);
    auto durations = std::vector<scalar>(N);
    for (uint k = 0;k<N;k++){
        auto start = currentTime();
        test_function(test_sizes[k],obj);
        durations[k] = scalar(getTimeSinceTimePointInMicroseconds(start))/test_sizes[k];
    }
    L->new_2D_curve(nlin,durations)->set_axis_policy(min_max_range);
}

int main(int argc, char *argv[])
{
    uint max_size = 10000;
    uint N = max_size*0.9;

    std::vector<int> ts = algo::get_index_lin_space(N/10,N,10);
    std::cout << "nb tests " << ts.size() << std::endl;

    hashfunc h1 = [] (uint k) {
        return 25541*k;
    };
    rehashfunc rh1 = [] (uint k,uint) {
        return k+1;
    };

    rehashfunc rh2 = [] (uint k,uint i) {
        return k + i*i;
    };

    rehashfunc rh3 = [h1] (uint k,uint i) {
        return (k + i*35089) * 35527;
    };

    std::vector<HashTable> H;
    H.push_back(HashTable(max_size,h1,rh1));
    H.push_back(HashTable(max_size,h1,rh2));
    H.push_back(HashTable(max_size,h1,rh3));

    std::vector<key> keys(N);
    std::vector<value> values(N);
    for (uint k = 0;k<N;k++){
        keys[k] = rand();
        values[k] = rand();
    }

    std::function<void(uint,HashTable&)> test = [keys,values] (uint n,HashTable& H){
        for (uint k = 0;k<n;k++)
            H.insert(keys[k],values[k]);
        H.reset_values();
    };

    /*
    for (auto& h : H)
        for (uint k = 0;k<N;k++)
            h.insert(keys[k],values[k]);
            */

    //GRAPHICS PART
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("HashTable");
    auto G = T->add_frame()->add_plot_group();
    for (auto& h : H){
        //PlotLayer* L = T->add_frame()->add_grid_layer({0,max_size},{-0.5,1.5},false);
        //display_as_histogram(L,h.book_table());
        h.reset_values();
        display_benchmark<HashTable>(G,h,test,ts);
    }

    w.show();
    return App.exec();
}
