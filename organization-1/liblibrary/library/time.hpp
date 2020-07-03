#pragma once

namespace library {

void sleep_for(double seconds);
void sleep_unix(double deadline);

double seconds_monotonic();
double seconds_unix();

} // namespace library
