#include "tcb.h"
#include "systick.h"
#include "round_robin.h"
#include "scb.h"
#include "uart.h"

// Defines
static constexpr uint32_t TCB_STACK_SIZE_WORDS = 256;
static constexpr uint32_t TICK_RATE_TICKS_PER_SECOND = 1000; // 1 ms tick rate
static constexpr uint32_t RR_INTERVAL_MS = 10;               // 10 ms interval
static constexpr uint8_t PENDSV_PRIORITY = 7;
static constexpr uint32_t UART_BAUD_RATE = 115200;
static constexpr uint32_t LOG_SIZE = 30;

// Structs
struct LogBuffer
{
    // Task ID
    uint32_t id;
    // Tick count
    uint32_t tick;
};

// Variables
TCB tcb1, tcb2, tcb3; // Create TCBs

// Define a log buffer array
LogBuffer log_buf[LOG_SIZE];

// Shared index for log buffer
volatile uint32_t log_buffer_index;

// Define each TCB's task stack
alignas(8) uint32_t tcb1_stack[TCB_STACK_SIZE_WORDS];
alignas(8) uint32_t tcb2_stack[TCB_STACK_SIZE_WORDS];
alignas(8) uint32_t tcb3_stack[TCB_STACK_SIZE_WORDS];

// Prototypes
static void error_fn();
static void tcb1_entry_fn();
static void tcb2_entry_fn();
static void tcb3_entry_fn();
static void log_event(uint32_t id);

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

    // Set PendSV priority
    if (Scb::set_pendsv_priority(PENDSV_PRIORITY))
    {
        error_fn();
    }

    // Configure UART0
    if (Uart0::init(SYSTICK_CLOCK_FREQ, UART_BAUD_RATE))
    {
        error_fn();
    }

    // Pass message to UART0
    Uart0::put_str("Hello from UART\r\n");

    // Log TCB 1 ID
    log_event(tcb1.id);

    // Start SysTick peripheral
    SysTick::start();

    // Launch the first task, switch to thread mode (PSP), and never return
    __asm volatile("svc 0");

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
    // Define local variables
    static uint32_t now = 0;
    static uint32_t last = 0;
    static bool dumped = false;

    // Task 1 logic goes here
    while (1)
    {
        // Check if we need to log an event
        now = SysTick::get_tick_counter_val();
        if (now - last > 1)
        {
            log_event(tcb1.id);
        }
        last = now;

        // Dump the log_buf if full
        if ((LOG_SIZE == log_buffer_index) && !dumped)
        {
            for (uint32_t i = 0; i < LOG_SIZE; i++)
            {
                // Print the task ID
                Uart0::put_str("id: ");
                Uart0::put_hex(log_buf[i].id);
                Uart0::put_str("\r\n");

                // Print the tick
                Uart0::put_str("tick: ");
                Uart0::put_hex(log_buf[i].tick);
                Uart0::put_str("\r\n");
            }
            dumped = true;
        }
    }
}

/**
 * @brief Entry function for TCB2
 */
static void tcb2_entry_fn()
{
    // Define local variables
    static uint32_t now = 0;
    static uint32_t last = 0;

    // Task 2 logic goes here
    while (1)
    {
        // Check if we need to log an event
        now = SysTick::get_tick_counter_val();
        if (now - last > 1)
        {
            log_event(tcb2.id);
        }
        last = now;
    }
}

/**
 * @brief Entry function for TCB3
 */
static void tcb3_entry_fn()
{
    // Define local variables
    static uint32_t now = 0;
    static uint32_t last = 0;

    // Task 3 logic goes here
    while (1)
    {
        // Check if we need to log an event
        now = SysTick::get_tick_counter_val();
        if (now - last > 1)
        {
            log_event(tcb3.id);
        }
        last = now;
    }
}

/**
 * @brief Record a log event for task ID and tick count
 */
static void log_event(uint32_t id)
{
    // Mask interrupts
    __asm volatile("cpsid i" ::: "memory");

    // Check if buffer is full
    if (LOG_SIZE > log_buffer_index)
    {
        // Store current values and increment index
        log_buf[log_buffer_index++] = {id, SysTick::get_tick_counter_val()};
    }

    // Unmask interrupts
    __asm volatile("cpsie i" ::: "memory");
}
