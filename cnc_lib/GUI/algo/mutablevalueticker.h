#ifndef MUTABLEVALUETICKER_H
#define MUTABLEVALUETICKER_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "mutablevalue.h"
#include <chrono>
#include <QTimer>
#include "GUI/plotwindow.h"

namespace cnc {

class MutableValueTicker : QObject,public MutableValue
{
public:
    MutableValueTicker(PlotWindow*,uint tickrate);
    ~MutableValueTicker();
private:
    uint tickrate;
    scalar value() const override;
    std::chrono::time_point<std::chrono::steady_clock> start;
    QTimer* timer;
};
}

#endif // MUTABLEVALUETICKER_H
