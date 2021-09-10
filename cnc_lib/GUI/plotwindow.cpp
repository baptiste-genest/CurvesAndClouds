#include "plotwindow.h"

using namespace cnc;

PlotWindow::PlotWindow()
    : QMainWindow(nullptr)
{
    init = false;

    tabs = new QTabWidget(this);
    tabs->setGeometry(0,0,500,500);
    QObject::connect(tabs,SIGNAL(currentChanged(int)),this,SLOT(call_resize()));

    setWindowTitle("Double click on a frame to analyze it");
}

PlotWindow::~PlotWindow()
{
    if (sliders_panel)
        delete sliders_panel;
}

PlotTab* PlotWindow::add_tab(const QString& tab_name)
{
    PlotTab* tab = new PlotTab(this);
    tabs->addTab(tab,tab_name);
    tabs_list.push_back(tab);
    return tab;
}

int PlotWindow::run()
{
    /*
    int a = 0;
    char** b= nullptr;
    App = new QApplication(a,b);
    W->show();
    */
    return 1;
}

void PlotWindow::resizeEvent(QResizeEvent* event)
{
    init = true;
    float r = ((PlotTab*) tabs->currentWidget())->get_tab_ratio();
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
    const static int tbs = 28;
    resize(w,h+tbs);
    tabs->resize(w, h+tbs);
    tabs->currentWidget()->resize(w, h);
}

void PlotWindow::call_resize()
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
