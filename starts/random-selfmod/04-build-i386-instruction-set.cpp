#include "capstone.i"
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <unistd.h>

int main()
{
        Capstone capstone;
        //std::ofstream out("i386.opcodes.bin");
	FILE* out = fopen("i386.opcodes.bin", "wb");

	uint8_t data[6] = {0,0,0,0,0,0};

	uint8_t offset = 0;
	uint8_t size = offset + 1;

	while (true) {
		if (size == 2) { std::cout << (int)data[0]*256+(int)data[1] << "/65536" << std::endl; fflush(out); }
                if (capstone.check(data, size)) {
			fwrite(&size, sizeof(size), 1, out);
			fwrite(&data, sizeof(data), 1, out);
	                //out.write((char const *)&size, sizeof(size));
                        //out.write((char const *)&data, sizeof(data));
			//std::cout << (int)size << " " << (int)data[offset] << std::endl;
                } else if (size < sizeof(data)) {
			++ offset;
			++ size;
			continue;
		}
		++ data[offset];
		while (data[offset] == 0) {
			if (offset == 0) { return 0; }
			-- offset;
			-- size;
			++ data[offset];
		}
	}

	/*
	uint32_t little_endian = 0x85d8558b;
	assert(*(uint8_t*)&little_endian == 0x8b);

        union {
                struct {
                        uint32_t loword;
                        uint16_t hiword;
                };
                uint8_t data[6];
        } opcode;
        opcode.loword = 0;
        do {
		lowordagain:
                for (uint8_t size = 1; size < 4; ++ size) {
                        if (capstone.check(opcode.data, size)) {
                                out.write((char const *)&size, sizeof(size));
                                out.write((char const *)&opcode.data, 6);
				loword +=
					// for a size of 1, we want to add 1
				goto next;
                        }
                }
                opcode.hiword = 0;
                do {
                        for (uint8_t size = 5; size < 6; ++ size) {
                                if (capstone.check(opcode.data, size)) {
                                        out.write((char const *)&size, sizeof(size));
                                        out.write((char const *)&opcode.data, 6);
					break;
                                }
                        }
                        ++ opcode.hiword;
                } while (opcode.hiword != 0);
                ++ opcode.loword;
        } while (opcode.loword != 0);
	*/
}
