#ifndef TIME_HANDLING_H
#define TIME_HANDLING_H

#include <time.h>
#include <chrono>
#include "cnc_types.h"

namespace cnc {
namespace timeHandling {

//static std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

inline static scalar getTimeSinceStartMilliseconds(){
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    auto current_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count()*0.001;
}

}

}


#endif // TIME_HANDLING_H
