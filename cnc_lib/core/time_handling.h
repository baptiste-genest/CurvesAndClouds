#ifndef TIME_HANDLING_H
#define TIME_HANDLING_H

#include <time.h>
#include <chrono>
#include <QTimer>
#include "cnc_types.h"
#include <memory>
#include <QObject>

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

class Chronometer {
public:
    Chronometer() {
        last = currentTime();
    }
    void tick(){
        auto now = currentTime();
        std::cout <<"time since last: " << getTimeSinceTimePointInMicroseconds(last) << std::endl;
        last = now;
    }
private:
    PointInTime last;
};

}

}


#endif // TIME_HANDLING_H
