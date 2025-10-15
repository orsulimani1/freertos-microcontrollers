/*
 * examples_config.h
 *
 *  Created on: Oct 15, 2025
 *      Author: Orr Fox
 *  @brief Configuration file for FreeRTOS examples selection
 *
 *  Uncomment ONE example at a time to run different demonstrations
 */

#ifndef EXAMPLES_EXAMPLES_CONFIG_H_
#define EXAMPLES_EXAMPLES_CONFIG_H_

/* =============================================================================
 * EXAMPLE SELECTION - Uncomment ONE example at a time
 * =============================================================================
 */

// Example 1: Logger Coherency Problem with Mutex Solution
#define EXAMPLE_1_LOGGER_MUTEX

// Example 2: Task Creation - Static vs Dynamic Allocation
// #define EXAMPLE_2_TASK_CREATION

// Example 3: Priority-Based Scheduling Demonstration
// #define EXAMPLE_3_PRIORITY_SCHEDULING

// Example 4: Counting Semaphore LED Control
// #define EXAMPLE_4_SEMAPHORE_LED

// Example 5: vTaskDelay vs vTaskDelayUntil Comparison
// #define EXAMPLE_5_DELAY_COMPARISON


/* =============================================================================
 * Validation - Ensure only one example is selected
 * =============================================================================
 */
#define COUNT_EXAMPLES \
    (defined(EXAMPLE_1_LOGGER_MUTEX) + \
     defined(EXAMPLE_2_TASK_CREATION) + \
     defined(EXAMPLE_3_PRIORITY_SCHEDULING) + \
     defined(EXAMPLE_4_SEMAPHORE_LED) + \
     defined(EXAMPLE_5_DELAY_COMPARISON))

#if COUNT_EXAMPLES == 0
    #error "No example selected! Uncomment one example in examples_config.h"
#elif COUNT_EXAMPLES > 1
    #error "Multiple examples selected! Only one example can be active at a time"
#endif

#endif /* EXAMPLES_EXAMPLES_CONFIG_H_ */
