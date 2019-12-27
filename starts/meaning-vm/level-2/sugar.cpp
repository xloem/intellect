#include "sugar.hpp"

#include <stdlib.h> // int rand(); void srand(int seed);
#include <time.h> // int time(0); int clock_gettime(CLOCK_REALTIME, struct timespec *tp{tv_sec,tv_nsec})
#include <unistd.h> // usleep(unsigned int usecs)

namespace intellect {
namespace level2 {
namespace sugar {

double rand(double min, double max)
{
	// seed random number generator statically, for habit delay
	static struct timespec tp;
	static int seed = (
			clock_gettime(CLOCK_REALTIME, &tp),
			srand(tp.tv_nsec),
			tp.tv_nsec
	);
	(void)(seed);

	return double(::rand()) / RAND_MAX * (max - min) + min;
}

void usleep(unsigned int usecs)
{
	::usleep(usecs);
}

}
}
}
