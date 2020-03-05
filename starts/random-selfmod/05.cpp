
// to manage function ordering, we don't use libraries and headers atm.

using real = float;
//using size_t = unsigned long;
using uint8_t = unsigned char;
using uint16_t = unsigned short;

constexpr unsigned long fixed_point_unity = 1024;

#define NDEBUG
// FOR TESTING
#undef NDEBUG
#include <assert.h>
#include <stdio.h>
// FOR TESTING

int main();

template <typename Value>
struct vector_t
{
	using value_t = Value;

	value_t * data;
	size_t size;
	size_t allocated;
};

template <typename Value> vector_t<Value> vector_init(size_t size = 0);
template <typename Value> vector_t<Value> vector_shadow(Value * array, size_t bytes);
template <typename Value, typename OriginalValue> vector_t<Value> vector_copy(vector_t<OriginalValue> const & original);
template <typename Value> void vector_set(vector_t<Value> & vector, Value value);
template <typename Value> void vector_resize(vector_t<Value> & vector, size_t size);
template <typename Value> void vector_swap(vector_t<Value> & vector, size_t index1, size_t index2);
template <typename Value> Value & vector_front(vector_t<Value> & vector);
template <typename Value> Value & vector_back(vector_t<Value> & vector);
template <typename Value> void vector_free(vector_t<Value> & vector);
template <typename Value, typename Index = size_t>
void vector_sort(vector_t<Value> & vector, vector_t<Index> * indices = 0, bool reverse = false);

template <typename Count, typename Frequency, typename Option, size_t _options>
struct sbc_t {
	using count_t = Count;
	using frequency_t = Frequency;
	using option_t = Option;

	static size_t constexpr options = _options;

	struct direction_t {
		vector_t<Count> count[options];
		vector_t<Frequency> chance[options];
		vector_t<Option> guess[options];
		//vector_t<Count> totals; // TODO: move totals over here if you want >1 axes to be easier
	};
	vector_t<Count> totals;

	vector_t<direction_t> directions;

	option_t maximum_guesses;
};

template <typename C, typename F, typename O, size_t o>
sbc_t<C,F,O,o> * sbc_init(size_t axes);
template <typename SBC>
void sbc_update_axis(SBC * sbc, typename SBC::option_t option1, typename SBC::option_t option2, size_t axis);
template <typename SBC, typename Source>
void sbc_update_block(SBC * sbc, vector_t<Source> & block, size_t axis);
template <typename SBC>
void sbc_generate_priority_row(SBC * sbc, size_t axis, typename SBC::option_t option);
template <typename SBC>
void sbc_generate_priority(SBC * sbc);
template <typename C, typename F, typename O, size_t o>
void sbc_free(sbc_t<C,F,O,o> * sbc);

vector_t<uint8_t> memblock_from_range(void * head, void * tail);

template <typename SBC> void try_to_reach(SBC * sbc, vector_t<typename SBC::option_t> & target);

using SBC = sbc_t<unsigned long,unsigned long,uint16_t,257>;

void * functions[]={
	(void*)main,

	(void*)vector_init<SBC::direction_t>,
	(void*)vector_shadow<SBC::direction_t>,
	(void*)vector_copy<SBC::direction_t,SBC::direction_t>,
	(void*)vector_resize<SBC::direction_t>,
	(void*)vector_swap<SBC::direction_t>,
	(void*)vector_front<SBC::direction_t>,
	(void*)vector_back<SBC::direction_t>,
	(void*)vector_free<SBC::direction_t>,
	//(void*)vector_sort<SBC::direction_t>,

	(void*)vector_init<SBC::count_t>,
	(void*)vector_shadow<SBC::count_t>,
	(void*)vector_copy<SBC::count_t,SBC::count_t>,
	(void*)vector_resize<SBC::count_t>,
	(void*)vector_swap<SBC::count_t>,
	(void*)vector_front<SBC::count_t>,
	(void*)vector_back<SBC::count_t>,
	(void*)vector_free<SBC::count_t>,
	(void*)vector_sort<SBC::count_t>,

	(void*)vector_init<void *>,
	(void*)vector_shadow<void *>,
	(void*)vector_copy<void *,void *>,
	(void*)vector_resize<void *>,
	(void*)vector_swap<void *>,
	(void*)vector_front<uint8_t>,
	(void*)vector_back<uint8_t>,
	(void*)vector_free<void *>,
	(void*)vector_sort<void *>,

	(void*)vector_init<SBC::option_t>,
	(void*)vector_shadow<SBC::option_t>,
	(void*)vector_copy<SBC::option_t,SBC::option_t>,
	(void*)vector_copy<SBC::option_t,uint8_t>,
	(void*)vector_resize<SBC::option_t>,
	(void*)vector_swap<SBC::option_t>,
	(void*)vector_front<SBC::option_t>,
	(void*)vector_back<SBC::option_t>,
	(void*)vector_free<SBC::option_t>,
	(void*)vector_sort<SBC::option_t>,

	(void*)sbc_init<SBC::count_t,SBC::frequency_t,SBC::option_t,SBC::options>,
	(void*)sbc_update_axis<SBC>,
	(void*)sbc_update_block<SBC, uint8_t>,
	(void*)sbc_generate_priority_row<SBC>,
	(void*)sbc_generate_priority<SBC>,
	(void*)sbc_free<SBC::count_t,SBC::frequency_t,SBC::option_t,SBC::options>,

	(void*)memblock_from_range//,
//	(void*)printf
};

// sequential-behavior-chance, growing slowly
// this maps information from a prior state to best information
// for a following state.  the information type is 'Option'
// 'Option' must be an integral type for now, as it is used as an
// array index into a table of frequencies.

template <typename Count, typename Frequency, typename Option, size_t options>
sbc_t<Count,Frequency,Option,options> * sbc_init(size_t axes)
{
	auto * sbc = new sbc_t<Count,Frequency,Option,options>();
	sbc->totals = vector_init<Count>(options);
	vector_set<Count>(sbc->totals, 0);
	sbc->maximum_guesses = 0;
	sbc->directions = vector_init<typename decltype(sbc->directions)::value_t>(axes * 2);
	for (size_t d = 0; d < axes * 2; ++ d) {
		for (size_t i = 0; i < options; ++ i) {
			sbc->directions.data[d].count[i] = vector_init<Count>(options);
			sbc->directions.data[d].chance[i] = vector_init<Frequency>(options);
			sbc->directions.data[d].guess[i] = vector_init<Option>(options);
			vector_set<Count>(sbc->directions.data[d].count[i], 0);
		}
	}
	return sbc;
};
// how are we going to compare likelihoods.
// the chance is the count / total_occurences
// so we would pick the option2 with the highest chance.
// 	we could use that to inform exploring.
// 	we would simply want to _always_ pick the option that had the biggest chance.
// 	so like if we have 1   1   1   1   1   1
// 			   80% k
// 	say we started with 80%, 50% <-- add to 100%
// 	then under 80% we found 90%, 3% <-- add to 100%
// 	then under 90% we found 4%, 2% <-- add to 100%
// 		these numbers are not really indicative of likelihood overall.
// 		you could include the likelihood of finding the byte at all, that would be indicative of something.
// 		and then we start opening general probability, which is fun, but maybe nonrelevent.
// 		i think we want we want is the highest total probability of the system.
// 	probably not a likely chance, what is proper way to combine?
// 		ummmm probabilty&statistics ....
// 		ummmm ....
//
// 		in all cases this is 80% true
// 		in all cases this is 90% true
// 		in all cases this is 4% true
// 		is this bayesian inference?
// 			P(H|E) = P(E|H)*P(H)/P(E)
//	i think what we want is the highest total probability of the system.
//	we have many parts we can observe, and we can relate their likelihood.
//	if we're only considering next-guesses, we pick the next-guess that has the highest number.
//		how likely is it that all these rare things occur at once?
//		the product of their individual occurrences.
//			we might want to shift what we are working with
//			we could backtrack, or we could rewrite our start while keeping our end.
//	keep track of all your parts-likelihoods.
//	you want to tune your part to have higher likelihood.
//	so you will not change your leaf link.  that's your current best-guess.
//	but you'll like change your second-to-leaf link.  to see if you can find a better guess.
//	you may also want to change your root link, or your parts links.
// 				

template <typename SBC>
void sbc_update_axis(SBC * sbc, typename SBC::option_t option1, typename SBC::option_t option2, size_t axis)
{
	sbc->totals.data[option1] ++;
	//sbc->totals.data[option2] ++; // commented out because this function is usually called twice for each value
	
	//sbc->directions.data[axis*2+0].totals[option1] ++; // TODO: move totals over here if you want >1 axes to be easier
	//sbc->directions.data[axis*2+1].totals[option2] ++;

	sbc->directions.data[axis*2+0].count[option1].data[option2] ++;
	sbc->directions.data[axis*2+1].count[option2].data[option1] ++;

		// when i worry about totals, i am thinking about chance
		// so i want the total for option1 to be the sum of its data.
}
template <typename SBC, typename Source>
void sbc_update_block(SBC * sbc, vector_t<Source> & block, size_t axis)
{
	// for each optio , accumulate events of it being
	// before or after other options.
	// this means each frequency entry in the table is a count.
	
	for (size_t i = 1; i < block.size; ++ i) {
		sbc_update_axis(sbc, block.data[i - 1], block.data[i], axis);
	}
}

template <typename SBC>
void sbc_generate_priority_row(SBC * sbc, size_t axis_direction, typename SBC::option_t option)
{
	auto occurrences = sbc->totals.data[option];
	if (!occurrences) { return; }

	auto & direction = sbc->directions.data[axis_direction];
	auto & guess = direction.guess[option];
	auto & chance = direction.chance[option];
	auto & count = direction.count[option];

	typename SBC::frequency_t total = 0;
	for (size_t a = 0; a < sbc->options; ++ a) {
		guess.data[a] = a;
		chance.data[a] = count.data[a];
		total += count.data[a];
	}
	assert(total == sbc->totals.data[option]);

	vector_sort(chance, &guess, true);

	for (size_t a = 0; a < sbc->options; ++ a) {
		if (!chance.data[a]) {
			if (a >= sbc->maximum_guesses) {
				assert(size_t(a) + 1 < SBC::options);
				sbc->maximum_guesses = a + 1;
			}
			break;
		}
		// attempt to round up likelihood.
		chance.data[a] = (occurrences - 1 + chance.data[a] * fixed_point_unity) / occurrences;
	}
}
template <typename SBC>
void sbc_generate_priority(SBC * sbc)
{
	const auto directions = sbc->directions.size;
	for (size_t d = 0; d < directions; ++ d) {
		for (size_t a = 0; a < sbc->options; ++ a) {
			sbc_generate_priority_row(sbc, d, a);
		}
	}
}
template <typename C, typename F, typename O, size_t o>
void sbc_free(sbc_t<C,F,O,o> * sbc)
{
	for (size_t a = 0; a < sbc->directions.size; ++ a) {
		auto & direction = sbc->directions.data[a];
		for (size_t i = 0; i < o; ++ i) {
			vector_free(direction.count[i]);
			vector_free(direction.chance[i]);
			vector_free(direction.guess[i]);
		}
	}
	vector_free(sbc->totals);
	vector_free(sbc->directions);
	delete sbc;
}

vector_t<uint8_t> memblock_from_range(void * head, void * tail)
{
	return vector_shadow((uint8_t*)head, (uint8_t*)tail - (uint8_t*)head);
}

#define END_TOKEN 256

int main()
{
	// 1. sort functions so we know the bounds of each.
	vector_t<void*> functions = vector_shadow(::functions, sizeof(::functions));
	vector_sort(functions);
	
	// 2. compare each interesting function with each other interesting function, to build statistics
	
	// UPDATE: we are just building them off the whole range now, since the result is the same for the present approach.
	auto * sbc = sbc_init<SBC::count_t, SBC::frequency_t, SBC::option_t, 257>(1);

	vector_t<vector_t<uint8_t>> function_blocks = vector_init<vector_t<uint8_t>>(functions.size - 1);
	vector_t<uint8_t> * shortest_function = 0;
	for (size_t i = 1; i < functions.size; ++ i) {
		function_blocks.data[i] = memblock_from_range(functions.data[i-1], functions.data[i]);
		if (function_blocks.data[i].size) {
			if (!shortest_function) {
				shortest_function = &function_blocks.data[i];
			}
			if (function_blocks.data[i].size < shortest_function->size) {
				shortest_function = &function_blocks.data[i];
			}
		}
	}

	sbc_update_block(sbc, *shortest_function, 0);
	sbc_update_axis(sbc, END_TOKEN, vector_front(*shortest_function), 0);
	sbc_update_axis(sbc, vector_back(*shortest_function), END_TOKEN, 0);

	sbc_generate_priority(sbc);

	///*
	// TESTING -> passed

	printf("Sorting ...\n");
	for (size_t a = 0; a < SBC::options; ++ a) {
		//printf("%02x -> ", sbc->directions.data[1].guess[a].data[0]);
		printf("%02x:", a);
		size_t b = 0;
		for (; b < SBC::options && sbc->directions.data[0].chance[a].data[b]; ++ b) {
			printf(" %02x", sbc->directions.data[0].guess[a].data[b]);
		}
		//for (int j = 0; b < SBC::options && j < 3; ++ b, ++ j) {
		//	printf(" (%02x)", sbc->next_priority[a*256+b]);
		//}
		printf("\n");
	}
	// TESTING
	//*/
	
	// 2b. use the statistics to generate a function?
	
	///*
	// TESTING
	printf("goal:");
	for (size_t i = 0; i < shortest_function->size; ++ i) {
		printf(" %02x", shortest_function->data[i]);
	}
	printf("\n");
	// TESTING
	//*/
	auto function_converted = vector_copy<SBC::option_t>(*shortest_function);
	try_to_reach(sbc, function_converted);
	vector_free(function_converted);

	// 3. use the statistics to morph a function into another <-- the reason for this is to pick nearby functions that
	// 							      can shrink the relevence space.
	
	// 4. discern how much data is needed for #3 to succeed rapidly, and present the missing space to the user.
	
	// tail
	sbc_free(sbc);
	vector_free(functions);
	return 0;
}

template <typename Value>
vector_t<Value> vector_init(size_t size)
{
	vector_t<Value> vector;
	vector.size = 0;
	vector.allocated = 16;
	vector.data = new Value[vector.allocated];
	vector_resize(vector, size);
	return vector;
}
template <typename Value>
vector_t<Value> vector_shadow(Value * array, size_t bytes)
{
	vector_t<Value> vector;
	vector.size = bytes / sizeof(Value);
	vector.allocated = 0;
	vector.data = array;
	return vector;
}
template <typename Value, typename OriginalValue>
vector_t<Value> vector_copy(vector_t<OriginalValue> const & original)
{
	vector_t<Value> copy;
	copy.size = original.size;
	if (original.allocated) {
		copy.allocated = original.allocated;
	} else if (original.size) {
		copy.allocated = original.size;
	} else {
		copy.allocated = 16;
	}
	copy.data = new Value[copy.allocated];
	for (size_t i = 0; i < original.size; ++ i) {
		copy.data[i] = original.data[i];
	}
	return copy;
}
template <typename Value>
void vector_set(vector_t<Value> & vector, Value value)
{
	for (size_t i = 0; i < vector.size; ++ i) {
		vector.data[i] = value;
	}
}
template <typename Value>
void vector_resize(vector_t<Value> & vector, size_t size)
{
	assert(vector.allocated);

	size_t allocated = vector.allocated;
	if (allocated < size) {
		do {
			allocated *= 2;
		} while (allocated < size);
		Value * new_data = new Value[allocated];
	
		for (size_t i = 0; i < vector.size; ++ i) {
			new_data[i] = vector.data[i];
		}
		delete [] vector.data;

		vector.data = new_data;
		vector.allocated = allocated;
	}
	vector.size = size;
}
template <typename Value>
void vector_free(vector_t<Value> & vector) {
	if (vector.allocated) {
		delete [] vector.data;
	}
	vector.allocated = 0;
	vector.size = 0;
}

template <typename Value>
void vector_swap(vector_t<Value> & vector, size_t index1, size_t index2)
{
	Value temp = vector.data[index1];
	vector.data[index1] = vector.data[index2];
	vector.data[index2] = temp;
}

template <typename Value>
Value & vector_front(vector_t<Value> & vector)
{
	assert(vector.size);
	return vector.data[0];
}

template <typename Value>
Value & vector_back(vector_t<Value> & vector)
{
	assert(vector.size);
	return vector.data[vector.size - 1];
}

template <typename Value, typename Index>
void vector_sort(vector_t<Value> & values, vector_t<Index> * indices, bool reverse)
{
	if (indices) { assert( values.size == indices->size); }
	size_t index = 1;
	while (index != values.size) {
		if ((!reverse && values.data[index - 1] > values.data[index]) ||
		    (reverse && values.data[index - 1] < values.data[index])) {
			vector_swap(values, index, index - 1);
			if (indices) {
				vector_swap(*indices, index, index - 1);
			}
			if (index > 1) {
				-- index;
				continue;
			}
		}
		++ index;
	}
}


// does transparency group see the habit we have to call-out-to-mind-control-dictatorship-leader-with-matching-symbols?
// okay.  we will try to build take-work-offline.
// we are now offline.
// if we want our work not to go online when karl is in a disparate state of mind, we should move the repo out of the care repo.
// the repo is now out of the online repo.  changes are being stored to Karl's 2020 laptop.

template <typename Frequency, typename Work, typename State>
struct step_t
{
	using function_t = Frequency(*)(Work & work, State & state, bool undo, bool peek, bool init);
	State state;
	Frequency chance;
	function_t function;
};

template <typename Frequency, typename Work, typename State>
struct plan_t
{
	Work work;
	vector_t<step_t<Frequency, Work, State>> steps;
	Frequency chance;
};

template <typename Frequency, typename Work, typename State>
plan_t<Frequency,Work,State> plan_init(Work work)
{
	plan_t<Frequency,Work,State> plan;
	plan.work = work;
	plan.steps = vector_init<step_t<Frequency, Work, State>>(0);
	plan.chance = fixed_point_unity;
	return plan;
}
template <typename Frequency, typename Work, typename State>
void plan_free(plan_t<Frequency,Work,State> & plan)
{
	work_free(plan.work);
	vector_free(plan.steps);
	plan.chance = 0;
}
template <typename Frequency, typename Work, typename State>
plan_t<Frequency,Work,State> plan_copy(plan_t<Frequency,Work,State> const & original)
{
	plan_t<Frequency,Work,State> copy;
	copy.work = work_copy(original.work);
	copy.steps = vector_copy(original.steps);
	copy.chance = original.chance;
	return copy;
}

template <typename Frequency, typename Work, typename State>
step_t<Frequency,Work,State> step_init(plan_t<Frequency,Work,State> & plan, typename step_t<Frequency,Work,State>::function_t function);

template <typename SBC>
struct sbc_state_t
{
	SBC * sbc;
	typename SBC::option_t next_try;
	size_t focus;
};
template <typename SBC>
struct sbc_work_t
{
	vector_t<typename SBC::option_t> data;
	SBC * sbc;
};
template <typename SBC>
sbc_work_t<SBC> sbc_work_init(SBC * sbc, size_t length)
{
	sbc_work_t<SBC> work;
	work.data = vector_init<typename SBC::option_t>(length);
	work.sbc = sbc;
	return work;
}
template <typename SBC>
sbc_work_t<SBC> work_copy(sbc_work_t<SBC> const & original)
{
	sbc_work_t<SBC> copy;
	copy.data = vector_copy(original.data);
	copy.sbc = original.sbc;
	return copy;
}
template <typename SBC>
void work_free(sbc_work_t<SBC> & work)
{
	vector_free(work->data);
	work->sbc = 0;
}
template <typename SBC>
using sbc_step_t = step_t<typename SBC::frequency_t, sbc_work_t<SBC>, sbc_state_t<SBC>>;
template <typename SBC>
using sbc_plan_t = plan_t<typename SBC::frequency_t, sbc_work_t<SBC>, sbc_state_t<SBC>>;

template <typename SBC>
sbc_step_t<SBC> & step_init(sbc_plan_t<SBC> & plan, typename sbc_step_t<SBC>::function_t function)
{
	vector_resize(plan.steps, plan.steps.size + 1);
	sbc_step_t<SBC> & step = vector_back(plan.steps);
	step.function = function;
	if (plan.steps.size > 1) {
		auto  last_step = plan.steps.data[plan.steps.size - 2];
		step.chance = last_step.chance;
		step.state.sbc = last_step.state.sbc;
		step.state.focus = last_step.state.focus + 1;
	} else {
		step.chance = plan.chance;
		step.state.sbc = plan.work.sbc;
		step.state.focus = 0;
	}
	function(plan.work, step.state, false, true, true);
	return step;
}
template <typename Frequency, typename Work, typename State>
Frequency step_try(plan_t<Frequency, Work, State> & plan, bool undo, bool peek)
{
	assert(plan.steps.size);
	auto & step = vector_back(plan.steps);
	return step.function(plan.work, step.state, undo, peek, false);
}

template <typename SBC>
typename SBC::frequency_t try_next_byte(sbc_work_t<SBC> & work, sbc_state_t<SBC> & state, bool undo, bool peek, bool init)
{
	if (init) {
		state.next_try = 0;
		return 0;
	}

	if (state.next_try >= state.sbc->maximum_guesses) {
		return 0;
	}

	if (work.data.size <= state.focus) {
		vector_resize(work.data, state.focus + 1);
	}

	if (!undo) {
		typename SBC::option_t last_value;
		if (state.focus == 0) {
			last_value = END_TOKEN;
		} else {
			last_value = work.data.data[state.focus-1];
		}
		auto & direction = state.sbc->directions.data[0];
		typename SBC::frequency_t chance = direction.chance[last_value].data[state.next_try];
		if (!peek) {
			work.data.data[state.focus] = direction.guess[last_value].data[state.next_try];
			++ state.next_try;
		}
		return chance;
	} else {
		// undo
		assert(false);
	}
}

// GOAL: build an SBC out of what choices in the SBC work to reach all the functions.

	// for now, we are starting 'from the left'.  growing an array until it reaches the size of the function, as the same as the function.
	// it looks pretty likely this will be generalizable to more if needed.
		// we only have a few functions, so this SBC won't be very big.

// each entry in this sbc is an entry in the table of the other sbc
// that means it includes prior byte, and next decision.
// 	note that each decision is paired with a byte.
// 		let's try considering just prior byte, for now.

// so, for 'prior' information, we'll use the previous byte,
// and for 'next' information, we'll use the sbc selection.
//
// 	it sounds like we may want to try different things
// 	for 'next' and 'prior'.
// 		we're roughly forming an exhaustive mapping from
// 		information-from-state to
// 		decision-making-material.
// 	can we separate that out in the 'class', for clarity?

// so SBC can help us inform decisions.  we can use it in many ways.  we may want to normalize that so it can decide how it is used.

// decisions will be mapped to integers, and so will summaries.

// we have a space of trying to build a buffer from the start.
	// we have an SBC of likely bytes following others that can start things.
	// we'll want to form an integer mapping of information about the goal, eventually.  for now we just care that it's a function in this program.
	
	// integer things:
	// 	- which priority is picked
	// 	- what byte came before
	// 	- what byte we pick
	// 	- what priority was picked before
	// 	- how many spots back into history we are looking
	//
	// 	- an index into a table of sbcs used

// same as Step.
enum class Property {
	value,
	priority_depth,
	reference_depth,
	reference_category,
};

/*
 * if you wanted to have this drive behavior, you could number lists of things to do.
 * i guess this is similar to reference_category
 */
// please integrate contexts
/* step, then
 * 	previous step.
 * 	values vector used.
 * 	spot used in values vector.
 * 	property selected. [a mutator, a behavior]
 */


// when we look at a reference, we are looking at previous values or previous behaviors.
// behaviors are what now?
// 	they are instances of Property in context.  this means summarizing our context?
// 		we'll want enough properties to make our historical context out of them.
// 		so which-property-is-chosen is one of them.
// 			hmm different groups

enum class SBCType {
	value,
	value_priority,
	step,
};

template <typename Value>
struct Work/*?*/ {
	vector_t<Value> value_history; // this is an sbc output, so it is stored.  different sbcs might output this.

	// propose store other sbc outputs.  likely one will merge with step_history.
	
	vector_t<Value> step_history; // do not make an SBC for this yet.  we need to find proper relevence-path.
};

// so then we have depth-in-value-history as a possible input for an sbc.
// we have value as a possible output
//
// missing bit: priority of sbc.  we can find correct priority quick.  this can train an sbc.

// a function is a context and there is only 1 function.

// so, each set of properties spawns from the core set.
// they each have a parent set.
// so we'll want to make a vector-hierarchy that can allocate new parts.
// might be nice to associate a name with some of them.

vector_t<char *> property_names;

// we have many vectors, notable history vectors.
// this gives us properties for finding values.
// context
// vector-id
// vector-depth
// output-id
// map-priority

template <typename Value>
struct context_t
{
	vector_t<Value> inputs;
	vector_t<Value> outputs;
};

template <typename Value>
struct guess_goal_t
{
	context_t<Value> * context;
	size_t output;
};

// okay, we have little bitty pair-bits that want more values.
// history-lookup needs a property-id, and a depth
// sbc-maps need a priority, and a value that matches their type


// guess-way-selection needs: input-and-output-meaning

// sbc-using-meaning: input-and-output-meaning
// sbcs make guesses
// each sbc is a guess-way for its input-and-output-meaning.  there is only one for each for now.  it is just a 2d array of input-output-combo-being-right-frequency.  can return highest-frequency for input or output, or second-highest, etc.
// 	we made one for next-byte, given working-code.  it is fast to make.  planning how-to-use.
// 	we-have-not-yet-used-it-to-exhaustively-make-code.
// 		part of the issue was not knowing the size.  the bigger part was having to inform the decision-making spread.
// 		we've had some confusion around switching paths of exploration, and there was no ready clue regarding when.

// sbc-using needs input-meaning, guess-depth, and output-meaning
// sbc-map-getting needs an sbc-context, which is input-meaning and output-meaning
// sbc-maps need a priority and a value.

template <typename Value>
struct property_map_context_t
{
	context_t<Value> * context;
	vector_t<Value> inputs; // 0: priority, 1: map-input
	size_t output;
	sbc_t<unsigned long, unsigned long, Value, 256> * map; // <-- using a map takes a priority input.

	property_map_context_t<Value> * parent;
	// each index will be a combination of an input with a guessed output
	// we'll allocate new submaps when we want to try a new combination.
	vector_t<char *> names;
	vector_t<property_map_context_t<Value>> submaps;
};

#if 0
template <typename Work, typename Advice>
void core/*?*/(Work & work, Advice & advice, Advice prev_to_next_sbc)
{
	// assume we have a previous value.
	
	// property VALUE

	unsigned long priority = 0;
	unsigned long property_kind = VALUE;
	//attribute_vector_kind = HISTORY * num_value_kinds // may always be the same


	// after we pick property_kind = value, we'll want to use an SBC to pick the depth.
	if (!work.depth_guess) {
		// call self to guess depth for this.
			// idea 1: ongoing set of needs, fill them all in
			// idea 2: concept of a property_kind that is needed
			// idea 3: concept of a state, to spew things out
		// we need a property=DEPTH for property=VALUE

		// each behavior uses some properties.  propose if recursion is too deep we use 50% randomness to guess.
	}
	
	unsigned long history_depth = 0;
	unsigned long history_index = property_kind;
	vector_t<unsigned long>& vector = work.history[property_kind];

	auto & sbc = prev_to_next_sbc // <- this sbc is for taking from depth=1 from VALUE history, and providing VALUE

	unsigned long value = vector[vector.size - 1 - vector_depth];
	unsigned long guess = sbc.get(value, priority);

	answer = advice.get_value(work.value_history.size);
	correct_priority = prev_to_next_sbc.inverse(last_value, answer);

	// we might want an abstract history of planning values.
	// there is also an idea of categorizing or typing things that hasn't been expanded, useful for storing/finding sbcs
}
#endif

// okay, so sbcs expand backwards from needs and we will be solving that space.
// we need a whole vector of bytes that is the function in question.
// 	we might need a location in the vector to write to.
// 	or the size of the vector.
// we need 1 byte to place next in the vector. <- for now we need this.  size is known.
// 		we need an sbc to use, a value to put into the sbc, and what priority to select from the sbc.

// there is a good strategy that works with current sbc if test-for-success is available.
// it is to walk the priorities until test is passed.

// so, a way to meet the need for the priority selection, assuming present test,
// is to act on history of priority selection, picking the next one.
// 	this would be a very simple sbc that adds one to its input if the input is last-selection.

// if we could set up above as framework, it would help confidence and clarity.

// okay hmm
// we have a vector of placed-bytes.
// we learn by success and failure to place bytes based on previous-placed-byte.
// then we pull out the hardcoded things and turn them into properties and behavior-choices to learn better.
// on the second run, we don't fail as much.  once enough things are turned into, we don't fail at all.
	// is likely helpful to keep the idea of step-history.  this makes sure behaviors are enumerable.
	// we also value general history-vectors.

// we do a step.
// we might place-a-byte using a next-byte sbc, based on a previous byte.
// 	the following is a decision:
// 			^-- this means we are property=value as input, and 0 steps into its backlog, and 0 steps into sbc priorities.
// 				the sbc is selected based on property=value.
// 	what-property-to-use
// 	how-many-steps-into-backlog
// 	how-many-steps-into-sbc-priorities
// 		it fuels another decision:
// 	what-value-to-place

// that looks like enough stuff.
// so we'll want to take our aspects of state, and put them into an array, so they can be chosen with an sbc that decides what kind of sbc to use.  no templates needed.

// this generalizes step quite a bit.  and the steps might become the work.
// 	-> we need a byte selection to act.
// 	we can wait until one is made, and act then.

template <typename SBC>
void try_to_reach(SBC * sbc, vector_t<typename SBC::option_t> & target)
{
	auto plans = vector_init<sbc_plan_t<SBC>>(1);

	vector_front(plans) = plan_init<typename SBC::frequency_t,sbc_work_t<SBC>,sbc_state_t<SBC>>(sbc_work_init(sbc, 0));

	// guesses are now parallel to chances, which are fixed-point integers where fixed_point_unity = 1.0

	// ==============================================
	// ...
	// we start off with 1 step prepared in the plan.
	// we get 2 guesses from the step.  both of these should be placed on plans, with updated chances.
	// so now we have 2 plans.  we find the plan with the highest chance, and get 2 guesses from it again.
	// ==============================================
	// update below to do above.

	size_t first_error = 0;
	size_t last_adjustment = target.size;

	while (true) {
		// 1. find the best available guess
		size_t best_plan = 0;
		for (size_t i = 1; i < plans.size; ++ i) {
			if (plans.data[i].chance > plans.data[best_plan].chance) {
				best_plan = i;
			}
		}

		bool keep_going = true;

		if (plans.data[best_plan].work.data.size > target.size) {
			// we've gotten long enough
			if (vector_back(plans.data[best_plan].work.data) != END_TOKEN) {
				plans.data[best_plan].chance = 0;
				//printf("Missed plan\n");
				keep_going = false;
			} else for (size_t i = 0; i < target.size; ++ i) {
				if (plans.data[best_plan].work.data.data[i] != target.data[i]) {
					plans.data[best_plan].chance = 0;
					//printf("Missed plan\n");
					keep_going = false;
				}
			}
			if (keep_going) {
				printf("Success\n");
				// success
				return;
			}
		}

		if (keep_going) {
			step_init(plans.data[best_plan], try_next_byte);
		}

		while (true) { // this no longer does 2.  it just gets a guess, which is also written to the data, and checks if it's ok.
			// 2. make two guesses that assume it, spawning 1 new plan
			// 	if a guess makes a wrong result, stop guessing there.

			// 2 a -> call step_try to fill this data
			auto chance1 = step_try(plans.data[best_plan], false, false);// * plans.data[best_plan].chance/ fixed_point_unity;

			if (vector_back(plans.data[best_plan].steps).state.focus == first_error && plans.data[best_plan].work.data.data[first_error] == target.data[first_error]) {
				printf("Got byte %i right\n", first_error);
				first_error ++;
				last_adjustment = target.size;
			}
			if (chance1) { break; }

			/*
			 * I think we could try walking back along our steps until we find the alternate path with the highest chance,
			 * for now.
			 * This would mean each step holding accumulated chance.
			 *
			 * Alternatively we could track the second-best option as we go.
			 */
			// let's just undo back for now
			// up the step index, and then resize steps down when it reaches 0 chance.
			assert(plans.data[best_plan].steps.size);
			//printf("Backtracking.\n");
			vector_resize(plans.data[best_plan].work.data, plans.data[best_plan].work.data.size - 1);
			vector_resize(plans.data[best_plan].steps, plans.data[best_plan].steps.size - 1);
			assert(plans.data[best_plan].work.data.size == vector_back(plans.data[best_plan].steps).state.focus + 1);
			if (plans.data[best_plan].work.data.size < last_adjustment) {
				last_adjustment = plans.data[best_plan].work.data.size;
				printf("work:");
				for (size_t i = 0; i < plans.data[best_plan].work.data.size; ++ i) {
					printf(" %02x", plans.data[best_plan].work.data.data[i]);
				}
				printf("\n");
			}

			// chance1 == 0, therefore
			// can't make any more guesses here
			
			/*
			// 2 b -> copy the plan
			size_t second_best_plan = plans.size;
			vector_resize(plans, plans.size + 1); // <- this invalidates best_plan.
			plans.data[second_best_plan] = plan_copy(plans.data[best_plan]);
			// 2 c -> call step_try to fill the other data
			auto chance2 = plans.data[second_best_plan].chance * step_try(plans.data[second_best_plan], false) / fixed_point_unity;
			// 2 d -> call step_init to make further steps
			// */
			//plans.data[best_plan].chance = chance1;
			/*
			plans.data[second_best_plan].chance = chance2;

			// 3. repeat
			*/
		};
		continue;
//failure:
		
		/*
failed:

		// we failed.  but that was our best guess.  we look for a better guess.
		// we'd better clone the plan and remove some work.
		// *chance contains our total chance of this being a good thing based on what we know.
		// when we have only a partial plan, our chance will be bigger.
		// when we pick a spot to add back onto it, the chance will drop again.
		// why don't we go back far enough, that the chance doesn't drop to below this chance, and stop.
		plans.resize(plans.size + 1);
		//chances.resize(chances.size + 1);
			// alternatively, we could keep a list of alternate approaches, and always pick the one with the
			// highest chance.  this might work out better with this approach.
			// so, when we take a next step, we pick from among all the branches, the one with the best chance.
			// that is: we pursue the branch that has the highest chance.
		auto * plan = & plans.front();
		plan->work.sbc = sbc;
		plan->steps = vector_init<decltype(plan->steps)::value_t>(0);
		auto * chance = & chances.front();
		*chance = fixed_point_unity;

		// we failed.
		// we're going to want to undo this step.
		// let's plan tree expansion.
		//
		//
		// 1 1 1 1 1 [leaf turns out to be wrong]
		//     undo up until best alternative guess.
		//     use probability to do this.
		// 1 1 1 1 2 [this is acceptable but kinda cheating, turns out to be wrong]
		// 1 1 1 2 1 [this is next approach]
		// 1 1 1 2 2 
		// 1 1 2 1 1 
		// eventually we do
		// 2 2 2 2 2 <- after this, what next?
		// 	there are too many parallel branches to store them all?
		// 		to a depth of 2 it's reasonable.  space is 2^size
		// 1 1 1 1 3 <- this is what makes sense.  once the structure for this is in place we can try more likely things.
		//
		// OR if we used chance, we could pursue branch with most likelihood that hasn't failed.
		// undoing would still be okay.
		// 
		// */
	}

	// for now, it doesn't matter, it's just where it checks
	// we have to decide when to test.
	// option 1, test every byte.  easy to get there.
	// 	this opens a space for learning what is good.
	// option 2, don't test until length is right.
	// 	this is kinda contrived but tests the
	// 	speed of the approach.
}





// ---
// for proper progress success, kinda,
// let's make the code that generates itself, using the sbcs.
// let's only add meta-sbc if it is too slow.
// we will want to pick first guesses.  once all our first guesses are known to be failures, we will pick the most-likely
// second guess.
// 		this means accumulating byte totals so that chances can be compared.
// 	[it would be nice if 'END-NOW' could be a guess, but seeing the work-space propose ignore this for now.]
// ---
// there is worry the process won't close in reasonable time.
// a quick way to fix that would be by giving it hints from solution.
// the original plan was to make code that morphs from one thing into another, and examine the data needed to morph,
// to see what patterns could be used to learn better with.
// 	our approach would inform sbcs with hints, using further sbcs.
// 	note: morphing is more human, and is doable with very similar work to present work.
// 	consider focus-direction selection.
// 		yes that merges with hints a little
// ---





// logic flow error

// => fix try_to_reach as below

// 1. make a new step
// 2. try the next guess on the newest step
// 3. if guess is bad, do what?
//		^-- this is actually checked prior to 1
// 		FIX: if guess is bad, retry newest step until it has no more guesses.
// 		FIX: this means goto 2
// 3. if can't make any more guesses, do what?
// 		FIX: if can't make any more guesses, go back to previous step and try better guess.
// 		FIX: this means erase last step and goto 2
// 			since its actually the 2 block (which coincidentally is also called 2.) that discerns this,
// 			i turned it into a loop that checks, and moved the backtrack code inside it.
// 4. if 3, erase last step <- FIX only erase for 3b
// 5. goto 1 <-- FIX we should only go to 1 if our guess was good.

// this is hard to sustain clarity around. [might consider raising the priority of more central work.]














// NOTE: sbc is a summarization.
// it could even be applied to itself.
// it could be used to infer good decisions, if it is known
// about the result.

