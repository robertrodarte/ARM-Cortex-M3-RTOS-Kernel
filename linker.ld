/* Define the starting address for ELF file's e_entry header */
/* to the Reset_Handler's address */
ENTRY(Reset_Handler)

/* Define the memory peripherals (pg. 51) */
MEMORY 
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Define the sections in memory */
SECTIONS
{   
    /* Compute the top of the RAM */
    _estack = ORIGIN(RAM) + LENGTH(RAM);

    /* Grab the data sections LMA address */
    /* Where in flash the copy loop should read from */
    _sidata = LOADADDR(.data);

    /* Vector table must be first thing in flash as the CPU reads */
    /* word 0 (initial SP) and word 1 (reset handler address) */
    /* from address 0x0000_0000 before any code runs. */
    /* Something (vector table) has to supply the actual value for  */
    /* the initial stack pointer (the top of RAM) */
    /* Whatever content is tagged with .isr_vector, route it to this memory location */
    .isr_vector :
    {
        KEEP(*(.isr_vector)) /* KEEP = Tells garbage collector to never discard this section  */
    } > FLASH

    /* Zero from start address to end address */
    .bss : 
    {
        . = ALIGN(4);
        _sbss = .; /* Capture the current address before content */
        *(.bss)
        *(.bss*)
        *(COMMON) /* Safeguard for legacy C... just do it */
        . = ALIGN(4);
        _ebss = .; /* Capture the current address after content */
    } > RAM

    /* Copy data from flash to RAM */
    .data :
    {   
        . = ALIGN(4);
        /* Where in RAM it should write to (start) */
        _sdata = .; /* Capture current address before content */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        /* Where in RAM it should write to (end) */
        _edata = .; /* Capture current address after content */
    } > RAM AT> FLASH /* > Where it lives at runtime > Where it is initalized */

    /* Loop over an array of function pointers to run  */
    /* Initializes C++ static constructors */
    .init_array :
    {
        . = ALIGN(4);
        __init_array_start__ = .; /* Capture current address before content */
        KEEP(*(SORT(.init_array*)))
        KEEP(*(.init_array))
        . = ALIGN(4);
        __init_array_end__ = .; /* Capture current address after content */
    } > FLASH

    /* Text and rodata just need to exist anywhere in FLASH */
    /* text is code */
    .text :
    {
        *(.text)
        *(.text*)
    } > FLASH

    /* rodata is const data */
    .rodata :
    {
        *(.rodata)
        *(.rodata*)
    } > FLASH
}