#include "mpu.h"

MpuRegs *Mpu::regs()
{
    return reinterpret_cast<MpuRegs *>(MPU_BASE_ADDRESS);
}

uint8_t Mpu::configure_region(uint8_t region, uint32_t base, uint32_t attributes)
{
    // Catch out of range region
    if (7 < region)
    {
        return 1;
    }

    // Set memory region referenced by MPUBASE
    regs()->MPUNUMBER = region;

    // Set the base
    regs()->MPUBASE = base;

    // Write the attributes
    regs()->MPUATTR = attributes;

    // Return success
    return 0;
}

uint8_t Mpu::init(uint32_t first_tasks_base)
{
    // Configure the flash region
    if (configure_region(FLASH_REGION, FLASH_BASE_ADDRESS, MPU_FLASH_CONFIG))
    {
        return 1;
    }

    // Configure the stack region
    if (configure_region(STACK_REGION, first_tasks_base, MPU_STACK_CONFIG))
    {
        return 1;
    }

    // Return success
    return 0;
}

void Mpu::enable()
{
    // Enable the MPU
    regs()->MPUCTRL = MPU_MPUCTRL_EN | MPU_MPUCTRL_PRIVDEFEN_EN;
    // Wait until MPUCTRL write has completed
    __asm volatile("dsb" ::: "memory");
    // Flush so following instructions are fetched with new MPU settings
    __asm volatile("isb" ::: "memory");
}

void Mpu::set_base_stack(uint32_t base)
{
    // Set the range of the STACK_REGION to the new base address passed
    regs()->MPUBASE = base | MPU_BASE_VALID_BIT_SET | STACK_REGION;
}

/**
 * @brief Code that runs when there is a memory fault
 */
extern "C" void MemManage_Handler()
{
    while (1)
    {
        // Handle fault
    }
}
