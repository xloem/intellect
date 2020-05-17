#includeu "puzzle.hpp"

template <typename Number>
class three_way_number_puzzle : public puzzle_connection<Number,std::vector<std::shared_ptr<Part>>,Number>
{ 
public:
	public three_way_number_puzzle(std::string name, std::function<Number(Number,Number)> down, std::function<Number(Number,Number)> up_left, std::function<Number(Number,Number)> up_right)
	: name(name), directions{down, up_left, up_right}
	{ }
	public three_way_number_puzzle(three_way_number_puzzle const & other)
	: puzzle_connection(other), namee(other.name), directions(other.directions)
	{ }
	virtual std::string name() { return name; }
	virtual std::pair<size_t,size_t> number() { return 3; }
	virtual Number guess(size_t part) {
		return directions[part](*parts[(part+1)%number()],*(parts[(part+2)%number()]);
	}
private:
	std::string name;
	std::function<Number(Number,Number)> directions[3];
};

#include <cmath>

three_way_number_puzzle<double> sum("sum",
	[](double b, double c)->double{
		return b + c;
	}, [](double c, double a)->double{
		return a - c;
	}, [](double a, double b)->double{
		return a - b;
	}
);
three_way_number_puzzle<double> mul("mul",
	[](double b, double c)->double{
		return b * c;
	}, [](double c, double a)->double{
		return a / c;
	}, [](double a, double b)->double{
		return a / b;
	}
);
three_way_number_puzzle<double> pow("pow",
	[](double b, double c)->double{
		return pow(b, c);
	}, [](double c, double a)->double{
		return pow(a, 1/c);
	}, [](double a, double b)->double{
		return log(a) / log(b);
	}
);
