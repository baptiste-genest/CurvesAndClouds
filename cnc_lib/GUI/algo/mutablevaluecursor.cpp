#include "mutablevaluecursor.h"

cnc::MutableValueCursor::MutableValueCursor(PlotWindow* W,cnc::range v,uint nb_steps,const QString& label): bounds(v) , nb_ticks(nb_steps)
{

    slider = new QSlider(Qt::Orientation::Horizontal,W->sliders_panel);
    slider->setMinimum(0);
    slider->setMaximum(nb_ticks);
    slider->setTracking(true);
    slider->setValue(nb_ticks/2);

    W->sliders_box->addWidget(slider);

    values = cnc::algo::calculus::get_lin_space(v.first,v.second,(int)nb_ticks+1);

    uint id = W->sliders_text.size()-1;
    const MutableValueCursor* C = this;
    timeHandling::PointInTime* pit = &last_update;
    QObject::connect(slider,&QSlider::valueChanged,[W,id,C,label,pit] (int) {
        W->update();
        W->sliders_text[id]->setText(label + QString(":		") + QString::number(C->value()));
        W->sliders_panel->update();
        *pit = std::chrono::high_resolution_clock::now();
    });

    /*
    slider->resize(200,50);
    slider->show();
    */
}

cnc::scalar cnc::MutableValueCursor::value() const
{
    int v = slider->value();
    return values[v];

}

cnc::MutScalar cnc::PlotWindow::add_mutable_scalar_by_cursor(range v,const QString& Label,uint nb_steps)
{
    if (sliders_panel == nullptr){
        sliders_box = new QVBoxLayout;
        sliders_panel = new QWidget;
        sliders_panel->setLayout(sliders_box);
    }
    sliders_text.push_back(new QLabel(Label));
    sliders_box->addWidget(sliders_text.back());
    MutableValue* V = new MutableValueCursor(this,v,nb_steps,Label);
    mv.push_back(V);
    sliders_panel->show();
    return MutScalar(V);
}


