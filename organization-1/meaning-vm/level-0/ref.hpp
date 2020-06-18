#pragma once

#include "common.hpp"
#include "baseref.hpp"

#include <string>

namespace intellect {
namespace level0 {

struct ref : public baseref<ref>
{
	ref(concept *p) : baseref(p) { }

	std::string dump(ref skipmarkertype, ref skipmarkertarget);
};

}
}
