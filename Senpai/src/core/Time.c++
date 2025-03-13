#include <core/Time.h++>
#include <thread>

TimePoint get_TimePoint() {
   return std::chrono::system_clock::now();
}

double get_time_diff(TimePoint start, TimePoint end){
   return std::chrono::duration_cast<
      std::chrono::duration<double>>(end - start).count();
}

// Start measuring time
void Stopwatch::start() {
    endTime     = std::chrono::system_clock::now();
    startTime   = std::chrono::system_clock::now();
}

double Stopwatch::time() {
    TimePoint currentTime;
    // If the stopwatch hasn't been stopped, we use the current system time
    // Otherwise, use the recorded `endTime`.
    if (endTime <= startTime) { // This handles uninitialized or improperly reset `endTime`
        currentTime = std::chrono::system_clock::now();
    } else {
        currentTime = endTime;
    }
    // Calculate the elapsed time in seconds as a double
    return std::chrono::duration_cast<
        std::chrono::duration<double>>(currentTime - startTime).count();
}

// Stop measuring time and print the total elapsed time
void Stopwatch::stop() {
    endTime = std::chrono::system_clock::now();
}

// Sleep for x seconds
void sleep(double x) {
    auto duration = std::chrono::duration<double>(x); // Creates a duration in seconds (double type)
    std::this_thread::sleep_for(duration);
}