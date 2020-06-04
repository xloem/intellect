// part contains std::map keyed_parts, and std::any data

namespace std { template <> struct hash<reference> {
	size_t operator()(const reference & to_hash) const
	{
		return hash<shared_ptr<part>>()(to_hash);
	}
}; }

class part
{
private:
	friend class reference;
	//? vector<reference> ordered_parts;
	
	unordered_map<reference, reference> keyed_parts;

	any data;

	part(any data)
	: data(move(data))
	{ }
};

template <typename... parameter_types>
reference reference::operator()(parameter_types... parameters)
{
	return (function<reference(parameter_types...)>)(*this)(parameters...);
}

template <typename data_type>
reference::operator data_type &()
{
	if ((*this)->data.type() != typeid(data_type)) {
		// kind of data was assumed wrongly
		// not-kind to treat as something other than self
		throw kindness_mistake;
	}
	return *any_cast<data_type>(&(*this)->data);
}
