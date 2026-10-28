#pragma once
#include "tcb.h"
#include <cstdint>

// Needs to be global and volatile since its shared with PendSV
extern "C" TCB *volatile current;

/**
 * @brief Selects the next task to be run
 */
extern "C" void rr_select_next();

/**
 * @brief Round Robin scheduler
 */
class RoundRobinScheduler
{
public:
    /**
     * @brief Initialize the scheduler
     * @param uint16_t interval_ms Time in ms of each RR interval
     */
    static void init(uint16_t interval_ms);

    /**
     * @brief Add a TCB to the scheduler
     * @return uint8_t 0 if success. 1 otherwise.
     */
    static uint8_t add_task(TCB *tcb);

    /**
     * @brief Determines if we need to switch tasks
     * @return uint8_t 0 - don't need to switch, 1 - switch
     */
    static uint8_t check_for_switch();
};
