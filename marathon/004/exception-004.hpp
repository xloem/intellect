#include "dump.hpp"
#include "ref.hpp"

namespace sym {
	symbol(todo);
	symbol(during);
	symbol(within);
	symbol(assumed_present);
	symbol(assumed_absent);
	symbol(assumed_is);
	symbol(assumed_has);
	symbol(actually_present);
	symbol(actually_absent);
	symbol(actually_is);
	symbol(actually_has);
}

#include <stdexcept>
class exception : public ref, public std::runtime_error
{
public:
	// solutions to good dumps:
	//  could add operator- to ref to remove 'what'
	//  could add depth attribute to dumping
	exception(il<std::pair<ref,ref>> refs)
	: ref(refs, std::make_any<std::string>()), runtime_error(dump(as<ref>()))
	{
		ref::data<std::string>() = runtime_error::what();
	}
};
