# Makefile for TinyFPGA_BX

DIR_PROJECT := $(shell pwd)
DIR_PACKAGE := ${HOME}/NextMiconIDE/Package
DIR_TARGET  := ${HOME}/NextMiconIDE/Board/NextMicon/TinyFPGA_BX/0.0.0

# Tools
CMD_SYNTH := yosys
CMD_PNR   := nextpnr-ice40
CMD_TIME  := icetime
CMD_PACK  := icepack
CMD_PROG  := tinyprog

CMD_CROSS := riscv64-unknown-elf-

CMD_SIMU := iverilog
CMD_EXE  := vvp
CMD_WAVE := ${HOME}/oss-cad-suite/bin/gtkwave

DIR_SCRIPTS := $(DIR_TARGET)/tools

###############################################################################

all: upload

###############################################################################

FILE_PIN := $(DIR_TARGET)/hw/fpga.pcf
FILE_HW  := $(wildcard $(DIR_TARGET)/hw/*.sv) $(wildcard $(DIR_PACKAGE)/*/*/*/*.sv) $(wildcard micon/*.sv)

build/hw_synth.json: $(FILE_HW)
	$(CMD_SYNTH) -ql $@.log -p 'synth_ice40 -top hw -json $@' $^
build/hw_pnr.asc: build/hw_synth.json
	$(CMD_PNR) -ql $@.log --json $^ --pcf $(FILE_PIN) --asc $@ \
	--lp8k --package cm81
build/hw_time.rpt: build/hw_pnr.asc
	$(CMD_TIME) -d lp8k -c 12 -mtr $@ $^
build/hw_bit.bin: build/hw_pnr.asc
	$(CMD_PACK) $^ $@

hw: build/hw_time.rpt build/hw_bit.bin

###############################################################################

FILE_LINK := $(DIR_TARGET)/sw/link.ld
FILE_ASM  := $(DIR_TARGET)/sw/start.S
FILES_CPP  := $(wildcard $(DIR_TARGET)/sw/*.cpp) $(wildcard $(DIR_PACKAGE)/*/*/*/*.cpp) $(wildcard $(DIR_PROJECT)/micon/*.cpp) $(wildcard $(DIR_PROJECT)/sw/*.cpp)

build/sw.elf: $(FILE_ASM) $(FILES_CPP)
	$(CMD_CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(FILE_LINK),--strip-debug,-Map=build/sw.map,--cref \
			-O3 -ffreestanding -nostdlib \
			-I $(DIR_TARGET)/sw -I $(DIR_PACKAGE) -I $(DIR_PROJECT)/micon \
			-o $@ \
			$^
build/sw.objdump: build/sw.elf
	$(CMD_CROSS)objdump --demangle -D $^ > $@
build/sw.nm: build/sw.elf
	$(CMD_CROSS)nm --demangle --numeric-sort $^ > $@
build/sw.bin: build/sw.elf
	$(CMD_CROSS)objcopy -O binary $^ /dev/stdout > $@

sw: build/sw.objdump build/sw.nm build/sw.bin

###############################################################################

upload: hw sw
	$(CMD_PROG) -p build/hw_bit.bin -u build/sw.bin

###############################################################################

SIMU_HW := simu/*.sv
SIMU_SW := .template/cpu/*.cpp .packages/*/*.cpp micon/*.cpp simu/*.cpp

build/simu_sw.elf: $(FILE_ASM) $(FILES_CPP)
	$(CMD_CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(FILE_LINK),--strip-debug,-Map=build/sw.map,--cref \
			-O3 -ffreestanding -nostdlib \
			-I $(DIR_TARGET)/sw -I $(DIR_PACKAGE) -I $(DIR_PROJECT)/micon \
			-o $@ \
			$^ \
			-DSIMU
build/simu_sw.objdump: build/simu_sw.elf
	$(CMD_CROSS)objdump --demangle -D $^ > $@
build/simu_sw.nm: build/simu_sw.elf
	$(CMD_CROSS)nm --demangle --numeric-sort $^ > $@
build/simu_sw.bin: build/simu_sw.elf
	$(CMD_CROSS)objcopy -O binary $^ $@
build/simu_sw.hex: build/simu_sw.bin
	xxd $^ > $@
build/simu_flash.bin: build/simu_sw.bin
	sh $(DIR_SCRIPTS)/zeropadding.sh $^ > $@
build/simu_flash.hex: build/simu_flash.bin
	xxd -c 1 -p $^ > $@
build/simu_tb.vvp: $(SIMU_HW) $(FILE_HW)
	$(CMD_SIMU) -g2005-sv -s tb -o $@ $^ \
	           `yosys-config --datdir/ice40/cells_sim.v` \
			   -DNO_ICE40_DEFAULT_ASSIGNMENTS \
			   -DDEBUG -DDEBUGNETS -DDEBUGREGS
build/simu.vcd: build/simu_tb.vvp
	$(CMD_EXE) $^ > build/simu.log
	sh $(DIR_SCRIPTS)/serial.sh build/simu.log build/simu_serial.log

simu: build/simu_flash.hex build/simu.vcd build/simu_sw.objdump build/simu_sw.nm

wave: build/simu.vcd
	$(CMD_WAVE) build/simu.gtkw

###############################################################################

.PHONY: all hw sw upload simu wave
