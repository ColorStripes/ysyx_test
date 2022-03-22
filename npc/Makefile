all:
	@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	verilator -Wall --cc --bulid --exe -- trace vsrc/*.v csrc/*.cpp	
include ../Makefile
