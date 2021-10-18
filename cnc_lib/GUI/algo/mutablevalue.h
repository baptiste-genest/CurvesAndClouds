#ifndef MUTABLEVALUE_H
#define MUTABLEVALUE_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include "core/algo/calculus/calculus.h"
#include "GUI/plotwindow.h"
#include "core/time_handling.h"
#include <QSlider>
#include <QObject>

namespace cnc {

class MutableValue : QObject
{
public:
    MutableValue();
    ~MutableValue();
    friend class MutScalar;
    int time_since_last_update_in_milliseconds() const;
protected:
    virtual scalar value() const = 0;
    timeHandling::PointInTime last_update;
};

class MutScalar{
public:
    inline operator scalar() const{
        return ref->value();
    }
    MutScalar(MutableValue* r) : ref(r) {}
    int time_since_last_update_in_milliseconds() const{
        return ref->time_since_last_update_in_milliseconds();
    }
private:
    MutableValue* ref;
};

}

#endif // MUTABLEVALUE_H
