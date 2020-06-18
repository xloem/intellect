#pragma once

namespace library {

#if defined(NDEBUG)
#warn "Computers are fast; recommend not using NDEBUG so errors are found."
#define assert(throw_if_false)
#else
void assert_with(bool throw_if_false, char const * assertion);
#define assert(throw_if_false) assert_with(throw_if_false, #throw_if_false);
#endif

} // namespace library
