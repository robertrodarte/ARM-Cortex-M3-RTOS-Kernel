#pragma once
#include <cstdint>
#include <cstddef>

// Define all UART base addresses
constexpr uint32_t UART0_BASE_ADDRESS = 0x4000C000;
constexpr uint32_t UART1_BASE_ADDRESS = 0x4000D000;
constexpr uint32_t UART2_BASE_ADDRESS = 0x4000E000;

// Define UART register offsets
constexpr uint32_t UARTDR_OFFSET = 0x000;
constexpr uint32_t UARTRSR_OFFSET = 0x004;
constexpr uint32_t UARTFR_OFFSET = 0x018;
constexpr uint32_t UARTILPR_OFFSET = 0x020;
constexpr uint32_t UARTIBRD_OFFSET = 0x024;
constexpr uint32_t UARTFBRD_OFFSET = 0x028;
constexpr uint32_t UARTLCRH_OFFSET = 0x02C;
constexpr uint32_t UARTCTL_OFFSET = 0x030;
constexpr uint32_t UARTIFLS_OFFSET = 0x034;
constexpr uint32_t UARTIM_OFFSET = 0x038;
constexpr uint32_t UARTRIS_OFFSET = 0x03C;
constexpr uint32_t UARTMIS_OFFSET = 0x040;
constexpr uint32_t UARTICR_OFFSET = 0x044;
constexpr uint32_t UARTPERIPHID4_OFFSET = 0xFD0;
constexpr uint32_t UARTPERIPHID5_OFFSET = 0xFD4;
constexpr uint32_t UARTPERIPHID6_OFFSET = 0xFD8;
constexpr uint32_t UARTPERIPHID7_OFFSET = 0xFDC;
constexpr uint32_t UARTPERIPHID0_OFFSET = 0xFE0;
constexpr uint32_t UARTPERIPHID1_OFFSET = 0xFE4;
constexpr uint32_t UARTPERIPHID2_OFFSET = 0xFE8;
constexpr uint32_t UARTPERIPHID3_OFFSET = 0xFEC;

// System control: clock gating
constexpr uint32_t SYSCTL_RCGC1_ADDRESS = 0x400FE104;
constexpr uint32_t SYSCTL_RCGC2_ADDRESS = 0x400FE108;
constexpr uint32_t SYSCTL_RCGC1_UART0 = (1u << 0);
constexpr uint32_t SYSCTL_RCGC2_GPIOA = (1u << 0);

// GPIO Port A - UART0 pins (PA0 = U0RX, PA1 = U0TX)
constexpr uint32_t GPIOA_AFSEL_ADDRESS = 0x40004420;
constexpr uint32_t GPIOA_DEN_ADDRESS = 0x4000451C;
constexpr uint32_t GPIOA_UART0_PINS = (1u << 0) | (1u << 1);

// UART register bits
constexpr uint32_t UARTFR_TXFF = (1u << 5);
constexpr uint32_t UARTLCRH_FEN = (1u << 4);
constexpr uint32_t UARTLCRH_WLEN_8_BITS = (0x3u << 5);
constexpr uint32_t UARTCTL_UARTEN = (1u << 0);
constexpr uint32_t UARTCTL_TXE = (1u << 8);
constexpr uint32_t UARTCTL_RXE = (1u << 9);

/**
 * @brief Defines UART registers
 */
struct UartRegs
{
    volatile uint32_t UARTDR;
    volatile uint32_t UARTRSR;
    volatile uint32_t RESERVED0[4];
    volatile uint32_t UARTFR;
    volatile uint32_t RESERVED1[1];
    volatile uint32_t UARTILPR;
    volatile uint32_t UARTIBRD;
    volatile uint32_t UARTFBRD;
    volatile uint32_t UARTLCRH;
    volatile uint32_t UARTCTL;
    volatile uint32_t UARTIFLS;
    volatile uint32_t UARTIM;
    volatile uint32_t UARTRIS;
    volatile uint32_t UARTMIS;
    volatile uint32_t UARTICR;
    volatile uint32_t RESERVED2[994];
    volatile uint32_t UARTPERIPHID4;
    volatile uint32_t UARTPERIPHID5;
    volatile uint32_t UARTPERIPHID6;
    volatile uint32_t UARTPERIPHID7;
    volatile uint32_t UARTPERIPHID0;
    volatile uint32_t UARTPERIPHID1;
    volatile uint32_t UARTPERIPHID2;
    volatile uint32_t UARTPERIPHID3;
};

// Ensure that the members are at expected offsets
static_assert(offsetof(UartRegs, UARTDR) == UARTDR_OFFSET);
static_assert(offsetof(UartRegs, UARTRSR) == UARTRSR_OFFSET);
static_assert(offsetof(UartRegs, UARTFR) == UARTFR_OFFSET);
static_assert(offsetof(UartRegs, UARTILPR) == UARTILPR_OFFSET);
static_assert(offsetof(UartRegs, UARTIBRD) == UARTIBRD_OFFSET);
static_assert(offsetof(UartRegs, UARTFBRD) == UARTFBRD_OFFSET);
static_assert(offsetof(UartRegs, UARTLCRH) == UARTLCRH_OFFSET);
static_assert(offsetof(UartRegs, UARTCTL) == UARTCTL_OFFSET);
static_assert(offsetof(UartRegs, UARTIFLS) == UARTIFLS_OFFSET);
static_assert(offsetof(UartRegs, UARTIM) == UARTIM_OFFSET);
static_assert(offsetof(UartRegs, UARTRIS) == UARTRIS_OFFSET);
static_assert(offsetof(UartRegs, UARTMIS) == UARTMIS_OFFSET);
static_assert(offsetof(UartRegs, UARTICR) == UARTICR_OFFSET);
static_assert(offsetof(UartRegs, UARTPERIPHID4) == UARTPERIPHID4_OFFSET);
static_assert(offsetof(UartRegs, UARTPERIPHID5) == UARTPERIPHID5_OFFSET);
static_assert(offsetof(UartRegs, UARTPERIPHID6) == UARTPERIPHID6_OFFSET);
static_assert(offsetof(UartRegs, UARTPERIPHID7) == UARTPERIPHID7_OFFSET);
static_assert(offsetof(UartRegs, UARTPERIPHID0) == UARTPERIPHID0_OFFSET);
static_assert(offsetof(UartRegs, UARTPERIPHID1) == UARTPERIPHID1_OFFSET);
static_assert(offsetof(UartRegs, UARTPERIPHID2) == UARTPERIPHID2_OFFSET);
static_assert(offsetof(UartRegs, UARTPERIPHID3) == UARTPERIPHID3_OFFSET);

/**
 * @brief Helper to write registers quickly
 */
static volatile uint32_t &reg32(uint32_t address)
{
    return *reinterpret_cast<volatile uint32_t *>(address);
}

/**
 * @brief UART peripheral class
 */
template <uint32_t Base> class Uart
{
    /**
     * @brief Returns a pointer to the UartRegs struct address
     */

    static UartRegs *regs()
    {
        return reinterpret_cast<UartRegs *>(Base);
    }

public:
    /**
     * @brief Initialize a UART peripheral. Note: Clock and pin setup is UART0 only (Port A)
     * @param clock_freq - Clock frequency
     * @param baud - baud rate of UART
     * @return 0 - init complete, 1 - init errored
     */
    static uint8_t init(uint32_t clock_freq, uint32_t baud)
    {
        // Catch divide by zero error
        if (0 == baud)
        {
            return 1;
        }

        // Calculate baud divisor
        uint32_t div64 = (clock_freq * 4 + baud / 2) / baud;
        uint32_t ibrd = div64 >> 6;   // Int part of the baud rate divisor
        uint32_t fbrd = div64 & 0x3F; // Fractional part of the baud rate divisor

        // Catch too fast of baud rate
        if (0 == ibrd)
        {
            return 1;
        }

        // Configure clock for UART
        reg32(SYSCTL_RCGC1_ADDRESS) |= SYSCTL_RCGC1_UART0;
        // Enable clock for GPIOA
        reg32(SYSCTL_RCGC2_ADDRESS) |= SYSCTL_RCGC2_GPIOA;

        // Delay after enabling clocks
        uint32_t dummy = reg32(SYSCTL_RCGC2_ADDRESS);
        // Suppress warning
        (void)dummy;

        // Enable GPIOA alternate function for UART
        reg32(GPIOA_AFSEL_ADDRESS) |= GPIOA_UART0_PINS;
        // Enable digital function on PA0 and PA1
        reg32(GPIOA_DEN_ADDRESS) |= GPIOA_UART0_PINS;

        // Ensure UART is disabled
        regs()->UARTCTL &= ~(UARTCTL_UARTEN);
        // Set the integer baud rate divisor
        regs()->UARTIBRD = ibrd;
        // Set the fractional portion of the baud rate divisor
        regs()->UARTFBRD = fbrd;
        // Set word length and enable use of FIFO buffering and latches the divisors
        regs()->UARTLCRH = (UARTLCRH_WLEN_8_BITS | UARTLCRH_FEN);
        // Enable UART, UART receive, and UART transmit
        regs()->UARTCTL = (UARTCTL_UARTEN | UARTCTL_RXE | UARTCTL_TXE);

        // Return success
        return 0;
    }

    /**
     * @brief Pass a char to UART
     */
    static void put_char(char c)
    {
        // Wait if transmit FIFO full
        while (regs()->UARTFR & UARTFR_TXFF)
        {
        };

        // Write character to data register
        regs()->UARTDR = c;
    }

    /**
     * @brief Pass a string to UART
     * @return uint8_t 0 - success, 1- otherwise
     */
    static uint8_t put_str(const char *s)
    {
        // Catch null pointer
        if (nullptr == s)
        {
            return 1;
        }

        while (*s != '\0')
        {
            put_char(*s++);
        }

        // Return success
        return 0;
    }

    /**
     * @brief Pass a hexadecimal value to UART
     */
    static void put_hex(uint32_t hex)
    {
        uint8_t nibble = 0;

        // Print the start of the hexadecimal
        put_str("0x");

        // Loop through each nibble in the hex and print it
        for (int i = 7; i >= 0; i--)
        {
            nibble = (hex >> (i * 4)) & 0xF;
            if (9 >= nibble)
            {
                // Conversion for 0-9
                put_char('0' + nibble);
            }
            else
            {
                // Conversion for 10-15
                put_char('A' + (nibble - 10));
            }
        }
    }
};

// Create an alias for UART0
using Uart0 = Uart<UART0_BASE_ADDRESS>;
