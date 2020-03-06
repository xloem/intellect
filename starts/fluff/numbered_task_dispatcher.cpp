#include "numbered_task_dispatcher.hpp"

#include <algorithm>


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

	auto scalarstart = ++self.scalars.begin();
	assert(&self.scalars[TASK_IN] == &*scalarstart);
	auto scalarend = self.scalars.end();
	for (size_t i = self.scalars.size(); i > task.scalars.size(); -- i) {
		-- scalarend;
	}

	std::copy(scalarstart, scalarend, task.scalars.begin());
	task.vectors = self.vectors;
	task.spec.genes(task);
	std::copy(task.scalars.begin(), task.scalars.end(), scalarstart);
	self.vectors = task.vectors;
}
