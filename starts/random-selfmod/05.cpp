
// to manage function ordering, we don't use libraries and headers atm.

//using size_t = unsigned long;
using uint8_t = unsigned char;

#define NDEBUG
// FOR TESTING
#undef NDEBUG
#include <assert.h>
#include <stdio.h>
// FOR TESTING

int main();

template <typename Value>
struct vector_t;

template <typename Value> vector_t<Value> vector_init(size_t size = 0);
template <typename Value> vector_t<Value> vector_shadow(Value * array, size_t bytes);
template <typename Value> void vector_set(vector_t<Value> & vector, Value value);
template <typename Value> void vector_resize(vector_t<Value> & vector, size_t size);
template <typename Value> void vector_swap(vector_t<Value> & vector, size_t index1, size_t index2);
template <typename Value> Value & vector_front(vector_t<Value> & vector);
template <typename Value> Value & vector_back(vector_t<Value> & vector);
template <typename Value> void vector_free(vector_t<Value> & vector);
template <typename Value, typename Index = size_t>
void vector_sort(vector_t<Value> & vector, vector_t<Index> * indices = 0, bool reverse = false);

template <typename Frequency, typename Option, size_t options>
struct sbc_t;

template <typename F, typename O, size_t o>
sbc_t<F,O,o> * sbc_init();
template <typename F, typename O, size_t o>
void sbc_update_pair(sbc_t<F,O,o> * sbc, O option1, O option2);
template <typename F, typename O, size_t o>
void sbc_update_block(sbc_t<F,O,o> * sbc, vector_t<O> & block);
//template <typename F, typename O, size_t o>
//void sbc_update(sbc_t<F,O,o> * sbc, vector_t<void*> const & sorted_functions);
template <typename F, typename O, size_t o>
void sbc_generate_priority_row(sbc_t<F,O,o> * sbc, vector_t<F> & value_row, vector_t<O> & index_row);
template <typename F, typename O, size_t o>
void sbc_generate_priority(sbc_t<F,O,o> * sbc);
template <typename F, typename O, size_t o>
void sbc_free(sbc_t<F,O,o> * sbc);

vector_t<uint8_t> memblock_from_range(void * head, void * tail);

void * functions[]={
	(void*)main,

	(void*)vector_init<unsigned long>,
	(void*)vector_shadow<unsigned long>,
	(void*)vector_resize<unsigned long>,
	(void*)vector_swap<unsigned long>,
	(void*)vector_front<unsigned long>,
	(void*)vector_back<unsigned long>,
	(void*)vector_free<unsigned long>,
	(void*)vector_sort<unsigned long>,

	(void*)vector_init<void *>,
	(void*)vector_shadow<void *>,
	(void*)vector_resize<void *>,
	(void*)vector_swap<void *>,
	(void*)vector_front<uint8_t>,
	(void*)vector_back<uint8_t>,
	(void*)vector_free<void *>,
	(void*)vector_sort<void *>,

	(void*)vector_init<uint8_t>,
	(void*)vector_shadow<uint8_t>,
	(void*)vector_resize<uint8_t>,
	(void*)vector_swap<uint8_t>,
	(void*)vector_front<uint8_t>,
	(void*)vector_back<uint8_t>,
	(void*)vector_free<uint8_t>,
	(void*)vector_sort<uint8_t>,

	(void*)sbc_init<unsigned long,uint8_t,256>,
	(void*)sbc_update_pair<unsigned long,uint8_t,256>,
	(void*)sbc_update_block<unsigned long,uint8_t,256>,
	//(void*)sbc_update<unsigned long,uint8_t,256>,
	(void*)sbc_generate_priority_row<unsigned long,uint8_t,256>,
	(void*)sbc_generate_priority<unsigned long,uint8_t,256>,
	(void*)sbc_free<unsigned long,uint8_t,256>,

	(void*)memblock_from_range//,
//	(void*)printf
};

template <typename Value>
struct vector_t
{
	Value * data;
	size_t size;
	size_t allocated;
};

// sequential-behavior-chance, growing slowly
template <typename Frequency, typename Option, size_t options>
struct sbc_t {
	using frequency_t = Frequency;
	using option_t = Option;
	using index_t = size_t;
	vector_t<Frequency> next_table[options];
	vector_t<Frequency> prev_table[options];
	vector_t<Option> next_prio[options];
	vector_t<Option> prev_prio[options];
};

template <typename Frequency, typename Option, size_t options>
sbc_t<Frequency,Option,options> * sbc_init()
{
	sbc_t<Frequency,Option,options> * sbc = new sbc_t<Frequency,Option,options>();
	for (typename sbc_t<Frequency,Option,options>::index_t i = 0; i < options; ++ i) {
		sbc->next_table[i] = vector_init<Frequency>(options);
		sbc->prev_table[i] = vector_init<Frequency>(options);
		sbc->next_prio[i] = vector_init<Option>(options);
		sbc->prev_prio[i] = vector_init<Option>(options);
		vector_set<Frequency>(sbc->next_table[i], 0);
		vector_set<Frequency>(sbc->prev_table[i], 0);
		vector_set<Option>(sbc->next_prio[i], 0);
		vector_set<Option>(sbc->prev_prio[i], 0);
	}
	return sbc;
};
template <typename F, typename O, size_t o>
void sbc_update_pair(sbc_t<F,O,o> * sbc, O option1, O option2)
{
	sbc->next_table[option1].data[option2] ++;
	sbc->prev_table[option2].data[option1] ++;
}
template <typename F, typename O, size_t o>
void sbc_update_block(sbc_t<F,O,o> * sbc, vector_t<O> & block)
{
	// for each optio , accumulate events of it being
	// before or after other options.
	// this means each frequency entry in the table is a count.
	
	for (size_t i = 1; i < block.size; ++ i) {
		sbc_update_pair(sbc, block.data[i - 1], block.data[i]);
	}
}
/*
template <typename F, typename O, size_t o>
void sbc_update(sbc_t * sbc, vector_t<O*> const & sorted_functions)
{
	for (size_t i = 1; i < sorted_functions.size; ++ i) {
		vector_t<O> block = memblock_from_range(sorted_functions.data[i-1], sorted_functions.data);
		sbc_update_block(sbc, block);
	}
}*/
template <typename F, typename O, size_t o>
void sbc_generate_priority_row(sbc_t<F,O,o> * sbc, vector_t<F> & value_row, vector_t<O> & index_row)
{
	for (size_t a = 0; a < o; ++ a) {
		index_row.data[a] = a;
	}
	for (size_t a = 0; a < o; ++ a) {
		vector_sort(value_row, &index_row, true);
	}
}
template <typename F, typename O, size_t o>
void sbc_generate_priority(sbc_t<F,O,o> * sbc)
{
	for (size_t a = 0; a < o; ++ a) {
		sbc_generate_priority_row(sbc, sbc->next_table[a], sbc->next_prio[a]);
		sbc_generate_priority_row(sbc, sbc->prev_table[a], sbc->prev_prio[a]);
	}
}
template <typename F, typename O, size_t o>
void sbc_free(sbc_t<F,O,o> * sbc)
{
	for (typename sbc_t<F,O,o>::index_t i = 0; i < o; ++ i) {
		vector_free(sbc->next_table[i]);
		vector_free(sbc->prev_table[i]);
		vector_free(sbc->next_prio[i]);
		vector_free(sbc->prev_prio[i]);
	}
	delete sbc;
}

vector_t<uint8_t> memblock_from_range(void * head, void * tail)
{
	return vector_shadow((uint8_t*)head, (uint8_t*)tail - (uint8_t*)head);
}

int main()
{
	// 1. sort functions so we know the bounds of each.
	vector_t<void*> functions = vector_shadow(::functions, sizeof(::functions));
	vector_sort(functions);
	//sortVector(functions, sizeof(functions));
	
	// 2. compare each interesting function with each other interesting function, to build statistics
	
	// UPDATE: we are just building them off the whole range now, since the result is the same for the present approach.
	auto * sbc = sbc_init<unsigned long, uint8_t, 256>();

	//sbc_update(sbc, functions);
	vector_t<uint8_t> block = memblock_from_range(vector_front(functions), vector_back(functions));
	sbc_update_block(sbc, block);

	sbc_generate_priority(sbc);

	///*
	// TESTING -> passed

	printf("Sorting ...\n");
	for (int a = 0; a < 256; ++ a) {
		printf("%02x:", a);
		int b = 0;
		for (; b < 256 && sbc->next_table[a].data[b]; ++ b) {
			printf(" %02x", sbc->next_prio[a].data[b]);
		}
		//for (int j = 0; b < 256 && j < 3; ++ b, ++ j) {
		//	printf(" (%02x)", sbc->next_prio[a*256+b]);
		//}
		printf("\n");
	}
	// TESTING
	//*/
	
	// 3. use the statistics to morph a function into another
	
	// 4. discern how much data is needed for #3 to succeed rapidly, and present the missing space to the user.
	
	// tail
	sbc_free(sbc);
	vector_free(functions);
	return 0;
}

template <typename Value, typename Index>
void sortVector(Value * values, size_t bytes, Index * indices, bool reverse)
{
	size_t count = bytes / sizeof(decltype(*values));
	size_t index = 1;
	while (index != count) {
		if ((!reverse && values[index - 1] > values[index]) || (reverse && values[index - 1] < values[index])) {
			Value temp;
			temp = values[index];
			values[index] = values[index - 1];
			values[index - 1] = temp;
			if (indices) {
				Index temp;
				temp = indices[index];
				indices[index] = indices[index - 1];
				indices[index - 1] = temp;
			}
			if (index > 1) {
				-- index;
				continue;
			}
		}
		++ index;
	}
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
	while (allocated < size) { allocated *= 2; }
	Value * new_data = new Value[allocated];

	for (size_t i = 0; i < vector.size; ++ i) {
		new_data[i] = vector.data[i];
	}
	delete [] vector.data;

	vector.data = new_data;
	vector.allocated = allocated;
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

template <typename Work, typename State>
struct step_t
{
	using function_t = bool(*)(Work & work, State & state, bool undo, bool init);
	State state;
	function_t function;
};

template <typename Work, typename State>
struct plan_t
{
	Work work;
	vector_t<step_t<Work, State>> steps;
};

template <typename Work, typename State>
step_t<Work,State> step_init(plan_t<Work,State> & plan, typename step_t<Work,State>::function_t function);

template <typename SBC>
struct sbc_state_t
{
	SBC * sbc;
	typename SBC::index_t next_try;
	size_t focus;
};
template <typename SBC>
struct sbc_work_t
{
	vector_t<typename SBC::option_t> data;
	SBC * sbc;
};
template <typename SBC>
using sbc_step_t = step_t<sbc_work_t<SBC>, sbc_state_t<SBC>>;
template <typename SBC>
using sbc_plan_t = plan_t<sbc_work_t<SBC>, sbc_state_t<SBC>>;

template <typename SBC>
sbc_step_t<SBC> & step_init(sbc_plan_t<SBC> & plan, typename sbc_step_t<SBC>::function_t function)
{
	vector_resize(plan.steps, plan.steps.size + 1);
	sbc_step_t<SBC> & step = vector_back(plan.steps);
	step.function = function;
	if (plan.steps.size > 1) {
		auto  last_step = plan.steps.data[plan.steps.size - 2];
		step.state.sbc = last_step.state.sbc;
		step.state.focus = last_step.state.focus ++;
	} else {
		step.state.sbc = plan.work.sbc;
		step.state.focus = 0;
	}
	function(plan.work, step.state, false, true);
	return step;
}
template <typename Work, typename State>
bool step_try(plan_t<Work, State> & plan, bool undo)
{
	assert(plan.steps.size());
	auto & step = vector_back(plan.steps);
	step.function(plan.work, step.state, undo, false);
}

template <typename SBC>
bool try_next_byte(sbc_work_t<SBC> & work, sbc_state_t<SBC> & state, bool undo, bool init)
{
	if (init) {
		state.next_try = 0;
		return true;
	}

	if (work.data.size < state.focus) {
		vector_resize(work.data, state.focus);
	}

	if (!undo) {
		if (state.focus == 0) {
			while (state.sbc->prev_prio[state.next_try].data[0] == 0) {
				++ state.next_try;
				if (state.next_try == 0) {
					return false;
				}
			}
			work.data.data[state.focus] = state.next_try;
			++ state.next_try;
			if (state.next_try == 0) {
				return false;
			}
			while (state.sbc->prev_prio[state.next_try].data[0] == 0) {
				++ state.next_try;
				if (state.next_try == 0) {
					return false;
				}
			}
		} else {
			work.data.data[state.focus] = state.sbc->next_prio[work.data.data[state.focus-1]].data[state.next_try];
			++ state.next_try;
			if (state.next_try == 0) {
				return false;
			}
			if (state.sbc->next_prio[work.data.data[state.focus-1]].data[state.next_try] == 0) {
				state.next_try = 0;
				return false;
			}
		}
		return true;
	} else {
		// undo
		assert(false);
	}
}

template <typename SBC>
void try_to_reach(SBC * sbc, vector_t<typename SBC::option_t> & target)
{
	sbc_plan_t<SBC> plan;
	plan.work.sbc = sbc;

	step_init(plan, try_next_byte);

	while (true) {
		bool success = step_try(plan, false);
		//if (
	}

		// this path doesn't reach very fast.
		// given size of data.
		// it may reach if data is shrunk.
			// it would be better if it
			// were to try the most likely
			// option of all the options.
			// to prefer all steps be low index.

			// let's get to working-point so we
			// can grow with relevence rather
			// than guessing.

		// wait, what about the alternative of learning
		// an effective sbc path?

	// for now, it doesn't matter, it's just where it checks
	// we have to decide when to test.
	// option 1, test every byte.  easy to get there.
	// 	this opens a space for learning what is good.
	// option 2, don't test until length is right.
	// 	this is kinda contrived but tests the
	// 	speed of the approach.
}









// NOTE: sbc is a summarization.
// it could even be applied to itself.
// it could be used to infer good decisions, if it is known
// about the result.
