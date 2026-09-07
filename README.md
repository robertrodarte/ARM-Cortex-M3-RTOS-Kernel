# ARM-Cortex-M3-RTOS-Kernel

## Running project

### Compiling

```bash
# Compiles startup.S into itsn own object file
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -g -c src/startup.S -o build/startup.o

# Compiles main.cpp into its own object file
arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -g -ffreestanding -fno-exceptions -fno-rtti -c src/main.cpp -o build/main.o
```

### Linking

```bash
# Combines both object files into one ELF, using linker.ld as the rulebook to decide every final address
arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib -T linker.ld build/startup.o build/main.o -o build/kernel.elf
```

## Debugging

### Launch QEMU

TODO: Update project to include QEMU debug toolchain

Step 1: In a terminal launch QEMU, halted at reset

```bash
~/toolchains/qemu-src/build/qemu-system-arm -M lm3s6965evb -nographic -kernel build/kernel.elf -s -S
```

Step 2: In a second terminal, launch GDP, connect, and drive it

```bash
arm-none-eabi-gdb build/kernel.elf
```

## Milestone 1: Testing .bss, .data, and initial SP

After building the `linker.ld` and `startup.S` file, I needed to test that
it was actually doing what it needed to do. I did this by creating
the minimal main() function below:

```cpp
/* Used to test zeroed bss and copied data */
int uninitGlobal;     // Define an uninitialized global variable (.bss)
int initGlobal = 115; // Define a global variable (.data)

int main()
{
    // Infinite loop
    while (1)
    {
        // Logic goes here
    }

    return 0;
}
```

### Testing GDB Commands:

---

```bash
# Connect to QEMU target
(gdb) target remote localhost:1234
```

### Test 1: SP Initialization

```bash
(gdb) info registers SP
(gdb) print/x &\_estack
```

### Test 2: .data copy

```bash
(gdb) print initGlobal # Should not print anything yet

# Set break point to step past zero loop
(gdb) break zero_loop
(gdb) continue
(gdb) print initGlobal # Should print initGlobal value
```

### Test 3: .bss zeroing

```bash
# Corrupt address on purpose to see if .bss zeros it
(gdb) set {int}0x20000000 = 0xdeadbeef
(gdb) print unintGlobal # Should print 0xdeadbeef

# Set breakpoint past the zero loop
(gdb) break init_loop
(gdb) continue
(gdb) print unintGlobal # Should print 0
```

## Referenced documentation

[1] Stellaris®LM3S6965 Microcontroller Datasheet  
[2] Arm®v7-M Architecture Reference Manual  
[3] https://developer.arm.com/community/arm-community-blogs/b/architectures-and-processors-blog/posts/writing-your-own-startup-code-for-cortex-m

## In Progress

Author: Robert Rodarte  
Date: 09/05/2026
