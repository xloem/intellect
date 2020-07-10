#pragma once

namespace library {

void sleep_for(double seconds);
void sleep_unix(double deadline);

double seconds_monotonic();
double seconds_unix();

char const * timestamp_iso(double seconds = 0);
double seconds_iso(char const * timestamp);

} // namespace library
