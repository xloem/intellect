#include <library/time.hpp>

#include <chrono>
#include <cmath>

#include <cerrno>
#include <ctime>

namespace library {

void sleep_for(double seconds)
{
	struct timespec time;

	clock_gettime(CLOCK_MONOTONIC, &time);

	double seconds_fractional, seconds_integral;
	seconds_fractional = std::modf(seconds, &seconds_integral);

	time.tv_nsec += seconds_fractional * 1000000000;
	time.tv_sec += seconds_integral + time.tv_nsec / 1000000000;
	time.tv_nsec %= 1000000000;

	while (EINTR == clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &time, nullptr)) {
		// TODO? report interruption
	}
}

void sleep_unix(double deadline)
{
	struct timespec time;

	double seconds_fractional, seconds_integral;
	seconds_fractional = std::modf(deadline, &seconds_integral);

	time.tv_nsec = seconds_fractional * 1000000000;
	time.tv_sec = seconds_integral;

	while (EINTR == clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &time, nullptr)) {
		// TODO? report interruption
	}
}

double seconds_monotonic()
{
	std::chrono::duration<double> now = std::chrono::steady_clock::now().time_since_epoch();
	return now.count();
}

double seconds_unix()
{
	std::chrono::duration<double> now = std::chrono::system_clock::now().time_since_epoch();
	return now.count();
}

}
