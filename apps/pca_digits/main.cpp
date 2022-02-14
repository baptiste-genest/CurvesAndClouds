#include <QApplication>
#include "curvesandcloud.h"

using namespace cnc;

algo_GUI::texture_function plot_vec(const vec& x,uint S){
    return [x,S] (uint i,uint j){
        int v = x(j*S+i);
        return QColor::fromRgb(v,v,v);
    };
}

vec project_on_KPCA(const std::vector<chaskal::eigen_pair<float>>& K,const cloud& c,const algo::stat::kernel& ker,const vec& a,uint n){
    vec proj(n);
    uint d = c.size();
    for (uint j = 0;j<n;j++)
        for (uint i = 0;i<d;i++){
            proj(j) += K[j].vector(i)*ker(c[i],a);
        }
    return proj;
}

int main(int argc, char *argv[])
{
    try {
        std::string path = "../../data/data";
        cloud digits;
        cloud digits2;
        cloud numbers[10];
        uint S = 28,NB = 100;
        for (int i = 0;i<10;i++){
            numbers[i] = algo::parse_from_uchar(path + std::to_string(i),S*S,NB);
            digits += numbers[i];
        }
        digits2 = digits;

        QApplication a(argc, argv);
        PlotWindow w;w.resize(500,500);

        auto T = w.add_tab("handwritten digits");
        {
            auto F = T->add_frame();
            for (uint k = 0;k<digits.size();k++)
                F->add_layer()->new_figure_from_texturing(S,S,plot_vec(digits[k],S));
            F->set_nb_layer_per_second(30);


            auto C = algo::stat::compute_covariance_matrix(digits);
            auto U = C.lanczos(2);

            cloud proj_digits[10];
            for (uint i = 0;i<10;i++){
                proj_digits[i] = algo::stat::init_empty_cloud(NB,2);
                for (uint j = 0;j<NB;j++){
                    proj_digits[i][j] = vec({numbers[i][j].scalar_product(U[0].vector),numbers[i][j].scalar_product(U[1].vector)});
                }
            }
            for (uint j = 0;j<digits.size();j++)
                digits[j] = vec({digits[j].scalar_product(U[0].vector),digits[j].scalar_product(U[1].vector)});
            auto Qdigits = algo_GUI::convert_cloud_to_qcloud(digits);
            auto bounds = algo_GUI::compute_cloud_bounds(Qdigits);


            auto F2 = T->add_frame()->add_layer();
            for (uint i = 0;i<10;i++)
                F2->new_point_cloud(proj_digits[i],3)->fix_plot_range(bounds.first,bounds.second);

        }


        w.show();
        return a.exec();
    }  catch (Cnc_error& e) {
        std::cout << e.what() << std::endl;
    }
    return -1;
}
