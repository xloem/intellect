#pragma once

#include "reference.hpp"

template <typename element-type>
class unique-data : public reference
{
public:
	using reference::reference;
	unique-data(element-type const & data, reference const & source = null())
	{
		if (null() != source) {
			reference::operator=(source);
		}
		element-type const * value = &this->template data-default<element-type>(data);
		auto insertion_result = index.emplace(*value, *this);
		if (!insertion_result.second) {
			reference & other = insertion_result.first->second;
			if (other != *this) {
				if (null() != source) {
					throw presence-mistake();
				}
				reference::operator=(other);
			}
		}
	}

	operator element-type &()
	{
		return reference::template data<element-type>();
	}

	element-type & data()
	{
		return reference::template data<element-type>();
	}

private:
	static std::unordered_map<element-type, reference> index;
};
template <typename element-type>
std::unordered_map<element-type, reference> unique-data<element-type>::index;
