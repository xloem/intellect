#include <iostream>
#include "touch.hpp"

class main_node : public node
{
public:
	main_node() { touch_behavior = main_node::touch; }

	static reference touch(reference _self, reference arg)
	{
		main_node & self = *(main_node*)_self.value();

		self.final_result = SUCCESS();

		node n;
		self.handle("node", n._test());

		reference r(n);
		self.handle("reference", r._test());

		return self.final_result;
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
