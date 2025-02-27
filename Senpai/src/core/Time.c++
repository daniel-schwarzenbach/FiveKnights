#include <core/Time.h++>
#include <thread>

// Start measuring time
void Stopwatch::start() {
    endTime     = std::chrono::system_clock::now();
    startTime   = std::chrono::system_clock::now();
}

double Stopwatch::time() {
    Time currentTime;
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
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
}

// Sleep for x seconds
void sleep(double x) {
    auto duration = std::chrono::duration<double>(x); // Creates a duration in seconds (double type)
    std::this_thread::sleep_for(duration);
}