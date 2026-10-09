#pragma once
#include <cstdint>
#include <cstddef>
#include "tcb.h"

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
     * @brief Run the scheduler
     * @return uint8_t 0 - if sucess. 1 - otherwise.
     */
    static uint8_t run();
};