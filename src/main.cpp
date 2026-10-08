#include "tcb.h"
#include "systick.h"
#include "round_robin.h"

// Defines
static constexpr uint32_t TCB_STACK_SIZE_WORDS = 256;
static constexpr uint32_t TICK_RATE_TICKS_PER_SECOND = 1000; // 1 ms tick rate
static constexpr uint32_t RR_INTERVAL_MS = 10;               // 10 ms interval

// Variables
TCB tcb1, tcb2, tcb3; // Create TCBs

// Define each TCB's task stack
alignas(8) uint32_t tcb1_stack[TCB_STACK_SIZE_WORDS];
alignas(8) uint32_t tcb2_stack[TCB_STACK_SIZE_WORDS];
alignas(8) uint32_t tcb3_stack[TCB_STACK_SIZE_WORDS];

// Prototypes
static void error_fn();
static void tcb1_entry_fn();
static void tcb2_entry_fn();
static void tcb3_entry_fn();

/**
 * @brief Main function
 */
int main()
{
    // Initialize the SysTick peripheral
    if (SysTick::init(SYSTICK_CLOCK_FREQ, TICK_RATE_TICKS_PER_SECOND))
    {
        error_fn();
    }

    // Initialize the TCBs
    if (0 != tcb_init(tcb1_stack, TCB_STACK_SIZE_WORDS, 1, tcb1_entry_fn, &tcb1))
    {
        error_fn();
    }
    if (0 != tcb_init(tcb2_stack, TCB_STACK_SIZE_WORDS, 2, tcb2_entry_fn, &tcb2))
    {
        error_fn();
    }
    if (0 != tcb_init(tcb3_stack, TCB_STACK_SIZE_WORDS, 3, tcb3_entry_fn, &tcb3))
    {
        error_fn();
    }

    // Initialize and start RR scheduler
    RoundRobinScheduler::init(RR_INTERVAL_MS);
    if (RoundRobinScheduler::add_task(&tcb1))
    {
        error_fn();
    }
    if (RoundRobinScheduler::add_task(&tcb2))
    {
        error_fn();
    }
    if (RoundRobinScheduler::add_task(&tcb3))
    {
        error_fn();
    }

    // Start SysTick peripheral
    SysTick::start();

    // Infinite loop
    while (1)
    {
        // Logic goes here
    }

    return 0;
}

/**
 * @brief Infinite loop to catch errors
 */
static void error_fn()
{
    while (1)
    {
    }
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