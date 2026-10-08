# Toolchain
##################################################
# CC: 	C-compiler
# CXX: 	Cpp-compiler
##################################################
CC  = arm-none-eabi-gcc
CXX = arm-none-eabi-g++

# Flags
##################################################
# -mcpu: 			Target cpu
# -mthumb: 			Thumb instructions
# -g: 				Debug info
# -Wall -Wextra: 	Turn on all and extra warnings
# -ffreestanding: 	Stop assuming standard functions
# -fno-exceptions:	Turn off cpp exceptions
# -fno-rtti:		Turn off run time type information
# -Iinclude: 		Look in include/ for headers
# -nostdlib: 		Don't include std standard libary
# -T: 				Use specific linker script
##################################################
CPUFLAGS = -mcpu=cortex-m3 -mthumb
ASFLAGS  = $(CPUFLAGS) -g
CXXFLAGS = $(CPUFLAGS) -g -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti -Iinclude
LDFLAGS  = $(CPUFLAGS) -ffreestanding -nostdlib -T linker.ld

# Set object files that get linked into the kernel
OBJS = build/startup.o build/main.o build/tcb.o build/systick.o build/round_robin.o

# Set default target
all: build/kernel.elf

# Link all objects into the final ELF
build/kernel.elf: $(OBJS) linker.ld
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

# Compile any C++ source in src/ into build/
build/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Assemble any assembly source in src/ into build/
build/%.o: src/%.S
	mkdir -p $(@D)
	$(CC) $(ASFLAGS) -c $< -o $@

# Remove all build output
clean:
	rm -rf build

# Define as commands for Make
.PHONY: all clean