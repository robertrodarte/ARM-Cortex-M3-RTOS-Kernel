#![no_std] // Tells compiler not to link Rust standard library
#![no_main] // Tells compiler not to use normal entry point (main())

static mut UNINIT_GLOBAL: Option<i32> = None; // Define an uninitialized global variable (.bss)
static mut INIT_GLOBAL: i32 = 115; // Define a global variable (.data)

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
