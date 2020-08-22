#pragma once

#include "ref.hpp"

namespace sym
{
	symbol(data);
	symbol(unique);
}

#define self (*this)

template <class> struct data;
template <class Type>
il<il<ref>> assumes_has<data<Type>> = {
	{sym::is, data<Type>::type()},
	{sym::is, sym::data}
};

template <class Type>
struct data : public ref
{
	// replacing assumes_has with this norm resolve static definition issues and provides greater flexibility
	static ref model() { return ref({{sym::is, data<Type>::type},{sym::is, sym::data}}); }

	// this is a function because i haven't understood the static initialization order of templates to make it not need to be
	static ref type() { static ref static_ref({{sym::is,sym::data}}, std::string(typeid(Type).name())); return static_ref; }

	data(Type data, bool unique = true)
	: ref(sym::nothing)
	{
		if (unique) {
			auto & storage = data::storage();
			auto result = storage.find(data);
			if (result != storage.end()) {
				*this = result->second;
				return;
			}
			// fix: lock around access to storage
			self = ref({{sym::is,type()},{sym::is,sym::unique},{sym::is,sym::data}}, data);
			storage.emplace(data, self);
		} else {
			self = ref({{sym::is,type()},{sym::is,sym::data}}, data);
		}
	}
	/*
	data(ref other, Type data, bool unique)
	: ref(other)
	{
		//adding data to other refs invovles a lot of checks
		// if (unique) {
		// 	// fix: lock around access to storage
		// 	auto & storage = data<Type>::storage;
		// 	auto result = storage.find(data);
		// 	if (result != storage.end()) {
		// 		// fix: THROW ERROR IF 'other' PROVIDED
		// 		*this = result->second;
		// 		return;
		// 	}
		// 	// fix: DO NOT SET IF ALREADY SET
		// 	self.add(sym::is, data<Type>::type);
		// 	self.add(sym::is, sym::unique);
		// 	std::any & wrapped_data = self.template data<std::any>();
		// 	// fix: CHECK AND THROW ERROR
		// 	//		if data already set
		// 	wrapped_data = data;
		// 	// fix: CHECK AND THROW ERROR
		// 	//		if data already present
		// 	storage.emplace(data, self);
		// } else {
		// 	// fix: DO NOT SET IF ALREADY SET
		// 	self.add(sym::is, data<Type>::type);
		// 	self.add(sym::is, sym::unique);
		// 	std::any & wrapped_data = self.template data<std::any>();
		// 	// fix: CHECK AND THROW ERROR
		// 	//		if data already set
		// 	wrapped_data = data;
		// }
	}
	*/

	bool unique() { return self.has(sym::is, sym::unique); }

	Type & get() { return ref::template data<Type>(); }
	Type const & get() const { return ref::template data<Type>(); }

protected:
	// i tried placing an .as<> call in a constructor here, to run verifications, but the assumes_has template isn't defined yet when it is used statically.  templates maybe produce more complexity to static ordering of definitions.  moving the constructor definition outside of this class declaration did not help.
	// 		^-- this is reason to place assumes_has within the class declaration.  that likely simplifies annotation.
	data(ref symb0l) // TODO: change .as and verify type using model() to resolve static definition order
	: ref(symb0l)
	{ }

private:
	// as above, this is likely a function because i haven't understood the static initialization order of templates to make it not need to be
	static std::unordered_map<Type, ref> & storage()
	{
		static std::unordered_map<Type, ref> lookup;
		return lookup;
	}
};



#undef self
