#include "life.hpp"

#include <cassert>

LifeSpec & LifeSpec::make(
		LifeSpec & environment,
		std::string name,
		std::initializer_list<std::initializer_list<std::string>> communication_channels,
		LifeSpec::instructions function)
{
	assert(communication_channels.size() == 4);

	LifeSpec & spec = environment.organs.emplace_back(LifeSpec{environment, 0, name});

	auto it = communication_channels.begin();

	spec.scalars_in.reserve(it->size());
	spec.scalars_in = *it;
	++ it;

	spec.vectors_in.reserve(it->size());
	spec.vectors_in = *it;
	++ it;

	spec.scalars_out.reserve(it->size());
	spec.scalars_out = *it;
	++ it;

	spec.vectors_out.reserve(it->size());
	spec.vectors_out = *it;
	++ it;

	spec.genes = function;

	return spec;
}

LifeSpec & LifeSpec::grow(
		LifeSpec & environment,
		LifeSpec & old,
		std::string name)
{
	LifeSpec & spec = environment.organs.emplace_back(LifeSpec{environment, &old, name});
	spec.scalars_in = old.scalars_in;
	spec.vectors_in = old.vectors_in;
	spec.scalars_out = old.scalars_out;
	spec.vectors_out = old.vectors_out;
	spec.genes = old.genes;
	return spec;
}

Life & Life::make(LifeSpec & spec, Life & environment)
{
	Life & self = environment.organs.emplace_back(Life{spec, environment});

	self.scalars_in.resize(spec.scalars_in.size());
	self.vectors_in.resize(spec.vectors_in.size());
	self.scalars_out.resize(spec.scalars_out.size());
	self.vectors_out.resize(spec.vectors_out.size());

	for (auto & organ_spec : spec.organs) {
		Life::make(organ_spec, self);
	}
	self.body = 0;
	return self;
}

LifeSpec EtherSpec = LifeSpec{
	EtherSpec,
	0,
	"ether spec",
	{}, {}, {}, {},
	0
};

Life Ether = Life{
	EtherSpec,
	Ether,
	{}, {}, {}, {},
	{},
	0
};
