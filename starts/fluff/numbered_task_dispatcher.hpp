#pragma once

#include "life.hpp"

struct Numbered_Task_Dispatcher
{
	static LifeSpec & spec;
	static void instructions(Life & self);

	enum { TASK_IN };
};
