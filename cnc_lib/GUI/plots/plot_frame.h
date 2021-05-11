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
#include "GUI/display_info.h"
#include <vector>
#include <QFrame>
#include <QTimer>

namespace cnc {
class Stat_window;
class Frame_duplicata;

/**
 * @brief The Plot_frame class defines a plot frame contains plot layers
 */
class Plot_frame : protected QFrame
{
    Q_OBJECT;

public:
    /**
     * @brief Plot_frame initialize empty frame
     * @param parent pointer to parent (typically a Plot_tab)
     */
    Plot_frame(QWidget* parent);
    virtual ~Plot_frame();

    /**
     * @brief add_layer adds an empty layer in the frame
     * @return reference to the new layer
     */
    Plot_layer* add_layer();

    /**
     * @brief set_nb_layer_per_second changes the rythm to update the frame
     * @param layer_rate number of layer per second
     */
    void set_nb_layer_per_second(float layer_rate);

    friend class Plot_tab;

    /**
     * @brief list_types_contained list all types of plot contained in this frame
     */
    std::vector<bool> list_types_contained() const;

protected:
    void paintEvent(QPaintEvent*);
    void drawWidget(QPainter&);
    void mouseDoubleClickEvent(QMouseEvent*);
private:
    friend class Stat_window;
    friend class Stat_displayer;

    void start_timer();

    Plot_frame *duplicate_frame(QWidget* parent) const;

    std::vector<Stat_window*> popups;

    QTimer* timer;
    float nb_layers_per_second = 2.f;

    uint current_frame;
    std::vector<Plot_layer*> layers;

    inline void draw_current_layer(frame_draw_object& fdo){
        layers[current_frame]->display_layer(fdo);
    }

public slots:
    void draw_next_frame();
};

}

#endif // PLOT_FRAME_H
