# Makefile for TinyFPGA_BX

BOARD    := NextMicon/TinyFPGA_BX/0.0.0

# Tools
SYNTH := yosys
PNR   := nextpnr-ice40
TIME  := icetime
PACK  := icepack
PROG  := tinyprog

CROSS    := riscv64-unknown-elf-

IVERILOG := iverilog
VVP      := vvp
GTKWAVE  := gtkwave

SCRIPTS  := ${HOME}/NextMiconIDE/Board/$(BOARD)/tools

###############################################################################

all: upload

###############################################################################

PIN := ${HOME}/NextMiconIDE/Board/$(BOARD)/hw/fpga.pcf
HW  := ${HOME}/NextMiconIDE/Board/$(BOARD)/hw/*.sv \
       ${HOME}/NextMiconIDE/Package/*/*/*/*.sv \
       micon/*.sv

build/hw_synth.json: $(HW)
	$(SYNTH) -ql $@.log -p 'synth_ice40 -top hw -json $@' $^
build/hw_pnr.asc: build/hw_synth.json
	$(PNR) -ql $@.log --json $^ --pcf $(PIN) --asc $@ \
	--lp8k --package cm81
build/hw_time.rpt: build/hw_pnr.asc
	$(TIME) -d lp8k -c 12 -mtr $@ $^
build/hw_bit.bin: build/hw_pnr.asc
	$(PACK) $^ $@

hw: build/hw_time.rpt build/hw_bit.bin

###############################################################################

LINK := ${HOME}/NextMiconIDE/Board/$(BOARD)/sw/link.ld
ASM  := ${HOME}/NextMiconIDE/Board/$(BOARD)/sw/start.S
CPP  := ${HOME}/NextMiconIDE/Board/$(BOARD)/sw/*.cpp \
		${HOME}/NextMiconIDE/Package/*/*/*/*.cpp \
		micon/*.cpp \
		sw/*.cpp

INC_BRD := ${HOME}/NextMiconIDE/Board/$(BOARD)/sw
INC_LIB := ${HOME}/NextMiconIDE/Package
INC_FW  := micon

build/sw.elf: $(ASM) $(CPP)
	$(CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(LINK),--strip-debug,-Map=build/sw.map,--cref \
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
# $(PROG) -p build/hw_bit.bin -u build/sw.bin
	powershell.exe -c \
	"tinyprog -p \$$env:WSLHome\NextMiconIDE\Project\1_hello\build\hw_bit.bin \
	          -u \$$env:WSLHome\NextMiconIDE\Project\1_hello\build\sw.bin"

###############################################################################

SIMU_HW := simu/*.sv
SIMU_SW := .template/cpu/*.cpp .packages/*/*.cpp micon/*.cpp simu/*.cpp

build/simu_sw.elf: $(ASM) $(CPP)
	$(CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(LINK),--strip-debug,-Map=build/sw.map,--cref \
			-O3 -ffreestanding -nostdlib \
			-I $(INC_BRD) \
			-I $(INC_LIB) \
			-I $(INC_FW) \
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
build/simu_flash.hex: build/simu_flash.bin
	xxd -c 1 -p $^ > $@
build/simu_tb.vvp: $(SIMU_HW) $(HW)
	$(IVERILOG) -g2005-sv -s tb -o $@ $^ \
	           `yosys-config --datdir/ice40/cells_sim.v` \
			   -DNO_ICE40_DEFAULT_ASSIGNMENTS \
			   -DDEBUG -DDEBUGNETS -DDEBUGREGS
build/simu.vcd: build/simu_tb.vvp
	$(VVP) $^ > build/simu.log
	sh $(SCRIPTS)/serial.sh build/simu.log build/simu_serial.log

simu: build/simu_flash.hex build/simu.vcd build/simu_sw.objdump build/simu_sw.nm
	$(GTKWAVE) build/simu.vcd

###############################################################################

.PHONY: all hw sw upload simu
