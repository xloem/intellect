#pragma once

#include <any>
#include <functional>
#include <memory>
#include <unordered_map>

template <class t>
using il = std::initializer_list<t>;

struct basic_concept;
using basic_ref = std::shared_ptr<basic_concept>;

struct basic_concept {
	std::unordered_multimap<basic_ref,basic_ref> refs;
	std::any data;
};

basic_ref create_basic(il<std::pair<basic_ref,basic_ref>> refs = {}, std::any data = {});
#define sym(name) basic_ref name = create_basic({}, std::string(#name));

namespace syms {
	sym(nothing);
	sym(is);
	sym(what);
}

// note: link objects could be temporary data refs where data is an iterator

class ref : public basic_ref
{
public:
	ref (il<std::pair<basic_ref,basic_ref>> refs = {}, std::any data = {})
	: basic_ref(new basic_concept{{refs.begin(), refs.end()}, data})
	{ }

	ref(basic_ref const & basic)
	: basic_ref(basic)
	{ }

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
		return *this != syms::nothing;
	}

	ref operator[](ref what) const
	{
		auto result = (**this).refs.find(what);
		if (result == (**this).refs.end()) {
			return syms::nothing;
		} else {
			return result->second;
		}
	}

	ref & operator[](ref what)
	{
		auto result = (**this).refs.find(what);
		if (result == (**this).refs.end()) {
			result = (**this).refs.insert({what, syms::nothing});
		}
		return *(ref*)&result->second;
	}
};

basic_ref create_basic(il<std::pair<basic_ref,basic_ref>> refs, std::any data)
{
	return ref(refs, data);
}

/*&
class text : public ref
{
public:
	text(std::string data) : ref({}, data) { }
	std::string & data() { return ref::data<std::string>(); }
};

namespace syms {
	sym(inputs);
	sym(outputs);
}

// we're going to have to annotate the arguments; we just want to not have to always
// pull them out of the context object and push them in, inside.
//
#define cxxhabit(
class cxxhabit : public ref
{
public:
	cxxhabit(text output_name, text input1_name, std::function<ref(ref)> func)
	: ref({}, std::function<void(ref)>(
	cxxhabit(il<text> outputs, il<text> inputs, std::function<result(parameters...)> func)
	{
		content->data = std::function<void(ref)>([=](ref context){

			(parameters...)
		});
	}
};
*/
