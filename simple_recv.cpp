// receives information as a text line on stdin
// works on StringValue 'text'

#include <opencog/atomspace/Handle.h>
#include <iostream>

extern "C" void simple_recv(opencog::Handle data)
{
	std::string s(4096);
	std::cin.getline(&s[0], s.size(), std::endl);
	opencog::Handle text = data.getAtomSpace()->add_node(opencog::STRING_VALUE, "text");
	data.setValue(text, new opencog::StringValue(s));
}
