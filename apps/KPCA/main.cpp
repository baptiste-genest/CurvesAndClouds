#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::stat;
using namespace cnc::algo::stat::random_var;

algo::stat::kernel build_gaussian_kernel(float s){
    float s2 = s*s;
    float l1 = 1.f/(2*s2);
    float l2 = 1.f/(2*M_PI*s2);
    return [l1,l2] (const vec& x,const vec& y){
        float d = (x-y).norm2();
        return exp(-d*l1)*l2;
    };
}

algo::stat::kernel build_scalar_kernel(){
    return [] (const vec& x,const vec& y){
        float d = x.scalar_product(y);
        return (d+1)*(d+1);
    };
}

algo::calculus::scalar_function_2D curryfy_kernel_2D(const algo::stat::kernel& k,const cnc::vec& S){
    return [k,S] (float x,float y) {
        return k(S,cnc::vec({x,y}));
    };
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Plot_window w; w.resize(500,500);
    auto T = w.add_tab("Kernel-PCA");

    uint N = 200;
    cloud X[3];cloud C;
    for (uint k = 0;k<3;k++) X[k] = sample_vector_on_unit_sphere(2,N);
    auto F1 = T->add_frame()->add_layer();
    for (uint k = 0;k<3;k++){
        for (vec& p : X[k].points)
            p *= random_scalar(1.f + k,1.5f+k);
        F1->new_point_cloud(X[k],2)->fix_plot_in_rect(0,0,4);
        C += X[k];
    }

    {
        auto P = get_PCA_projector(C,2);
        auto CB = algo_GUI::compute_cloud_bounds(algo_GUI::convert_cloud_to_qcloud(C.apply_to_copy(P)));
        algo::scale_range(CB.first,1.2);
        algo::scale_range(CB.second,1.2);
        auto F2 = T->add_frame()->add_layer();
        for (uint k = 0;k<3;k++){
            F2->new_point_cloud(X[k].apply_to_copy(P))->fix_plot_range(CB.first,CB.second);;
        }
    }
    {
        tex_stream ts;
        ts << "Difference between" << tex::endl << "PCA and KPCA projection:" << tex::endl << "KPCA discriminates data" << tex::endl << "exploiting distances" << tex::endl;
        T->add_frame_at(0,1)->add_layer()->add_text_frame_from_tex_stream(ts);
    }
    {
        tex_stream ts;
        ts << "PCA projection" << tex::endl << "on sample \\textit{X} at dim \\textit{d}" << tex::endl << tex("\\text{Proj}(\\mathbf{x})_i = \\langle \\mathbf{u_i},\\mathbf{x} \\rangle, i \\leq d") << tex::endl;
        ts << "where " << tex("\\text{Cov } \\mathbf{u}_i = \\lambda_i \\mathbf{u}_i");
        ts << tex::endl << tex("\\text{Cov} = X^TX");
        ts << tex::endl << tex("\\text{with } \\lambda_i") << " decreasinlgy sorted";
        T->add_frame_at(1,1)->add_layer()->add_text_frame_from_tex_stream(ts);
    }
    {
        auto P = get_KPCA_projector(C,build_gaussian_kernel(2.f),2);
        auto CB = algo_GUI::compute_cloud_bounds(algo_GUI::convert_cloud_to_qcloud(C.apply_to_copy(P)));
        algo::scale_range(CB.first,1.2);
        algo::scale_range(CB.second,1.2);
        auto F2 = T->add_frame()->add_layer();
        for (uint k = 0;k<3;k++)
            F2->new_point_cloud(X[k].apply_to_copy(P))->fix_plot_range(CB.first,CB.second);;

    }
    {
        auto P = get_KPCA_projector(C,build_scalar_kernel(),2);
        auto CB = algo_GUI::compute_cloud_bounds(algo_GUI::convert_cloud_to_qcloud(C.apply_to_copy(P)));
        algo::scale_range(CB.first,1.2);
        algo::scale_range(CB.second,1.2);
        auto F2 = T->add_frame()->add_layer();
        for (uint k = 0;k<3;k++)
            F2->new_point_cloud(X[k].apply_to_copy(P))->fix_plot_range(CB.first,CB.second);;
    }
    {
        tex_stream ts;
        ts << "Kernel-PCA projection" << tex::endl << "Cov is replaced by Kernel Matrix " << tex("K_{ij} = \\phi(\\mathbf{x}_i,\\mathbf{x}_j)") << tex::endl ;
        ts << "here with " << tex::endl << tex("\\phi_1(\\mathbf{x,y}) = \\exp(-||\\mathbf{x-y}||^2)") << tex::endl << tex("\\phi_2(\\mathbf{x,y}) = (\\mathbf{x}^T\\mathbf{y}+1)^2") << tex::endl;
        T->add_frame_at(2,1,2,1)->add_layer()->add_text_frame_from_tex_stream(ts);
    }
    {
        range unit = {-1.f,1.f};
        T->add_frame_at(2,2,1,1)->add_layer()->new_colormap(curryfy_kernel_2D(build_gaussian_kernel(1.f),cnc::vec(2)),unit,unit);
        T->add_frame_at(3,2,1,1)->add_layer()->new_colormap(curryfy_kernel_2D(build_scalar_kernel(),cnc::vec({1.f,1.f})),unit,unit);
    }
    w.show();

    return a.exec();
}
