#pragma once

#include "reference.hpp"
#include "context.hpp"
#include "unique-data.hpp"

// we could use unique-data links as kinds in the context to our references

template <typename element-type>
class contextual-data : public reference
{
public:
	DECLARE kind-data;

	/*
	contextual-data(reference const & other, reference const & context = context::get(context::default-category()))
	: reference(other), context(context)
	{
		try {
			unique-data data(other);
			(*this) = reference();
		} catch (reference const &) {
			
		}
		verify-inserted();
	}
	*/
	contextual-data(element-type const & data, reference const & context = context::get(context::default-category()))
	{

	}
	/* copied from unique-data
	contextual-data(element-type const & data, reference const & source = null())
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
	*/
private:
	private void verify-inserted() const
	{
		// throws if data not present.
		element-type const * value = &data();

		reference context = this->context;
		context.kind-get(kind-data());
		kind-data.kind-get
	}

	reference context;
	reference data;
};
//template <typename element-type>
//std::unordered_map<element-type, reference> contextual-data<element-type>::index;
