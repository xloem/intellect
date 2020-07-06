#pragma once

#include <library/type.hpp>

namespace library {

// multiany is similar to the type interfaces,
// except it does not reject wrong types at all
// the difference is that it does not _have_ a type.  it's not typed.
class multiany
{
	template <typename T>
	T & get(

	// use a map of type_info to any under the hood
};

} // library
