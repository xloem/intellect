#pragma once

#include <library/type.hpp>

namespace library {

class any : public typed_typable_valued
{
public:
	any(type_info const & type = library::type<void>(), void const * data = 0);
	template <typename Type>
	any(Type const & value) : any(library::type<Type>(), &value) { }
	any(typed_valued const & value) : any(value.type(), value.void_pointer()) { }
	~any();

	any(any const & value) : any((typed_valued const &)value) { }
	any & operator=(any const & value) { operator=((typed_valued const &)value); return *this; }

	using typable_valued::operator=;
	using typed_valued::reference;
	using typed_valued::pointer;

	void reset();

	virtual type_info const & type() const override final;
	virtual void const * void_pointer() const override final;
	virtual void * void_pointer() override final;

	// const returns nullptr if type is different
	virtual void const * void_pointer(type_info const & type) const override final;
	// non-const destroys old data if type is different
	virtual void * void_pointer(type_info const & type) override final;

protected:
	virtual void assign(void const * value) override final;
	virtual void assign(void const * value, type_info const & type) override final;

private:
	unsigned char _data[sizeof(void*)];
	type_info const * _type;
};


} // namespace library