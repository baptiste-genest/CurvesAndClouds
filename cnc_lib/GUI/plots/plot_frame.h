/**
  * @file plot_frame.h
  *
  *	defines Plot frame class
  */
#ifndef PLOT_FRAME_H
#define PLOT_FRAME_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plot_layer.h"
#include "grid_layer.h"
#include "3D/mesh_displayer.h"
#include "GUI/display_info.h"
#include <vector>
#include <QFrame>
#include <QTimer>

namespace cnc {
class StatWindow;
class Frame_duplicata;
class PlotGroup;

/**
 * @brief The Plot_frame class defines a plot frame contains plot layers
 */
class PlotFrame : protected QFrame
{
    Q_OBJECT;

public:
    /**
     * @brief Plot_frame initialize empty frame
     * @param parent pointer to parent (typically a Plot_tab)
     */
    PlotFrame(QWidget* parent);
    virtual ~PlotFrame();

    /**
     * @brief add_layer adds an empty layer in the frame
     * @return reference to the new layer
     */
    PlotLayer* add_layer();

    GridLayer* add_grid_layer(range x,range y,bool display_grid = true);

    PlotGroup* add_plot_group();

    MeshDisplayer* add_mesh_viewer();

    /**
     * @brief set_nb_layer_per_second changes the rythm to update the frame
     * @param layer_rate number of layer per second
     */
    void set_nb_layer_per_second(float layer_rate);

    friend class PlotTab;

    /**
     * @brief list_types_contained list all types of plot contained in this frame
     */
    std::vector<bool> list_types_contained() const;

protected:
    void paintEvent(QPaintEvent*) override;
    void drawWidget(QPainter&,QPaintEvent* e);
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void resizeEvent(QResizeEvent* event) override;
private:
    friend class StatWindow;
    friend class StatDisplayer;

    void start_timer();

    PlotFrame *duplicate_frame(QWidget* parent) const;

    std::vector<StatWindow*> popups;

    QTimer* timer;
    float nb_layers_per_second = 2.f;

    uint current_frame;
    std::vector<PlotLayer*> layers;

    inline void draw_current_layer(frame_draw_object& fdo,QPaintEvent*){
        if (layers.size())
            layers[current_frame]->display_layer(fdo);
    }

public slots:
    void draw_next_frame();
};

}

#endif // PLOT_FRAME_H
