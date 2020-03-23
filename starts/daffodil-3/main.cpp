#include "touch.hpp"

#include <iostream>
class main_node : public node
{
public:
	virtual reference touch(reference arg) override
	{
		final_result = SUCCESS();

		node n;
		handle("node", n._test());

		reference r(n);
		handle("reference", r._test());

		return final_result;
	}

private:
	virtual void handle(char const * name, reference result)
	{
		std::cout << name << ": ";
		if (result != SUCCESS()) {
			std::cout << "non-success";
			final_result = result;
		} else {
			std::cout << "success";
		}
		std::cout << std::endl;
	}

	reference final_result;
};

int main()
{
	main_node the_main;
	if (the_main() == node::SUCCESS()) {
		std::cout << "Success." << std::endl;
		return 0;
	} else {
		std::cout << "Failure." << std::endl;
		return -1;
	}
}
