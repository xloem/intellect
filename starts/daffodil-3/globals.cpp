#include "daffodil.hpp"

globals_t globals_t::storage;

// might be able to work around 3-places-for-each-class with
// some kind of class-registration system.
globals_t::globals_t()
: setup_done(YES)
{ }
