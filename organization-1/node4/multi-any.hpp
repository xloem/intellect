#pragma once

#include <experimental/any>
	namespace std { using experimental::any; using experimental::any_cast; }
#include <typeinfo>
#include <unordered_map>
#include <vector>


class multi-any
{
public:
	template <typename T>
	T & get(T default-value = {})
	{
		auto result = data.emplace(&typeid(T), default-value);
		return *std::any_cast<T>(&result.first->second);
	}

	std::any set(std::any value)
	{
		std::any old;
		auto location = data.find(&value.type());
		if (location != data.end()) {
			old = std::move(location->second);
			location->second = value;
		} else {
			data.emplace(&value.type(), value);
		}
		return old;
	}

	template <typename T>
	void set(T value)
	{
		get<T>() = value;
	}

	template <typename T>
	T * pointer()
	{
		auto location = data.find(&typeid(T));
		if (location != data.end()) {
			return std::any_cast<T>(&location->second);
		} else {
			return nullptr;
		}
	}

	template <typename T>
	T const * pointer() const
	{
		auto location = data.find(&typeid(T));
		if (location != data.end()) {
			return std::any_cast<T>(&location->second);
		} else {
			return nullptr;
		}
	}

	template <typename T>
	bool has()
	{
		return pointer<T>();
	}

	size_t size()
	{
		return data.size();
	}

	std::vector<std::type_info const *> types()
	{
		std::vector<std::type_info const *> results;
		for (auto & index : data) {
			results.push_back(index.first);
		}
		return results;
	}

	struct hash_type_info_pointer {
		size_t operator()(std::type_info const * const & to-hash) const
		{
			return to-hash->hash_code();
		}
	};

	std::unordered_map<std::type_info const *, std::any> data;
};
