#include <library/any.hpp>
#include <library/type_definition.cpp>

namespace library {

any::any(type_info const & type, void const * data)
: _type(&library::type<void>())
{
	assign(data, type);
}

any::~any()
{
	reset();
}

void any::reset()
{
	assign(nullptr, library::type<void>());
}

type_info const & any::type() const
{
	return *_type;
}

void const * any::void_pointer() const
{
	if (_type->size > sizeof(_data)) {
		return *(void const*const*)&_data;
	} else {
		return _data;
	}
}

void * any::void_pointer()
{
	if (_type->size > sizeof(_data)) {
		return *(void **)&_data;
	} else {
		return _data;
	}
}

void const * any::void_pointer(type_info const & type) const
{
	if (*_type != type) {
		return nullptr;
	}
	return void_pointer();
}

void * any::void_pointer(type_info const & type)
{
	if (*_type != type) {
		assign(nullptr, type);
	}
	return void_pointer();
}

void any::assign(void const * data)
{
	_type->assign(void_pointer(), data);
}

void any::assign(void const * data, type_info const & type)
{
	if (*_type == type) {
		assign(data);
		return;
	}
	if (*_type != library::type<void>()) {
		_type->destroy(void_pointer());
		if (_type->size > sizeof(_data)) {
			delete (unsigned char *)void_pointer();
		}
	}
	_type = &type;
	if (*_type != library::type<void>()) {
		if (_type->size > sizeof(_data)) {
			*(unsigned char**)&this->_data = new unsigned char[_type->size];
		}
		if (nullptr != data) {
			_type->construct_copy(void_pointer(), data);
		} else {
			_type->construct_default(void_pointer());
		}
	}
}

} // namespace library
