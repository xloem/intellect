#pragma once

#include "life.hpp"

// this is being replaced by graph.hpp .
// by linking the parts togeter arbitrarily, arbitrary behavior may be produced.
#if 0

struct Numbered_Instruction_Follower
{
	static LifeSpec & spec;
	static void instructions(Life & self);

	enum { INSTRUCTIONS_IN };

	enum { TASK_DISPATCH };
	struct Task_Dispatcher
	{
		static LifeSpec & spec;

		enum { DONE };
		struct Done {
			static LifeSpec & spec;
		};
	};
};

#endif
