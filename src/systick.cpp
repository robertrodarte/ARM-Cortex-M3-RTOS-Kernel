#include "systick.h"
#include "round_robin.h"

static volatile uint32_t tick_counter;

SysTickRegs *SysTick::regs()
{
    return reinterpret_cast<SysTickRegs *>(SYSTICK_BASE_ADDRESS);
}

uint8_t SysTick::init(uint32_t clock_frequency, uint32_t tick_rate)
{
    // Catch error
    if (0 == tick_rate)
    {
        return 1;
    }

    // Determine load value
    uint32_t load_val = (clock_frequency / tick_rate) - 1;

    // Catch error
    if ((STRELOAD_MAX_VAL < load_val) || (0 == load_val))
    {
        return 1;
    }

    // Initialize the load register
    regs()->STRELOAD = load_val;

    // Initialize STCURRENT register
    regs()->STCURRENT = 0;

    // Initialize STCTRL register
    regs()->STCTRL = STCTRL_CLOCK_SOURCE;

    // Return success
    return 0;
}

uint32_t SysTick::get_tick_counter_val()
{
    return tick_counter;
}

void SysTick::start()
{
    // Set enable bits
    regs()->STCTRL |= (STCTRL_ENABLE | STCTRL_INTERRUPT_ENABLE);
}

extern "C" void SysTick_Handler()
{
    // Run the scheduler
    RoundRobinScheduler::run();

    // SysTick counter reached 0, increment tick count
    tick_counter++;
}