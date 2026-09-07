# Rust-Related

To practice my Rust skills, I am implementing Rust code as I implement
my C++ code throughout this project. This implementation is a stretch
goal and not required for the final submission of the project.

## Compile

```bash
# Compile main.rs into object file
cargo rustc -- --emit=obj -o build/main_rust.o
```

## Link

```bash
# Combines  object file into an ELF, using linker.ld as the rulebook to decide every final address
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -nostdlib -T linker.ld build/startup.o rust-related/build/main_rust*.o -o rust-related/build/kernel_rust.elf
```

## Milestone 1 - Rust Implementation

Recreating main.cpp using Rust

`main.rs`

```rust
#![no_std] // Tells compiler not to link Rust standard library
#![no_main] // Tells compiler not to use normal entry point (main())

static UNINIT_GLOBAL: Option<i32> = None; // Define an uninitialized global variable (.bss)
static INIT_GLOBAL: i32 = 115; // Define a global variable (.data)

#[panic_handler] // Defines what happens when a panic occurs since we don't have std lib
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {
        // Infinite loop to halt the program
    }
}

#[unsafe(no_mangle)] // Tells compiler not to mangle the name of this function
pub extern "C" fn main() -> ! {
    // Infinite loop
    loop {
        // Logic goes here
    }
}
```

## Project Details

Author: Robert Rodarte  
Date: 09/06/2026
