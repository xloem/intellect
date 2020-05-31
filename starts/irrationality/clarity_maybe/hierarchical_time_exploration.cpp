#include <cstdlib>
#include <unordered_map>

using namespace std;

double random_double()
{
	return double(rand())/RAND_MAX;
}

// in future would be nice to knwo about compile-time strings for this quick-request
struct need_new_option {/*
	need_new_option(
	choice * provide()
	{
	}*/
};

class choice
{
public:
	choice()
	{ }
	choice * select(double selection)
	{
		double sum = 0;
		for (auto option : result)
		{
			sum += option.first->value;
			if (sum > selection) {
				return option.first;
			}
		}
		throw need_new_option();
	}
	choice * newchoice(double degree = 0)
	{
		if (degree == 0) { degree = random_double(); }
		if (degree < 0 || degree >= 1) { throw "impossible"; }
		double sum = 0;
		for (auto : result) {
			sum += result.first->value;
		}
		choice * newoption = new choice();
		newoption->value = (1 - sum) * degree;
		result[newoption] = newoption;
		return newoption;
	}

	// want to explore in hierarchy.
	// plan
	
	// this recursive fucntion is a model for e.g. a queue-based approach
	// 	queue-based approaches can epxlore in greater depth.
	// 	recursive models can crash if used extensively without a queue.
	
	// we need ways of relating new information to what it is relevent to, from inside a process <-
	
	// this is severely aided by "wayness"-relevence
	void explore(double time_provided, double & time_used, unordered_map<choice*,choice*> & active_state) // i would like tou e the _- switching script not now though
	{
		// one option is terminating the fucntion after time_provided ticks
		// note: this kinda leaves out feedback if new value is found, for now
		//       handling change-of-property seems unaddressed for now too.

		// we are designing recursive first because it helps us
		// resist the anti-generalization in our design.

		time_used = 0;

		// active-state.
		// when we return to this choice, we want to try new untried
		// options, not old ones.

			// we could mis-use choices class to hold state.
			// it has a map of choice* to choice*
			// we could put our last-used decision in that map.
		
		// let's make labels for generalization needs
			// NOTE: this new use of ourselves violates an undiscussed norm
			// let's use something different for active-state for now.
		auto & state = special_state[this].result;
		
		for (auto & option : result)
		{
			double time_value = option.first->value;


			// let us assume that time amounts smaller than 1
			// do not happen, for now.  is easy to plug in
			// clock and make system deadlineable.
			
			// option.second->explore(time_provided * time_used);

		}
	}

	double value;
	unordered_map<choice *, choice *> result;
};

// i'm kind of holding the context now,
// seeing how excited you guys are about this.
// we have unify-work-with-other-work
// and we have keep-making-this-part-here

int main()
{
	// we'll need multiple choices
	auto first = new choice();


	first->value = 1.0;

};
