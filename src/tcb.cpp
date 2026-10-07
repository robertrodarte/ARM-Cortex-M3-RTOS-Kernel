#include "tcb.h"

/**
 * @brief Exit function for tasks that loops forever
 */
static void exit_fn(void);

/**
 * @brief Initialize a TCB structure
 */
uint8_t tcb_init(uint32_t *stack_base, uint32_t stack_size, uint32_t id, void (*entry_fn)(void), TCB *tcb)
{
    // Catch invalid stack size
    if (stack_size < 16)
    {
        return 1;
    }

    // Initialize the TCB structure
    tcb->sp = stack_base + stack_size;
    tcb->next = nullptr;
    tcb->stack_base = stack_base;
    tcb->stack_size = stack_size;
    tcb->id = id;

    // Write 16 words downward for initial stack frame
    *--tcb->sp = 0x01000000;                                 // xPSR: Set the Thumb bit
    *--tcb->sp = reinterpret_cast<uint32_t>(entry_fn) & ~1u; // PC: Entry point for the task
    *--tcb->sp = reinterpret_cast<uint32_t>(exit_fn);        // LR: Return address for the task
    *--tcb->sp = 0;                                          // R12: General purpose register
    *--tcb->sp = 0;                                          // R3: General purpose register
    *--tcb->sp = 0;                                          // R2: General purpose register
    *--tcb->sp = 0;                                          // R1: General purpose register
    *--tcb->sp = 0;                                          // R0: General purpose register
    *--tcb->sp = 0;                                          // R11: General purpose register
    *--tcb->sp = 0;                                          // R10: General purpose register
    *--tcb->sp = 0;                                          // R9: General purpose register
    *--tcb->sp = 0;                                          // R8: General purpose register
    *--tcb->sp = 0;                                          // R7: General purpose register
    *--tcb->sp = 0;                                          // R6: General purpose register
    *--tcb->sp = 0;                                          // R5: General purpose register
    *--tcb->sp = 0;                                          // R4: General purpose register

    // Return success
    return 0;
}

/**
 * @brief Exit function for tasks that loops forever
 *
 */
static void exit_fn(void)
{
    while (1)
    {
    }
}