#pragma once
#include <chrono>

using Time = std::chrono::time_point<std::chrono::system_clock>;

// used to Track time
struct Stopwatch
{
    Time startTime;
    Time endTime;
    
    // start measuring time
    void start();
    // get the current time
    double time();
    // stop measuring time
    void stop();
};

// Sleep for x seconds
void sleep(double x);
