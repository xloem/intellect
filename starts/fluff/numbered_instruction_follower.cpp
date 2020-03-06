#include "numbered_instruction_follower.hpp"


LifeSpec & Numbered_Instruction_Follower::spec = LifeSpec::make(
	EtherSpec,
	"Numbered Instruction Follower",
	{},
	{"instructions in"},
	Numbered_Instruction_Follower::instructions
);

LifeSpec & Numbered_Instruction_Follower::Task_Dispatcher::spec = LifeSpec::grow(
	Numbered_Instruction_Follower::spec,
	Numbered_Task_Dispatcher::spec,
	"Task Dispatcher");
LifeSpec & Numbered_Instruction_Follower::Task_Dispatcher::Done::spec = LifeSpec::make(
	Numbered_Instruction_Follower::Task_Dispatcher::spec,
	"Done",
	{},
	{},
	Numbered_Instruction_Follower::Task_Dispatcher::Done::instructions
);

// VM: we left out arguments in the instruction list.  and moving.
//
// 	okay so a kind of central concept is a way, in a vector, to move numbers between spots.
// 	this could be a number-moving behavior.
//
// 	we might say calling involves
// 		1 instruction
// 		1 # scalars
// 		scalars
// 		1 # vectors
// 	hmm
//
// 	what is the virtual machine state like?
// 		vector of scalars
// 		vector of numbers
// 	each behavior expects specific values at low points in the vectors.
// 	we can also add arbitrary state of our own.
//
// 		so the map-a-vector-to-a-number behavior, has a scalar length of 1 (its output) and a vector length of 1 (its input)
// 		but the caller need not set the output.
// 			maybe it is time to specify the difference between outputs and inputs.  these can simply be offsets
// 			into the vectors.

void Numbered_Instruction_Follower::instructions(Life & self)
{
	auto & instructions = self.vectors[INSTRUCTIONS_IN];

	auto & dispatcher = self.organs[TASK_DISPATCHER];
	
	for (auto instruction : instructions) {
		dispatcher.scalars[Numbered_Task_Dispatcher::TASK_IN] = instructions;
		dispatcher.spec.instructions(dispatcher);
	}
}
