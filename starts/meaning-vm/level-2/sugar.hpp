#pragma once

#include <stdlib.h> // int rand(); void srand(int seed);
#include <time.h> // int time(0); int clock_gettime(CLOCK_REALTIME, struct timespec *tp{tv_sec,tv_nsec})
#include <unistd.h> // usleep(unsigned int usecs)

#define habitdelay \
	static int thisdelay = (double(rand()) / RAND_MAX * 400000 + 200000); \
	usleep(thisdelay)

#undef self
#define ahabit(name, ...) \
	a(habit, name); \
	(name).fun((std::function<ref(ref)>) \
	[=](ref ctx) \
	{ \
		habitdelay; \
		ref self = name; \
		(void)self; \
		__VA_ARGS__ \
		return intellect::level1::concepts::nothing; \
	});

// seed random number generator statically, for habitdelay
namespace __internal {
static struct timespec __tp;
static int __timeres = clock_gettime(CLOCK_REALTIME, &__tp);
static int __seed = (srand(__tp.tv_nsec), __tp.tv_nsec);
}
