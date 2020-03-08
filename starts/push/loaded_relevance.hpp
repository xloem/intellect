#pragma once

#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
struct shadow_allocator
{
	typedef T value_type;

	static void shadowAddress(T * address, size_t size)
	{
		expected_shadow_address = address;
		expected_shadow_size = size;
	}

	/*
	static void shadowCopyFromSegment(void * address, void * segment)
	{
		// does this work? is there a point?
		// notably, if a vector is copied out of a segment, the segment won't be copied a second time, unless the copier calls this again.
	}
	*/
	   
	shadow_allocator() = default;
	template <class U> constexpr shadow_allocator (const shadow_allocator <U>&) noexcept {}
	       
	[[nodiscard]] T* allocate(std::size_t n)
	{
		if (expected_shadow_address && n == expected_shadow_size) {
			auto address = expected_shadow_address;
			shadowed_addresses.insert(address);
			expected_shadow_address = 0;
			return address;
		}

		if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
			throw std::bad_alloc();
		}
		if (auto p = static_cast<T*>(std::malloc(n*sizeof(T)))) {
			return p;
		}
		
		throw std::bad_alloc();
	}

	void deallocate(T* p, std::size_t) noexcept
	{
		auto shadow = shadowed_addresses.find(p);
		if (shadow != shadowed_addresses.end()) {
			shadowed_addresses.erase(shadow);
			return;
		}
		std::free(p);
	}

private:
	static thread_local size_t expected_shadow_size;
	static thread_local T * expected_shadow_address;
	static thread_local std::unordered_multiset<T*> shadowed_addresses;
};
template <class T, class U>
bool operator==(const shadow_allocator <T>&, const shadow_allocator <U>&) { return true; }
template <class T, class U>
bool operator!=(const shadow_allocator <T>&, const shadow_allocator <U>&) { return false; }
template<typename T>
thread_local size_t shadow_allocator<T>::expected_shadow_size = 0;
template<typename T>
thread_local T* shadow_allocator<T>::expected_shadow_address = 0;
template<typename T>
thread_local std::unordered_multiset<T*> shadow_allocator<T>::shadowed_addresses;

#include <sys/mman.h>
class Data
{
public:
	Data()
	: shadowed(false),
	  length(0),
	  pointer(0)
	{ }
	template <typename T>
	Data(T * shadow, size_t size)
	: shadowed(true),
	  length(size * sizeof(T)),
	  pointer((uint8_t*)shadow)
	{
		init();
	}
	Data(Data const & other)
	: shadowed(false),
	  length(other.size()),
	  allocated(length),
	  pointer(allocated.data())
	{
		std::memcpy(pointer, other.data(), size());
		init();
	}
	Data(Data && other)
	: shadowed(other.shadowed),
	  length(other.length),
	  allocated(std::move(other.allocated)),
	  pointer(other.pointer)
	{
		other.length = 0;
		other.pointer = 0;
		init();
	}
	virtual Data & operator=(Data const & other)
	{
		shadowed = false;
		length = other.size();
		allocated.resize(length);
		pointer = allocated.data();
		std::memcpy(pointer, other.data(), other.size());
		init();
	}
	virtual Data & operator=(Data && other)
	{
		shadowed = other.shadowed;
		length = other.size();
		pointer = other.pointer;
		allocated = std::move(other.allocated);
		other.pointer = 0;
		init();
	}
	virtual uint8_t & operator[](size_t idx)
	{
		return pointer[idx];
	}
	virtual uint8_t * data()
	{
		return pointer;
	}
	virtual uint8_t const * data() const
	{
		return pointer;
	}
	virtual size_t size() const
	{
		return length;
	}
private:
	bool shadowed;
	size_t length;
	std::vector<uint8_t> allocated;
	uint8_t * pointer;

	void init()
	{
		uint8_t* base = (uint8_t*)((uintptr_t)pointer & (uintptr_t)~0xfff);
		int success = mprotect(base, length, PROT_READ | PROT_WRITE | PROT_EXEC);
		if (success) { perror("mprotect"); exit(success); }
	}
};

class Set
{
public:
	virtual size_t count(char const * key)
	{
		return data.count(key);
	}
	virtual size_t count(std::string const & key)
	{
		return data.count(key);
	}
	virtual void insert(char const * key)
	{
		data.insert(key);
	}
	virtual void insert(std::string const & key)
	{
		data.insert(key);
	}
	virtual void clear()
	{
		data.clear();
	}
	virtual size_t size()
	{
		return data.size();
	}
	virtual decltype(auto) find(std::string const &key) -> auto
	{
		return data.find(key);
	}
	virtual decltype(auto) find(char const *key) -> auto
	{
		return data.find(key);
	}
	virtual decltype(auto) begin() -> auto
	{
		return data.begin();
	}
	virtual decltype(auto) end() -> auto
	{
		return data.end();
	}
private:
	std::unordered_set<std::string> data;
};

struct Context
{
	std::unordered_map<std::string, std::string> relevance;

	virtual Context & peer(char const *name);
	//virtual String & get(char const *name);
	virtual Data & peer_data(char const *name);
};

extern std::unordered_map<std::string, Set> sets;
extern std::unordered_map<std::string, Data> data;
extern std::unordered_map<std::string, Context> contexts;

void example_behavior(Context & context);
void copy_data(Context & context);
void find_relevant_string(Context & context);
void run(Context & context);
void core(Context & context);
