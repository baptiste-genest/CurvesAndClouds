#ifndef MUTABLEVALUECURSOR_H
#define MUTABLEVALUECURSOR_H

#include "cnc_types.h"
#include "core/algo/calculus/calculus.h"
#include "GUI/plotwindow.h"
#include <QSlider>
#include <QObject>

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "mutablevalue.h"
#include <QLabel>

namespace cnc {


class MutableValueCursor : QObject, public MutableValue
{
public:
    MutableValueCursor(PlotWindow*,range v,uint nb_steps,const QString& label);
    MutableValueCursor(PlotWindow*,range v,uint nb_steps,const QString& label,const updateRoutine& update);
    inline bool hasChanged(){
        if (!changed)
            return false;
        changed = false;
        return true;
    }

private:
    bool changed = false;
    scalar value() const override;
    range bounds;
    uint nb_ticks;

    cnc::algo::calculus::nodes values;

    QSlider* slider;
};

}

#endif // MUTABLEVALUECURSOR_H
