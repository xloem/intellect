
#include "loaded_relevance.hpp"

#include <cstring>
#include <sys/mman.h>
#include <map>
#include <iostream>


#include <dlfcn.h>
#include <link.h>


std::map<uint8_t*,uint8_t*> segments;
// need an allocator that copies the whole segment.

// it could be nice if function-calls had entry-points as well
// as data.
// but now for just making-run, I guess we can try to shore
// up quick.

template <typename A>
Data memory_for(A * head, bool copy) {
	auto it = segments.upper_bound((uint8_t*)head);
	-- it;

	/*
	Dl_info info;
	//const ElfW(Sym) * sym;
	//dladdr1((void*)head, &info, (void**)&sym, RTLD_DL_SYMENT);
	link_map * linkmap;
	dladdr1((void*)head, &info, (void**)&linkmap, RTLD_DL_LINKMAP);
	std::cout << " prev-addr = " << std::hex << linkmap->l_prev->l_addr << std::endl;
	std::cout << " addr = " << std::hex << linkmap->l_addr << std::endl;
	std::cout << " next-addr = " << std::hex << linkmap->l_next->l_addr << std::endl;

	size_t segment_size = it->second - it->first;
	//size_t symbol_size = sym->st_size;
	//std::cout << sym->st_name << " length = " << std::hex << symbol_size << std::endl;
	std::cout << " offset = " << ((uint8_t*)head - it->first) << std::endl;
	*/
	auto vecsize = it->second - (uint8_t*)head;

	uint8_t * newseg = copy ? new uint8_t[it->second - it->first] : 0;
	auto offset = (uint8_t*)head - it->first;
	uint8_t * addr = copy ? newseg + offset : (uint8_t*)head;

	uint8_t* base = (uint8_t*)((uintptr_t)addr & (uintptr_t)~0xfff);
	int success = mprotect(base, vecsize + (addr - base), PROT_READ | PROT_WRITE | PROT_EXEC);
	if (success) { perror("mprotect"); exit(success); }

	shadow_allocator<uint8_t>::shadowAddress(addr, vecsize);
	if (copy) {
		std::memcpy(newseg, it->first, it->second - it->first);
	}
	Data block(addr, vecsize);
	//block.reserve(vecsize);
	//block.assign((uint8_t*)head, (uint8_t*)head + vecsize);
	/*
	memcpy(block.data(), (void*)head, block.size());
	//*/
	return block;
}

int handle_phdr(struct dl_phdr_info *info, size_t size, void *data)
{
	auto name = info->dlpi_name;
	//std::cout << name << ":" << std::endl;
	for (size_t i = 0; i < info->dlpi_phnum; ++ i) {
		auto & phdr = info->dlpi_phdr[i];
		auto addr = (uint8_t*)(info->dlpi_addr + phdr.p_vaddr);
		auto size = phdr.p_memsz;
		if (phdr.p_type == 1) {//< 0x10) {
			segments[addr] = addr + size;
		}
		//std::cout << std::hex << "  " << addr << " - " << (addr + size) << " " << phdr.p_type << std::endl;
	}
	return 0;
}

int main()
{
	dl_iterate_phdr(handle_phdr, 0);

	data["example-behavior"] = std::move(memory_for(example_behavior, true));
	auto pos = data["example-behavior"].data();
	data["copy-data"] = std::move(memory_for(copy_data, false));
	data["find-relevant-string"] = std::move(memory_for(find_relevant_string, false));
	data["core"] = std::move(memory_for(core, false));
	data["run"] = std::move(memory_for(run, false));
	data["system-printf"] = std::move(memory_for(printf, false));

	// steps we need to find: copy-data call-function
	// can we add abstraction?

	// we want a concept associated with the goal or context
	// to lead us to a concept needed to accomplish it.

	contexts["current"].relevance["goal"] = "copy-example-behavior-and-run-the-copy";
	
	// at some point we want a context that has "example-behavior" in "input"
	// and "new-behavior" or whatever in "output"
	// and we want to run "copy-data" on them.  "copy-data" can go in "behavior"

	// we then want to run "run" on "new-behavior", but this can be done manually for now.
	
	//         v--- contains word 'behavior' as substring
	contexts["available-behaviors"].relevance["1"] = "example-behavior";
	contexts["available-behaviors"].relevance["2"] = "copy-data";
	contexts["available-behaviors"].relevance["3"] = "find-relevant-string";
	contexts["available-behaviors"].relevance["4"] = "core";
	contexts["available-behaviors"].relevance["5"] = "run";

	contexts["copy-output"].relevance["name-idea"] = "new-";

	contexts["current"].relevance["behavior"] = "example-behavior";
	//core(contexts["current"]);
	run(contexts["current"]);
	contexts["current"].relevance["behavior"] = "core";
	run(contexts["core"]);
}
