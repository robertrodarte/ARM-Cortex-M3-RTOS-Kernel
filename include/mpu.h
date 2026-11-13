#pragma once
#include <cstdint>
#include <cstddef>

// Define flash and stack addresses
constexpr uint32_t FLASH_BASE_ADDRESS = 0x00000000;

// Define flash and stack region
constexpr uint8_t FLASH_REGION = 0;
constexpr uint8_t STACK_REGION = 1;

// Define base address and register offsets for MPU
constexpr uint32_t MPU_BASE_ADDRESS = 0xE000E000;
constexpr uint32_t MPU_MPUTYPE_OFFSET = 0xD90;
constexpr uint32_t MPU_MPUCTRL_OFFSET = 0xD94;
constexpr uint32_t MPU_MPUNUMBER_OFFSET = 0xD98;
constexpr uint32_t MPU_MPUBASE_OFFSET = 0xD9C;
constexpr uint32_t MPU_MPUATTR_OFFSET = 0xDA0;
constexpr uint32_t MPU_MPUBASE1_OFFSET = 0xDA4;
constexpr uint32_t MPU_MPUATTR1_OFFSET = 0xDA8;
constexpr uint32_t MPU_MPUBASE2_OFFSET = 0xDAC;
constexpr uint32_t MPU_MPUATTR2_OFFSET = 0xDB0;
constexpr uint32_t MPU_MPUBASE3_OFFSET = 0xDB4;
constexpr uint32_t MPU_MPUATTR3_OFFSET = 0xDB8;

// Define MPUBASE bits
constexpr uint32_t MPU_BASE_VALID_BIT_SET = (1u << 4); // Allows REGION field in MPUBASE to define region

// Define MPUCTRL bits
constexpr uint32_t MPU_MPUCTRL_EN = (1u << 0);           // MPU enable
constexpr uint32_t MPU_MPUCTRL_HFNMIENA_EN = (1u << 1);  // MPU enabled during faults
constexpr uint32_t MPU_MPUCTRL_PRIVDEFEN_EN = (1u << 2); // MPU default region

// Define MPUATTR bits
constexpr uint32_t MPU_MPUATTR_EN = (1u << 0);
constexpr uint32_t MPU_MPUATTR_REGION_SIZE_SHIFT = 1; // Bits 5:1 define size of region
constexpr uint32_t MPU_MPUATTR_SRD_SHIFT = 8;         // Bits 15:8 define subregion enable/disable
constexpr uint32_t MPU_MPUATTR_SCB_SHIFT = 16;        // Bits 18:16 define SCB config
constexpr uint32_t MPU_MPUATTR_TEX_SHIFT = 19;        // Bits 21:19 define type extension mask
constexpr uint32_t MPU_MPUATTR_AP_SHIFT = 24;         // Bits 26:24 define access priviledge
constexpr uint32_t MPU_MPUATTR_XN = (1u << 28);       // Instruction fetches are disabled

// Define SIZE values (2^(SIZE + 1))
constexpr uint32_t MPU_STACK_REGION_SIZE = (9u << MPU_MPUATTR_REGION_SIZE_SHIFT);  // 1KB
constexpr uint32_t MPU_FLASH_REGION_SIZE = (17u << MPU_MPUATTR_REGION_SIZE_SHIFT); // 256 KB

// Define S,C,B bit configurations
constexpr uint32_t MPU_FLASH_SCB_CONFIG = (2u << MPU_MPUATTR_SCB_SHIFT);
constexpr uint32_t MPU_SRAM_SCB_CONFIG = (6u << MPU_MPUATTR_SCB_SHIFT);

// Define AP bit configurations
constexpr uint32_t MPU_FLASH_AP_CONFIG = (6u << MPU_MPUATTR_AP_SHIFT);
constexpr uint32_t MPU_STACK_AP_CONFIG = (3u << MPU_MPUATTR_AP_SHIFT);

// Define flash and stack configurations
constexpr uint32_t MPU_FLASH_CONFIG =
    (MPU_MPUATTR_EN | MPU_FLASH_REGION_SIZE | MPU_FLASH_SCB_CONFIG | MPU_FLASH_AP_CONFIG);
constexpr uint32_t MPU_STACK_CONFIG =
    (MPU_MPUATTR_EN | MPU_STACK_REGION_SIZE | MPU_SRAM_SCB_CONFIG | MPU_STACK_AP_CONFIG | MPU_MPUATTR_XN);

/**
 * @brief Define MPU registers
 */
struct MpuRegs
{
    volatile uint32_t RESERVED[MPU_MPUTYPE_OFFSET / 4];
    volatile uint32_t MPUTYPE;
    volatile uint32_t MPUCTRL;
    volatile uint32_t MPUNUMBER;
    volatile uint32_t MPUBASE;
    volatile uint32_t MPUATTR;
    volatile uint32_t MPUBASE1;
    volatile uint32_t MPUATTR1;
    volatile uint32_t MPUBASE2;
    volatile uint32_t MPUATTR2;
    volatile uint32_t MPUBASE3;
    volatile uint32_t MPUATTR3;
};

// Ensure that the members are at expected offsets
static_assert(offsetof(MpuRegs, MPUTYPE) == MPU_MPUTYPE_OFFSET);
static_assert(offsetof(MpuRegs, MPUCTRL) == MPU_MPUCTRL_OFFSET);
static_assert(offsetof(MpuRegs, MPUNUMBER) == MPU_MPUNUMBER_OFFSET);
static_assert(offsetof(MpuRegs, MPUBASE) == MPU_MPUBASE_OFFSET);
static_assert(offsetof(MpuRegs, MPUBASE1) == MPU_MPUBASE1_OFFSET);
static_assert(offsetof(MpuRegs, MPUBASE2) == MPU_MPUBASE2_OFFSET);
static_assert(offsetof(MpuRegs, MPUBASE3) == MPU_MPUBASE3_OFFSET);
static_assert(offsetof(MpuRegs, MPUATTR) == MPU_MPUATTR_OFFSET);
static_assert(offsetof(MpuRegs, MPUATTR1) == MPU_MPUATTR1_OFFSET);
static_assert(offsetof(MpuRegs, MPUATTR2) == MPU_MPUATTR2_OFFSET);
static_assert(offsetof(MpuRegs, MPUATTR3) == MPU_MPUATTR3_OFFSET);

/**
 * @brief MPU peripheral class
 */
class Mpu
{
    /**
     * @brief Returns a pointer to the MpuRegs struct address
     */
    static MpuRegs *regs();

    /**
     * @brief Configures a region in the MPU
     *
     * @param uint8_t region: Defines a region 0-7
     * @param uint32_t base: Region start address.
     * @param uint32_t attributes: Raw MPUATTR value defining attributes of region
     */
    static uint8_t configure_region(uint8_t region, uint32_t base, uint32_t attributes);

public:
    /**
     * @brief Initialize the MPU
     *
     * @return uint8_t: Return 0 on success, non-zero on failure
     */
    static uint8_t init(uint32_t first_tasks_base);

    /**
     * @brief Enable the MPU
     */
    static void enable();

    /**
     * @brief Set a new base stack for the stack region
     *
     * @param base: Base address of TCB stack
     */
    static void set_base_stack(uint32_t base);
};
