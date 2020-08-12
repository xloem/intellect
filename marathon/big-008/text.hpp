#pragma once

#include "ref.hpp"

namespace sym
{
	symbol(text);
}

struct text : public ref
{
	text(std::string data, bool unique = true)
	: ref({{sym::is,sym::text}}, data)
	{
		if (unique) {
			auto & storage = text::storage();
			auto result = storage.find(data);
			if (result != storage.end()) {
				*this = result->second;
				return;
			}
			*this = ref({{sym::is,sym::text}}, data);
			storage.emplace(data, *this);
		} else {
			*this = ref({{sym::is,sym::text}}, data);
		}
	}
	text(ref symb0l)
	: ref(symb0l)
	{
		this->set(sym::is,sym::text);
	}
	std::string & data() { return ref::template data<std::string>(); }
	std::string const & data() const { return ref::template data<std::string>(); }

private:
	std::unordered_map<std::string, ref> & storage()
	{
		static std::unordered_map<std::string, ref> lookup;
		return lookup;
	}
};
