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
	LifeSpec const & environment;
	LifeSpec const * model;

	// label
	std::string name;

	// ears, eyes
	PropertySpec scalars_in;
	PropertySpec vectors_in;
	// mouth, muscles
	PropertySpec scalars_out;
	PropertySpec vectors_out;

	// private chemistry
	using instructions = void(*)(Life & life);
	instructions genes;
	Properties<LifeSpec> organs; // todo: change to community-member or somesuch, organ sounds less-friendly

	static LifeSpec & make(
		LifeSpec & environment,
		std::string name,
		std::initializer_list<std::initializer_list<std::string>> communication_channels,
		LifeSpec::instructions genes);
	static LifeSpec & grow(
		LifeSpec & environment,
		LifeSpec & old,
		std::string name);
};

struct Life
{
	static Life & make(
		LifeSpec & spec,
		Life & environment);

	LifeSpec const & spec;

	Life & environment;

	// ears, eyes
	Properties<Value> scalars_in;
	Properties<std::vector<Value>> vectors_in;
	// mouth, muscles
	Properties<Value> scalars_out;
	Properties<std::vector<Value>> vectors_out;

	// private parts
	Properties<Life> organs;
	void * body;

	/*
	void grow(
		std::initializer_list<Value,Value> scalars_in,
		std::initializer_list<Value,std::vector<Value>> vectors_in,
		std::initializer_list<Value,Value&> scalars_out,
		std::initializer_list<Value,std::vector<Value>*> vectors_out) {

	}
	*/
};

// a place for life with no home
extern LifeSpec EtherSpec;
extern Life Ether;
