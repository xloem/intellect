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
void testvector(unsigned long size, unsigned long max_size)
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
	
	for (unsigned long splice_start = 0; splice_start < size; ++ splice_start) {
		for (unsigned long erased_length = 0; erased_length < size - splice_start; ++ erased_length) {
			for (unsigned long inserted_length = 0; inserted_length < size && size + inserted_length - erased_length <= max_size; ++ inserted_length) {
				test1 = test2;
				test1.splice(splice_start, erased_length, base.data(), inserted_length);
				for (unsigned long i = 0; i < base.size(); ++ i) {
					worry(test2[i] != base[i], "splice corrupted memory of test2");
				}
				worry(test1.size() != base.size() - erased_length + inserted_length, "wrong-sized splice result");
				for (unsigned long i = 0; i < splice_start; ++i) {
					worry(test1[i] != base[i], "prior vector values not preserved after splice");
				}
				for (unsigned long i = 0; i < inserted_length; ++ i) {
					worry(test1[splice_start + i] != base[i], "spliced data not correct");
				}
				for (unsigned long i = splice_start + erased_length; i < size; ++ i) {
					worry(test1[i + inserted_length - erased_length] != base[i], "post vector values not preserved after splice");
				}
			}
		}
	}
}

template <typename... types>
void testvectors(int size, int max_size)
{
	int null[] = {(testvector<types>(size, max_size),0) ...};
	(void)null;
}

int main()
{
	srand(time(0));
	for (auto size : {0, 4, 16}) {
		testvectors<heapvector<int>, stackvector<int,16>>(size, 16);
	}
	stderr::line("vectors passed test");
}
