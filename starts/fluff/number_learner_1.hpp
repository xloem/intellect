#pragma once

#include "life.hpp"
#include "numbered_task_dispatcher.hpp"

#include <unordered_map>

LifeSpec::instructions number_learner_1;

struct Number_Learner_1 : public Numbered_Task_Dispatcher
{
	static LifeSpec & spec;

	using Numbered_Task_Dispatcher::TASK_IN;
	enum { TASK_OBSERVE, TASK_REPORT };

	enum { NUMBER_IN, NUMBER_OUT };
	enum { NUMBERS_OUT };
	std::unordered_multimap<Value, Value> memory;

	struct Observer
	{
		static LifeSpec & spec;
		static void instructions(Life & self);
	};

	struct Reporter
	{
		static LifeSpec & spec;
		static void instructions(Life & self);
	};
};

LifeSpec & Number_Learner_1::spec = LifeSpec::grow(
		EtherSpec(),
		Numbered_Task_Dispatcher::spec(),
		"number learner 1");
LifeSpec & Number_Learner_1::Observer::spec = LifeSpec::make(
		Number_Learner_1::spec,
		"observer", 
		{{"number in", "number out"},{},{},{}},
		Number_Learner_1::Observer::instructions
);
LifeSpec & Number_Learner_1::Reporter::spec = LifeSpec::make(
		Number_Learner_1::spec,
		"reporter",
		{{"number in"},{},{},{"numbers out"}},
		Number_Learner_1::Reporter::instructions
);

void Number_Learner_1::Observer::instructions(Life & self)
{
	auto & environment_body = (Number_Learner_1*&)self.environment.body;
	if (!environment_body) {
		environment_body = new Number_Learner_1();
	}
	
	environment_body->memory.emplace(self.scalars_in[NUMBER_IN], self.scalars_in[NUMBER_OUT]);
}

void Number_Learner_1::Reporter::instructions(Life & self)
{
	auto & environment_body = (Number_Learner_1*&)self.environment.body;
	if (!environment_body) {
		throw AloneLostAndConfused(self);
	}

	auto matching = environment_body->memory.equal_range(self.scalars_in[NUMBER_IN]);
	auto & guesses = self.vectors_out[NUMBERS_OUT];
	for (auto it = matching.first; it != matching.second; ++ it) {
		guesses.push_back(it->second);
	}
}
