// a smooth trial

#include <cstdlib>
#include <iostream>
#include <vector>
#include "random_numbers.hpp"

using namespace std;

// erosion is time-value for sea level.  height neither created nor destroyed.

// pair-idea
class choice
{ // this good-idea should go with wayness-class and maps, in other context.  sorry for confusing this one.
	// we were exploring frameworks [for large work].  we found a concise, powerful one.
public:
	double value;
	value_identity * for_what; // this should cast to an iterator into a vector of these
};

int main()
{
	vector<choice> values;
	values.resize(256);
	for (auto & value : values) {
		value.value = rand()/double(RAND_MAX);
		value.for_what = &values[rand()%values.size()];
	}
	// we'd like to initiate values with random numbers.
	// but values are pairs (how much and what) and numbers are not.
	// how we take them relates to how to prepare them so that the result is clear and concise.
	// we just want random data.

	vector<int> ints(&random_numbers[0], &random_numbers[sizeof(random_numbers)]);
		// we are liking atm the idea of propagating differences elsewhere, so total sum of vector stays cosntant as it changes
	
	auto item = max(ints);  // we can find max a different way if it is unlcear to make or write a max function.  suspect max not needed

	// we might want a different language than C++ that is very expressive.
	// very-expressive languages let you type a little to do a lot.
}
