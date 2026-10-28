#include "round_robin.h"
#include "systick.h"
#include "tcb.h"

static uint32_t interval_ms;
static uint32_t last_switch_tick;
static TCB *head;
static TCB *tail;
TCB *volatile current;

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
    if ((nullptr == tcb) || (nullptr == tcb->sp))
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

uint8_t RoundRobinScheduler::check_for_switch()
{
    // Check if tick count reached threshold
    if (interval_ms <= (SysTick::get_tick_counter_val() - last_switch_tick))
    {
        // Reset interval
        last_switch_tick = SysTick::get_tick_counter_val();
        // Return 1 for switch
        return 1;
    }

    // Return 0 for no switch
    return 0;
}

extern "C" void rr_select_next()
{
    //  Move to next TCB
    current = current->next;
}
