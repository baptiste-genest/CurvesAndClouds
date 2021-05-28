#include "plot_frame.h"

using namespace cnc;

Plot_frame::Plot_frame(QWidget* parent) : QFrame(parent)
{
    current_frame = 0;
    setLineWidth(1);
    setFrameStyle(1);
}


Plot_layer* Plot_frame::add_layer()
{
    Plot_layer* l = new Plot_layer(this);
    layers.push_back(l);
    return l;
}


void Plot_frame::set_nb_layer_per_second(float layer_rate)
{
    if (layer_rate < 1e-4)
        throw Cnc_error("can't set a frame rate to small or negative");
    nb_layers_per_second = layer_rate;
    timer->start((uint) (1000 / nb_layers_per_second));
}

std::vector<bool> Plot_frame::list_types_contained() const
{
    std::vector<bool> types(7,false);//NUMBER OF TYPES
    for (uint k = 0;k<layers.size();k++){
        for (Plot_layer* l : layers){
            for (Plottable* p : l->plots){
                types[(int)p->get_type()] = true;
            }
        }
    }

    return types;
}

void Plot_frame::paintEvent(QPaintEvent* e)
{

    QPainter qp(this);
    qp.setRenderHint(QPainter::Antialiasing);
    drawWidget(qp);

    QFrame::paintEvent(e);
}

void Plot_frame::drawWidget(QPainter& painter)
{
    frame_info fi = {(uint)QFrame::width(),(uint)QFrame::height()};
    frame_draw_object fdo = {painter,fi};
    painter.setBrush(QColorConstants::White);
    painter.drawRect(QRect(0,0,fi.width,fi.height));
    painter.setBrush(QColorConstants::Black);
    draw_current_layer(fdo);
}

void Plot_frame::start_timer() {
    timer = new QTimer(this);

    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(draw_next_frame()));
    timer->start((uint) (1000 / nb_layers_per_second));
}

void Plot_frame::draw_next_frame()
{
    current_frame++;
    current_frame %= layers.size();
    QWidget::update();
}

Plot_frame *Plot_frame::duplicate_frame(QWidget* parent) const{
    Plot_frame* D = new Plot_frame(parent);
    D->timer = timer;
    QObject::connect(timer,SIGNAL(timeout()),D,SLOT(draw_next_frame()));

    D->layers.resize(layers.size());
    for (uint i = 0;i<layers.size();i++)
        D->layers[i] = layers[i]->duplicate_layer(parent);

    return D;
}
