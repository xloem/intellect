#pragma once

namespace std {
	struct type_info;
}

namespace library {

struct type_info
{
	std::type_info const * std;
	unsigned long size;

	// be nice to have unqualified name
	// be nice to have unmangled name
	char const * name;
	
	void (*construct_default)(void * placement);
	void (*construct_copy)(void * placement, void const * other);
	void (*assign)(void * object, void const * other);
	void (*destroy)(void * object);

	bool operator<(const type_info & other) const;
	bool operator<=(const type_info & other) const;
	bool operator==(const type_info & other) const;
	bool operator!=(const type_info & other) const;
	bool operator>=(const type_info & other) const;
	bool operator>(const type_info & other) const;
};

template <typename Type>
type_info const & type();

template <typename Type>
type_info const & type(Type const &);

} // namespace library;
