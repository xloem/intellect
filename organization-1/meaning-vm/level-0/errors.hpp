#pragma once
#include "concept.hpp"

#include <stdexcept>

namespace intellect {
namespace level0 {

struct no_such_link_type : public std::out_of_range
{
	no_such_link_type(concept* source, concept* type)
	: std::out_of_range("no such concept link type"),
	  source(source),
	  type(type)
	{ }

	concept* const source;
	concept* const type;
};

struct no_such_link_type_target : public std::out_of_range
{
	no_such_link_type_target(concept* source, concept* type, concept* target)
	: std::out_of_range("no such concept link type and target"),
	  source(source),
	  type(type),
	  target(type)
	{ }

	concept* const source;
	concept* const type;
	concept* const target;
};

struct crucial_link_type_target : public std::out_of_range
{
	crucial_link_type_target(concept* source, concept* type, concept* target)
	: std::out_of_range("concept part is crucial"),
	  source(source),
	  type(type),
	  target(type)
	{ }

	concept* const source;
	concept* const type;
	concept* const target;
};

struct crucial_concept : public std::invalid_argument
{
	crucial_concept(concept* topic)
	: std::invalid_argument("concept is crucial"),
	  topic(topic)
	{ }

	concept* const topic;
};

struct link_type_not_unique : public std::invalid_argument
{
	link_type_not_unique(concept* source, concept* type)
	: std::invalid_argument("more than one such concept link type"),
	  source(source),
	  type(type)
	{ }

	concept* const source;
	concept* const type;
};

struct still_referenced : public std::invalid_argument
{
	still_referenced(concept* topic)
	: std::invalid_argument("concept is still referenced"),
	  topic(topic)
	{ }

	concept* const topic;
};

struct still_referenced_by : public std::invalid_argument
{
	still_referenced_by(concept* topic, concept* referrer)
	: std::invalid_argument("concept is still referenced"),
	  topic(topic),
	  referrer(referrer)
	{ }

	concept* const topic;
	concept* const referrer;
};

struct no_such_concept : public std::invalid_argument
{
	no_such_concept(concept* topic)
	: std::invalid_argument("no such concept reference"),
	  topic(topic)
	{ }

	concept* const topic;
};

struct null_reference : public std::invalid_argument
{
	null_reference()
	: std::invalid_argument("null reference")
	{ }
};

struct memory_norm_disrespected : public std::invalid_argument
{
	memory_norm_disrespected(concept* allocator, concept* allocated)
	: std::invalid_argument("memory norm disrespected"),
	  allocator(allocator),
	  allocated(allocated)
	{ }

	concept* allocator;
	concept* allocated;
};

}
}
