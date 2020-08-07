#pragma once

#include <any>
#include <functional>
#include <memory>
#include <unordered_map>

template <class t>
using il = std::initializer_list<t>;

using basic_ref = std::shared_ptr<struct basic_concept>;
basic_ref create(il<std::pair<basic_ref,basic_ref>> refs = {}, std::any data = {});

#define symbol(name) basic_ref name = create({}, std::string(#name));

namespace sym {
	symbol(nothing);
	symbol(is);
	symbol(what);
};

// note: link objects could be temporary data refs where data is an iterator
struct basic_concept {
	// although a map of steps might be faster,
	// it makes construction more verbose, and
	// produces a possible issue of accidentally
	// overwriting the sets on insertion
	std::unordered_multimap<basic_ref,basic_ref> refs;
	std::any data;
};
using basic_link = decltype(basic_concept::refs)::iterator;

class ref : public basic_ref
{
public:
	ref (il<std::pair<ref,ref>> refs = {}, std::any data = {})
	: basic_ref(new basic_concept{{refs.begin(), refs.end()}, data})
	{ }

	ref(basic_ref const & basic)
	: basic_ref(basic)
	{ }

	template <class t>
	t & as()
	{
		return static_cast<t&>(*this);
	}

	template <class t>
	t const & as() const
	{
		return static_cast<t const&>(*this);
	}

	ref & operator+=(il<std::pair<ref,ref>> refs)
	{
		(**this).refs.insert(refs.begin(), refs.end());
		return *this;
	}

	ref & operator-=(il<ref> refs)
	{
		for (auto item : refs)
		{
			(**this).refs.erase(item);
		}
		return *this;
	}

	template <typename t>
	t & data()
	{
		return *std::any_cast<t>(&(**this).data);
	}

	operator bool() const
	{
		return *this != sym::nothing;
	}

	basic_link getonelink(ref what) const
	{
		return (**this).refs.find(what);
	}

	basic_link getemptylink() const
	{
		return (**this).refs.find();
	}

	ref const get(ref what) const
	{
		auto result = getonelink(what);
		if (result == getemptylink()) {
			return sym::nothing;
		} else {
			return result->second;
		}
	}

	std::pair<basic_link,basic_link> getalllinks(ref what) const
	{
		return (**this).refs.equal_range(what);
	}

	std::unordered_set const getall(ref what) const
	{
		std::unordered_set result;
		auto range = getalllinks(what);
		for (auto & entry : range) {
			result.insert(entry.second);
		}
		return result;
	}

	void add(ref what, ref value)
	{
		// if(!what) { throw }
		// if(!value) { throw }
		(**this).refs.insert(std::pair<basic_ref,basic_ref>(what, value));
	}

	void set(ref what, ref value = sym::nothing)
	{
		if (!value) { value = what; }
		(**this).refs.insert(wipe(what), std::pair<basic_ref,basic_ref>(what, value));
	}

	basic_link wipe(ref what)
	{
		auto range = getalllinks(what);
		return (**this).refs.erase(range.first, range.second);
	}

	ref const operator[](ref what) const
	{
		return get(what);
	}

	/*

	ref & operator[](ref what)
	{
		auto result = (**this).refs.find(what);
		if (result == (**this).refs.end()) {
			result = (**this).refs.insert({what, sym::nothing});
		}
		return *(ref*)&result->second;
	}
	*/

	char const * dump(bool contents = true)
	{
		static thread_local std::string storage;
		static thread_local unsigned long long id = 0;
		static thread_local std::unordered_map<basic_ref, std::string> found;
		std::string result;
		if (found.count(*this)) {
			result = found[*this];
			contents = false;
		} else if (!(**this).refs.size()) {
			contents = false;
		}
		if (!result.size()) {
			std::any & data = (**this).data;
			++ id;
			if (data.has_value()) {
				if (data.type() == typeid(std::function<void(ref)>)) {
					result = std::to_string(id) + "<habit>";
				} else if (data.type() == typeid(std::string)) {
					result = this->data<std::string>();
				} else {
					result = std::to_string(id) + std::string("<") + data.type().name() + ">";
				}
			} else {
				result = std::to_string(id);
			}
			found[*this] = result;
		}
		if (contents) {
			result = "[" + result + ":";
			bool first = true;
			for (auto refs : (**this).refs) {
				if (!first) {
					result += ",";
				} else {
					first = false;
				}
				result += ((ref*)&refs.first)->dump(false);
				result += "=";
				result += ((ref*)&refs.second)->dump();
			}
			result += "]";
		}
		storage = result;
		return storage.c_str();
	}
};

basic_ref create(il<std::pair<basic_ref,basic_ref>> refs, std::any data)
{
	return ref((il<std::pair<ref,ref>> &)refs, data);
}
