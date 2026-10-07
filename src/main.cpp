#include "tcb.h"

// Define TCB stack size
constexpr uint32_t TCB_STACK_SIZE_WORDS = 256;

// Define TCBs
TCB tcb1, tcb2, tcb3;

// Define each TCB's task stack
alignas(8) uint32_t tcb1_stack[TCB_STACK_SIZE_WORDS];
alignas(8) uint32_t tcb2_stack[TCB_STACK_SIZE_WORDS];
alignas(8) uint32_t tcb3_stack[TCB_STACK_SIZE_WORDS];

// Prototypes for tasks entry functions
static void tcb1_entry_fn();
static void tcb2_entry_fn();
static void tcb3_entry_fn();

/**
 * @brief Main function
 */
int main()
{
    // Initialize the TCBs
    if (0 != tcb_init(tcb1_stack, TCB_STACK_SIZE_WORDS, 1, tcb1_entry_fn, &tcb1))
    {
        while (1)
        {
        }
    }

    if (0 != tcb_init(tcb2_stack, TCB_STACK_SIZE_WORDS, 2, tcb2_entry_fn, &tcb2))
    {
        while (1)
        {
        }
    }

    if (0 != tcb_init(tcb3_stack, TCB_STACK_SIZE_WORDS, 3, tcb3_entry_fn, &tcb3))
    {
        while (1)
        {
        }
    }

    // Infinite loop
    while (1)
    {
        // Logic goes here
    }

    return 0;
}

/**
 * @brief Entry function for TCB1
 */
static void tcb1_entry_fn()
{
    // Task 1 logic goes here
    while (1)
    {
    }
}

/**
 * @brief Entry function for TCB2
 */
static void tcb2_entry_fn()
{
    // Task 2 logic goes here
    while (1)
    {
    }
}

/**
 * @brief Entry function for TCB3
 */
static void tcb3_entry_fn()
{
    // Task 3 logic goes here
    while (1)
    {
    }
}