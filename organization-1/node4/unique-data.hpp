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
			this->template default-data<element-type>() = data;
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
