#include <library/time.hpp>

#include "test.hpp"

using namespace library;

int main()
{
	double start_unix = seconds_unix();
	double start_monotonic = seconds_monotonic();
	sleep_for(0.1);
	worry(differ_more_than(start_monotonic + 0.1, seconds_monotonic(), 0.001), "0.1 second monotonic sleep actually lasted " + string(seconds_monotonic() - start_monotonic) + " monotonic seconds");
	worry(differ_more_than(start_unix + 0.1, seconds_unix(), 0.001), "0.1 second monotonic sleep actually lasted " + string(seconds_unix() - start_unix) + " seconds unix time");

	start_unix = seconds_unix();
	start_monotonic = seconds_monotonic();
	sleep_unix(seconds_unix() + 0.1);
	worry(differ_more_than(start_monotonic + 0.1, seconds_monotonic(), 0.001), "0.1 second unix sleep actually lasted " + string(seconds_monotonic() - start_monotonic) + " monotonic seconds");
	worry(differ_more_than(start_unix + 0.1, seconds_unix(), 0.001), "0.1 second unix sleep actually lasted " + string(seconds_unix() - start_unix) + " seconds unix time");

	string time = timestamp_iso(start_unix);
	double reconverted_time = seconds_iso(time.c_str());
	worry(start_unix != reconverted_time, "Time conversion inequality " + string(start_unix, 10, false, 10) + " -> " + time +  " -> " + string(reconverted_time, 10, false, 10));

	double now = seconds_iso(timestamp_iso());
	worry(differ_more_than(now, seconds_unix(), 0.001), "default iso time of " + string(now) + " differs from current time of " + seconds_unix());

	stderr::line("time passed test");
}
