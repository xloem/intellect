#pragma once

#include "ref.hpp"

namespace sym
{
	symbol(data);
	symbol(unique);
}

#define self (*this)

template <class Type>
struct data : public ref
{
	static ref type({{sym::is,sym::data}});

	data(Type data, bool unique = true)
	: ref(sym::nothing)
	{
		if (unique) {
			// auto & storage = data<Type>::storage();
			auto result = storage.find(data);
			if (result != storage.end()) {
				*this = result->second;
				return;
			}
			// fix: lock around access to storage
			self = ref({{sym::is,data<Type>::type},{sym::is,sym::unique}}, data)
			storage.emplace(data, self);
		} else {
			self = ref({{sym::is,data<Type>::type}}, data)
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

	Type & data() { return ref::template data<Type>(); }
	Type const & data() const { return ref::template data<Type>(); }

private:
	static std::unordered_map<std::string, ref> storage;
};

#undef self
