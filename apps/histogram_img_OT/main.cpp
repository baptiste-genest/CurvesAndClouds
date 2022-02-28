#include <curvesandcloud.h>

using namespace cnc;

algo_GUI::texture_function plot_vec(const vec& x,uint S){
    return [x,S] (uint i,uint j){
        int v = algo::clamp(x(j*S+i),0.,1.)*255;
        return QColor::fromRgb(v,v,v);
    };
}

class Permutation {
    std::map<int,int> sigma;
public:
    Permutation() {}
    Permutation(const std::vector<int>& s){
        for (int i = 0;i<(int)s.size();i++)
            sigma[i] = s[i];
    }
    Permutation invert() const {
        Permutation p;
        for (const auto& s : sigma){
            p.sigma[s.second] = s.first;
        }
        return p;
    }
    int operator()(int i) const{
        auto f = sigma.find(i);
        if (f == sigma.end())
            return i;
        return f->second;
    }
};

struct pixel {
    scalar val;
    uint index;
};

bool pixelComp(const pixel& A,const pixel& B){
    return A.val < B.val;
}

Permutation pixelToPermut(const std::vector<pixel>& p){
    std::vector<int> order;
    for (const auto& pix : p)
        order.push_back(pix.index);
    return Permutation(order);
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow Window; Window.resize(500,500);

    PlotTab* T = Window.add_tab("my first tab");

    uint w,h;
    auto flower = algo::load_bw_img_as_vec("/home/eulerson/Documents/data/images/flower.png",w,h);
    auto boat = algo::load_bw_img_as_vec("/home/eulerson/Documents/data/images/boat.png",w,h);

    std::vector<pixel> Fp,Bp;
    for (uint i = 0;i<w*h;i++){
        Fp.push_back({flower(i),i});
        Bp.push_back({boat(i),i});
    }
    std::sort(Fp.begin(),Fp.end(),pixelComp);
    std::sort(Bp.begin(),Bp.end(),pixelComp);

    auto s1inv = Permutation(pixelToPermut(Fp)).invert();
    auto s2 = Permutation(pixelToPermut(Bp));
    vec result(w*h);
    for (uint j = 0;j<w*h;j++)
        result(j) = boat(s2(s1inv(j)));

    T->add_frame()->add_layer()->new_figure_from_texturing(w,h,plot_vec(flower,w));
    T->add_frame()->add_layer()->new_figure_from_texturing(w,h,plot_vec(boat,w));
    T->add_frame()->add_layer()->new_figure_from_texturing(w,h,plot_vec(result,w));

    Window.show();
    return App.exec();
}
