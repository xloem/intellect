#include <cassert>
#include <initializer_list>
//#include <library/assert.hpp>
#include <library/string.hpp>
//#include <library/heapvector.hpp>
//#include <vector>
//#include <expressions/expressions.hpp>
#include <expressions/syntax_operator.hpp>
#include <expressions/syntax_operator_virtual_unified_by_value.implementation.cpp>
//using namespace std;
using namespace library;
using namespace expressions;

#include <any>
#include <memory>
#include <typeinfo>

class operator_printer : public syntax_operator_virtual_unified_by_value<operator_printer>
{
public:
	template <typename T>
	operator_printer(T value)
	: text(value) { }

	operator string const & () const
	{
		return text;
	}
protected:

	operator_printer syntax_operate(syntax_operator_identifier identifier, std::initializer_list<operator_printer> parameters) override
	{
		return string(operators[identifier].name) + "(" + text + ", " + string(parameters, string(", ")) + ")";
	}

private:
	string text;
};

class expression_part : public string::stringable
{
public:
	virtual string to_string() = 0;
};

class literal : public expression_part
{
public:
	template <typename T>
	literal(T value)
	: data(value), text(value) { }

	virtual string to_string()
	{
		return text;
	}

	std::any data;
	string text;
};



class operator_tracker : public syntax_operator_virtual_unified_by_value<operator_printer>
{
public:
	/*template <typename T>
	operator_tracker(T value)
	: data(value), text(value) { }*/

	/*
	operator string const & () const
	{
		return text;
	}
	*/
private:
	syntax_operator & op;
	std::shared_ptr<expression_part> parts;
};


int main()
{
	// slow to compile on arm64
	
	stdout::line("Hello, world!");

	operator_printer one = 1;
	operator_printer two = 2;
	operator_printer three = 3;

	stdout::line( (one("a","number") + two/"sedge") * 3 );


	/*
	// to do this all we need is the templates with operators
	// not the implementations of their fucntions that use the enums
	expression_templated<literal, int const> int1 = 1;
	expression_templated<literal, int const> int2 = 2;
	auto sum = int1 + int2;
	*/
	//stdout::line(sum);
	//stdout::line(typeid(sum).name());

	/*
	vector<any> arguments, verify;
	arguments.emplace_back(1);
	arguments.emplace_back(2);
	arguments.emplace_back(3);
	// please use a arrays of any's rather than pasting
	// every operator and type in, to test all types

	assert(operators::plus_any<int>.solve(arguments, 0)[0] == 5);
	verify = arguments;
	assert(operators::plus_any<int>.solve(arguments, 0) == verify);
	assert(operators::plus_any<int>.solve(arguments, 1) == verify);
	assert(operators::plus_any<int>.solve(arguments, 2) == verify);

	arguments[0] = true;
	assert(operators::equal_to_any<int>.solve(arguments, 0)[0] == false);
	arguments[0] = true;
	assert(operators::equal_to_any<int>.solve(arguments, 1)[1] == verify[2]);
	arguments[1] = verify[1];
	assert(operators::equal_to_any<int>.solve(arguments, 2)[2] == verify[1]);

	cerr << "Success" << endl;
	*/
}
