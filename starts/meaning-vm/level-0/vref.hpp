#pragma once

#include "common.hpp"
#include "memorystore.hpp"
#include "ref.hpp"
#include "value.hpp"

namespace intellect {
namespace level0 {

template <typename T>
struct vref
{
	vref(value<T> *p) : ptr(p) { }
	value<T>* operator->() { return ptr; }
	operator T const &() const { return *ptr; }

	vref(T const & val) : ptr(alloc(new value<T>(val))) { }

	vref(ref const & other) : ptr(static_cast<value<T>*>(other.ptr)) { }
	operator ref() { return ptr; }

	// for use by containers
	bool operator<(vref<T> const & other) const { return self.ptr < other.ptr; }

	value<T> * const ptr;
};


}
}
