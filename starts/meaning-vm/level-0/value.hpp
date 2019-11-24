#pragma once

#include "common.hpp"

#include "concept.hpp"

namespace intellect {
namespace level0 {

template <typename T>
struct value : public concept
{
	value(T const & val) : data(val) { }

	value(value<T> const & val) = default;

	operator T&() { return data; }
	operator T const &() const { return data; }

	T data;
};


}
}
