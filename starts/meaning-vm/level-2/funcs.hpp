#pragma once

#include "common.hpp"

#include <functional>
#include <initializer_list>

namespace intellect {
namespace level2 {

ref context();
ref makehabit(ref name, std::initializer_list<ref> argnames, std::function<void(ref)> code);
ref dohabit(ref habit, std::initializer_list<ref> args);

}
}
