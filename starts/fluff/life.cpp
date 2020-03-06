#include "life.hpp"

LifeSpec LifeSpec::make(LifeSpec & environment, std::string name, std::initializer_list<std::string> scalars, std::initializer_list<std::string> vectors, LifeSpec::instructions function)
{
	LifeSpec & spec = environment.organs.emplace_back(LifeSpec{environment, name});
	spec.scalars = scalars;
	spec.vectors = vectors;
	spec.genes = function;
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
