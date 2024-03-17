# Makefile for TangNano9K

BOARD  := NextMicon/TangNano9K/0.0.0

# Tools
SYNTH := yosys
PNR   := nextpnr-gowin
# TIME  := gowin_time
PACK  := gowin_pack
PROG  := openFPGALoader

CROSS    := riscv64-unknown-elf-

IVERILOG := iverilog
VVP      := vvp
GTKWAVE  := gtkwave

SCRIPTS  := ${HOME}/NextMiconIDE/Board/$(BOARD)/tools

###############################################################################

all: upload

###############################################################################

PIN := ${HOME}/NextMiconIDE/Board/$(BOARD)/hw/fpga.cst
HW  := ${HOME}/NextMiconIDE/Board/$(BOARD)/hw/*.sv \
       ${HOME}/NextMiconIDE/Package/*/*/*/*.sv \
       micon/*.sv

build/hw_synth.json: $(HW)
	$(SYNTH) -ql $@.log -p 'synth_gowin -top hw -json $@' $^
build/hw_pnr.json: build/hw_synth.json
	$(PNR) -ql $@.log --json $^ --cst $(PIN) --write $@ \
	--device GW1NR-LV9QN88PC6/I5 --family GW1N-9C
build/hw_bit.fs: build/hw_pnr.json
	$(PACK) -d GW1NR-9C -o $@ $^

hw: build/hw_bit.fs

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
	$(PROG) -b tangnano9k build/hw_bit.fs --external-flash build/sw.bin

###############################################################################

SIMU_HW := simu/*.v
SIMU_SW := .template/cpu/*.cpp .packages/*/*.cpp micon/*.cpp simu/*.cpp

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
