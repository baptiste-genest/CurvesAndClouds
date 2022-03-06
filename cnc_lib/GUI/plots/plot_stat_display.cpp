#include "functionplot.h"
#include "point_cloud.h"
#include "stat_list_window.h"
#include "core/algo/calculus/vector_calculus.h"
#include "QFrame"
#include "plottable.h"
#include "QLabel"

using namespace cnc;

void FunctionPlot::init_stat_display(PlotFrame*,PlotLayer* ,QVBoxLayout* V,const StatList& SL) const {
    using namespace cnc::algo::calculus;
    using namespace cnc::stat_prop;
    if (SL(function_plot,INTEGRAL)){
        QLabel* L = new QLabel;
        QString text = "integral value ";
        auto M = get_lin_space(x_range.first,x_range.second,100);
        text += QString::number((float)integrate(integration_scheme::simpson(value),M));
        L->setText(text);
        V->addWidget(L);
    }
    if (SL(function_plot,FIRST_DER)){
        QLabel* L = new QLabel;
        int N = 1000;
        float idx = N/(x_range.second-x_range.first);
        auto M = get_lin_space(x_range.first,x_range.second,N);

        float val = 0.f;
        for (uint i = 0;i<M.size()-1;i++){
            val += (value(M[i+1]) - value(M[i]))*idx;
        }
        val /= N;

        QString text = "average value of the first derivative ";
        text += QString::number(val);
        L->setText(text);
        V->addWidget(L);
    }
    if (SL(function_plot,SCND_DER)){
        int N = 1000;
        float idx = N/(x_range.second-x_range.first);
        float idx2 = idx*idx;
        auto M = get_lin_space(x_range.first,x_range.second,N);

        float val = 0.f;
        for (uint i = 1;i<M.size()-1;i++){
            val += (2.f*value(M[i]) - value(M[i-1]) - value(M[i+1]))*idx2;
        }
        val /= N;

        QLabel* L = new QLabel;
        QString text = "average value of the second derivative ";
        text += QString::number(val);
        L->setText(text);
        V->addWidget(L);
    }
}

void ScalarField::init_stat_display(PlotFrame*,PlotLayer *L, QVBoxLayout *V, const StatList &SL) const
{
    using namespace cnc::stat_prop;
    if (SL(scalar_field,GRAD)){
        auto Y = get_y_range(),X = get_x_range();
        float dw = std::min((X.second-X.first),(Y.second-Y.first))/1000.f;
        L->new_vector_field(cnc::algo::vector_calculus::build_gradient(value,dw),get_x_range(),get_y_range(),20);
        QLabel* L = new QLabel;
        QString text = "Gradient is displayed ";
        L->setText(text);
        V->addWidget(L);
    }
}

void PointCloud::init_stat_display(PlotFrame*,PlotLayer * lay , QVBoxLayout *V, const StatList &SL) const{
    using namespace cnc::algo::stat;
    using namespace cnc::stat_prop;
    auto D2_coords = project_into_2D(coords);
    if (SL(point_cloud,PCA)){
        auto PCA = compute_PCA(D2_coords);
        auto mean = compute_mean(D2_coords);
        for (uint j = 0;j<2;j++){
            auto axis = lay->new_2D_curve({mean,mean + PCA[j]});
            axis->set_range_reference((Plottable*)this);
            axis->set_color(QColor());
        }
        QLabel* L = new QLabel;
        QString text = "Principal Componants Axis displayed ";
        L->setText(text);
        V->addWidget(L);
    }
    if (SL(point_cloud,MEAN)){
        cloud M;
        auto me = compute_mean(D2_coords);
        std::cout << me << std::endl;
        M.add_point(compute_mean(D2_coords));
        auto mean_plot = lay->new_point_cloud(M,4);
        mean_plot->set_range_reference((Plottable*)this);
        mean_plot->set_color(QColorConstants::Cyan);
        QLabel* L = new QLabel;
        QString text = "Mean displayed ";
        L->setText(text);
        V->addWidget(L);
    }
}

void VectorField::init_stat_display(PlotFrame* F,PlotLayer*,QVBoxLayout*V,const StatList& SL) const {
    using namespace cnc::algo;
    using namespace cnc::stat_prop;
    if (SL(vector_field,DIV)){
        F->add_layer()->new_colormap(vector_calculus::build_divergence(value),get_x_range(),get_y_range(),from_zero);
        F->set_nb_layer_per_second(0.5);
        QLabel* L = new QLabel;
        QString text = "Divergence displayed ";
        L->setText(text);
        V->addWidget(L);
    }
    if (SL(vector_field,ROT_NORM)){
        F->add_layer()->new_colormap(vector_calculus::build_rot_norm(value),get_x_range(),get_y_range(),from_zero);
        F->set_nb_layer_per_second(0.5);
        QLabel* L = new QLabel;
        QString text = "Signed norm of the curl is displayed";
        L->setText(text);
        V->addWidget(L);
    }
}
