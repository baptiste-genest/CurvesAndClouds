#include "plot_layer.h"
using namespace cnc;
#include <iostream>

PlotLayer::PlotLayer(QWidget* parent) : QWidget(parent)
{

}


void PlotLayer::display_layer(frame_draw_object& fdo)
{
    std::vector<uint> free_pl,dependant;
    for (uint p = 0;p<plots.size();p++){
        if (plots[p]->dynamic)
            plots[p]->compute_values(fdo.fi);
        if (plots[p]->range_reference == nullptr)
            free_pl.push_back(p);
        else
            dependant.push_back(p);
    }
    for (uint f : free_pl)
        plots[f]->compute_value_range(fdo.fi);

    for (uint d : dependant){
        plots[d]->x_range = plots[d]->get_x_range();
        plots[d]->y_range = plots[d]->get_y_range();
    }

    for (uint p = 0;p<plots.size();p++){
        fdo.painter.setPen(QPen(plots[p]->plot_color, 2));
        plots[p]->plot(fdo);
    }
    if (plots.size())
        plots[0]->call_draw_axis(fdo);
}

FunctionPlot *PlotLayer::new_function_plot(const algo::calculus::scalar_function_1D &f, const range &x_range,plot_axis_policy fpp)
{
    FunctionPlot* FP = new FunctionPlot(f,x_range,fpp);
    insert_plot(FP);
    return FP;
}

PointCloud *PlotLayer::new_point_cloud(const cloud &c, const uint radius)
{
    PointCloud* PC = new PointCloud(c, radius);
    insert_plot(PC);
    return (PC);
}

PointCloud *PlotLayer::new_point_cloud(const cloud &c, const std::vector<uint> &radius)
{
    PointCloud* PC = new PointCloud(c, radius);
    insert_plot(PC);
    return (PC);
}

D2Curve *PlotLayer::new_2D_curve(const algo::calculus::nodes &X, const algo::calculus::nodes &Y,bool color_by_order)
{
    D2Curve* C = new D2Curve(X,Y,color_by_order);
    insert_plot(C);
    return (C);
}

D2Curve *PlotLayer::new_2D_curve(const std::vector<vec> &P, bool color_by_order)
{
   if (P[0].rowNum() != 2)
       throw Cnc_error("can't plot a non 2D Curve");
   algo::calculus::nodes X(P.size());
   algo::calculus::nodes Y(P.size());
   for (uint i = 0;i<P.size();i++){
       X[i] = P[i](0);
       Y[i] = P[i](1);
   }
   return new_2D_curve(X,Y,color_by_order);
}

Trajectory *PlotLayer::new_trajectory(const algo::calculus::parametrization_1D &P, scalar t0, scalar dt)
{
    Trajectory* T = new Trajectory(P,t0,dt);
    insert_plot(T);
    return (T);

}


Colormap *PlotLayer::new_colormap(const algo::calculus::scalar_function_2D &f, const range &x, const range &y,color_policy cp)
{
    Colormap* CM = new Colormap(f,x,y,cp);
    insert_plot(CM);
    return (CM);
}

ComplexFunctionPlot *PlotLayer::new_complex_function_plot(const algo::calculus::complex_function &f, const range &re, const range &im)
{
    ComplexFunctionPlot* CFP = new ComplexFunctionPlot(f,re,im);
    insert_plot(CFP);
    return CFP;
}

BooleanField* PlotLayer::new_boolean_field(const algo::calculus::property_2D& p,const range& x,const range& y){
    BooleanField* BF = new BooleanField(p,x,y);
    insert_plot(BF);
    return (BF);
}


LevelCurve *PlotLayer::new_level_curve(const algo::calculus::scalar_function_2D &f, const range &x, const range &y, uint n)
{
    LevelCurve* LC = new LevelCurve(f,x,y,n);
    insert_plot(LC);
    return (LC);
}

LevelCurve *PlotLayer::new_level_curve_unique(const algo::calculus::scalar_function_2D &f, const range &x, const range &y, float level)
{
    LevelCurve* LC = new LevelCurve(f,x,y,level);
    insert_plot(LC);
    return (LC);
}

VectorField* PlotLayer::new_vector_field(const algo::calculus::vector_function_2D &f, const range &x, const range &y, uint n)
{
    VectorField* VF = new VectorField(f,x,y,n);
    insert_plot(VF);
    return (VF);
}

Figure *PlotLayer::new_figure_from_file(const QString &filename)
{
    Figure* F = new Figure(filename);
    insert_plot(F);
    return (F);
}

Figure *PlotLayer::new_void_figure(uint w, uint h)
{
    Figure* F = new Figure(w,h);
    insert_plot(F);
    return (F);
}

Figure *PlotLayer::new_figure_from_texturing(uint w,uint h,const algo_GUI::texture_function& tf)
{
    Figure* F = new Figure(w,h,tf);
    insert_plot(F);
    return (F);
}

formula* PlotLayer::add_text_frame(const std::string& ts,formula_disposition d)
{
    tex_stream text; text << ts << tex::endl;
    return add_text_frame_from_tex_stream(text,d);
}

formula* PlotLayer::add_text_frame_from_tex_stream(const tex_stream& ts,formula_disposition d)
{
    formula* F = new formula(ts,d);
    insert_plot(F);
    return F;
}

PlotLayer::~PlotLayer()
{
    for (Plottable* p : plots)
        delete p;
}

PlotLayer *PlotLayer::duplicate_layer(QWidget *parent) const
{
    PlotLayer* D = new PlotLayer(parent);
    D->plots.resize(plots.size());
    for (uint k = 0;k<plots.size();k++)
        D->plots[k] = plots[k];
    return D;
}


