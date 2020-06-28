#pragma once

#include <cstddef>

#include <library/string>

namespace library {

// unused for now.  feel free to wipe/use/change.

class abstract_vector
{
public:
	virtual size_t size();
	virtual void * abstract(size_t index);
	virtual string string(size_t index);
};

template <typename element_type>
class vector : public abstract_vector
{
public:
	virtual element_type * data();
	virtual element_type * begin();
	virtual element_type * end();
};

}
