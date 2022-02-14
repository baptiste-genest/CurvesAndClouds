#include <curvesandcloud.h>

using namespace cnc;

algo_GUI::texture_function plot_vec(const vec& x,uint S){
    return [x,S] (uint i,uint j){
        int v = algo::clamp(x(j*S+i),0.,255.);
        return QColor::fromRgb(v,v,v);
    };
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow W; W.resize(500,500);

    PlotTab* T = W.add_tab("Eigen Faces");

    uint w,h;
    std::string prefix = "/home/eulerson/Documents/data/CroppedYale/yaleB01/face1_";
    std::vector<vec> imgs;
    uint N = 5;
    for (uint i = 0;i<N*N;i++){
        imgs.push_back(algo::load_bw_img_as_vec(prefix + std::to_string(i+1)+ ".png",w,h));
        std::cout << i << ": " << w << ' ' << h <<std::endl;
    }
    for (uint j = 0;j<N;j++){
        for (uint i = 0;i<N;i++){
            T->add_frame_at(i,j)->add_layer()->new_figure_from_texturing(w,h,plot_vec(imgs[j*N+i]*255,w));
        }
    }
    if (true){
        imgs.resize(N*N);
        auto X = chaskal::build_basis(imgs);
        std::cout << "basis done" << std::endl;
        std::cout << w*h << std::endl;
        auto XXt = X*X.transpose();
        std::cout << XXt.rowNum()  << ' ' << XXt.colNum() << std::endl;
        auto ef = (XXt.lanczos(2,1e-2));
        auto L =T->add_frame_at(N,0,N,N)->add_layer();
        for (const auto& x : ef)
            L->new_figure_from_texturing(w,h,plot_vec(-x.vector*255./x.vector.norm_inf(),w));
    }
    else
        T->add_frame_at(N,0,N,N)->add_layer()->new_figure_from_texturing(w,h,[](uint,uint){
            return QColor::fromRgb(255,0,0);
        });

    W.show();
    return App.exec();
}
