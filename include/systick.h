#pragma once
#include <cstdint>
#include <cstddef>

// Define SysTick clock frequency
constexpr uint32_t SYSTICK_CLOCK_FREQ = 12500000; // Cycles per second

// Define the base address and register offsets for SysTick
constexpr uint32_t SYSTICK_BASE_ADDRESS = 0xE000E000;
constexpr uint32_t SYSTICK_STCTRL_OFFSET = 0x010;
constexpr uint32_t SYSTICK_STRELOAD_OFFSET = 0x014;
constexpr uint32_t SYSTICK_STCURRENT_OFFSET = 0x018;

// Define STCTRL bits
constexpr uint32_t STCTRL_ENABLE = 1u << 0;           // 1 = Enabled, 0 = Disabled
constexpr uint32_t STCTRL_INTERRUPT_ENABLE = 1u << 1; // 1 = Enabled, 0 = Disabled
constexpr uint32_t STCTRL_CLOCK_SOURCE = 1u << 2;     // 1 = System clock, 0 = External Reference

// Define STRELOAD bits
constexpr uint32_t STRELOAD_MAX_VAL = 0x00FFFFFF;

/**
 * @brief Defines SysTick registers
 */
struct SysTickRegs
{
    volatile uint32_t RESERVED[4];
    // SysTick Control and Status Register
    volatile uint32_t STCTRL;
    // SysTick Reload Value Register
    volatile uint32_t STRELOAD;
    // SysTick Current Value Register
    volatile uint32_t STCURRENT;
};

// Ensure that the members are at expected offsets
static_assert(offsetof(SysTickRegs, STCTRL) == SYSTICK_STCTRL_OFFSET);
static_assert(offsetof(SysTickRegs, STRELOAD) == SYSTICK_STRELOAD_OFFSET);
static_assert(offsetof(SysTickRegs, STCURRENT) == SYSTICK_STCURRENT_OFFSET);

/**
 * @brief SysTick peripheral class
 */
class SysTick
{
    /**
     * @brief Returns a pointer to the SysTickRegs struct address
     */
    static SysTickRegs *regs();

public:
    /**
     * @brief Method to intialize SysTick peripheral
     * @param uint32_t clock_frequency: Frequency of the CPU clock
     * @param uint32_t tick_rate: Desired tick rate per second
     * @return uint8_t 1 - Failure. 0 - Success
     */
    static uint8_t init(uint32_t clock_frequency, uint32_t tick_rate);

    /**
     * @brief Returns the value of the tick_count.
     * @return uint32_t Value of tick_count.
     */
    static uint32_t get_tick_counter_val();

    /**
     * @brief Starts SysTick peripheral
     */
    static void start();
};