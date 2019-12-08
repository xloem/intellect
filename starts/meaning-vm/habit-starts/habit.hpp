#pragma once

#include <stdlib.h> // int rand(); void srand(int seed);
#include <time.h> // int time(0);
#include <unistd.h> // usleep(unsigned int usecs)

__attribute__ ((visibility ("hidden"))) int __seed = (srand(time(0)), time(0));
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
