#include $(NEMU_HOME)/src/utils/filelist.mk
DISASMSRC = $(NEMU_HOME)/src/utils/disasm.cc
DISASMFLAGS = $(shell llvm-config --cxxflags) -fPIE
TOPNAME = top
NXDC_FILES = ./top.nxdc
INC_PATH ?=

VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build -cc  \
                                -O3 --x-assign fast --x-initial fast --noassert
VERILATOR_WARNING = -Wno-UNOPTFLAT

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)

default: $(BIN)

$(shell mkdir -p $(BUILD_DIR))

# constraint file
SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)
$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

# project source
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
CSRCS += $(SRC_AUTO_BIND)

# rules for NVBoard
include $(NVBOARD_HOME)/scripts/nvboard.mk

# rules for verilator
INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""
LDFLAGS += -lSDL2 -lSDL2_image

$(BIN): $(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
                --top-module $(TOPNAME) $^ \
                $(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
                --Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))

WAVE_FLAG = wave.vcd

all:
	@echo "Write this Makefile by your self."


sim:$(VSRCS) $(CSRCS) $(DISASMSRC)
	$(call git_commit, "sim RTL")
	rm -rf ./obj_dir
	verilator --cc -Ivsrc vsrc/top.v csrc/*.cpp $(DISASMSRC) --build --exe --trace -LDFLAGS $(shell llvm-config --libs) -LDFLAGS -ldl

simw:siming
	#gtkwave $(WAVE_FLAG)
siming:
	$(call git_commit, "sim RTL")
	rm -rf ./obj_dir
	echo $(IMG)
	verilator -j 4 --cc -Ivsrc --trace $(VERILATOR_WARNING) vsrc/top.v csrc/*.cpp $(DISASMSRC) --build --exe -LDFLAGS $(shell llvm-config --libs) -LDFLAGS -ldl
	-./obj_dir/Vtop $(IMG) 


run:$(BIN)
	echo $(BIN)
	#@$^

clean:
	rm -rf $(BUILD_DIR)
.PHONY: default all clean run

