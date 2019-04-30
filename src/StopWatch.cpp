#include "StopWatch.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

StopWatch::StopWatch() {
    #ifdef _WIN32
        QueryPerformanceFrequency(&freq_);
    #endif
}
    
void StopWatch::start() {
    #ifdef _WIN32
        QueryPerformanceCounter(&starttime_);
    #else
        starttime_ = current_time();
    #endif
}
    
double StopWatch::stop() {
    #ifdef _WIN32
        QueryPerformanceCounter(&endtime_);
    #else
        endtime_ = current_time();
    #endif
    return elapsed();
}
    
double StopWatch::elapsed() const {
    #ifdef _WIN32
        return ((double)(endtime_.QuadPart - starttime_.QuadPart)
                / (double)freq_.QuadPart * 1000.0f);
    #else
        return ((endtime_.tv_sec  - starttime_.tv_sec )*1000.0 +
                (endtime_.tv_usec - starttime_.tv_usec)*0.001);
    #endif
}

#ifndef _WIN32
timeval StopWatch::current_time() const {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv;
}
#endif
