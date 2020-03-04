#include <capstone/capstone.h>
#include <vector>

class Capstone
{
public:
        Capstone(int bits = 32)
        {
                cs_mode mode;
                if (bits == 32) { mode = CS_MODE_32; }
                else if (bits == 64) { mode = CS_MODE_64; }
                else { throw "oops"; }
                if (cs_open(CS_ARCH_X86, mode, &handle) != CS_ERR_OK) {
                        throw "oops";
                }
        }
        ~Capstone()
        {
                cs_close(&handle);
        }
        size_t check(std::vector<uint8_t> const & data)
        {
                return check(data.data(), data.size());
        }
        size_t check(uint8_t const * data, size_t size)
        {
                count = cs_disasm(handle, data, size, (uint64_t)data, 0, &insn);
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
