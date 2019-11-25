#pragma once

#include "../level-0/vref.hpp"
#include "ref.hpp"

#include <sstream>

namespace intellect {
namespace level1 {

template <typename T>
struct vref : public level0::vref<T>
{
	vref(level0::vref<T> other) : level0::vref<T>((level0::concept*)other.ref) { }
	vref(level0::value<T> *p) : level0::vref<T>(p) { }
	vref(ref const & other) : level0::vref<T>(other) { }
	vref(T const & val)
	: level0::vref<T>(val)
	{
		std::stringstream ss;
		ss << "v:" << val;
		ref(self).set(ref("name"), (level0::ref)level0::vref<std::string>(ss.str()));
	}

	using level0::vref<T>::operator->;
	using level0::vref<T>::operator T const &;

	vref<std::string> name()
	{
		return ref(self).name();
	}

	operator ref() { return level0::ref(level0::vref<T>::ptr); }
};


}
}
