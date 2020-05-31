#pragma once

using namespace std;

class TTY
{
private: static void put(int character)
	{
		cout << "\x1b\x5b" << (char)character;
	}
public:
	static void clear()
	{
		put(0x48); put(0x4a);
	}
	static void home()
	{
		put(0x48);
	}
};

template <typename T>
class looping_log : public vector<T>
{
public:
	looping_log(size_t size)
	{
		vector<T>::resize(size);
	}
	T & operator[](size_t & size)
	{
		while (size < 0) {
			size += this->size();
		}
		size = size % this->size();
		return vector<T>::operator[](size);
	}
	using vector<T>::size;
};

int random_int(int min, int max)
{
	static random_device rd;
	static mt19937 gen(rd());
	uniform_int_distribution<> distrib(min, max);
	return distrib(gen);
}
