/*
 * examples.h
 *
 *  Created on: Oct 15, 2025
 *      Author: Orr Fox
 *  @brief Header file for FreeRTOS examples
 */

#ifndef EXAMPLES_EXAMPLES_H_
#define EXAMPLES_EXAMPLES_H_

#include "main.h"
#include "cmsis_os.h"
#include "logger.h"

#define EXAMPLE_1_LOGGER_MUTEX

/* =============================================================================
 * Example 1: Logger Coherency with Mutex
 * =============================================================================
 */
#ifdef EXAMPLE_1_LOGGER_MUTEX

// Task function prototypes
void Example1_LoggerTask(void *argument);
void Example1_LEDTask(void *argument);

// RTOS object creation function
void Example1_CreateTasks(void);

// Mutex handle (external, defined in main.c)
extern osMutexId_t loggerMutexHandle;

#endif // EXAMPLE_1_LOGGER_MUTEX


/* =============================================================================
 * Example 2: Task Creation - Static vs Dynamic
 * =============================================================================
 */
#ifdef EXAMPLE_2_TASK_CREATION

// Task function prototypes
void Example2_DynamicTask(void *argument);
void Example2_StaticTask(void *argument);
void Example2_MonitorTask(void *argument);

// RTOS object creation function
void Example2_CreateTasks(void);

// Static task buffers (external, defined in examples.c)
extern StackType_t xStaticTaskStack[512];
extern StaticTask_t xStaticTaskTCB;

#endif // EXAMPLE_2_TASK_CREATION


/* =============================================================================
 * Example 3: Priority-Based Scheduling
 * =============================================================================
 */
#ifdef EXAMPLE_3_PRIORITY_SCHEDULING

// Task function prototypes
void Example3_HighPriorityTask(void *argument);
void Example3_MediumPriorityTask(void *argument);
void Example3_LowPriorityTask(void *argument);

// RTOS object creation function
void Example3_CreateTasks(void);

#endif // EXAMPLE_3_PRIORITY_SCHEDULING


/* =============================================================================
 * Example 4: Counting Semaphore LED Control
 * =============================================================================
 */
#ifdef EXAMPLE_4_SEMAPHORE_LED

// Task function prototypes
void Example4_LEDProducerTask(void *argument);
void Example4_LEDConsumerTask(void *argument);

// RTOS object creation function
void Example4_CreateTasks(void);

// Semaphore handle (external, defined in main.c)
extern osSemaphoreId_t ledSemaphoreHandle;

#endif // EXAMPLE_4_SEMAPHORE_LED


/* =============================================================================
 * Example 5: vTaskDelay vs vTaskDelayUntil
 * =============================================================================
 */
#ifdef EXAMPLE_5_DELAY_COMPARISON

// Task function prototypes
void Example5_TaskDelayTask(void *argument);
void Example5_TaskDelayUntilTask(void *argument);

// RTOS object creation function
void Example5_CreateTasks(void);

#endif // EXAMPLE_5_DELAY_COMPARISON


/* =============================================================================
 * Common API - Used by main.c
 * =============================================================================
 */

/**
 * @brief Initialize and create all tasks for the selected example
 * @note This function should be called from main() after logger_init()
 *       and before osKernelStart()
 */
void Examples_Init(void);

/**
 * @brief Create RTOS mutexes and semaphores for the selected example
 * @note This function should be called after osKernelInitialize()
 */
void Examples_CreateSyncObjects(void);

#endif /* EXAMPLES_EXAMPLES_H_ */
