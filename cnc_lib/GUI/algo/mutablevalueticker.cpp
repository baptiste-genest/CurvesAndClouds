#include "mutablevalueticker.h"

cnc::MutableValueTicker::MutableValueTicker(cnc::PlotWindow *W,uint tickr) : tickrate(tickr)
{
    start = std::chrono::steady_clock::now();
    timer = new QTimer;
    timer->start(tickrate);

    QObject::connect(timer,&QTimer::timeout,[W]() {
        W->update();
    });
}

cnc::MutableValueTicker::~MutableValueTicker()
{
    delete timer;
}

cnc::scalar cnc::MutableValueTicker::value() const
{
    std::chrono::duration<scalar> elapsed = std::chrono::steady_clock::now() - start;
    return elapsed.count();
}



cnc::MutScalar cnc::PlotWindow::get_time_variable(uint tickrate)
{
    MutableValue* V = new MutableValueTicker(this,tickrate);
    mv.push_back(V);
    return MutScalar(V);
}
