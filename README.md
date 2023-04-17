## Requeriments

 - CMake 3.16 or great
 - ninja build tool 1.10.0 or great

## Build steps

 - Create `build` directory with `mkdir build` and move to this (`cd build`)
 - Generate cmake project using ninja (or your preferred tool): `cmake ../ -G Ninja`
 - If you preffer gnu make try `cmake ../ -G 'Unix Makefile'` or replace to your preferred tool)
 - Launch ninja to build all of code: `ninja`
 - If all pass ok, the files `firmware.elf`, `firmware.elf.bin` and `firmware.elf.hex` appears in `build` directory
 - Additionally, you found the files `firmware.elf.map` and `firmware.elf.lst` containing memmory mapping and disassembler of the ELF file
