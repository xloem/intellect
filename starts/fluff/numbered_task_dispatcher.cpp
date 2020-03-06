#include "numbered_task_dispatcher.hpp"


LifeSpec & Numbered_Task_Dispatcher::spec = LifeSpec::make(
	EtherSpec,
	"Numbered Task Dispatcher",
	{"task in"},
	{},
	Numbered_Task_Dispatcher::instructions
);


void Numbered_Task_Dispatcher::instructions(Life & self)
{
	auto & task = self.organs[self.scalars[TASK_IN]];
	task.scalars = self.scalars;
	task.vectors = self.vectors;
	task.spec.genes(task);
	self.scalars = task.scalars;
	self.vectors = task.vectors;
}
