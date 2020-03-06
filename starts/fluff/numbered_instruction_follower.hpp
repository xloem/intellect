#pragma once

#include "life.hpp"

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
