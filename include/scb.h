#pragma once
#include <cstdint>
#include <cstddef>

// Define base address and register offsets for SCB
constexpr uint32_t SCB_BASE_ADDRESS = 0xE000E000;
constexpr uint32_t SCB_CPUID_OFFSET = 0xD00;
constexpr uint32_t SCB_INTCTRL_OFFSET = 0xD04;
constexpr uint32_t SCB_VTABLE_OFFSET = 0xD08;
constexpr uint32_t SCB_APINT_OFFSET = 0xD0C;
constexpr uint32_t SCB_SYSCTRL_OFFSET = 0xD10;
constexpr uint32_t SCB_CFGCTRL_OFFSET = 0xD14;
constexpr uint32_t SCB_SYSPRI1_OFFSET = 0xD18;
constexpr uint32_t SCB_SYSPRI2_OFFSET = 0xD1C;
constexpr uint32_t SCB_SYSPRI3_OFFSET = 0xD20;
constexpr uint32_t SCB_SYSHNDCTRL_OFFSET = 0xD24;
constexpr uint32_t SCB_FAULTSTAT_OFFSET = 0xD28;
constexpr uint32_t SCB_HFAULTSTAT_OFFSET = 0xD2C;
constexpr uint32_t SCB_MMADDR_OFFSET = 0xD34;
constexpr uint32_t SCB_FAULTADDR_OFFSET = 0xD38;

// Define INTCTRL bits
constexpr uint32_t INTCTRL_PEND_PENDSV_BIT = (1u << 28);

// Define PendSV priority values
constexpr uint16_t SYSPRI3_PENDSV_PRIORITY_BIT_SHIFT = 21;
constexpr uint32_t SYSPRI3_PENDSV_MASK = (7u << SYSPRI3_PENDSV_PRIORITY_BIT_SHIFT);
constexpr uint8_t MAX_PENDSV_PRIORITY = 7;

/**
 * @brief Define SCB registers
 */
struct ScbRegs
{
    volatile uint32_t RESERVED0[SCB_CPUID_OFFSET / 4];
    volatile uint32_t CPUID;
    volatile uint32_t INTCTRL;
    volatile uint32_t VTABLE;
    volatile uint32_t APINT;
    volatile uint32_t SYSCTRL;
    volatile uint32_t CFGCTRL;
    volatile uint32_t SYSPRI1;
    volatile uint32_t SYSPRI2;
    volatile uint32_t SYSPRI3;
    volatile uint32_t SYSHNDCTRL;
    volatile uint32_t FAULTSTAT;
    volatile uint32_t HFAULTSTAT;
    volatile uint32_t RESERVED1[1];
    volatile uint32_t MMADDR;
    volatile uint32_t FAULTADDR;
};

// Ensure that the members are at expected offsets
static_assert(offsetof(ScbRegs, CPUID) == SCB_CPUID_OFFSET);
static_assert(offsetof(ScbRegs, INTCTRL) == SCB_INTCTRL_OFFSET);
static_assert(offsetof(ScbRegs, VTABLE) == SCB_VTABLE_OFFSET);
static_assert(offsetof(ScbRegs, APINT) == SCB_APINT_OFFSET);
static_assert(offsetof(ScbRegs, SYSCTRL) == SCB_SYSCTRL_OFFSET);
static_assert(offsetof(ScbRegs, CFGCTRL) == SCB_CFGCTRL_OFFSET);
static_assert(offsetof(ScbRegs, SYSPRI1) == SCB_SYSPRI1_OFFSET);
static_assert(offsetof(ScbRegs, SYSPRI2) == SCB_SYSPRI2_OFFSET);
static_assert(offsetof(ScbRegs, SYSPRI3) == SCB_SYSPRI3_OFFSET);
static_assert(offsetof(ScbRegs, SYSHNDCTRL) == SCB_SYSHNDCTRL_OFFSET);
static_assert(offsetof(ScbRegs, FAULTSTAT) == SCB_FAULTSTAT_OFFSET);
static_assert(offsetof(ScbRegs, HFAULTSTAT) == SCB_HFAULTSTAT_OFFSET);
static_assert(offsetof(ScbRegs, MMADDR) == SCB_MMADDR_OFFSET);
static_assert(offsetof(ScbRegs, FAULTADDR) == SCB_FAULTADDR_OFFSET);

/**
 * @brief SCB peripheral class
 */
class Scb
{
    /**
     * @brief Returns a pointer to the ScbRegs struct address
     */
    static ScbRegs *regs();

public:
    /**
     * @brief Pend the PendSV handler
     */
    static void pend_pendsv();

    /**
     * @brief Set the PendSV priority (Note: Range = 0-7, Low = High Priority)
     * @return uint8_t 0 - Set successful, 1 - otherwise
     */
    static uint8_t set_pendsv_priority(uint8_t priority);
};
