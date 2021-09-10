#include "plot_tab.h"
#include <iostream>
#include "GUI/display_info.h"
#include <QPushButton>
using namespace cnc;

PlotTab::PlotTab(QWidget* parent) : QWidget(parent)
{
    Grid = new QGridLayout;
    QWidget::setLayout(Grid);


}

void PlotTab::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    drawWidget(qp);

    QWidget::paintEvent(e);
    /*
    for (auto f: frames3D)
        f->show();
        */
}

void PlotTab::drawWidget(QPainter&)
{
}

QRect PlotTab::get_frame_free_pos(int w, int h)
{
    if (w <= 0 || h <= 0)
        throw Cnc_error("frame size must be strictely positive");
    for (uint y = 0;y<MAX_CELL_HEIGHT;y++){
        for (uint x = 0;x<MAX_CELL_WIDTH;x++){
            QRect tmp(x,y,w,h);
            bool place_free = true;
            for (const QRect& R : frame_grid){
                if (tmp.intersects(R)){
                    place_free = false;
                    break;
                }
            }
            if (place_free){
                return tmp;
            }
        }
    }
    throw Cnc_error("Couldn't place frame in grid");
}

QRect PlotTab::check_frame_free_pos(int px, int py, int w, int h)
{
    if (px < 0 || py < 0)
        throw Cnc_error("position in frame grid must be strictely positive");
    else if (w <= 0 || h <= 0)
        throw Cnc_error("frame size must be positive");

    QRect tmp;
    tmp.setX(px);
    tmp.setY(py);
    tmp.setWidth(w);
    tmp.setHeight(h);

    for (const QRect& R : frame_grid)
        if (tmp.intersects(R))
            throw Cnc_error("can't place a frame here, place unavailable");
    return tmp;
}

PlotFrame *PlotTab::insert_frame(const QRect& R)
{
    frame_grid.push_back(R);
    PlotFrame* f = new PlotFrame(this);
    f->start_timer();

    QSizePolicy qsp(QSizePolicy::Preferred,QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    f->setSizePolicy(qsp);

    Grid->addWidget(f,R.y(),R.x(),R.height(),R.width());
    frames.push_back(f);

    int max_x = 0,max_y = 0;
    for (const auto& r : frame_grid){
        max_x = std::max(r.right()+1,max_x);
        max_y = std::max(r.bottom()+1,max_y);
    }
    ratio =  float(max_y)/max_x;

    return f;
}

#if CNC_OPENGL == TRUE
MeshDisplayer *PlotTab::insert_3D_frame(const QRect &R)
{
    frame_grid.push_back(R);
    MeshDisplayer* f = new MeshDisplayer(this);

    QSizePolicy qsp(QSizePolicy::Preferred,QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    f->setSizePolicy(qsp);

    Grid->addWidget(f,R.y(),R.x(),R.height(),R.width());
    frames3D.push_back(f);

    int max_x = 0,max_y = 0;
    for (const auto& r : frame_grid){
        max_x = std::max(r.right()+1,max_x);
        max_y = std::max(r.bottom()+1,max_y);
    }
    ratio =  float(max_y)/max_x;

    return f;

}

MeshDisplayer *PlotTab::add_3D_frame(int w, int h)
{
    return insert_3D_frame(get_frame_free_pos(w,h));
}

MeshDisplayer *PlotTab::add_3D_frame_at(int px, int py, int w, int h)
{
    return insert_3D_frame(check_frame_free_pos(px,py,w,h));
}

SceneViewer *PlotTab::insert_scene_frame(const QRect & R)
{
    frame_grid.push_back(R);
    SceneViewer* f = new SceneViewer(this);

    QSizePolicy qsp(QSizePolicy::Preferred,QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    f->setSizePolicy(qsp);

    Grid->addWidget(f,R.y(),R.x(),R.height(),R.width());
    Scenes.push_back(f);

    int max_x = 0,max_y = 0;
    for (const auto& r : frame_grid){
        max_x = std::max(r.right()+1,max_x);
        max_y = std::max(r.bottom()+1,max_y);
    }
    ratio =  float(max_y)/max_x;

    return f;

}

SceneViewer *PlotTab::add_3D_scene(int w, int h)
{
    return insert_scene_frame(get_frame_free_pos(w,h));
}
#endif

PlotFrame* PlotTab::add_frame_at(int px, int py, int w, int h)
{
    return insert_frame(check_frame_free_pos(px,py,w,h));
}

PlotFrame* PlotTab::add_frame(int w,int h)
{
    return insert_frame(get_frame_free_pos(w,h));
}


float PlotTab::get_tab_ratio() const
{
    return ratio;
}

PlotTab::~PlotTab()
{
}
