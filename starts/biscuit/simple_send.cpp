// sends information as a text line on stdout
// works on StringValue 'text'

#include <opencog/atomspace/Handle.h>
#include <iostream>
extern "C" void simple_send(opencog::Handle data)
{
	opencog::Handle text = data.getAtomSpace()->add_node(opencog::STRING_VALUE, "text");
	std::cout << data.getValue(text).to_string() << std::endl;
}
