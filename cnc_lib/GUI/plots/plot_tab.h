/**
  * @file plot_tab.h
  *
  *	defines Plot frame class
  */
#ifndef PLOT_TAB_H
#define PLOT_TAB_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plot_frame.h"
#include "cnc_error.h"
#include "GUI/display_info.h"
#include <vector>
#include <QWidget>
#include <QGridLayout>
#include <QTimer>

#if CNC_OPENGL == TRUE
#include "3D/mesh_displayer.h"
//#include "3D/scene_viewer.h"
#endif

namespace cnc {


/**
 * @brief The Plot_tab class defines a tab which contains plot frames
 */
class PlotTab : public QWidget
{
    Q_OBJECT;

public:
    /**
     * @brief Plot_tab initialize empty tab, displaying handled by QGridLayout
     * @param parent Qt parent to display and automatic destruction
     */
    PlotTab(QWidget* parent);

    /**
     * @brief add frame to a given location and size in frame grid throw error if place unavailable
     * @param px x position in grid
     * @param py y position in grid
     * @param w width number of cells of the frame
     * @param h height number of cells of the frame
     * @return reference to the created frame
     */
    PlotFrame* add_frame_at(int px,int py,int w = 1,int h = 1);


    /**
     * @brief add frame to the next available location
     * @param w width number of cells of the frame
     * @param h height number of cells of the frame
     * @return pointer to the created frame
     */
    PlotFrame* add_frame(int w = 1,int h = 1);

#if CNC_OPENGL == TRUE
    MeshDisplayer* add_3D_frame(int w = 1,int h = 1);
    MeshDisplayer *add_3D_frame_at(int px,int py,int w = 1,int h = 1);
    //SceneViewer* add_3D_scene(int w = 1,int h = 1);
#endif

    float get_tab_ratio() const;

    ~PlotTab();

protected:
    void paintEvent(QPaintEvent*) override;
    void drawWidget(QPainter&);
    //void resizeEvent(QResizeEvent* event) override;

private:
    QRect get_frame_free_pos(int w,int h);
    QRect check_frame_free_pos(int x,int y,int w,int h);

    float ratio;

    QGridLayout* Grid;

    std::vector<QRect> frame_grid;
    std::vector<PlotFrame*> frames;

#if CNC_OPENGL == TRUE
    std::vector<MeshDisplayer*> frames3D;
    //std::vector<SceneViewer*> Scenes;
#endif


    static constexpr int MAX_CELL_WIDTH = 10;
    static constexpr int MAX_CELL_HEIGHT = 8;
    static constexpr int CELL_SIZE = 100;

    PlotFrame* insert_frame(const QRect&);
#if CNC_OPENGL == TRUE
    MeshDisplayer* insert_3D_frame(const QRect&);
    //SceneViewer* insert_scene_frame(const QRect&);
#endif
};

}

#endif // PLOT_TAB_H
