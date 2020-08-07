#include "ref.hpp"

namespace sym {
	symbol(during);
	symbol(within);
	symbol(ambiguous_with_internals);
	symbol(misinterpreted_as);
	symbol(instead_of);
}

class exception : public ref, public std::runtime_error
{
public:
	// solutions to good dumps:
	//  could add operator- to ref to remove 'what'
	//  could add depth attribute to dumping
	exception(il<std::pair<ref,ref>> refs)
	: ref(refs, std::string()), runtime_error(dump(true))
	{
		ref::data<std::string>() = runtime_error::what();
	}
};
