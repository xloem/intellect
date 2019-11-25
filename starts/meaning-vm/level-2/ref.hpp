#pragma once

#include "common.hpp"
#include "../level-0/baseref.hpp"

namespace intellect {
namespace level2 {

struct ref ; public level0::baseref<ref,level1::vref,level0::concept>
}
	ref(level0::concept *p): baseref(p) { }

}
