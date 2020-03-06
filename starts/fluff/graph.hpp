#pragma once

#include "life.hpp"

struct Graph : public Numbered_Task_Dispatcher
{
	static LifeSpec & spec;

	using Numbered_Task_Dispatcher::TASK_IN;
	enum {
		TASK_GO,
		TASK_ALLOCATE,
		TASK_CONNECT,
	};

	std::vector<size_t> uses; // content is part used
	std::vector<connection> connections;

	struct endpoint {
		size_t use;
		size_t index;
	};
	struct connection {
		endpoint output;
		endpoint input;
	};

	enum { NODE_USES_IN };
	enum { NODE_USES_OUT };
	struct Goer
	{
		static LifeSpec & spec;
		static void instructions(Life & self);
	};

	enum { ORGAN_IN };
	enum { NODE_USE_OUT };
	struct Allocator
	{
		static LifeSpec & spec;
		static void instructions(Life & self);
	};
};

// the graph wires parts together.
// 
// 	[so, it has a map of how to move information from inputs,
// 	 to outputs.]
// 	[it can likely be engaged by filling its inputs.
// 	 this makes it function like a vm-run-instance.]
//	[we will likely want to use it at runtime to add and remove things from itself.  for generality, let's be able to do that with numbers.]
//	[so it will need a reference to what-to-wire.  maybe it wires its own organs up.]
//	[so 
//	[we want to be able to multiply-use organs.
//	 like, send this output back to the input, 1ce.
//	 1 way is multiple actual copies of organs.  unideal.
//	   would prefer multiple uses of organs.
//	   wiring mapping that provides for this?
//	   	we'll want to generate instances of organs
//	   	we could use bitshifting to do this]
//
//	   	[above we thought multiple-uses-of-organs
//	   	 could add bits to the numbers, so they are
//	   	 greater than number of organs, to refer
//	   	 to the same organ via masking but be another
//	   	 instance.]
//	   	 	[it's a little hard to do bitmasking
//	   	 	 with doubles.  but i guess it's a
//	   	 	 solution that will work.]
//	   	 [bitmasking makes number-description of wiring
//	   	  obscure to human reviewer.]
//	   	 [a little time spent looking for alternative?]
//		 [we could have a behavior to generate a use
//		  of an organ, and refer to it by number.
//		  this makes this life-thing have an additional
//		  type inside it, "uses-of-organs".]
//		 [that sounds way more clear.]
//		 [bitmasking is a technique used for embedded development of new technology.  it is not to be used in most code.]
//		 		it's associated with pushing-enveloped, put-lots-of-thoughts-into-forcing-it-to-be-faster-or-smaller.  it's likely not taught to common programmers anymore.
