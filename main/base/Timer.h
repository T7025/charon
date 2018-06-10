//
// Created by thomas on 5/3/18.
//

#ifndef CHARON_TIMER_H
#define CHARON_TIMER_H


#include <omp.h>

class Timer {
public:
    Timer() : currentTime{omp_get_wtime()}, startTime{currentTime}, lapTime{currentTime} {}
    double lap() {
        currentTime       = omp_get_wtime();
        const auto result = currentTime - lapTime;
        lapTime           = currentTime;
        return result;
    }
    double finish() { return omp_get_wtime() - startTime; }

private:
    double currentTime;
    double startTime;
    double lapTime;
};


#endif //CHARON_TIMER_H
