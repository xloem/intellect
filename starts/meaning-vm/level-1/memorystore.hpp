#pragma once

#include "common.hpp"

namespace intellect {
namespace level1 {

ref alloc(ref group, std::any data = {});
void keep(ref allocated, ref group);
void dealloc(ref group, ref metagroup = concepts::nothing);
std::size_t allocatedgroups();

}
}
