#ifndef TIME_HANDLING_H
#define TIME_HANDLING_H

#include <time.h>
#include <chrono>
#include "cnc_types.h"

namespace cnc {
namespace timeHandling {

using PointInTime = std::chrono::time_point<std::chrono::high_resolution_clock>;

extern PointInTime start_time;

inline PointInTime currentTime(){
    return std::chrono::high_resolution_clock::now();
}

inline int getTimeSinceTimePointInMilliseconds(const PointInTime& pit){
    auto now = currentTime();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - pit).count();
}

inline int getTimeSinceTimePointInMicroseconds(const PointInTime& pit){
    auto now = currentTime();
    return std::chrono::duration_cast<std::chrono::microseconds>(now - pit).count();
}

inline scalar getTimeSinceStartMilliseconds(){
    auto current_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count()*0.001;
}

}

}


#endif // TIME_HANDLING_H
