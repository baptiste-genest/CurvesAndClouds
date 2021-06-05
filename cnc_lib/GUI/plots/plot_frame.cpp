#include "plot_frame.h"

using namespace cnc;

PlotFrame::PlotFrame(QWidget* parent) : QFrame(parent)
{
    current_frame = 0;
    setLineWidth(1);
    setFrameStyle(1);
}


PlotLayer* PlotFrame::add_layer()
{
    PlotLayer* l = new PlotLayer(this);
    layers.push_back(l);
    return l;
}

GridLayer *PlotFrame::add_grid_layer(range x, range y, bool display_grid)
{
    GridLayer* g = new GridLayer(x,y,display_grid,this);
    layers.push_back((PlotLayer*)g);
    return g;

}


void PlotFrame::set_nb_layer_per_second(float layer_rate)
{
    if (layer_rate < 1e-4)
        throw Cnc_error("can't set a frame rate to small or negative");
    nb_layers_per_second = layer_rate;
    timer->start((uint) (1000 / nb_layers_per_second));
}

std::vector<bool> PlotFrame::list_types_contained() const
{
    std::vector<bool> types(7,false);//NUMBER OF TYPES
    for (uint k = 0;k<layers.size();k++){
        for (PlotLayer* l : layers){
            for (Plottable* p : l->plots){
                types[(int)p->get_type()] = true;
            }
        }
    }

    return types;
}

void PlotFrame::paintEvent(QPaintEvent* e)
{

    QPainter qp(this);
    qp.setRenderHint(QPainter::Antialiasing);
    drawWidget(qp);

    QFrame::paintEvent(e);
}

void PlotFrame::drawWidget(QPainter& painter)
{
    frame_info fi = {(uint)QFrame::width(),(uint)QFrame::height()};
    frame_draw_object fdo = {painter,fi};
    painter.setBrush(QColorConstants::White);
    painter.drawRect(QRect(0,0,fi.width,fi.height));
    painter.setBrush(QColorConstants::Black);
    draw_current_layer(fdo);
}

void PlotFrame::start_timer() {
    timer = new QTimer(this);

    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(draw_next_frame()));
    timer->start((uint) (1000 / nb_layers_per_second));
}

void PlotFrame::draw_next_frame()
{
    current_frame++;
    current_frame %= layers.size();
    QWidget::update();
}

PlotFrame *PlotFrame::duplicate_frame(QWidget* parent) const{
    PlotFrame* D = new PlotFrame(parent);
    D->timer = timer;
    QObject::connect(timer,SIGNAL(timeout()),D,SLOT(draw_next_frame()));

    D->layers.resize(layers.size());
    for (uint i = 0;i<layers.size();i++)
        D->layers[i] = layers[i]->duplicate_layer(parent);

    return D;
}
