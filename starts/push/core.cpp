#include "loaded_relevance.hpp"

#include <iostream>
#include <unordered_set>

std::string _next_idea(std::string input, std::string context)
{
	auto & context_call = contexts[input + "-" + context + "-call"];
	context_call.relevance["input"] = input;
	context_call.relevance["context"] = context;
	find_relevant_string(context_call);
	return context_call.relevance["output"];
}
auto constexpr *  next_idea = _next_idea;

// let's make a test function instead of making core be immediately-in-memory.

void core(Context & context)
{
	// when we make decisions, we want to check something contextual to determine them, right?
	
	// we need to call find_relevant_string enough times until we find the desired behavior, and its input.
	
	// we're going to have to place "example-behavior" in "input"
	// we need to find behaviors, inputs, and outputs.
	// we'll need a way to handle failure, later.
	
	auto string = context.relevance["goal"]; // until we make String class for virtual-only functions

	std::array<std::vector<std::string>,3> ideas;
	std::array<uint64_t,ideas.size()> indices;

	ideas[0].push_back(next_idea(string, "behavior"));
	ideas[1].push_back(next_idea(string, "input"));
	ideas[2].push_back(next_idea(string, "output"));

	for (size_t i = 0; i < indices.size(); ++ i) { indices[i] = 0; }

	std::unordered_set<uint64_t> tried_combinations;

	auto & call = contexts["core-call"];

	
	while(true) {
		uint64_t hash = indices[0] + (indices[1]<<21) + (indices[2]<<42);
		if (tried_combinations.count(hash)) { continue; }
		tried_combinations.insert(hash);

		call.relevance["behavior"] = ideas[0][indices[0]];
		call.relevance["input"] = ideas[1][indices[1]];
		call.relevance["output"] = ideas[2][indices[2]];

		std::cout << "can I try " << call.relevance["output"] << " = " << call.relevance["behavior"] << "(" << call.relevance["input"] << ")?\n";
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
	}
}
