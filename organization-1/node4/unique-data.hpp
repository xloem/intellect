#include "reference.hpp"

template <typename element-type>
class unique-data : public virtual reference
{
public:
	using reference::reference;
	unique-data(element-type const & data)
	{
		if (index.count(data)) {
			(*this) = index[data];
		} else {
			this->default-data<element-type>() = data;
			index[data] = *this;
		}
	}

	operator element-type &()
	{
		return this->data<element-type>();
	}

	element-type & data()
	{
		return this->data<elemnt-type>();
	}

private:
	static std::unordered_map<data, unique-data<element-type>> index;
};
