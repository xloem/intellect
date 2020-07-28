#include <library/type_definition.cpp>
#include <library/string.hpp>

#include <unordered_map>

//#include <iostream> // debugging

namespace library {

static type_info const * type_initializations[] = {
	&type<void>(),
	&type<void const>(),
	&type<  signed char>(),
	&type<  signed char const>(),
	&type<unsigned char>(),
	&type<unsigned char const>(),
	&type<  signed int>(),
	&type<  signed int const>(),
	&type<unsigned int>(),
	&type<unsigned int const>(),
	&type<  signed short>(),
	&type<  signed short const>(),
	&type<unsigned short>(),
	&type<unsigned short const>(),
	&type<  signed long>(),
	&type<  signed long const>(),
	&type<unsigned long>(),
	&type<unsigned long const>(),
	&type<  signed long long>(),
	&type<  signed long long const>(),
	&type<unsigned long long>(),
	&type<unsigned long long const>(),
};

typed_valued & typed_valued::operator=(typed_valued const & other)
{
	assign(other);
	return *this;
}

void typed_valued::assign(typed_valued const & other)
{
	if (this->type() != other.type()) {
		throw type_mismatch();
	}
	assign(other.void_pointer());
}

typable_valued & typable_valued::operator=(typed_valued const & other)
{
	assign(other);
	return *this;
}

void typable_valued::assign(typed_valued const & other)
{
	assign(other.void_pointer(), other.type());
}

typed_typable_valued & typed_typable_valued::operator=(typed_typable_valued const & other)
{
	typable_valued::operator=((typed_valued const &)other);
	return *this;
}

#if defined(TYPE_ALLOCATION_DEBUG)
allocation_debugger_t allocation_debugger;

struct allocation {
	type_info const * type;
	unsigned long id;
};
using allocation_storage = std::unordered_map<void const *, allocation>;
allocation_debugger_t::allocation_debugger_t()
: storage(new allocation_storage()), storage_2(new allocation_storage())
{ }

allocation_debugger_t::~allocation_debugger_t() noexcept(false)
{
	auto & allocations = *(allocation_storage*)storage;
	if (allocations.size()) {
		throw std::logic_error((library::string(allocations.size()) + " typed allocation leak(s)").std());
	}
	delete (allocation_storage*)storage_2;
	delete (allocation_storage*)storage;
}

void dump(allocation_storage & allocations)
{
	/*
	std::cerr << "dump: ";
	for (auto & entry : allocations) {
		std::cerr << "(" << entry.first << "," << entry.second.id << ")";
	}
	std::cerr << std::endl;
	*/
}

void allocation_debugger_t::allocate(void const * pointer, type_info const & type)
{
	static thread_local unsigned long tlid = 0;
	auto & allocations = *(allocation_storage*)storage;
	auto & deallocations = *(allocation_storage*)storage_2;
	dump (deallocations);
	if (allocations.count(pointer)) {
		throw std::logic_error("double construction");
	}
	auto & allocation = allocations[pointer];
	allocation.type = &type;
	//std::cerr << "allocating " << tlid << " at " << pointer << std::endl;
	allocation.id = tlid ++;
	dump (deallocations);
}

void allocation_debugger_t::access(void const * pointer, type_info const & type)
{
	auto & allocations = *(allocation_storage*)storage;
	auto & deallocations = *(allocation_storage*)storage_2;
	dump (deallocations);
	if (!allocations.count(pointer)) {
		if (deallocations.count(pointer)) {
			auto & deallocation = deallocations[pointer];
			throw std::logic_error(("use of "_s + pointer + " id=" + deallocation.id + " after destruction (" + type.name + ")").std());
		//} else {
		//	throw std::logic_error("use without construction");
		}
	} else {
		if (type.size > allocations[pointer].type->size) {
			throw std::logic_error("use as larger type than allocated");
		}
	}
	dump (deallocations);
}

void allocation_debugger_t::deallocate(void const * pointer, type_info const & type)
{
	access(pointer, type);
	auto & allocations = *(allocation_storage*)storage;
	auto & deallocations = *(allocation_storage*)storage_2;
	dump (deallocations);
	if (type != *allocations[pointer].type) {
		throw std::logic_error("destruction with wrong type");
	}
	auto & allocation = allocations[pointer];
	deallocations[pointer] = allocation;
	//std::cerr << "deallocating " << allocation.id << "/" << deallocations[pointer].id << " at " << pointer << std::endl;
	allocations.erase(pointer);
	dump (deallocations);
}

unsigned long allocation_debugger_t::id(void const * pointer, type_info const & type)
{
	access(pointer, type);
	auto & allocations = *(allocation_storage*)storage;
	return allocations[pointer].id;
}
#endif // TYPE_ALLOCATION_DEBUG


} // namespace library
