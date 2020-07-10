#include <library/heapvector.hpp>
#include <library/stackvector.hpp>
#include <library/string.hpp>

#include <cstdlib>
#include <ctime>
#include <vector>

#define TYPE int
#define RESERVED 16
#include <library/stackvector_definition.cpp>
#include <library/heapvector_definition.cpp>

#include "test.hpp"

using namespace library;

// kinda unclear/hard to reuse: some more clear way to iterate types?

template <typename T>
T random_value();

template <>
int random_value<int>()
{
	return rand();
}

template <typename vector>
void testvector(unsigned long size)
{
	using elem = typename vector::element_type;
	std::vector<elem> base(size);
	vector test1(size);
	worry(test1.size() != size, "vector not iniitalized to size");
	for (unsigned long i = 0; i < size; ++ i) {
		test1[i] = base[i] = random_value<elem>();
	}
	for (unsigned long i = 0; i < size; ++ i) {
		worry(test1[i] != base[i], "vector values not preserved");
	}
	vector test2;
	worry(test2.size() != 0, "vector not initialized empty");
	for (unsigned long i = 0; i < size; ++ i) {
		test2.push_back(base[i]);
		test1 = test2;
		worry(test2.size() != i + 1, "vector not grown to size");
		worry(test1.size() != i + 1, "vector not copied to size");
		for (unsigned long j = 0; j <= i; ++ j) {
			worry(test2[j] != base[j], "vector values not preserved");
			worry(test1[j] != base[j], "vector values not preserved");
		}
	}
}

template <typename... types>
void testvectors(int size)
{
	int null[] = {(testvector<types>(size),0) ...};
	(void)null;
}

int main()
{
	srand(time(0));
	for (auto size : {0, 4, 16}) {
		testvectors<heapvector<int>, stackvector<int,16>>(size);
	}
	stderr::line("vectors passed test");
}
