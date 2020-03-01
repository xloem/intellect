#include <capstone/capstone.h>

class Capstone
{
public:
        Capstone()
        {
                if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
                        throw "oops";
                }
        }
        ~Capstone()
        {
                cs_close(&handle);
        }
        size_t check(std::vector<uint8_t> const & data)
        {
                count = cs_disasm(handle, data.data(), data.size(), (uint64_t)data.data(), 0, &insn);
                if (count) {
                        cs_free(insn, count);
                        return count;
                } else {
                        return 0;
                }
        }
private:
        csh handle;
        cs_insn * insn;
        size_t count;
};
