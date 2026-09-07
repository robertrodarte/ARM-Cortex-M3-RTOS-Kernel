.syntax unified                             /* Use modern assembler syntax */
                                            /* Auto-generate IT instructions */
.thumb                                      /* Thumb/Thumb2 encoding */

/* Define ISR vector section here. Found on page 
   85 of lm3s6965.pdf mcu datasheet */
.section    .isr_vector
.align 2                                    /* Align 2^2 */
.long _estack                               /* Stack pointer */
.long Reset_Handler                         /* Reset */
.long NMI_Handler                           /* NMI */
.long HardFault_Handler                     /* Hard fault */
.long MemManage_Handler                     /* Memory management fault */
.long BusFault_Handler                      /* Bus fault */
.long UsageFault_Handler                    /* Usage fault */
.long 0                                     /* Reserved */
.long 0                                     /* Reserved */
.long 0                                     /* Reserved */
.long 0                                     /* Reserved */
.long SVC_Handler                           /* SVCall */
.long 0                                     /* Reserved for debug */
.long 0                                     /* Reserved */
.long PendSV_Handler                        /* PendSV */
.long SysTick_Handler                       /* Systick */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */
.long Default_Handler                       /* IRQ Handlers */

/* Start of startup code */
.text                       
.align 2

.thumb_func                                 /* Thumb mode function entry point */

Default_Handler:                            /* Define default handler */
    b Default_Handler                       /* Temporary infinite loop */

.weak NMI_Handler                           /* Weak alias pattern  */
.set NMI_Handler, Default_Handler           /* Weak alias pattern  */
.weak HardFault_Handler                     /* Weak alias pattern  */
.set HardFault_Handler, Default_Handler     /* Weak alias pattern  */
.weak MemManage_Handler                     /* Weak alias pattern  */
.set MemManage_Handler, Default_Handler     /* Weak alias pattern  */
.weak BusFault_Handler                      /* Weak alias pattern  */
.set BusFault_Handler, Default_Handler      /* Weak alias pattern  */
.weak UsageFault_Handler                    /* Weak alias pattern  */
.set UsageFault_Handler, Default_Handler    /* Weak alias pattern  */
.weak SVC_Handler                           /* Weak alias pattern  */
.set SVC_Handler, Default_Handler           /* Weak alias pattern  */
.weak PendSV_Handler                        /* Weak alias pattern  */
.set PendSV_Handler, Default_Handler        /* Weak alias pattern  */
.weak SysTick_Handler                       /* Weak alias pattern  */
.set SysTick_Handler, Default_Handler       /* Weak alias pattern  */

.global Reset_Handler                       /* Define reset handler */
.thumb_func                                 /* Thumb mode function entry point */

Reset_Handler:
    /* Step 1: Copy .data from FLASH to RAM */
    ldr r1, =_sidata                        /* Load start of FLASH address */
    ldr r2, =_sdata                         /* Load start of RAM address */
    ldr r3, =_edata                         /* Load end of RAM address */

    copy_loop:
        ldr r4, [r1], #4                    /* Move word from FLASH to R4 and increment FLASH address to next location */
        str r4, [r2], #4                    /* Store word from R4 into RAM and increment RAM address to next location */
        cmp r2, r3                          /* Check if we are at end of data */
        bne copy_loop                       /* Go to top of loop if not at end, otherwise move on*/

    /* Step 2: Zero .bss */
    ldr r1, =_sbss                          /* Load bss start address */
    ldr r2, =_ebss                          /* Load bss end address */
    movs r3, #0                             /* Load r3 with constant of 0 */

    zero_loop:
        str r3, [r1], #4                    /* Zero value at current bss address and increment to next location */
        cmp r1, r2                          /* Check if we are at end of bss */
        bne zero_loop                       /* Go to top of loop if not at end, otherwise move on*/

    /* Step 3: Walk .init_array, calling each constructor */
    ldr r4, =__init_array_start__           /* Load array start address */
    ldr r5, =__init_array_end__             /* Load array end address */

    init_loop:                              /* Use r4-r11 as they are callee-saved registers */
        ldr r6, [r4], #4                    /* Load r4 contents into r6 and increment to next location */
        blx r6                              /* Call function */
        cmp r4, r5                          /* Check if we are at end of array */
        bne init_loop                       /* Go to top of loop if not at end, otherwise move on*/

    /* Step 4: Call main() */
    bl main                                 /* Main entry point. Sets LR to loop*/

    /* Step 5: Infinite loop if main() returns */
    loop:           
        b loop                              /* Loop forever failsafe */