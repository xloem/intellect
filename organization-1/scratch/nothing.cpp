#include <library/string.hpp>
#include <library/time.hpp>

#include <stdlib.h>

double random_double()
{
	return double(random()) / RAND_MAX;
}

int main(int argc, char const * const * argv)
{
	while ("time passes") {

		double seconds = random_double() * 10;

		library::stdout::line("Sleeping for " + library::string(seconds) + " seconds ...");

		// wait
		library::sleep_for(seconds);

		// do it again.
	}

	return 0;
}
