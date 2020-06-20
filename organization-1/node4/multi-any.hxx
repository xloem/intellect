
#define ___STATIC_
#define ___EXTERN_ extern
#pragma once

#include <experimental/any>
	namespace std { using experimental::any; using experimental::any_cast; }
#include <typeinfo>
#include <unordered_map>


#undef ___STATIC_
#undef ___EXTERN_
#define ___CLASSNAME_ multi__any
#define ___CLASSNAMESTR_ "multi-any"
#define ___STATIC_ static
#define ___EXTERN_ static
class multi__any
//# 10 "multi-any.hpp"
{
public:
	template <typename T>
	T & get(T default__value = {})
	{
		auto result = data.emplace(&typeid(T), default__value);
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

#define ___CLASSNAME__ hash_type_info_pointer
#define ___CLASSNAMESTR__ "hash_type_info_pointer"
	struct hash_type_info_pointer {
//# 70 "multi-any.hpp"
		size_t operator()(std::type_info const * const & to__hash) const
		{
			return to__hash->hash_code();
		}
	};
#undef ___CLASSNAME__
#undef ___CLASSNAMESTR__
//# 75 "multi-any.hpp"

	std::unordered_map<std::type_info const *, std::any> data;
};
#undef ___CLASSNAME_
#undef ___CLASSNAMESTR_
#undef ___STATIC_
#define ___STATIC_
#undef ___EXTERN_
#define ___EXTERN_ extern
