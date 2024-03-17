# Directory
IDE_HOME  = ~/NextMiconIDE
PROJ      = ${PROJ_NAME}
BOARD     = NextMicon/TangNano9K/0.0.0

# Tools
OSS_BIN = ~/oss-cad-suite/bin
YOSYS   = $(OSS_BIN)/yosys
NEXTPNR = $(OSS_BIN)/nextpnr-gowin
GW_PACK = $(OSS_BIN)/gowin_pack

CROSS   = riscv64-unknown-elf-

TOOLS = $(IDE_HOME)/Board/$(BOARD)/tools

# Target Config
PART      = GW1NR-LV9QN88PC6/I5

# Input Files
SV_FILES  = $(IDE_HOME)/Board/$(BOARD)/hardware/*.sv \
			$(IDE_HOME)/Package/*/*/*/*.sv \
			micon/*.sv

PCF_FILE = $(IDE_HOME)/Board/$(BOARD)/hardware/*.pcf

START     = $(IDE_HOME)/Board/$(BOARD)/software/start.S
LINKER    = $(IDE_HOME)/Board/$(BOARD)/software/sections.lds

SOFTWARE  = $(IDE_HOME)/Board/$(BOARD)/software/*.cpp \
			$(IDE_HOME)/Package/*/*/*/*.cpp \
			micon/*.cpp \
			software/*.cpp

INCLUDE_BOARD    = $(IDE_HOME)/Board/$(BOARD)/software
INCLUDE_LIBRARY  = $(IDE_HOME)/Package
INCLUDE_FIRMWARE = micon

SIMU_HARD = simulation/*.v
SIMU_SOFT = .template/cpu/*.cpp .packages/*/*.cpp micon/*.cpp simulation/*.cpp

###############################################################################

all: upload

install:
	sudo apt update
	sudo apt install gcc-riscv64-unknown-elf yosys nextpnr-ice40 iverilog gtkwave python3
	pip install tinyprog

init:
	mkdir -p .build micon

###############################################################################

hardware: .build/hardware.bin
.build/hardware.json: $(SV_FILES)
	make init
	yosys -ql $@.log -p 'synth_gowin -top hardware -json $@' $^
.build/hardware.fs: .build/hardware.json
	$(NEXTPNR) -ql $@.log --lp8k --package cm81 --asc $@ --pcf $(PCF_FILE) --json $<
.build/hardware.bin: .build/hardware.fs
	$(GW_PACK) -d tangnano9k $^

software: .build/software.objdump .build/software.nm .build/software.bin
.build/software.elf: $(START) $(SOFTWARE)
	$(CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(LINKER),--strip-debug,-Map=.build/software.map,--cref \
			-O3 -ffreestanding -nostdlib \
			-I $(INCLUDE_BOARD) \
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
	"tinyprog -p \$$env:WSLHome\NextMiconIDE\Project\$(PROJ)\.build\hardware.bin \
	          -u \$$env:WSLHome\NextMiconIDE\Project\$(PROJ)\.build\software.bin"

###############################################################################

.build/simu_software.elf: $(START) $(SOFTWARE)
	$(CROSS)g++ -march=rv32imc -mabi=ilp32 -nostartfiles \
	        -Wl,-Bstatic,-T,$(LINKER),--strip-debug,-Map=.build/software.map,--cref \
			-O3 -ffreestanding -nostdlib \
			-I $(INCLUDE_BOARD) \
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
.build/simu_testbench.vvp: $(SIMU_HARD) $(SV_FILES)
	iverilog -g2005-sv -s testbench -o $@ $^ \
	         `yosys-config --datdir/ice40/cells_sim.v` \
			 -DNO_ICE40_DEFAULT_ASSIGNMENTS \
			 -DDEBUG -DDEBUGNETS -DDEBUGREGS
.build/simulation.vcd: .build/simu_testbench.vvp
	vvp $^ > .build/simulation.log
	sh $(TOOLS)/serial.sh .build/simulation.log .build/simu_serial.log
simu: .build/simulation.vcd .build/simu_software.objdump .build/simu_software.nm
#	gtkwave .build/simulation.vcd
	powershell.exe -c \
	"gtkwave \$$env:WSLHome\NextMiconIDE\Project\music\.build\simulation.vcd"

###############################################################################

.PHONY: all init install gen hardware software upload simu
