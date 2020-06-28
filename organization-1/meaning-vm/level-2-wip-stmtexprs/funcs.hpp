#pragma once

#include "common.hpp"
#include "../level-1/ref.hpp"

namespace intellect {
namespace level2 {

statementref commaop(ref self, ref other);
statementref assignop(ref self, ref other);
ref subop(ref self, ref other);

}
}
