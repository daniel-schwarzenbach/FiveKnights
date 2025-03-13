#pragma once
#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

// get the current time
TimePoint get_TimePoint();

// get the difference between two time points in seconds
double get_time_diff(TimePoint start, TimePoint end);


// used to Track time
struct Stopwatch
{
    TimePoint startTime;
    TimePoint endTime;
    
    // start measuring time
    void start();
    // get the current time
    double time();
    // stop measuring time
    void stop();
};

// Sleep for x seconds
void sleep(double x);
