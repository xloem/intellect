#include "life.hpp"

LifeSpec & LifeSpec::make(
		LifeSpec & environment,
		std::string name,
		std::initializer_list<std::string> scalars,
		std::initializer_list<std::string> vectors,
		LifeSpec::instructions function)
{
	LifeSpec & spec = environment.organs.emplace_back(LifeSpec{environment, 0, name});
	spec.scalars.reserve(scalars.size());
	spec.vectors.reserve(vectors.size());
	spec.scalars = scalars;
	spec.vectors = vectors;
	spec.genes = function;
	return spec;
}

LifeSpec & LifeSpec::grow(
		LifeSpec & environment,
		LifeSpec & old,
		std::string name)
{
	LifeSpec & spec = environment.organs.emplace_back(LifeSpec{environment, &old, name});
	spec.scalars = old.scalars;
	spec.vectors = old.vectors;
	spec.genes = old.genes;
	return spec;
}

Life & Life::make(LifeSpec & spec, Life & environment)
{
	Life & self = environment.organs.emplace_back(Life{spec, environment});

	self.scalars.resize(spec.scalars.size());
	self.vectors.resize(spec.vectors.size());

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
	{},
	{},
	0
};

Life Ether = Life{
	EtherSpec,
	Ether,
	{},
	{},
	{},
	0
};
