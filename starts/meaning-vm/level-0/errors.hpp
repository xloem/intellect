#pragma once
#include "ref.hpp"

#include <stdexcept>

namespace intellect {
namespace level0 {

struct no_such_link_type : public std::out_of_range
{
	no_such_link_type(ref source, ref type)
	: std::out_of_range("no such concept link type"),
	  source(source),
	  type(type)
	{ }

	ref const source;
	ref const type;
};

struct no_such_link_type_target : public std::out_of_range
{
	no_such_link_type_target(ref source, ref type, ref target)
	: std::out_of_range("no such concept link type and target"),
	  source(source),
	  type(type),
	  target(type)
	{ }

	ref const source;
	ref const type;
	ref const target;
};

struct link_type_not_unique : public std::invalid_argument
{
	link_type_not_unique(ref source, ref type)
	: std::invalid_argument("more than one such concept link type"),
	  source(source),
	  type(type)
	{ }

	ref const source;
	ref const type;
};

struct still_referenced_by : public std::invalid_argument
{
	still_referenced_by(ref topic, ref referrer)
	: std::invalid_argument("concept is still referenced"),
	  topic(topic),
	  referrer(referrer)
	{ }

	ref const topic;
	ref const referrer;
};

struct no_such_concept : public std::invalid_argument
{
	no_such_concept(ref topic)
	: std::invalid_argument("no such concept reference"),
	  topic(topic)
	{ }

	ref const topic;
};

struct null_reference : public std::invalid_argument
{
	null_reference()
	: std::invalid_argument("null reference")
	{ }
};

}
}
