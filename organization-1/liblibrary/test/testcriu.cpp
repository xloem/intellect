#include <library/criu.hpp>

#include <library/string.hpp>

#include <filesystem>

#include "test.hpp"

using namespace library;

int main()
{
	string data = "hello";
	criu stasis;

	bool restored;
	char const * dumpname = stasis.dump(restored, "test", "testcriu-files");
	string filename = "testcriu-files/" + string(dumpname) + "/hello-file";
	if (restored) {
		stderr::line("stderr on restored process");
		stdout::line("stdout on restored process");
		worry(data != "hello", "'hello' data restored to '" + data + "'");
		data = string::file(filename);
		worry(data != "hello", "'hello' file data restored to '" + data + "'");
		data = "how's it going";
		data.to_file(filename);
		return 0;
	}

	data.to_file(filename);

	stasis.restore(dumpname, "testcriu-files"); // enters `if (restored) { }` above and waits for return

	data = string::file(filename);
	worry(data != "how's it going", "'how's it going' file data restored to '" + data + "'");

	std::filesystem::remove_all("testcriu-files");

	stderr::line("criu passed test");
}
