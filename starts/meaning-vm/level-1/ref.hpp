#pragma once

#include "common.hpp"
#include "baseref.hpp"

namespace intellect {
namespace level1 {

struct ref : public baseref<ref>
{
	using baseref<ref>::baseref;

	std::string dump(ref skipmarkertype, ref skipmarkertarget);
};

}
}
