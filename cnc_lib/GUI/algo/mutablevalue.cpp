#include "mutablevalue.h"

cnc::MutableValue::MutableValue(){}

cnc::MutableValue::~MutableValue()
{
}

int cnc::MutableValue::time_since_last_update_in_milliseconds() const
{
    return timeHandling::getTimeSinceTimePointInMicroseconds(last_update);
}

