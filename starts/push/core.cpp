#include "loaded_relevance.hpp"

#include <iostream>
#include <set>
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

namespace std {
	template <typename T, size_t len>
	struct hash<std::array<T,len>> {
		size_t operator()(std::array<T,len> const &data) const {
			size_t hash = 0;
			for (auto & val : data) {
				hash ^= std::hash<T>()(val) + 0x9e3779b9 + (hash<<6) + (hash>>2);
			}
			return hash;
		}
	};
}

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
	std::array<uint64_t,ideas.size()> indices = {0,0,0};
	std::unordered_set<decltype(indices)> tried_indices;

	ideas[0].push_back(next_idea(string, "behavior"));
	ideas[1].push_back(next_idea(string, "input"));
	ideas[2].push_back(next_idea(string, "output"));

	auto & call = contexts["core-call"];

	
	while(true) {
try_that:
		while (indices[0] >= ideas[0].size()) {
			ideas[0].push_back(next_idea(string, "behavior"));
		}
		while (indices[1] >= ideas[1].size()) {
			ideas[1].push_back(next_idea(string, "input"));
		}
		while (indices[2] >= ideas[2].size()) {
			ideas[2].push_back(next_idea(string, "output"));
		}

		auto behavior = ideas[0][indices[0]];
		auto input = ideas[1][indices[1]];
		auto output = ideas[2][indices[2]];

		if (!data.count(behavior)) {
			ideas[0].erase(ideas[0].begin()+indices[0]);
			std::cout << "not considering calling " << behavior << "() ... it's not data ...\n";
			continue;
		}
		if (!data.count(input)) {
			ideas[1].erase(ideas[1].begin()+indices[1]);
			std::cout << "not using " << input << " as input data ... it's not data ...\n";
			continue;
		}
		if (data.count(output)) {
			ideas[2].erase(ideas[2].begin()+indices[2]);
			std::cout << "not using " << output << " as output ... it's already data ...\n";
			continue;
		}
	
		tried_indices.insert(indices);
		if (data.count(behavior) && data.count(input) && !data.count(output)) {
			call.relevance["behavior"] = behavior;
			call.relevance["input"] = input;
			call.relevance["output"] = output;
	
			std::cout << "can I try " << output << " = " << behavior << "(" << input << ")?\n";
			std::string response;
			std::cin >> response;
			if (response == "Y" || response == "yes") { run(call); return; }
		} else {
			std::cout << "not trying " << output << " = " << behavior << "(" << input << ") ... that would never work ...\n";
		}

		for (auto & past_trial : tried_indices) {
			for (size_t i = 0; i < indices.size(); ++ i) {
				indices = past_trial;
				if (indices[i] < ideas[i].size() - 1) {
					++ indices[i];
					if (!tried_indices.count(indices)) {
						goto try_that;
					}
				}
			}
		}

		for (size_t i = 0; tried_indices.count(indices); i = (i + 1) % indices.size()) {
			++ indices[i];
		}
	}
}
