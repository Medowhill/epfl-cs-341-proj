#ifndef STOPWATCH_H
#define STOPWATCH_H

#ifdef _WIN32
#  include <windows.h>
#else
#  include <sys/time.h>
#endif

#include <iostream>

class StopWatch {
public:
    StopWatch();
    void start();
    double stop();
    double elapsed() const;
    
private:
#ifdef _WIN32
    LARGE_INTEGER starttime_, endtime_;
    LARGE_INTEGER freq_;
#else
    timeval current_time() const;
    timeval starttime_, endtime_;
#endif
};

inline std::ostream& operator<<(std::ostream& _os, const StopWatch& _timer) {
    _os << _timer.elapsed() << " ms";
    return _os;
}

#endif
