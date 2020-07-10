#include <library/time.hpp>

#include <chrono>
#include <cmath>

#include <cerrno>
#include <ctime>

#include <library/string.hpp>

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

char const * timestamp_iso(double seconds)
{
	if (0 == seconds) { seconds = seconds_unix(); }

	time_t now = seconds;
	struct tm time_struct;
	gmtime_r(&now, &time_struct);

	static thread_local string result;
	result.resize(256);
	size_t size = strftime(result.data(), result.size(), "%F-%T", &time_struct);
	result.resize(size);

	double integer_seconds;
	double fractional_seconds = std::modf(seconds, &integer_seconds);
	string nanoseconds(fractional_seconds, 10, false, 9);

	result += string(nanoseconds.data() + 1, nanoseconds.size() - 1);

	return result.c_str();
}

double seconds_iso(char const * timestamp)
{
	struct tm time_struct;
	char * remaining = strptime(timestamp, "%F-%T", &time_struct);
	time_t now = mktime(&time_struct) - timezone;

	return now + string(remaining).to_double();
}

}
