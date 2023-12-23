# Directory
IDE_ROOT  = /home/ohta/NextMiconIDE
PROJ      = /home/ohta/NextMiconIDE/Projects/sample
TEMPLATE  = TinyFPGA_BX/0.0.0

# Input Files
HARDWARE  = $(IDE_ROOT)/Template/$(TEMPLATE)/hardware/*.v \
			$(IDE_ROOT)/Library/*/*/*/*/*.v \
			micon/*.v

PCF       = $(IDE_ROOT)/Template/$(TEMPLATE)/hardware/*.pcf

START     = $(IDE_ROOT)/Template/$(TEMPLATE)/software/start.S
LINKER    = $(IDE_ROOT)/Template/$(TEMPLATE)/software/sections.lds

SOFTWARE  = $(IDE_ROOT)/Template/$(TEMPLATE)/software/*.cpp \
			$(IDE_ROOT)/Library/*/*/*/*/*.cpp \
			micon/*.cpp \
			software/*.cpp

INCLUDE_TEMPLATE = $(IDE_ROOT)/Template/$(TEMPLATE)/software
INCLUDE_LIBRARY  = $(IDE_ROOT)/Library
INCLUDE_FIRMWARE = micon

# Tools
CROSS     = riscv64-unknown-elf-

SIMU_HARD = simulation/*.v
SIMU_SOFT = .template/cpu/*.cpp .packages/*/*.cpp micon/*.cpp simulation/*.cpp

TOOLS = $(IDE_ROOT)/Template/$(TEMPLATE)/tools

###############################################################################

install:
	sudo apt update
	sudo apt install gcc-riscv64-unknown-elf yosys nextpnr-ice40 iverilog gtkwave python3
	pip install tinyprog

init:
	mkdir -p .build micon

test:
	echo TEST

###############################################################################

hardware: .build/hardware.bin
.build/hardware.json: $(HARDWARE)
	yosys -ql $@.log -p 'synth_ice40 -top hardware -json $@' $^
.build/hardware.asc: .build/hardware.json
	nextpnr-ice40 -ql $@.log --lp8k --package cm81 --asc $@ --pcf $(PCF) --json $^
.build/hardware.bin: .build/hardware.asc
	icetime -d lp8k -c 12 -mtr .build/hardware.rpt $^
	icepack $^ $@

software: .build/software.objdump .build/software.nm .build/software.bin
.build/software.elf: $(START) $(SOFTWARE)
	$(CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(LINKER),--strip-debug,-Map=.build/software.map,--cref \
			-O3 -ffreestanding -nostdlib \
			-I $(INCLUDE_TEMPLATE) \
			-I $(INCLUDE_LIBRARY) \
			-I $(INCLUDE_FIRMWARE) \
			-o $@ \
			$^
.build/software.objdump: .build/software.elf
	$(CROSS)objdump --demangle -D $^ > $@
.build/software.nm: .build/software.elf
	$(CROSS)nm --demangle --numeric-sort $^ > $@
.build/software.bin: .build/software.elf
	$(CROSS)objcopy -O binary $^ /dev/stdout > $@

upload: hardware software
#	tinyprog -p .build/hardware.bin -u .build/software.bin
	powershell.exe -c \
	"tinyprog -p \$$env:WSLHome\NextMiconIDE\Projects\sample\.build\hardware.bin \
	          -u \$$env:WSLHome\NextMiconIDE\Projects\sample\.build\software.bin"

###############################################################################

.build/simu_software.elf: $(START) $(SOFTWARE)
	$(CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(LINKER),--strip-debug,-Map=.build/software.map,--cref \
			-O3 -ffreestanding -nostdlib \
			-I $(INCLUDE_TEMPLATE) \
			-I $(INCLUDE_LIBRARY) \
			-I $(INCLUDE_FIRMWARE) \
			-o $@ \
			$^ \
			-DSIMU
.build/simu_software.objdump: .build/simu_software.elf
	$(CROSS)objdump --demangle -D $^ > $@
.build/simu_software.nm: .build/simu_software.elf
	$(CROSS)nm --demangle --numeric-sort $^ > $@
.build/simu_software.bin: .build/simu_software.elf
	$(CROSS)objcopy -O binary $^ $@
.build/simu_software.hex: .build/simu_software.bin
	xxd $^ > $@
.build/simu_flash.bin: .build/simu_software.bin
	sh $(TOOLS)/zeropadding.sh $^ > $@
.build/simu_flash.hex: .build/simu_flash.bin
	xxd -c 1 -p $^ > $@
.build/simu_testbench.vvp: $(SIMU_HARD) $(HARDWARE)
	iverilog -g2005-sv -s testbench -o $@ $^ \
	         `yosys-config --datdir/ice40/cells_sim.v` \
			 -DNO_ICE40_DEFAULT_ASSIGNMENTS \
			 -DDEBUG -DDEBUGNETS -DDEBUGREGS
.build/simulation.vcd: .build/simu_testbench.vvp
	vvp $^ > .build/simulation.log
	sh $(TOOLS)/serial.sh .build/simulation.log .build/simu_serial.log
simu: .build/simulation.vcd .build/simu_software.objdump .build/simu_software.nm
	gtkwave .build/simulation.vcd

###############################################################################

.PHONY: all init install gen hardware software upload simu
