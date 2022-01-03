/**
  * @file plot_layer.h
  *
  *	defines Plot layer class
  * main class to interact with the lib where you can create plots
  */
#ifndef PLOT_LAYER_H
#define PLOT_LAYER_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plottable.h"
#include "functionplot.h"
#include "figure.h"
#include "colormap.h"
#include "d2_curve.h"
#include "core/algo/calculus/calculus.h"
#include "point_cloud.h"
#include "complex_function_plot.h"
#include "level_curve.h"
#include "2D_GEOM/euclidean_plane.h"
#include "boolean_field.h"
#include "vector_field.h"
#include "formula_tex.h"
#include "trajectory.h"
#include "graphplotter.h"
#include "2D_GEOM/diagramplotter.h"
#include "2D_GEOM/mesh2ddisplayer.h"
#include "femdisplayer.h"
//#include "3D/mesh_displayer.h"

namespace cnc {

/**
 * @brief The Plot_layer class will contain plots, if contains multiples plots, they will be displayed in the same frame
 */
class PlotLayer : protected QWidget
{
public:
    /**
     * @brief Plot_layer initialize empty layer
     */
    PlotLayer(QWidget*);

    /**
     * @brief new_function_plot adds the plot of a function to the layer
     * @param f function to plot
     * @param x_range range to plot it
     * @param pap Plot axis policy, you can choose : plot between the min and the max values,same than x range(default)
     * @return pointer to the plot
     */
    FunctionPlot* new_function_plot(const algo::calculus::scalar_function_1D& f,const range& x_range,plot_axis_policy pap = same_than_x_range);
    /**
     * @brief new_function_plot adds the plot of a function to the layer
     * @param f function to plot
     * @param x_range range to plot it
     * @param y_range fix the range of possible values to plot
     * @return pointer to the plot
     */
    FunctionPlot* new_function_plot(const algo::calculus::scalar_function_1D& f,const range& x_range,const range& y_range);

    /**
     * @brief new_point_cloud adds a point cloud to the layer
     * @param c cloud to plot
     * @param radius radius of each point (default = 2)
     * @return pointer to the plot
     */
    PointCloud* new_point_cloud(const cloud& c, const uint radius = 2,bool dl = false);
    /**
     * @brief new_point_cloud adds a point cloud to the layer
     * @param c cloud to plot
     * @param radius you can fix each radius of each point (must have same size than cloud )
     * @return pointer to the plot
     */
    PointCloud* new_point_cloud(const cloud& c, const std::vector<uint>& radius);

    /**
     * @brief new_2D_curve adds a 2D curve to the layer from the set of the control points, straight lines are drawn between them
     * @param X x coordinates of the control points
     * @param Y y coordinates of the control points
     * @return pointer to the plot
     */
    D2Curve *new_2D_curve(const algo::calculus::nodes& X,const algo::calculus::nodes& Y,bool color_by_order = false);
    /**
     * @brief new_2D_curve adds a 2D curve to the layer from the set of the control points, straight lines are drawn between them
     * @param P set of control points, in vector format, must be 2D
     * @return pointer to the plot
     */
    D2Curve *new_2D_curve(const std::vector<vec>& P,bool color_by_order = false);

    Trajectory* new_trajectory(const algo::calculus::parametrization_1D& P,scalar t0,scalar dt);

    /**
     * @brief new_colormap adds the plot of a 2D scalar function, plot in the x and y range, color goes from blue to black to red
     * @param f function to plot
     * @param x_range x values to plot between
     * @param y_range y values to plot between
     * @param cp color policy, you can choose between color from min (blue) to max (red) or from min to 0 in blue and 0 to max in red
     * @return pointer to the plot
     */
    Colormap* new_colormap(const algo::calculus::scalar_function_2D& f,const range& x_range,const range& y_range ,color_policy cp = min_max);

    ComplexFunctionPlot* new_complex_function_plot(const algo::calculus::complex_function& f,const range& re,const range& im);

    BooleanField* new_boolean_field(const algo::calculus::property_2D& p,const range& x_range,const range& y_range,const QColor& c = QColorConstants::Red);

    /**
     * @brief new_level_curve adds the plot of a 2D scalar function, plot in the x and y range, by displaying it's level curves
     * @param f function to plot
     * @param x_range x values to plot between
     * @param y_range y values to plot between
     * @param nb_slices number of level curves (evenly sliced from min to max)
     * @return pointer to the plot
     */
    LevelCurve* new_level_curve(const algo::calculus::scalar_function_2D& f,const range& x,const range&y,uint nb_slices = 10);
    /**
     * @brief new_level_curve adds the plot of a 2D scalar function, plot in the x and y range, by displaying one level curve of a given value
     * @param f function to plot
     * @param x_range x values to plot between
     * @param y_range y values to plot between
     * @param level value to slice
     * @return pointer to the plot
     */
    LevelCurve* new_level_curve_unique(const algo::calculus::scalar_function_2D&,const range&,const range&,float level = 0.f);

    /**
     * @brief new_level_curve adds the plot of a 2D vector field, plot in the x and y range
     * @param f function to plot
     * @param x_range x values to plot between
     * @param y_range y values to plot between
     * @param number_of_vector_per_side (default = 10)
     * @return pointer to the plot
     */
    VectorField* new_vector_field(const algo::calculus::vector_function_2D&,const range& x_range,const range& y_range,uint nb_vector_per_side= 10);

    /**
     * @brief new_figure_from_file displays a picture from a given file (supports most of the image format (check Qt doc for the list))
     * @param filename relative or absolute path to the file
     * @return pointer to the plot
     */
    Figure* new_figure_from_file(const QString& filename);
    /**
     * @brief new_figure_from_texturing displays a picture from a given texture function (associating each pixel to a color)
     * @param w width of the picture
     * @param h height of the picture
     * @return pointer to the plot
     */
    Figure* new_figure_from_texturing(uint w,uint h,const algo_GUI::texture_function& tf);
    /**
     * @brief new_void_figure displays a blank picture
     * @param w width of the picture
     * @param h height of the picture
     * @return pointer to the plot
     */
    Figure* new_void_figure(uint w,uint h);

    /**
     * @brief add_text_frame_from_tex_stream !!REQUIRES INTERNET CONNECTION!! adds all the text loaded in a tex_stream, displayed in latex
     * @param ts tex_stream to display
     * @return pointer to the plot
     */
    formula* add_text_frame_from_tex_stream(const tex_stream& ts,formula_disposition d = centered);

    /**
     * @brief add_text_frame
     * @param ts
     * @return
     */
    formula* add_text_frame(const std::string& ts,formula_disposition d = centered);

    euclid::EuclideanPlane* add_euclidean_plane(range rx,range ry);
    euclid::EuclideanPlane* add_euclidean_plane();

    GraphPlotter* addGraphPlotter(algo::Graph& G);

    template <typename PlottableType,typename ... Args>
    PlottableType* addPlot(Args&& ... args) {
        PlottableType* P = new PlottableType(std::forward<Args>(args)...);
        insert_plot(P);
        return P;
    }


    ~PlotLayer();

    friend class PlotFrame;
    friend class StatDisplayer;

protected:

    PlotLayer* duplicate_layer(QWidget*) const;

    inline void insert_plot(Plottable* P) {
        P->set_color(CNC_COLORS[plots.size() % NB_CNC_COLORS]);
        plots.push_back(P);
    }

    virtual void display_layer(frame_draw_object& fdo);

    std::vector<Plottable*> plots;

};

}

#endif // PLOT_LAYER_H
