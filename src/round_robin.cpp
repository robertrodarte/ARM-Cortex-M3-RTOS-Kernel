#include "round_robin.h"
#include "tcb.h"
#include "systick.h"

static uint32_t interval_ms;
static uint32_t last_switch_tick;
static TCB *head;
static TCB *tail;
static TCB *current;

void RoundRobinScheduler::init(uint16_t interval)
{
    // Initialize scheduler
    interval_ms = interval;
    last_switch_tick = 0;
    head = nullptr;
    tail = nullptr;
    current = nullptr;
}

uint8_t RoundRobinScheduler::add_task(TCB *tcb)
{
    // Error catching
    if ((nullptr == tcb) || (tcb->sp == nullptr))
    {
        return 1;
    }

    // First task being added
    if (nullptr == head)
    {
        // Assign to itself
        head = tcb;
        tail = tcb;
        current = tcb;
        // Point TCB next to head
        tcb->next = head;
    }
    else
    {
        // Point tail's next to new TCB
        tail->next = tcb;
        // Assign tail to new TCB
        tail = tcb;
        // Ensure new TCB points to head
        tcb->next = head;
    }

    return 0;
}

uint8_t RoundRobinScheduler::run()
{
    // Error checking
    if (nullptr == current)
    {
        return 1;
    }

    // Check if tick count reached threshold
    if (interval_ms <= (SysTick::get_tick_counter_val() - last_switch_tick))
    {
        // Reset interval
        last_switch_tick = SysTick::get_tick_counter_val();
        // Move to next TCB
        current = current->next;
    }

    return 0;
}