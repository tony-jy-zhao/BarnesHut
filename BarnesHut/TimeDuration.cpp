#include <ctime>
#include "TimeDuration.h"

TimeDuration::TimeDuration(int rounds): rounds_(rounds){}

double TimeDuration::calculateTime(const std::function<void(void)> &func) {
	std::clock_t t1 = clock();
	for (int i = 0; i < rounds_; i++) {
		func();
	}
	return (std::clock() - t1) / (double)CLOCKS_PER_SEC;
}
