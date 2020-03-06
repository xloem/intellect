#include "life.hpp"

LifeSpec LifeSpec::make(LifeSpec const * environment, std::string name, std::initializer_list<LifeSpec> organs, std::initializer_list<std::string> scalars, std::initializer_list<std::string> vectors, LifeSpec::instructions function)
{
	LifeSpec spec {
		environment,
		name
	};
	spec.organs = organs;
	spec.scalars = scalars;
	spec.vectors = vectors;
	spec.genes = function;
	return spec;
}

Life Life::make(LifeSpec & spec, Life * environment)
{
	Life life{
		spec,
		environment
	};

	life.scalars.resize(spec.scalars.size());
	life.vectors.resize(spec.vectors.size());

	for (auto & organ_spec : spec.organs) {
		life.organs.emplace_back(make_life(organ_spec));
	}
	life.body = 0;
	return life;
}
