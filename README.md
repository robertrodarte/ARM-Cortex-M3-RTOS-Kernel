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

## [Milestone 2: Testing TCB and Initial Stack Frame](#tags)

### Test 1: Validate frame

```bash
# Open debug session
(gdb) target remote localhost:1234

# Set breakpoint right before infinite loop
(gdb) break main.cpp:47

# Continue to breakpoint
(gdb) continue

# Print the contents of tcb1
(gdb) print tcb1
$5 = {sp = 0x20000400 <tcb1_stack+960>, next = 0x0, stack_base = 0x20000040 <tcb1_stack>, stack_size = 256, id = 1}

# Print the address of the top of the stack
(gdb) print &tcb1_stack[240]
$6 = (uint32_t *) 0x20000400 <tcb1_stack+960>

# Print the tcb1.sp frame
(gdb) x/16xw tcb1.sp
0x20000400 <tcb1_stack+960>:    0x00000000      0x00000000      0x00000000      0x00000000
0x20000410 <tcb1_stack+976>:    0x00000000      0x00000000      0x00000000      0x00000000
0x20000420 <tcb1_stack+992>:    0x00000000      0x00000000      0x00000000      0x00000000
0x20000430 <tcb1_stack+1008>:   0x00000000      0x00000379      0x000001ec      0x01000000

# Print the entry function address
(gdb) print tcb1_entry_fn
$7 = {void (void)} 0x1ec <tcb1_entry_fn()>

# Print the exit function address
(gdb) print exit_fn
$8 = {void (void)} 0x378 <exit_fn()>
```

### Visual of memory during debug

| Address     | Variable   | Comments                                  |
| ----------- | ---------- | ----------------------------------------- |
| 0x2000_0000 | tcb1       | 0x2000_0000 is where .bss starts          |
| 0x2000_0014 | tcb2       |                                           |
| 0x2000_0028 | tcb3       |                                           |
| 0x2000_003c | n/a        | Open due to alignas(8)                    |
| 0x2000_0040 | tcb1_stack | Stack base for tcb1                       |
| ...         |            |                                           |
| 0x2000_0400 | n/a        | Bottom of the 16 word initial frame       |
| 0x2000_0440 | tcb2_stack | End of tcb1_stack and start of tcb2_stack |
| ...         |            | Repeat for tcb2 and tcb3                  |

### Test 2: SysTick Interrupt Verification

```bash
# Open the debug session
(gdb) target remote localhost:1234

# Set a breakpoint on the SysTick_Handler interrupt function
(gdb) break SysTick_Handler
Breakpoint 1 at 0x44a: file src/systick.cpp, line 54.

# Continue to breakpoint
(gdb) continue
Continuing.
Breakpoint 1, SysTick_Handler () at src/systick.cpp:54
54          tick_counter++;

# Display info in lr register
(gdb) info registers lr
lr             0xfffffff9          -7

# Remove all breakpoints
(gdb) delete
Delete all breakpoints, watchpoints, tracepoints, and catchpoints? (y or n) y

# Let program run so SysTick_Handler increments and stop after a few seconds
(gdb) continue
Continuing.
^C
Program received signal SIGINT, Interrupt.
main () at src/main.cpp:53
53          while (1)

# Print the value of tick_counter
(gdb) print tick_counter
$1 = 5578
```

### Test 3: Round-Robin Scheduler Verification

```bash
# Open the debug session
(gdb) target remote localhost:1234

# Add a watchpoint on "current" variable to detect its value changing
(gdb) watch 'round_robin.cpp'::current

# Continue to the next value change for "current"
(gdb) continue
Continuing.
Hardware watchpoint 1: 'round_robin.cpp'::current
Old value = (TCB *) 0x0
New value = (TCB *) 0x20000000 <tcb1>
RoundRobinScheduler::add_task (tcb=0x20000000 <tcb1>) at src/round_robin.cpp:37
37              tcb->next = head;

# Continue to the next value change for "current"
(gdb) continue
Continuing.
Hardware watchpoint 1: 'round_robin.cpp'::current
Old value = (TCB *) 0x20000000 <tcb1>
New value = (TCB *) 0x20000014 <tcb2>
RoundRobinScheduler::run () at src/round_robin.cpp:69
69          return 0;

# Print the current TCB's ID
(gdb) print current->id
$1 = 2
# Print the tick_counter value
(gdb) print tick_counter
$2 = 10

# Continue to the next value change for "current"
(gdb) continue
Continuing.
Hardware watchpoint 1: 'round_robin.cpp'::current
Old value = (TCB *) 0x20000014 <tcb2>
New value = (TCB *) 0x20000028 <tcb3>
RoundRobinScheduler::run () at src/round_robin.cpp:69
69          return 0;

# Print the current TCB's ID
(gdb) print current->id
$3 = 3
# Print the tick_counter value
(gdb) print tick_counter
$4 = 20

# Continue to the next value change for "current"
(gdb) continue
Continuing.
Hardware watchpoint 1: 'round_robin.cpp'::current
Old value = (TCB *) 0x20000028 <tcb3>
New value = (TCB *) 0x20000000 <tcb1>
RoundRobinScheduler::run () at src/round_robin.cpp:69
69          return 0;

# Print the current TCB's ID
(gdb) print current->id
$5 = 1
# Print the tick_counter value
(gdb) print tick_counter
$6 = 30
```

## Tags

Milestone 1: [milestone-1-boot-memory](https://github.com/robertrodarte/ARM-Cortex-M3-RTOS-Kernel/releases/tag/v1.0.1)  
Milestone 2: [milestone-2-in-progress]()

## Referenced documentation

[1] Stellaris®LM3S6965 Microcontroller Datasheet  
[2] Arm®v7-M Architecture Reference Manual  
[3] https://developer.arm.com/community/arm-community-blogs/b/architectures-and-processors-blog/posts/writing-your-own-startup-code-for-cortex-m

## Project Details

Author: Robert Rodarte  
Date: 09/05/2026
