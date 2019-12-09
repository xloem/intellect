#pragma once

#include "common.hpp"
#include "baseref.hpp"

namespace intellect {
namespace level2 {

struct ref : public baseref<ref>
{
	using baseref<ref>::baseref;
};

}
}
