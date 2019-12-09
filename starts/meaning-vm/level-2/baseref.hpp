#pragma once

#include "common.hpp"

#include "funcs.hpp"

#include "../level-1/common.hpp"

#include <functional>

namespace intellect {
namespace level2 {

template <typename ref>
struct baseref : public level1::baseref<ref>
{
	using level1::template baseref<ref>::baseref;

	// thread-local context
	static ref context() { return level2::context(); }

};

}
}
