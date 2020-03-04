
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
template <typename Value> void vector_free(vector_t<Value> & vector);
template <typename Value> void vector_swap(vector_t<Value> & vector, size_t index1, size_t index2);
template <typename Value, typename Index = size_t>
void vector_sort(vector_t<Value> & vector, vector_t<Index> * indices = 0, bool reverse = false);

struct sbc_t;
sbc_t * sbc_init();
void sbc_update_pair(sbc_t * sbc, uint8_t byte1, uint8_t byte2);
void sbc_update_block(sbc_t * sbc, uint8_t* start, uint8_t * tail);
void sbc_update(sbc_t * sbc, vector_t<void*> const & sorted_functions);
void sbc_generate_priority_row(sbc_t * sbc, vector_t<unsigned long> & value_row, vector_t<uint8_t> & index_row);
void sbc_generate_priority(sbc_t * sbc);
void sbc_free(sbc_t * sbc);

void * functions[]={
	(void*)main,

	(void*)vector_init<unsigned long>,
	(void*)vector_shadow<unsigned long>,
	(void*)vector_resize<unsigned long>,
	(void*)vector_free<unsigned long>,
	(void*)vector_swap<unsigned long>,
	(void*)vector_sort<unsigned long>,

	(void*)vector_init<void *>,
	(void*)vector_shadow<void *>,
	(void*)vector_resize<void *>,
	(void*)vector_free<void *>,
	(void*)vector_swap<void *>,
	(void*)vector_sort<void *>,

	(void*)vector_init<uint8_t>,
	(void*)vector_shadow<uint8_t>,
	(void*)vector_resize<uint8_t>,
	(void*)vector_free<uint8_t>,
	(void*)vector_swap<uint8_t>,
	(void*)vector_sort<uint8_t>,

	(void*)sbc_init,
	(void*)sbc_update_pair,
	(void*)sbc_update_block,
	(void*)sbc_update,
	(void*)sbc_generate_priority_row,
	(void*)sbc_generate_priority,
	(void*)sbc_free//,
//	(void*)printf
};

template <typename Value>
struct vector_t
{
	Value * data;
	size_t size;
	size_t allocated;
};

// sequential-byte-chance 1
struct sbc_t {
	vector_t<unsigned long> next_table[256];
	vector_t<unsigned long> prev_table[256];
	vector_t<uint8_t> next_prio[256];
	vector_t<uint8_t> prev_prio[256];
};

sbc_t * sbc_init()
{
	sbc_t * sbc = new sbc_t();
	for (int i = 0; i < 256; ++ i) {
		sbc->next_table[i] = vector_init<unsigned long>(256);
		sbc->prev_table[i] = vector_init<unsigned long>(256);
		sbc->next_prio[i] = vector_init<uint8_t>(256);
		sbc->prev_prio[i] = vector_init<uint8_t>(256);
		vector_set<unsigned long>(sbc->next_table[i], 0);
		vector_set<unsigned long>(sbc->prev_table[i], 0);
		vector_set<uint8_t>(sbc->next_prio[i], 0);
		vector_set<uint8_t>(sbc->prev_prio[i], 0);
	}
	return sbc;
};
void sbc_update_pair(sbc_t * sbc, uint8_t byte1, uint8_t byte2)
{
	sbc->next_table[byte1].data[byte2] ++;
	sbc->prev_table[byte2].data[byte1] ++;
}
void sbc_update_block(sbc_t * sbc, uint8_t* start, uint8_t* tail)
{
	// for each byte, accumulate events of it being
	// before or after other bytes.
	// this means each entry in the table is a count.
	
	uint8_t * iterator = start;
	++ iterator;
	while (iterator != tail) {
		sbc_update_pair(sbc, *(iterator - 1), *iterator);

		++ iterator;
	}
}
void sbc_update(sbc_t * sbc, vector_t<void*> const & sorted_functions)
{
	for (size_t i = 1; i < sorted_functions.size; ++ i) {
		sbc_update_block(sbc, (uint8_t*)sorted_functions.data[i-1], (uint8_t*)sorted_functions.data[i]);
	}
}
void sbc_generate_priority_row(sbc_t * sbc, vector_t<unsigned long> & value_row, vector_t<uint8_t> & index_row)
{
	for (size_t a = 0; a < 256; ++ a) {
		index_row.data[a] = a;
	}
	for (size_t a = 0; a < 256; ++ a) {
		vector_sort(value_row, &index_row, true);
	}
}
void sbc_generate_priority(sbc_t * sbc)
{
	for (size_t a = 0; a < 256; ++ a) {
		sbc_generate_priority_row(sbc, sbc->next_table[a], sbc->next_prio[a]);
		sbc_generate_priority_row(sbc, sbc->prev_table[a], sbc->prev_prio[a]);
	}
}
void sbc_free(sbc_t * sbc)
{
	for (int i = 0; i < 256; ++ i) {
		vector_free(sbc->next_table[i]);
		vector_free(sbc->prev_table[i]);
		vector_free(sbc->next_prio[i]);
		vector_free(sbc->prev_prio[i]);
	}
	delete sbc;
}

int main()
{
	// 1. sort functions so we know the bounds of each.
	vector_t<void*> functions = vector_shadow(::functions, sizeof(::functions));
	vector_sort(functions);
	//sortVector(functions, sizeof(functions));
	
	// 2. compare each interesting function with each other interesting function, to build statistics
	sbc_t * sbc = sbc_init();
	sbc_update(sbc, functions);

	sbc_generate_priority(sbc);

	/*
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

// guessing.
struct guess
{
	vector_t<uint8_t> data;
	vector_t<unsigned long> certainty;
		// propose each entry in certainty links to previous guess-location.  to undo knowing it.
		// TODO LEFT OFF HERE.  Let's implement templates.
	size_t focus;
};
// okay, guess shows our working data.
// we want to be able to undo a guess.  we could turn certainty into a linked-list.

