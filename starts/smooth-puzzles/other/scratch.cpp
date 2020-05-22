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

		// some want it ongoing
		// others want to be careful it ends.
		// we could add another key
		// or limit the iterations
		// it soudns like a pause afterwards would be nice
		// and maybe reloading our data next time.
		// let's do the pause afterwards

	std::cout << "Space to start." << std::endl;

	bool stop_too_much = false; // <irritated at stop-too-much phrase>

	while (!events.read().is_press(events.spacebar_location()));

	while (!stop_too_much) {
		stop_too_much = events.read().is_release(events.spacebar_location());
		for (int i = 0; i < surface_size; ++ i) {
			std::cout << " " << surface[i];
		}
		std::cout << std::endl;
		scratch(&surface[surface_size/2]);
	}
	std::cout << "Space released." << std::endl;

	// concept of healing, repair, undoing, or compensation needed
	// properly, after stopping when it was too much.
	// too much left me confused.  please help me get back to normality.
	
	while (true) { // wait as long as needed to cool down
		stop_too_much = false;
		for (int i = 0; i < surface_size; ++ i) {
			if (surface[i] != 0) {
				surface[i] /= 2;
			}
			if (surface[i] > -1/1000000. && surface[i] < 1/1000000.) {
				// back in familiar zone.  know
				// how to stay where i go from here.
				surface[i] = 0;

				// this is the appears-discrete behavior of holding
				// yourself together at all
				// if one of your cells is moved in your body,
				// it gets very confused and dies.
				// but if it is only nudged, it goes right back.
			} else {
				stop_too_much = true;
			}
			// these are guesses.
			std::cout << " " << surface[i];
		}
		std::cout << std::endl;
		if (stop_too_much) {
			std::cout << "STOP-TOO-MUCH" << std::endl;
		} else {
			break;
		}
		//events.read(); // demnstrates key'blocking' issue
	}

	return 0;
}
