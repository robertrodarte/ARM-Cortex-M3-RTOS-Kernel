#pragma once
#include <cstdint>
#include <cstddef>

/**
 * @brief TCB structure definition
 *
 * @param uint32_t *sp: Saved stack pointer
 * @param TCB *next: Pointer to next TCB in linked list
 * @param uint32_t *stack_base: Pointer to base of stack
 * @param uint32_t stack_size: Size of stack in words
 * @param uint32_t id: Task ID
 */
struct TCB
{
    uint32_t *sp;
    TCB *next;
    uint32_t *stack_base;
    uint32_t stack_size;
    uint32_t id;
};

// Ensure that the members are at expected offsets
static_assert(offsetof(TCB, sp) == 0);
static_assert(offsetof(TCB, next) == sizeof(uint32_t *));
static_assert(offsetof(TCB, stack_base) == sizeof(uint32_t *) + sizeof(TCB *));
static_assert(offsetof(TCB, stack_size) == sizeof(uint32_t *) + sizeof(TCB *) + sizeof(uint32_t *));
static_assert(offsetof(TCB, id) == sizeof(uint32_t *) + sizeof(TCB *) + sizeof(uint32_t *) + sizeof(uint32_t));

/**
 * @brief Define the TCB function signature
 *
 * @param uint32_t *stack_base: Pointer to the base of the stack
 * @param uint32_t stack_size: Size of the stack in words
 * @param uint32_t id: Task ID
 * @param void (*entry_fn)(void): Pointer to the task entry function
 * @param TCB *tcb: Pointer to the TCB structure
 * @return uint8_t: Return 0 on success, non-zero on failure
 */
uint8_t tcb_init(uint32_t *stack_base, uint32_t stack_size, uint32_t id, void (*entry_fn)(void), TCB *tcb);
