#include "mutablevalue.h"

cnc::MutableValue::MutableValue(){}

cnc::MutableValue::~MutableValue()
{
}

int cnc::MutableValue::time_since_last_update_in_milliseconds() const
{
    auto current_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_update).count();
}

