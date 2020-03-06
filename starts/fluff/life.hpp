#pragma once

#include <functional>
#include <string>
#include <vector>

using Value = double;

template <typename T>
using Properties = std::vector<T>;
using PropertySpec = Properties<std::string>;

struct Life;
using AloneLostAndConfused = std::reference_wrapper<Life>;

struct LifeSpec
{
	static LifeSpec make(
		LifeSpec const * environment,
		std::string name,
		std::initializer_list<LifeSpec> organs,
		std::initializer_list<std::string> scalars,
		std::initializer_list<std::string> vectors,
		instructions genes);

	LifeSpec const * environment;

	// label
	std::string name;

	// muscles, mouth, ears
	PropertySpec scalars;
	PropertySpec vectors;

	// private chemistry
	using instructions = void(*)(Life & life);
	instructions genes;
	Properties<LifeSpec> organs;
};

struct Life
{
	static Life make(
		LifeSpec & spec,
		Life * environment = 0);

	LifeSpec const & spec;

	Life * environment;

	// muscles, mouth, ears
	Properties<Value> scalars;
	Properties<std::vector<Value>> vectors;

	// private parts
	Properties<Life> organs;
	void * body;
};
