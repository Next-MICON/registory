# Makefile for TinyFPGA_BX

BOARD    := NextMicon/TinyFPGA_BX/0.0.0

# Tools
YOSYS     = yosys
NEXTPNR   = nextpnr-ice40
ICETIME   = icetime
ICEPACK   = icepack
TINYPROG  = tinyprog

CROSS     = riscv64-unknown-elf-

IVERILOG  = iverilog
VVP       = vvp
GTKWAVE   = gtkwave

SCRIPTS   = ${HOME}/NextMiconIDE/Board/$(BOARD)/tools

###############################################################################

all: upload

###############################################################################

PCF := ${HOME}/NextMiconIDE/Board/$(BOARD)/hardware/fpga.pcf
HW  := ${HOME}/NextMiconIDE/Board/$(BOARD)/hardware/*.sv \
       ${HOME}/NextMiconIDE/Package/*/*/*/*.sv \
       micon/*.sv

build/hw_synth.json: $(HW)
	$(YOSYS) -ql $@.log -p 'synth_ice40 -top hw -json $@' $^
build/hw_pnr.asc: build/hw_synth.json
	$(NEXTPNR) -ql $@.log --lp8k --package cm81 --asc $@ --pcf $(PCF) --json $^
build/hw_time.rpt: build/hw_pnr.asc
	$(ICETIME) -d lp8k -c 12 -mtr build/hardware.rpt $^
build/hw_bit.bin: build/hw_pnr.asc
	$(ICEPACK) $^ $@

hw: build/hw_time.rpt build/hw_bit.bin

###############################################################################

LINK := ${HOME}/NextMiconIDE/Board/$(BOARD)/software/sections.lds
ASM  := ${HOME}/NextMiconIDE/Board/$(BOARD)/software/start.S
CPP  := ${HOME}/NextMiconIDE/Board/$(BOARD)/software/*.cpp \
		${HOME}/NextMiconIDE/Package/*/*/*/*.cpp \
		micon/*.cpp \
		software/*.cpp

INC_BRD = ${HOME}/NextMiconIDE/Board/$(BOARD)/software
INC_LIB = ${HOME}/NextMiconIDE/Package
INC_FW  = micon

build/sw.elf: $(ASM) $(CPP)
	$(CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(LINK),--strip-debug,-Map=build/software.map,--cref \
			-O3 -ffreestanding -nostdlib \
			-I $(INC_BRD) \
			-I $(INC_LIB) \
			-I $(INC_FW) \
			-o $@ \
			$^
build/sw.objdump: build/sw.elf
	$(CROSS)objdump --demangle -D $^ > $@
build/sw.nm: build/sw.elf
	$(CROSS)nm --demangle --numeric-sort $^ > $@
build/sw.bin: build/sw.elf
	$(CROSS)objcopy -O binary $^ /dev/stdout > $@

sw: build/sw.objdump build/sw.nm build/sw.bin

###############################################################################

upload: hw sw
	tinyprog -p build/hw.bin -u build/sw.bin

###############################################################################

SIMU_HW = simulation/*.v
SIMU_SW = .template/cpu/*.cpp .packages/*/*.cpp micon/*.cpp simulation/*.cpp

build/simu_sw.elf: $(ASM) $(CPP)
	$(CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(LINK),--strip-debug,-Map=build/sw.map,--cref \
			-O3 -ffreestanding -nostdlib \
			-I $(INCLUDE_BOARD) \
			-I $(INCLUDE_LIBRARY) \
			-I $(INCLUDE_FIRMWARE) \
			-o $@ \
			$^ \
			-DSIMU
build/simu_sw.objdump: build/simu_sw.elf
	$(CROSS)objdump --demangle -D $^ > $@
build/simu_sw.nm: build/simu_sw.elf
	$(CROSS)nm --demangle --numeric-sort $^ > $@
build/simu_sw.bin: build/simu_sw.elf
	$(CROSS)objcopy -O binary $^ $@
build/simu_sw.hex: build/simu_sw.bin
	xxd $^ > $@
build/simu_flash.bin: build/simu_sw.bin
	sh $(SCRIPTS)/zeropadding.sh $^ > $@
build/simu_flash.hex: .build/simu_flash.bin
	xxd -c 1 -p $^ > $@
build/simu_tb.vvp: $(SIMU_HW) $(HW)
	$(IVERILOG) -g2005-sv -s testbench -o $@ $^ \
	           `yosys-config --datdir/ice40/cells_sim.v` \
			   -DNO_ICE40_DEFAULT_ASSIGNMENTS \
			   -DDEBUG -DDEBUGNETS -DDEBUGREGS
build/simu.vcd: build/simu_tb.vvp
	$(VVP) $^ > build/simu.log
	sh $(SCRIPTS)/serial.sh build/simu.log build/simu_serial.log

simu: build/simu.vcd build/simu_sw.objdump build/simu_sw.nm
	$(GTKWAVE) build/simu.vcd

###############################################################################

.PHONY: all hw sw upload simu
