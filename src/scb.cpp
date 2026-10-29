#include "scb.h"

ScbRegs *Scb::regs()
{
    return reinterpret_cast<ScbRegs *>(SCB_BASE_ADDRESS);
}

void Scb::pend_pendsv()
{
    // Set the INTCTRL pend bit
    // (Note: Writing "=" instead of "|=" is okay here since writing 0s
    // to the other BITs do nothing as referenced in the datasheet)
    regs()->INTCTRL = INTCTRL_PEND_PENDSV_BIT;
}

uint8_t Scb::set_pendsv_priority(uint8_t priority)
{
    // Catch out of range priority
    if (MAX_PENDSV_PRIORITY < priority)
    {
        return 1;
    }

    // Clear the current priority
    regs()->SYSPRI3 &= ~(SYSPRI3_PENDSV_MASK);

    // Set the priority BITs
    regs()->SYSPRI3 |= (priority << SYSPRI3_PENDSV_PRIORITY_BIT_SHIFT);

    // Return success
    return 0;
}
