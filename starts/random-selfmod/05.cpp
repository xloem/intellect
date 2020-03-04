
// to manage function ordering, we don't use libraries and headers atm.

//using size_t = unsigned long;
using uint8_t = unsigned char;
#include <stdio.h>

int main();
void sortPointerVector(void ** vector, size_t bytes);
// NOTE: indexes are uint8_t to make implementation quick
// templates might be better.
void sortUnsignedLongVector(unsigned long * values, size_t bytes, uint8_t * indices = 0, bool reverse = false);

struct sbc_t;
sbc_t * sbc_init();
void sbc_update_pair(sbc_t * sbc, uint8_t byte1, uint8_t byte2);
void sbc_update_block(sbc_t * sbc, uint8_t* start, uint8_t * tail);
void sbc_update(sbc_t * sbc, void** sorted_Vector, size_t sorted_vector_length);
void sbc_generate_priority_row(sbc_t * sbc, unsigned long * value_row, uint8_t * index_row);
void sbc_generate_priority(sbc_t * sbc);
void sbc_free(sbc_t * sbc);

void * functions[]={
	(void*)main,
	(void*)sortPointerVector,
	(void*)sortUnsignedLongVector,
	(void*)sbc_init,
	(void*)sbc_update_pair,
	(void*)sbc_update_block,
	(void*)sbc_update,
	(void*)sbc_generate_priority_row,
	(void*)sbc_generate_priority,
	(void*)sbc_free//,
//	(void*)printf
};

// sort pointers by value.  simple swapping sort.
void sortPointerVector(void * * vector, size_t bytes)
{
	void * * tail = vector + bytes / sizeof(void *);
	++ vector;
	void * * iterator = vector;
	while (iterator != tail) {
		if (*(iterator - 1) > *iterator) {
			void * temp = *iterator;
			*iterator = *(iterator - 1);
			*(iterator - 1) = temp;
			if (iterator > vector) {
				-- iterator;
				continue;
			}
		}
		++ iterator;
	}
}
// every row has the same bogus content
void sortUnsignedLongVector(unsigned long * values, size_t bytes, uint8_t * indices, bool reverse)
{
	size_t count = bytes / sizeof(decltype(*values));
	size_t index = 1;
	while (index != count) {
		if ((!reverse && values[index - 1] > values[index]) || (reverse && values[index - 1] < values[index])) {
			uint8_t temp;
			temp = values[index];
			values[index] = values[index - 1];
			values[index - 1] = temp;
			if (indices) {
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

// CONCEPT comparing memory by following-and-preceding-likelihood.
// NAME: sequential-byte-chance, sbc

struct sbc_t {
	unsigned long next_table[65536];
	unsigned long prev_table[65536];
	uint8_t next_prio[65536];
	uint8_t prev_prio[65536];
};

sbc_t * sbc_init()
{
	sbc_t * sbc = new sbc_t();
	for (int i = 0; i < 65536; ++ i) {
		sbc->next_table[i] = 0;
		sbc->prev_table[i] = 0;
	}
	return sbc;
};
void sbc_update_pair(sbc_t * sbc, uint8_t byte1, uint8_t byte2)
{
	(sbc->next_table + byte1*256)[byte2] ++;
	(sbc->prev_table + byte2*256)[byte1] ++;
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
void sbc_update(sbc_t * sbc, void** sorted_vector, size_t sorted_vector_length)
{
	void ** tail = sorted_vector + sorted_vector_length / sizeof(void*);
	void ** head = sorted_vector;

	// we just iterate each pair
	void ** iterator = head;
	++ iterator;
	while (iterator != tail) {
		sbc_update_block(sbc, (uint8_t*)*(iterator - 1), (uint8_t*)*iterator);
		++ iterator;
	}
}
void sbc_generate_priority_row(sbc_t * sbc, unsigned long * value_row, uint8_t * index_row)
{
	for (size_t a = 0; a < 256; ++ a) {
		index_row[a] = a;
	}
	for (size_t a = 0; a < 256; ++ a) {
		sortUnsignedLongVector(value_row, 256, index_row, true);
	}
}
void sbc_generate_priority(sbc_t * sbc)
{
	for (size_t a = 0; a < 256; ++ a) {
		sbc_generate_priority_row(sbc, sbc->next_table + a*256, sbc->next_prio + a*256);
		sbc_generate_priority_row(sbc, sbc->prev_table + a*256, sbc->prev_prio + a*256);
	}
}
void sbc_free(sbc_t * sbc)
{
	delete sbc;
}

// FOR TESTING
#include <stdio.h>
// FOR TESTING

int main()
{
	// 1. sort functions so we know the bounds of each.
	sortPointerVector(functions, sizeof(functions));
	
	// 2. compare each interesting function with each other interesting function, to build statistics
	sbc_t * sbc = sbc_init();
	sbc_update(sbc, functions, sizeof(functions));

	// it appears to work.  I'd like to generate priority tables.

	printf("Sorting ...\n");
	sbc_generate_priority(sbc);
	// TESTING AT THIS POINT
	for (int a = 0; a < 256; ++ a) {
		printf("%02x:", a);
		int b = 0;
		for (; b < 256 && sbc->next_table[a*256+b]; ++ b) {
			printf(" %02x", sbc->next_prio[a*256+b]);
		}
		//for (int j = 0; b < 256 && j < 3; ++ b, ++ j) {
		//	printf(" (%02x)", sbc->next_prio[a*256+b]);
		//}
		printf("\n");
	}
	sbc_free(sbc);
	// TESTING AT THIS POINT
	
	// 3. use the statistics to morph a function into another
	
	// 4. discern how much data is needed for #3 to succeed rapidly, and present the missing space to the user.
}





// this made an incredibly smaller space to explore.
