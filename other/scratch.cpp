#include "../interfaces/event.hpp"

#include <stdio.h>
auto itch = fopen("/dev/urandom", "rb");

double crazy_within_bounds(double minimum, double maximum)
{
	unsigned long value;
	fread(&value, sizeof(value), 1, itch);
	return (maximum - minimum) * (value / double((decltype(value))~0)) + minimum;
}

void scratch(double * spot)
{
	*spot += crazy_within_bounds(*(spot - 1), *(spot + 1)) + crazy_within_bounds(-1, 1);
}

// scratching makes incredible discreteness

#include "../smoothness.hpp"

size_t surface_size = 32;
double * surface = (double*)malloc(sizeof(double) * surface_size);

#include <iostream>
int main()
{
	dev_input_event_source events("/dev/input/event2");
	dev_input_event event;

	std::cout << "Space to start." << std::endl;

	while (!events.read().is_press(events.spacebar_location()));

	while (!events.read().is_release(events.spacebar_location())) {
		for (int i = 0; i < surface_size; ++ i) {
			std::cout << " " << surface[i];
		}
		std::cout << std::endl;
		scratch(&surface[surface_size/2]);
	}
	//std::cout << "Space released." << std::endl;
	std::cout << "STOP-TOO-MUCH" << std::endl;

	return 0;
}
