#pragma once

#include "life.hpp"
#include <unordered_map>

LifeSpec::instructions number_learner_1;

struct Number_Learner_1
{
	static LifeSpec spec;
	static void instructions(Life & self, Life * environment);

	enum { NUMBER_IN, NUMBER_OUT };
	std::unordered_multimap<Value, Value> memory;

	struct Observer
	{
		static LifeSpec spec;
		static void instructions(Life & self);
	};

	struct Reporter
	{
		static LifeSpec spec;
		static void instructions(Life & self);
	};
};

LifeSpec Number_Learner_1::Observer::spec = LifeSpec::make(
		Number_Learner_1::spec,
		"observer", 
		{"number in", "number out"}, 
		{}, 
		Number_Learner_1::Observer::instructions
);
void Number_Learner_1::Observer::instructions(Life & self)
{
	auto & environment_body = (Number_Learner_1*&)self.environment.body;
	if (!environment_body) {
		environment_body = new Number_Learner_1();
	}
	
	environment_body->memory.emplace(self.scalars[NUMBER_IN], self.scalars[NUMBER_OUT]);
}


LifeSpec Number_Learner_1::Reporter::spec = LifeSpec::make(
		Number_Learner_1::spec,
		"reporter",
		{"number in"},
		{"number out"},
		Number_Learner_1::Reporter::instructions
);
void Number_Learner_1::Reporter::instructions(Life & self)
{
	auto & environment_body = (Number_Learner_1*&)self.environment.body;
	if (!environment_body) {
		throw AloneLostAndConfused(self);
	}

	auto matching = environment_body->memory.equal_range(self.scalars[NUMBER_IN]);
	auto guesses = self.vectors[NUMBER_OUT];
	for (auto it = matching.first; it != matching.second; ++ it) {
		guesses.push_back(it->second);
	}
}
