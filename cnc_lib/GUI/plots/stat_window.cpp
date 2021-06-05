#include "stat_window.h"

cnc::StatWindow::StatWindow(PlotFrame *r,const StatList& p) : QWidget(nullptr){
    setMinimumSize(QSize(1000,500));
    setMaximumSize(QSize(1000,500));


    ref = r;
    SL = p;
    init = false;
}

void cnc::StatWindow::paintEvent(QPaintEvent *e)
{
    if (!init)
        init_window();
    SD->QSL->setCurrentIndex(SD->ref->current_frame);
    SD->paintEvent(e);
}

void cnc::StatWindow::init_window()
{
    QHL = new QHBoxLayout;
    FD = ref->duplicate_frame(this);

    QHL->addWidget((QWidget*)FD);

    QFrame* F = new QFrame;
    F->setLineWidth(1);
    F->setFrameStyle(1);
    F->setMaximumSize(QSize(500,500));
    SD = new StatDisplayer(FD,F,SL);
    QHL->addWidget(F);

    QWidget::setLayout(QHL);
    init = true;
}

cnc::PlotFrame::~PlotFrame()
{
    for (uint i = 0;i<popups.size();i++)
        delete popups[i];
}

cnc::StatDisplayer::StatDisplayer(cnc::PlotFrame* f,QWidget* parent,const StatList& SL) : ref(f)
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
