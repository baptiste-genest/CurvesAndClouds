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
#include <QSlider>
#include <QObject>

namespace cnc {

class MutableValue : QObject
{
public:
    MutableValue();
    ~MutableValue();
    friend class MutScalar;
protected:
    virtual scalar value() const = 0;
};

class MutScalar{
public:
    inline operator scalar() const{
        return ref->value();
    }
    MutScalar(MutableValue* r) : ref(r) {}
private:
    MutableValue* ref;
};

}

#endif // MUTABLEVALUE_H
