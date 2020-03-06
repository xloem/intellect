// let's make some C/C++ fluff.

#include <string>
#include <vector>

using Value = double;

template <typename T>
using Properties = std::vector<T>;
using PropertySpec = Properties<std::string>;

struct Life;

// we're considering having the two organs have a shared environment.
// what do they do?
// 	nl1 listens for 'learn-from-this-example'
// 	and makes a huge sorted record of example-successes
// 	and example-failures, so as to quickly find
// 	what-worked and what-failed.
//
// 	nl2 listens for 'guess-what-might-work' and
// 	looks in the record, spitting out what might work.
//
// so nl1 is bound to the ears, and can only say 'understood'
// nl2 is bound to the mouth, and conly hear 'go with this'
// the ears&mouth are agreed-upon-communication-channels
//
// but the map is shared community work
// 	it's analogous to a shared resource, like a tribe
// 	of buffalo that are useful for all to eat from,
// 	and must be protected by all, in ways that work.
// 		[if you catch all the buffalo and farm them,
// 		 implicit agreement broken.]
// 			example-too-distant
// 	or a water supply in a public city.
// 	everybody may drink from, nobody may dump in.
// 	must always engage in ways that everybody does the same.
// 	shared-environment.
//
// so nl1 and nl2 live in a shared environment.
// they have a shared environment part.
// 	yes.  the body they are a part of, has a class structure.

/*
struct EnvironmentSpec
{
	PropertySpec scalars;
	PropertySpec vectors;
};
*/

struct LifeSpec
{
	// label
	std::string name;

	// muscles, mouth, ears
	PropertySpec scalars;
	PropertySpec vectors;

	// private chemistry
	using instructions = void(*)(Life & life);
	instructions genes;
	Properties<LifeSpec> organs;
};

LifeSpec make_life_spec(std::string name, std::initializer_list<LifeSpec> organs, std::initializer_list<std::string> scalars, std::initializer_list<std::string> vectors, LifeSpec::instructions function)
{
	LifeSpec spec;
	spec.name = name;
	spec.organs = organs;
	spec.scalars = scalars;
	spec.vectors = vectors;
	spec.genes = function;
	return spec;
}

struct Life
{
	LifeSpec spec;

	// muscles, mouth, ears
	Properties<Value> scalars;
	Properties<std::vector<Value>> vectors;

	// private parts
	Properties<Life> organs;
	void * body;
};

Life make_life(LifeSpec & spec)
{
	Life life;
	for (auto & organ_spec : spec.organs) {
		life.organs.emplace_back(make_life(organ_spec));
	}

	life.scalars.resize(spec.scalars.size());
	life.vectors.resize(spec.vectors.size());
	life.body = 0;
	return life;
}

#include <multimap>

LifeSpec::instructions number_learner_1;


struct Number_Learner_1
{
	static LifeSpec spec;
	static void instructions(Life & self);

	enum { NUMBER_IN, NUMBER_OUT, SUCCESS };
	std::unordered_multimap<Value, Value> guesses[2];
};
Number_Learner_1::spec =  make_life_spec(
		"number learner 1", 
		{}, 
		{"number in", "number out", "success"}, 
		{}, 
		Number_Learner_1::instructions
);
void Number_Learner_1::instructions(Life & self)
{
	auto body = (Number_Learner_1*)self.body;
	if (!body) {
		body = new Number_Learner_1();
	}
	
	body->guesses[body->scalars[SUCCESS] ? 0 : 1][body->scalars[NUMBER_IN]] = body->scalars[NUMBER_OUT];
}


// nl1 learns information.
// they have shared material with nl2.
// quick solution is to make nl2 access their body.

// i make a vector, not a number.
size_t number_guesser_1(void *&state, Value number1, std::vector<Value> & guesses)
{
	auto state = (number_learner_1_state*)spec.private_data;
	auto matching = state->guesses.equal_range(number1);
	size_t count = 0;
	for (auto it = matching.first; it != matching.second; ++ it) {
		++ count;
		guesses.push_back(it->second);
	}
	return count;
}
*/


int main(int argc, char **argv)
{
	return 0;
}

