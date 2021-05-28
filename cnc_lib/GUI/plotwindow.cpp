#include "plotwindow.h"

using namespace cnc;

Plot_window::Plot_window()
    : QMainWindow(nullptr)
{
    init = false;

    tabs = new QTabWidget(this);
    tabs->setGeometry(0,0,500,500);
    QObject::connect(tabs,SIGNAL(currentChanged(int)),this,SLOT(call_resize()));

    setWindowTitle("Double click on a frame to analyze it");
}

Plot_window::~Plot_window()
{
}

Plot_tab* Plot_window::add_tab(const QString& tab_name)
{
    Plot_tab* tab = new Plot_tab(this);
    tabs->addTab(tab,tab_name);
    tabs_list.push_back(tab);
    return tab;
}

int Plot_window::run()
{
    int a = 0;
    char** b= nullptr;
    QApplication A(a,b);
    show();
    return A.exec();
}

void Plot_window::resizeEvent(QResizeEvent* event)
{
    init = true;
    float r = ((Plot_tab*) tabs->currentWidget())->get_tab_ratio();
    int w,h;
    if (event->size().width()*r < event->size().height()){
        w = event->size().width();
        h = w*r;
    }
    else {
        h = event->size().height();
        w = h/r;
    }
    QSize S(w,h);
    QResizeEvent qre(S,QMainWindow::size());
    QMainWindow::resizeEvent(&qre);
    resize(w,h);
    tabs->resize(w, h);
    tabs->currentWidget()->resize(w, h-30);
}

void Plot_window::call_resize()
{
    if (!init)
        return;
    QSize S = size();
    int L = std::max(S.width(),S.height());
    S.rheight() = L;
    S.rwidth() = L;
    QResizeEvent qre(S,S);
    resizeEvent(&qre);
}
