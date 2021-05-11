#include "stat_window.h"

cnc::Stat_window::Stat_window(Plot_frame *r,const Stat_list& p) : QWidget(nullptr){
    setMinimumSize(QSize(1000,500));
    setMaximumSize(QSize(1000,500));


    ref = r;
    SL = p;
    init = false;
}

void cnc::Stat_window::paintEvent(QPaintEvent *e)
{
    if (!init)
        init_window();
    SD->QSL->setCurrentIndex(SD->ref->current_frame);
    SD->paintEvent(e);
}

void cnc::Stat_window::init_window()
{
    QHL = new QHBoxLayout;
    FD = ref->duplicate_frame(this);

    QHL->addWidget((QWidget*)FD);

    QFrame* F = new QFrame;
    F->setLineWidth(1);
    F->setFrameStyle(1);
    F->setMaximumSize(QSize(500,500));
    SD = new Stat_displayer(FD,F,SL);
    QHL->addWidget(F);

    QWidget::setLayout(QHL);
    init = true;
}

cnc::Plot_frame::~Plot_frame()
{
    for (uint i = 0;i<popups.size();i++)
        delete popups[i];
}

cnc::Stat_displayer::Stat_displayer(cnc::Plot_frame* f,QWidget* parent,const Stat_list& SL) : ref(f)
{
    QSL = new QStackedWidget(parent);

    uint nb_layers = f->layers.size();
    for (uint k = 0;k<nb_layers;k++){
        QWidget* W = new QWidget;
        QVBoxLayout* V = new QVBoxLayout;
        uint N = f->layers[k]->plots.size();
        for (uint i = 0;i<N;i++){
            f->layers[k]->plots[i]->init_stat_display(f,f->layers[k],V,SL);
        }
        W->setLayout(V);
        QSL->addWidget(W);
    }
}
