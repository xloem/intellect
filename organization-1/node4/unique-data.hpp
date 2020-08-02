#pragma once

#include "reference.hpp"

template <typename element-type>
class unique-data : public reference
{
public:
	unique-data(reference const & other)
	: reference(other)
	{
		verify-inserted();
	}
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
					// merging might be appropriate here
					throw presence-mistake();
				}
				reference::operator=(other);
			}
		}
	}

	operator element-type &()
	{
		return data();
	}

	operator element-type const &() const
	{
		return data();
	}

	element-type & data()
	{
		return reference::template data<element-type>();
	}

	element-type const & data() const
	{
		return reference::template data<element-type>();
	}

protected:
	void reseat(reference const & other)
	{
		reference::reseat(other);
		
		verify-inserted();
	}

private:
	void verify-inserted() const
	{
		// throws if data not present.
		element-type const * value = &data();
		auto insertion_result = index.emplace(*value, *this);
		if (!insertion_result.second) {
			reference & other = insertion_result.first->second;
			if (other != *this) {
				// merging I suppose would be appropriate here
				throw presence-mistake();
			}
		}
	}

	static std::unordered_map<element-type, reference> index;
};
template <typename element-type>
std::unordered_map<element-type, reference> unique-data<element-type>::index;
