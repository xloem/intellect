// It is apparently very hard to do the binary form.
// Let's make it clear and simple.

#include "capstone.i"
Capstone capstone;
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

// We have a vector of data.  We produce one instruction at a time.
// We iterate instruction-parts, and we iterate instructions.

using Word1 = unsigned long;

#define func1 Opcode1_go1
#define func2 Opcode1_step1

struct Opcode1 {
	std::vector<uint8_t> * code;

	size_t start;
	size_t low_depth;
	size_t high_depth;
};

struct Sequence1 {
	std::vector<uint8_t> code;
	std::vector<size_t> opcode_locations;
	Opcode1 instruction_workspace;
};

void Opcode1_init1(Opcode1 & wk, Sequence1 & seq)
{
	wk.code = & seq.code;
}

void Sequence1_init1(Sequence1 & wk)
{
	wk.code.reserve(64);
	wk.opcode_locations.push_back(0);
	Opcode1_init1(wk.instruction_workspace, wk);
}

void Opcode1_step1(Opcode1 & wk);
bool Opcode1_test1(Opcode1 & wk);
bool Sequence1_test1(Opcode1 & wk);


void Opcode1_go1(Opcode1 & wk, size_t start)
{
	wk.start = start;
	wk.high_depth = wk.code->size();
	wk.low_depth = wk.high_depth;

	while (!Opcode1_test1(wk)) {
		Opcode1_step1(wk);
	}
	std::cout << "opcode match" << std::endl;
}

// let's speed this up by using larger data values
void Opcode1_step1(Opcode1 & wk)
{
	uint64_t opcode;
	auto & code = *wk.code;
	while (true) {
		(*(Word1*)&code[wk.low_depth]) ++;
		if (*(Word1*)&code[wk.low_depth] == 0) {
			if (wk.low_depth == wk.start) {
				wk.high_depth = code.size();
				wk.low_depth = wk.high_depth;
				code.resize(wk.high_depth + sizeof(Word1));
				code[wk.high_depth] = 0;
				std::cout << "New opcode depth = " << code.size() <<  std::endl;
			} else {
				wk.low_depth -= sizeof(Word1);
				continue;
			}
		} else if (wk.low_depth != wk.high_depth) {
			wk.low_depth = wk.high_depth;
			continue;
		}
		return;
	}
}

bool Sequence1_test1(Opcode1 & wk)
{
	return std::memcmp(wk.code->data(), (void*)func1, ((uint8_t*)func2 - (uint8_t*)func1)) == 0;
}

bool Opcode1_test1(Opcode1 & wk)
{
	auto len = capstone.check(*wk.code);
	return len && len == wk.code->size();
}

/*
void Sequence1_go1(Sequence1 & wk)
{
	auto maxlen = ((uint8_t*)func2 - (uint8_t*)func1);
	while (!Sequence1_test1(wk)) {

		if (wk.code.size() >= maxlen) {
			wk.code.resize(
		}

		Opcode1_go1(wk.instruction_workspace, wk.opcode_locations.back());
		wk.opcode_locations.push_back(wk.code.size());
	}
}
*/

int main()
{
	Sequence1 wk1;
	Sequence1_init1(wk1);
	//Sequence1_go1(wk1);
	Opcode1_go1(wk1.instruction_workspace, 0);
	return 0;
}
