#include "curvesandcloud.h"
#include <iostream>
#include <QColor>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::calculus;

scalar g(scalar x){
    return std::sin(x);
}

scalar f(scalar x){
    return 1.f/(1+x*x);
}

scalar h(scalar x,scalar y){
    return std::sin(x)*std::sin(y);
}

scalar h2(scalar x,scalar y){
    return std::sin(x)*std::sin(y);
}
cnc::vec z(scalar x,scalar y){
    return cnc::vec({std::sin(y),std::sin(x)});
}

QColor mandelbrot(uint i,uint j){
    static const uint MAX_ITER = 30;
    auto Color = build_range_mapper({0,MAX_ITER},{255,0});

    auto M = build_range_mapper({0,500},{-2,2});
    std::complex<scalar> z(M(i),M(j)),c(z);
    for (uint i = 0;i<MAX_ITER;i++){
        if (std::abs(z) < 2.f)
            z = z*z+c;
        else {
            int it = Color(i);
            return QColor::fromRgb(it,it,it);
        }
    }
    return QColorConstants::Black;
}

int main(int argc, char *argv[])
{
    try {
        srand(time(NULL));

        QApplication a(argc, argv);

        PlotWindow w;w.resize(800,800);

        PlotTab* lag = w.add_tab("Fonctions et interpolation");
        PlotFrame* PFT = lag->add_frame(2,1);
        PlotFrame* PFU = lag->add_frame_at(0,1,2,1);
        PFT->set_nb_layer_per_second(4);

        scalar xa = -5.f,xb=-xa;
        range xr(xa,xb);

        std::vector<int> N = get_index_lin_space(2,20,1);
        for (int n : N){
            nodes T = get_tchebytchev_points(n,{xa,xb});
            nodes U = get_lin_space(xa,xb,n);

            lagrange_interpolator lt(T,sample(f,T));
            lagrange_interpolator lu(U,sample(f,U));

            PlotLayer* LT = PFT->add_layer();
            auto fp2 = LT->new_function_plot(f,xr);
            LT->new_function_plot(lt,xr);
            auto PT = LT->new_point_cloud(stat::sample_points_on_1D_function(f,T),2);
            PT->set_range_reference(fp2);

            PlotLayer* LU = PFU->add_layer();
            LU->new_function_plot(f,xr);
            LU->new_function_plot(lu,xr);
            LU->new_point_cloud(stat::sample_points_on_1D_function(f,U),2)->set_range_reference(fp2);
        }

        tex_stream to ;
        to << "Comparaison de la convergence" << tex::endl << "de l'interpolation de Lagrange" << tex::endl << "par les points de Tchebytchev" << tex::endl;
        to << tex_system({"T_0(x) = 1","T_1(x) = x","T_{n+1}(x) = 2xT_n(x) - T_{n-1}(x)"}) << tex::endl;
        to << tex("\\implies T_n(\\cos(x)) = \\cos(nx)")<< tex::endl;
        to << tex("\\text{Tcheb}_n = \\{x \\in \\mathbb{R},T_n(x) = 0\\}")<< tex::endl;
        to << "contre repartition uniforme" << tex::endl;
        lag->add_frame(2,2)->add_layer()->add_text_frame_from_tex_stream(to);



        PlotTab* field = w.add_tab("Champs scalaires et vectoriels 2D");

        auto D2F = field->add_frame();D2F->set_nb_layer_per_second(0.5f);
        D2F->add_layer()->new_colormap(h,xr,xr);
        D2F->add_layer()->new_level_curve(h,xr,xr,10);

        auto VFF = field->add_frame();
        VFF->add_layer()->new_vector_field(z,xr,xr,20);

        PlotTab* stats = w.add_tab("Simulation aléatoire et analyse statistique");

        vec axis_1({1.f,1.f}),axis_2({-1.f,1.f});

        cloud C = stat::random_var::sample_gaussian_vector_by_PC({axis_2,axis_1},{1.3f,1.0f},vec({3.f,1.f}),1000);
        C = C + stat::random_var::sample_gaussian_vector_by_PC({axis_1,axis_2},{1.f,0.3f},vec({-3.f,1.f}),1000);

        auto clusters = algo::stat::compute_clusters_by_k_means(C,2);

        PlotFrame* PFS = stats->add_frame_at(0,0,2,2);
        auto clusters_plot = PFS->add_layer();
        PointCloud* PC;
        for (uint i = 0;i<2;i++){
            PC = clusters_plot->new_point_cloud(clusters[i],2);
            PC->fix_plot_in_rect(0,0,5);
        }
        PlotTab* figs = w.add_tab("Chargement et édition d'images");

        auto dog = figs->add_frame(2,2)->add_layer();
        dog->new_figure_from_file("../../data/rodrigo.jpg");
        tex_stream ts;
        ts << "Rodrigo" << tex::endl << "the wildest dog";
        dog->add_text_frame_from_tex_stream(ts,upper_right);

        auto mandel = figs->add_frame(2,2)->add_layer();
        mandel->new_figure_from_texturing(500,500,mandelbrot);

        PlotTab* quadra = w.add_tab("Méthodes de quadrature");
        auto pi_approx = quadra->add_frame(2,2)->add_layer();
        auto quadra_legend = quadra->add_frame(2,2);

        uint max_step = 30;
        std::vector<scalar> error1(max_step);
        std::vector<scalar> error2(max_step);
        for (uint i = 0;i<max_step;i++){
            auto mesh = get_lin_space(0.f,M_PI*0.5f,(int)i+2);
            error1[i] = std::log(std::abs(1.f - integrate(integration_scheme::simpson(g),mesh)));
            error2[i] = std::log(std::abs(1.f - integrate(integration_scheme::trapezoidal(g),mesh)));

            tex_stream legend;
            legend << "Comparaison des methodes de quadratures" << tex::endl;
            legend << tex("\\int_{0}^{\\frac{\\pi}{2}}\\sin(t)dt = 1") << tex::endl;
            legend << "taille du mesh " << (int)i+2<< tex::endl;
            legend << "simpson: " << integrate(integration_scheme::simpson(g),mesh)<< tex::endl;
            legend << tex("\\sum_{x_i \\in \\text{nodes}} \\frac{x_{i+1}-x_i}{6}(f(x_i)+4f(\\frac{x_{i+1}+x_i}{2})+f(x_{i+1}))")<< tex::endl;
            legend << "trapeze: " << integrate(integration_scheme::trapezoidal(g),mesh)<< tex::endl;
            legend << tex("\\sum_{x_i \\in \\text{nodes}} \\frac{x_{i+1}-x_i}{2}(f(x_i)+f(x_{i+1}))")<< tex::endl;
            quadra_legend->add_layer()->add_text_frame_from_tex_stream(legend);
        }

        auto xs = get_lin_space(2,max_step+1,1.f);
        std::vector<scalar> log_xs;
        for (auto x : xs)
            log_xs.push_back(std::log(x));
        range x_range = {std::log(2),std::log(max_step+1)},y_range = {0,*std::min_element(error1.begin(),error1.end())};
        auto s = pi_approx->new_function_plot(build_1D_linear_interpolator(log_xs,error1),x_range);
        s->fix_plot_range(x_range,y_range);
        pi_approx->new_function_plot(build_1D_linear_interpolator(log_xs,error2),x_range)->set_range_reference(s);

        w.show();
        return a.exec();

    }  catch (Cnc_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return -1;
}
