#pragma once

#include <filesystem>
#include <iostream>
#include <unordered_map>

// Smooth proposal: (only here, in this file, until resolved.)
// 	Use repeated-experience to produce repeated-state.
// 	This facilitates test-passing and relevant structure.
//
// 		['test'ing is way to deter errors]
// 			[relevent structure is like smart-small]
// 	Store-and-load atm lets karl get sooner satisfied-experience in future.

namespace storeload {

static std::unordered_map<void*,uint64_t> pointers_by_pointer;
static std::unordered_map<uint64_t,void*> pointers_by_index;
static uint64_t max_pointer_index = 0;

class storing {
public:
	storing(std::string parent, std::string name)
	{
		filesystem::create_directories(parent);
		this->name = parent + "/" + name;
		stream.open(this->name + ".data");
	}
	template <typename T> store(T& data);

private:
	std::ostream stream;
	std::string name;
};

class loading {
public:
	storing(std::string parent, std::string name)
	{
		filesystem::create_directories(parent);
		this->name = parent + "/" + name;
		stream.open(this->name + ".data");
	}
	template <typename T> load(T& data);

private:
	std::istream stream;
	std::string name;
};

template <typename T>
void storing::store<std::shared_ptr<T>> (std::shared_ptr<T> & data)
{
	uint64_t index;
	if (!pointers_by_pointer.count(data.get())) {
		index = ++ max_pointer;
		pointers_by_pointer[data.get()] = index;
		pointers_by_index[index] = data.get();
	} else {
		index = pointers[data.get()];
		
	}
	stream << index;
	storing substoring(name, std::to_string(index));
	substoring.store(*data);
}

template <typename T>
void storing::load<std::shared_ptr<T>> (std::shared_ptr<T> & data)
{
	uint64_t index;
	stream >> index;
	if (!pointers_by_index.count(index)) {
		data.reset(new T());
		pointers_by_index[index] = data.get();
		pointers_by_pointer[data.get()] = index;

		loading subloading(name, std::to_string(index));
		subloading.load(*data);
	} else {
		data = pointers_by_index[index];
	}
}

template <typename K, typename V>
void loading::load<std::map<K,V>> (std::map<K,V> & data)
{
	size_t size;
	stream >> size;
	for (size_t i = 0; i < size; ++ i) {
		K key;
		V value;
		load(stream, key);
		load(stream, value);
		data.emplace(key, value);
	}
}

template <typename K, typename V>
void storing::store<std::map<K,V>> (std::map<K,V> & data)
{
	stream << data.size();
	for (auto & entry : data) {
		store(stream, entry.first());
		store(stream, entry.second());
		stream << std::endl;
	}
}

template <>
void storing::store<next_word::thought> (next_word::thought& data)
{
	stream << data;
}

template <typename T>
void storing::store<T> (T& data)
{
	stream << data;
}

template <typename T>
void loading::load<T> (T& data)
{
	stream >> data;
}

}
