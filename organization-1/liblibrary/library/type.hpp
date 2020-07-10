#pragma once

namespace std {
	struct type_info;
}

namespace library {

struct type_info
{
	std::type_info const * std;
	void const * id;
	unsigned long size;

	// be nice to have unqualified name
	// be nice to have unmangled name
	char const * name;

	bool is_const;
	bool is_mutable;

	type_info const * as_const;
	type_info const * as_mutable;
	
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

// this template might be usable for compile-time type comparison
template <typename Type>
constexpr void const * const type_id;

class type_mismatch { };

class typed
{
public:
	virtual type_info const & type() const = 0;
};

class typed_valued : public typed
{
public:
	template <typename Type> Type const & reference() const;
	template <typename Type> Type const * pointer() const;
	template <typename Type> Type & reference();
	template <typename Type> Type * pointer();

	virtual void const * void_pointer() const = 0;
	virtual void * void_pointer() = 0;

	typed_valued & operator=(typed_valued const & other);
	void assign(typed_valued const & other);

	template <typename Type>
		typed_valued & operator=(Type const & other);
	template <typename Type> void assign(Type const & other);

protected:
	virtual void assign(void const * data) = 0;
};

class typable_valued
{
public:
	template <typename Type> Type const & reference() const;
	template <typename Type> Type const * pointer() const;
	template <typename Type> Type & reference();
	template <typename Type> Type * pointer();

	virtual void const * void_pointer(type_info const & type) const = 0;
	virtual void * void_pointer(type_info const & type) = 0;

	typable_valued & operator=(typed_valued const & other);
	void assign(typed_valued const & other);

	template <typename Type>
		typable_valued & operator=(Type const & other);
	template <typename Type> void assign(Type const & other);

protected:
	virtual void assign(void const * data, type_info const & type) = 0;
};

} // namespace library;
