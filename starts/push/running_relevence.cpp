
// Use relevence to make code that runs, out of our own instructions.

// We will set up a quick relevence structure that just copies the code into memory and runs it.

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/mman.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Context
{
	std::unordered_map<std::string, std::string> relevence;
};
std::unordered_map<std::string, std::vector<uint8_t>> data;
std::unordered_map<std::string, Context> contexts;

#include <stdio.h>
void example_behavior(Context & context)
{
	printf("Hello, world.\n");
};
void example_behavior_tail() { } // compile in -O0 to keep this in quick order.  use the ordering tool from 05.cpp later.

void copy_data(Context & context)
{
	data[context.relevence["output"]] = data[context.relevence["input"]];
}
void copy_data_tail() { } // compile in -O0 to keep this in quick order.  use the ordering tool from 05.cpp later.

void find_relevent_string(Context & context)
{
	auto input = context.relevence["input"];
	auto contextuality = context.relevence["context"];

	auto inputcontext = input + "-" + contextuality + "-relevency";

	std::array<decltype(Context::relevence)*,3> ideas = {
		&contexts[inputcontext + "-1"].relevence,
		&contexts[inputcontext + "-2"].relevence,
		&contexts[inputcontext + "-3"].relevence
	};

	if (!ideas[0]->empty()) {
		auto it = ideas[0]->begin();
		context.relevence["output"] = it->second;
		ideas[1]->insert(*it);
		ideas[0]->erase(it);
		return;
	}

	if (contexts[inputcontext].relevence.empty()) {
		contexts[inputcontext].relevence["seen-before"] = "yes";
		ideas[1]->insert({input,input});
		ideas[1]->insert({contextuality,contextuality});
	}

	size_t index = 1;
	while (ideas[0]->empty()) {
		if (ideas[index]->empty()) {
			++ index;
			assert(index < ideas.size());
			continue;
		}
		auto & oldideas = *ideas[index];
		for (auto & pair : oldideas) {
			auto & oldidea = pair.second;
			if (!contexts.count(oldidea)) { continue; }

			auto & other = contexts[oldidea].relevence;

			// way #2: find context keys
			for (auto pair : other) {
				ideas[0]->insert({pair.first,pair.first});
			}

			// way #3: find context values based on using contents as keys and other contents as contexts
			for (auto & pair : oldideas) {
				auto & otheridea = pair.second;
				if (other.count(otheridea)) {
					auto & information = other[otheridea];
					ideas[0]->insert({information,information});
				}
			}
		}
		for (auto it = oldideas.begin(); it != oldideas.end();) {
			typename std::string::size_type idx, lastidx = ~0;
			auto oldidea = it->second;
			// way #1: break hyphens
			if ((idx = oldidea.find("-")) != std::string::npos) {
				do {
					auto str = oldidea.substr(lastidx + 1, idx);
					ideas[0]->insert({str, str});
					lastidx = idx;
				} while((idx = oldidea.find("-")) != std::string::npos);
				break;
			}


			if (index + 1 < ideas.size()) {
				ideas[index + 1]->insert(*it);
				// error here, deal with later if matters
			}
			it = oldideas.erase(it);
		}

		// for LATER consideration: we could use associative pointers instead of just strings, for more flexibility
		// 	-> you might be able to use contextual association.  might just be pointer-maps.
		// 	   looks efficient to walk.
		// we are also going to want to find strings that contain our ideas as substrings. [MAYBE LATER THOUGH]
		// 	-> one approach is a comparison or lookup function
		// 	-> another approach is to build links in named contexts to parts of strings.
		// 	-> e.g. maybe-relevence: [dehyphenation]:[maybe-it-will-work]
	}

	return find_relevent_string(context);

}
void find_relevent_string_tail() { }

void run(Context & context)
{
	((void(*)(Context&))(data[context.relevence["behavior"]].data()))(context);
}

template <typename A, typename B>
std::vector<uint8_t> memory_between(A * head, B * tail) {
	// compile in -O0 to keep this in quick order.  use the ordering tool from 05.cpp later.
	std::vector<uint8_t> block((uint8_t*)tail - (uint8_t*)head);
	memcpy(block.data(), (void*)head, block.size());
	uint8_t* base = (uint8_t*)((uintptr_t)block.data() & (uintptr_t)~0xfff);
	int success = mprotect(base, block.size() + (block.data() - base), PROT_READ | PROT_WRITE | PROT_EXEC);
	if (success) { perror("mprotect"); exit(success); }
	return block;
}
void run_tail() { }

std::string next_idea(std::string input, std::string context)
{
	auto & context_call = contexts[input + "-" + context + "-call"];
	context_call.relevence["input"] = input;
	context_call.relevence["context"] = context;
	find_relevent_string(context_call);
	return context_call.relevence["output"];
}

void core(Context & context)
{
	// when we make decisions, we want to check something contextual to determine them, right?
	
	// we need to call find_relevent_string enough times until we find the desired behavior, and its input.
	
	// we're going to have to place "example-behavior" in "input"
	// we need to find behaviors, inputs, and outputs.
	// we'll need a way to handle failure, later.
	
	auto string = context.relevence["goal"];

	std::array<std::vector<std::string>,3> ideas;
	std::array<uint64_t,ideas.size()> indices;

	ideas[0].push_back(next_idea(string, "behavior"));
	ideas[1].push_back(next_idea(string, "input"));
	ideas[2].push_back(next_idea(string, "output"));

	for (size_t i = 0; i < indices.size(); ++ i) { indices[i] = 0; }

	std::unordered_set<uint64_t> tried_combinations;

	auto & call = contexts["core-call"];

	
	do {
		uint64_t hash = indices[0] + (indices[1]<<21) + (indices[2]<<42);
		if (tried_combinations.count(hash)) { continue; }
		tried_combinations.insert(hash);

		call.relevence["behavior"] = ideas[0][indices[0]];
		call.relevence["input"] = ideas[1][indices[1]];
		call.relevence["output"] = ideas[2][indices[2]];

		std::cout << "can I try " << call.relevence["output"] << " = " << call.relevence["behavior"] << "(" << call.relevence["input"] << ")?\n";
		std::string response;
		std::cin >> response;
		if (response == "Y" || response == "yes") { run(call); return; }
		
		size_t which = 0;
		while (true) {
			indices[which] ++;
			if (indices[which] < ideas[which].size()) {
				break;
			}
			indices[which] = 0;
			++ which;
			if (which >= indices.size()) {
				size_t shortest = 0;
				for (size_t i = 0; i < indices.size(); ++ i) {
					if (ideas[i].size() < ideas[shortest].size()) {
						shortest = i;
					}
				}
				ideas[shortest].push_back(next_idea(string, shortest == 0 ? "behavior" : (shortest == 1 ? "input" : "output")));
				std::swap(ideas[shortest].front(), ideas[shortest].back());
				break;
			}
		}
	} while (std::next_permutation(ideas.begin(), ideas.end()));

	// 0123 0  { { {
	
	std::swap(ideas[0], ideas[1]);

	// } 1023 1 0000
	
	std::swap(ideas[1], ideas[2]);

	// } 1203 0 0000
	
	std::swap(ideas[0], ideas[1]);

	// 2103 1 1000 { {
	
	std::swap(ideas[1], ideas[2]);

	// } 2013 0 1100

	std::swap(ideas[0], ideas[1]);

	// } 0213 2 1100

	std::swap(ideas[2], ideas[3]);

	// } 0231 0 }
}

void core_tail() { }

int main()
{
	data["example-behavior"] = memory_between(example_behavior, example_behavior_tail);
	data["copy-data"] = memory_between(copy_data, copy_data_tail);
	data["find-relevent-string"] = memory_between(find_relevent_string, find_relevent_string_tail);
	data["core"] = memory_between(core, core_tail);
	data["run"] = memory_between(run, run_tail);

	// steps we need to find: copy-data call-function
	// can we add abstraction?

	// we want a concept associated with the goal or context
	// to lead us to a concept needed to accomplish it.

	contexts["current"].relevence["goal"] = "copy-example-behavior-and-run-the-copy";
	
	// at some point we want a context that has "example-behavior" in "input"
	// and "new-behavior" or whatever in "output"
	// and we want to run "copy-data" on them.  "copy-data" can go in "behavior"

	// we then want to run "run" on "new-behavior", but this can be done manually for now.
	
	//         v--- contains word 'behavior' as substring
	contexts["available-behaviors"].relevence["1"] = "example-behavior";
	contexts["available-behaviors"].relevence["2"] = "copy-data";
	contexts["available-behaviors"].relevence["3"] = "find-relevent-string";
	contexts["available-behaviors"].relevence["4"] = "core";
	contexts["available-behaviors"].relevence["5"] = "run";

	contexts["copy-output"].relevence["name-idea"] = "new-";

	contexts["current"].relevence["behavior"] = "core";
	run(contexts["current"]);
}
