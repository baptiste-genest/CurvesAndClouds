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
}

void PlotTab::drawWidget(QPainter&)
{
}

PlotFrame *PlotTab::insert_frame(const QRect& R)
{
    frame_grid.push_back(R);
    PlotFrame* f = new PlotFrame(this);
    f->start_timer();

    QSizePolicy qsp(QSizePolicy::Preferred,QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    f->setSizePolicy(qsp);

    Grid->addWidget(f,R.x(),R.y(),R.width(),R.height());
    frames.push_back(f);

    int max_x = 0,max_y = 0;
    for (const auto& r : frame_grid){
        max_x = std::max(r.right()+1,max_x);
        max_y = std::max(r.bottom()+1,max_y);
    }
    ratio =  float(max_x)/max_y;

    return f;
}

PlotFrame* PlotTab::add_frame_at(int px, int py, int w, int h)
{
    if (px < 0 || py < 0)
        throw Cnc_error("position in frame grid must be strictely positive");
    else if (w <= 0 || h <= 0)
        throw Cnc_error("frame size must be positive");

    QRect tmp(py,px,h,w);

    for (const QRect& R : frame_grid)
        if (tmp.intersects(R))
            throw Cnc_error("can't place a frame here, place unavailable");

    return insert_frame(tmp);
}

PlotFrame* PlotTab::add_frame(int w,int h)
{
    if (w <= 0 || h <= 0)
        throw Cnc_error("frame size must be strictely positive");
    for (uint y = 0;y<MAX_CELL_HEIGHT;y++){
        for (uint x = 0;x<MAX_CELL_WIDTH;x++){
            QRect tmp((int)y,(int)x,h,w);
            bool place_free = true;
            for (const QRect& R : frame_grid){
                if (tmp.intersects(R)){
                    place_free = false;
                    break;
                }
            }
            if (place_free)
                return insert_frame(tmp);

        }
    }
    throw Cnc_error("Couldn't place frame in grid");
}

float PlotTab::get_tab_ratio() const
{
    return ratio;
}

PlotTab::~PlotTab()
{
}
