#include "numbered_instruction_follower.hpp"

#include "numbered_task_dispatcher.hpp"


// this is being replaced by graph.hpp .
// by linking the parts togeter arbitrarily, arbitrary behavior may be produced.
#if 0

LifeSpec & Numbered_Instruction_Follower::spec = LifeSpec::make(
	EtherSpec,
	"Numbered Instruction Follower",
	{{},{"instructions in"},{},{}},
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
// 	1. we could copy meaning-vm quick-vm structure to provide for local values
//	2. we could just call functions on set values, without local values.  everything passed in
//	3. we could assume functions take no values.
//	4. something else
//
// 	we might say calling involves
// 		1 instruction
// 		1 # scalars
// 		scalars
// 		1 # vectors
// 	hmm
//
// 	what is the virtual machine state like?
// 		vector of scalars_in
// 		vector of numbers_in
// 		vector of scalars_out
// 		vector of numbers_out
// 		anything_you_want_to_add per lifespec
// 	each behavior expects some scalars_in and numbers_in, and provides some out, and the length of each is specified.
//
//


/*
 * say we have a learning process.  we are trying some things out.
 * we'd like to improve the learning process by adding life-parts to it that aid.
 * in the learning process, we have some ongoing state of vectors and numbers.
 * in the end, we want to produce good vectors and numbers.
 * so we are going to be wiring tasks into these vectors and numbers, to try to get better ones.
 * what structure would be good to represent this?
 *
 * 	it sounds like you have a reference to something, e.g. a test
 * 	and you are trying to manipulate its inputs to produce good outputs.
 * 		when you wire something to its inputs, you produce a bigger space of inputs, for this thing also has inputs.
 *
 * 			what structure helps that space exist?
 * 				a life that can accept new life into it
 * 				and map its inputs into theirs.
 * 				a graph-of-life life.
 * 				each sublife has its inputs and outputs wired to others.
 * 				this takes a map.
 *
 * 					could the graph-of-life life make a vm?
 * 						it sounds like it is a vm.
 * 						no, you would add a vm to it.
 *
 * 					what does the graph-of-life vm do?
 * 						
 * 					hmm
 */

// we are likely going to either need this vm, or a life-building life.
void Numbered_Instruction_Follower::instructions(Life & self)
{
	// TODO if you want.  seems it could basically be a copy of steps-list in meaning-vm
	/*
	auto & instructions = self.vectors[INSTRUCTIONS_IN];

	auto & dispatcher = self.organs[TASK_DISPATCHER];
	
	for (auto instruction : instructions) {
		dispatcher.scalars[Numbered_Task_Dispatcher::TASK_IN] = instructions;
		dispatcher.spec.instructions(dispatcher);
	}
	*/
}

/*
 * what would be needed to merge this with meaning-vm, that it has hit overlap?
 * 	i guess a bunch of ways to convert between the inforamtion representations?
 * 	you could store Life and LifeSpec as concepts, if you wanted to.  seems a little slower.
 */

#endif
