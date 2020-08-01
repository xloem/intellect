#pragma once

#include <ciso646> // empty in c++ but sets up glibcxx define to detect abi

namespace std {
	template <class T> class initializer_list;

	template <class C> struct char_traits;
	template <class C> class allocator;
#if _GLIBCXX_USE_CXX11_ABI
	inline namespace __cxx11 {
#endif
	template <class C, class T, class A> class basic_string;
#if _GLIBCXX_USE_CXX11_ABI
	} // namespace __cxx11
	using namespace __cxx11;
#endif
	using string = basic_string<char, char_traits<char>, allocator<char>>;
}

namespace library {

template <typename T>
class heapvector
{
public:
	using element_type = T;

	heapvector();
	heapvector(element_type const * source);
	heapvector(unsigned long size);
	heapvector(unsigned long size, element_type const * source);
	heapvector(heapvector const & other);
	heapvector(heapvector && other);
	heapvector(std::initializer_list<element_type> const & items);
	~heapvector();

	// should provide a virtual function interface for compilation speed
	template <template<typename> class Container>
	heapvector(Container<heapvector> const & source, heapvector join = {})
	: heapvector()
	{
		bool continuing = false;
		for (auto & item : source) {
			if (continuing) {
				(*this) += join;
			} else {
				continuing = true;
			}
			(*this) += item;
		}
	}

	struct serialisable
	{
		// TODO: make a general template for serialising to a writable
		//       sequence, i suppose.
		virtual void serialise(heapvector & to) const = 0;
		// if we implement sequences then strings don't need to be the
		// same as vectors.  everything's a sequence.
		// brain issue expanded.
		// 	would that impact compilation speed?
		//
		// 		everything you add lowers compilation speed.
		// 		to make it fast, you include only what you need.
		// so, small reusable clases i suppose.

		// so, if sequence is a lightweight template with virtual fucntions
		//   then we can mostly use it withot including much at all.
	};
	heapvector(const serialisable &source);

	heapvector & operator=(heapvector const & other);
	heapvector & operator=(heapvector && other);
	element_type & operator[](unsigned long index);
	element_type const & operator[](unsigned long index) const;
	void push_back(element_type const & value);
	void push_back(element_type && value);

	unsigned long size() const;
	unsigned long reserved() const;
	void resize(unsigned long new_size);

	void splice(unsigned long index, unsigned long old_length, element_type const * source, unsigned long new_length);
	void splice(unsigned long index, unsigned long old_length, element_type const & source, unsigned long new_length);

	element_type & front();
	element_type & back();

	element_type * data();
	element_type const * data() const;

	element_type * begin();
	element_type * end();

	element_type const * begin() const;
	element_type const * end() const;

	unsigned long begin_index() const;
	unsigned long end_index() const;
private:
	void * storage;
};


} // namespace library
