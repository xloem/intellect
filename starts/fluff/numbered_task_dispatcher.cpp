#include "numbered_task_dispatcher.hpp"

#include <algorithm>
#include <cassert>


// static iniitalization order: this must be initialized second
LifeSpec & Numbered_Task_Dispatcher::spec = LifeSpec::make(
	EtherSpec,
	"Numbered Task Dispatcher",
	{{"task in"},{},{},{}},
	Numbered_Task_Dispatcher::instructions
);


void Numbered_Task_Dispatcher::instructions(Life & self)
{
	auto & task = self.organs[self.scalars_in[TASK_IN]];

	auto scalarstart = ++self.scalars_in.begin();
	assert(&self.scalars_in[TASK_IN] == &*scalarstart);
	auto scalarend = self.scalars_in.end();
	for (size_t i = self.scalars_in.size(); i > task.scalars_in.size(); -- i) {
		-- scalarend;
	}

	std::copy(scalarstart, scalarend, task.scalars_in.begin());
	task.vectors_in = self.vectors_in;
	task.spec.genes(task);
	self.scalars_out = task.scalars_out;
	self.vectors_out = task.vectors_out;
}
