#include "reference.hpp"

template <typename element-type>
class unique-data : public reference
{
public:
	using reference::reference;
	unique-data(element-type const & data, reference const & source = null())
	{
		if (source != null()) { reference::operator=(source); }
		if (index.count(data)) {
			(*(reference*)this) = index[data];
		} else {
			auto & data = this->template data-default<element-type>(data);
			if (index.contains(data)) {
				throw presence-mistake();
			}
			index[data] = *this;
		}
	}

	operator element-type &()
	{
		return this->template data<element-type>();
	}

	element-type & data()
	{
		return this->template data<element-type>();
	}

private:
	static std::unordered_map<element-type, reference> index;
};
