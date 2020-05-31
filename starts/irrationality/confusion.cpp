#include <any>
#include <array>
#include <iostream>
#include <random>
#include <vector>

#include "tools.hpp"
// TTY
// looping_log
// random_int(min, max)

using namespace std;

using Number = array<int, 3>;


class Numbers
{
public:
	Numbers(size_t length)
	{
		TTY::clear();
		numbers.resize(length);
		for (auto & span : numbers) {
			span.resize(length);
			for (auto & content : span) {
				content = random_int(0, length * length);
			}
		}
	}

	void display()
	{
		TTY::home();
		cout << endl << endl;
		for (auto y = 0; y < numbers.size(); ++ y) {
			for (auto x = 0; x < numbers[y].size(); ++ x) {
				auto number = int_to_string(numbers[y][x]);
				for (auto pad = number.size(); pad < 4; ++ pad) {
					cout << " ";
				}
				cout << number;
			}
			cout << endl;
		}
		cout << flush;
	}


	string int_to_string(int integer)
	{
		return to_string(integer);
	}
	int string_to_int(string integer)
	{
		return stoul(integer);
	}

	string int_to_coordinate(int integer)
	{
		return to_string(integer % numbers.size()) + "," + to_string(integer / numbers.size());
	}
	int coordinate_to_int(string coordinate)
	{
		auto index = coordinate.find(',');
		return stoul(coordinate) * numbers.size() + stoul(coordinate.substr(index));
	}

	int & get(int & coordinate)
	{
		while (coordinate < 0) { coordinate += size(); }
		coordinate = coordinate % size();
		return numbers[coordinate % numbers.size()][coordinate / numbers.size()];
	}
	int size() {
		return numbers.size() * numbers.size();
	}

	vector<vector<int>> numbers;
};

int main()
{
	Numbers numbers(4);

	looping_log<int> history(numbers.size());
	size_t history_index = 0;
	history[history_index] = random_int(0, numbers.size());

	while (true) {
		numbers.display();

		auto & number = numbers.get(history[history_index]);
		++ history_index;
		history[history_index] = number;

		number = number + random_int(-1, 1);
		// not a fan of reusable fucntions

	}
}
