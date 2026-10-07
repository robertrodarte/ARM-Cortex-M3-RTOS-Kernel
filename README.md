# ARM-Cortex-M3-RTOS-Kernel

## Prerequisites

### TODO: Add toolchain setup

## Compiling, linking, building, and cleaning

From the repo root build `build/kernel.elf` by running the following command:

```bash
make
```

To remove all build outputs, run the following command:

```bash
make clean
```

To preview the make commands, run the following command:

```bash
make -n
```

## Debugging

### Launch QEMU

Step 1: In a terminal launch QEMU, halted at reset

```bash
~/toolchains/qemu-src/build/qemu-system-arm -M lm3s6965evb -nographic -kernel build/kernel.elf -s -S
```

Step 2: In a second terminal, launch GDB, connect, and drive it

```bash
arm-none-eabi-gdb build/kernel.elf
```

## [Milestone 1: Testing .bss, .data, and initial SP](#tags)

After building the `linker.ld` and `startup.S` file, I needed to test that
it was actually doing what it needed to do. I did this by creating
the minimal main() function below:

`main.cpp`

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
(gdb) info registers sp
(gdb) print/x &_estack
```

### Test 2: .data copy

```bash
(gdb) print initGlobal # Should print 0, not the initGlobal value yet

# Set break point to step past zero loop
(gdb) break zero_loop
(gdb) continue
(gdb) print initGlobal # Should print initGlobal value
```

### Test 3: .bss zeroing

```bash
# Corrupt address on purpose to see if .bss zeros it
(gdb) set {int}0x20000000 = 0xdeadbeef
(gdb) print uninitGlobal # Should print 0xdeadbeef

# Set breakpoint past the zero loop
(gdb) break init_loop
(gdb) continue
(gdb) print uninitGlobal # Should print 0
```

### Test 4: Reach main()

```bash
# Set breakpount at main()
(gdb) break main

# Run to breakpoint with expected result
(gdb) continue
Continuing.

Breakpoint 1, main () at src/main.cpp:8
```

## Tags

Milestone 1: [milestone-1-boot-memory](https://github.com/robertrodarte/ARM-Cortex-M3-RTOS-Kernel/releases/tag/milestone-1-boot-memory)

## Referenced documentation

[1] Stellaris®LM3S6965 Microcontroller Datasheet  
[2] Arm®v7-M Architecture Reference Manual  
[3] https://developer.arm.com/community/arm-community-blogs/b/architectures-and-processors-blog/posts/writing-your-own-startup-code-for-cortex-m

## Project Details

Author: Robert Rodarte  
Date: 09/05/2026
