/*
 * examples.c
 *
 *  Created on: Oct 15, 2025
 *      Author: Orr Fox
 *  @brief Implementation of FreeRTOS examples
 */

#include "examples.h"
#include "examples_config.h"

/* =============================================================================
 * Example 1: Logger Coherency with Mutex Protection
 * =============================================================================
 * Demonstrates:
 * - Problem: Multiple tasks logging simultaneously causes garbled output
 * - Solution: Mutex protects UART/logger from concurrent access
 * - Key Concepts: Mutual exclusion, critical sections, resource protection
 */
#ifdef EXAMPLE_1_LOGGER_MUTEX

// Task handles
static osThreadId_t example1_loggerHandle;
static osThreadId_t example1_ledHandle;

// Task attributes
const osThreadAttr_t example1_logger_attributes = {
    .name = "Logger",
    .stack_size = 1024 * 4,  // 4KB for 512-byte logger buffer
    .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t example1_led_attributes = {
    .name = "LED",
    .stack_size = 512 * 4,   // 2KB
    .priority = (osPriority_t) osPriorityNormal,
};

/**
 * @brief Logger task - logs messages periodically
 */
void Example1_LoggerTask(void *argument)
{
    uint32_t counter = 0;

    for(;;)
    {
        LOG_INFO("Logger", "FreeRTOS running - count=%lu", counter++);
        osDelay(250);  // 250ms delay
    }
}

/**
 * @brief LED task - toggles LEDs and logs status
 */
void Example1_LEDTask(void *argument)
{
    for(;;)
    {
        // Toggle all 4 LEDs
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);  // Orange LED
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);  // Green LED
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);  // Red LED
        HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);  // Blue LED

        LOG_DEBUG("LED", "LEDs toggled");
        osDelay(500);  // 500ms delay
    }
}

/**
 * @brief Create tasks for Example 1
 */
void Example1_CreateTasks(void)
{
    example1_loggerHandle = osThreadNew(Example1_LoggerTask, NULL, &example1_logger_attributes);
    example1_ledHandle = osThreadNew(Example1_LEDTask, NULL, &example1_led_attributes);

    if (example1_loggerHandle == NULL || example1_ledHandle == NULL) {
        Error_Handler();
    }

    LOG_INFO("Example1", "Logger Coherency with Mutex - Tasks created");
}

#endif // EXAMPLE_1_LOGGER_MUTEX


/* =============================================================================
 * Example 2: Task Creation - Static vs Dynamic Allocation
 * =============================================================================
 * Demonstrates:
 * - Dynamic allocation: Memory from FreeRTOS heap
 * - Static allocation: Pre-allocated memory at compile time
 * - Memory monitoring and comparison
 */
#ifdef EXAMPLE_2_TASK_CREATION

// Static task buffers (must be global/static)
StackType_t xStaticTaskStack[512];
StaticTask_t xStaticTaskTCB;

// Task handles
static osThreadId_t example2_dynamicHandle;
static osThreadId_t example2_staticHandle;
static osThreadId_t example2_monitorHandle;

// Task attributes - Dynamic
const osThreadAttr_t example2_dynamic_attributes = {
    .name = "Dynamic",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

// Task attributes - Static (with pre-allocated buffers)
const osThreadAttr_t example2_static_attributes = {
    .name = "Static",
    .stack_mem = xStaticTaskStack,
    .stack_size = sizeof(xStaticTaskStack),
    .priority = (osPriority_t) osPriorityNormal,
    .cb_mem = &xStaticTaskTCB,
    .cb_size = sizeof(xStaticTaskTCB),
};

// Task attributes - Monitor
const osThreadAttr_t example2_monitor_attributes = {
    .name = "Monitor",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityLow,
};

/**
 * @brief Dynamic task - allocated from heap
 */
void Example2_DynamicTask(void *argument)
{
    uint32_t counter = 0;

    for(;;)
    {
        LOG_INFO("Dynamic", "Task running (heap-allocated) - count=%lu", counter++);
        osDelay(1000);
    }
}

/**
 * @brief Static task - uses pre-allocated memory
 */
void Example2_StaticTask(void *argument)
{
    uint32_t counter = 0;

    for(;;)
    {
        LOG_INFO("Static", "Task running (pre-allocated) - count=%lu", counter++);
        osDelay(1000);
    }
}

/**
 * @brief Monitor task - reports heap usage
 */
void Example2_MonitorTask(void *argument)
{
    for(;;)
    {
        size_t freeHeap = xPortGetFreeHeapSize();
        size_t minEverFreeHeap = xPortGetMinimumEverFreeHeapSize();

        LOG_INFO("Monitor", "Heap Free: %u bytes, Min Ever: %u bytes",
                 freeHeap, minEverFreeHeap);

        osDelay(5000);  // Report every 5 seconds
    }
}

/**
 * @brief Create tasks for Example 2
 */
void Example2_CreateTasks(void)
{
    // Create dynamic task (uses heap)
    example2_dynamicHandle = osThreadNew(Example2_DynamicTask, NULL, &example2_dynamic_attributes);

    // Create static task (uses pre-allocated memory)
    example2_staticHandle = osThreadNew(Example2_StaticTask, NULL, &example2_static_attributes);

    // Create monitor task
    example2_monitorHandle = osThreadNew(Example2_MonitorTask, NULL, &example2_monitor_attributes);

    if (example2_dynamicHandle == NULL || example2_staticHandle == NULL || example2_monitorHandle == NULL) {
        Error_Handler();
    }

    LOG_INFO("Example2", "Static vs Dynamic Task Creation - Tasks created");
}

#endif // EXAMPLE_2_TASK_CREATION


/* =============================================================================
 * Example 3: Priority-Based Scheduling
 * =============================================================================
 * Demonstrates:
 * - Preemptive priority-based scheduling
 * - Higher priority tasks preempt lower priority
 * - Round-robin for equal priorities
 */
#ifdef EXAMPLE_3_PRIORITY_SCHEDULING

// Task handles
static osThreadId_t example3_highHandle;
static osThreadId_t example3_mediumHandle;
static osThreadId_t example3_lowHandle;

// Task attributes
const osThreadAttr_t example3_high_attributes = {
    .name = "HighPrio",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityHigh,  // Priority 4
};

const osThreadAttr_t example3_medium_attributes = {
    .name = "MediumPrio",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal,  // Priority 2
};

const osThreadAttr_t example3_low_attributes = {
    .name = "LowPrio",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityLow,  // Priority 1
};

/**
 * @brief High priority task - runs most frequently
 */
void Example3_HighPriorityTask(void *argument)
{
    uint32_t counter = 0;

    for(;;)
    {
        LOG_INFO("HighPrio", "Executing (Priority 4) - count=%lu", counter++);
        HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);  // Blue LED
        osDelay(500);  // Short delay - runs frequently
    }
}

/**
 * @brief Medium priority task - preempts low, preempted by high
 */
void Example3_MediumPriorityTask(void *argument)
{
    uint32_t counter = 0;

    for(;;)
    {
        LOG_INFO("MediumPrio", "Executing (Priority 2) - count=%lu", counter++);
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);  // Green LED
        osDelay(1000);  // Medium delay
    }
}

/**
 * @brief Low priority task - only runs when others are blocked
 */
void Example3_LowPriorityTask(void *argument)
{
    uint32_t counter = 0;

    for(;;)
    {
        LOG_INFO("LowPrio", "Executing (Priority 1) - count=%lu", counter++);
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);  // Red LED
        osDelay(2000);  // Long delay
    }
}

/**
 * @brief Create tasks for Example 3
 */
void Example3_CreateTasks(void)
{
    example3_highHandle = osThreadNew(Example3_HighPriorityTask, NULL, &example3_high_attributes);
    example3_mediumHandle = osThreadNew(Example3_MediumPriorityTask, NULL, &example3_medium_attributes);
    example3_lowHandle = osThreadNew(Example3_LowPriorityTask, NULL, &example3_low_attributes);

    if (example3_highHandle == NULL || example3_mediumHandle == NULL || example3_lowHandle == NULL) {
        Error_Handler();
    }

    LOG_INFO("Example3", "Priority-Based Scheduling - Tasks created");
}

#endif // EXAMPLE_3_PRIORITY_SCHEDULING


/* =============================================================================
 * Example 4: Counting Semaphore LED Control
 * =============================================================================
 * Demonstrates:
 * - Counting semaphore for resource pool management
 * - Producer-consumer pattern
 * - Semaphore signaling between tasks
 */
#ifdef EXAMPLE_4_SEMAPHORE_LED

// Task handles
static osThreadId_t example4_producerHandle;
static osThreadId_t example4_consumerHandle;

// Task attributes
const osThreadAttr_t example4_producer_attributes = {
    .name = "Producer",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t example4_consumer_attributes = {
    .name = "Consumer",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

/**
 * @brief Producer task - generates LED toggle events
 */
void Example4_LEDProducerTask(void *argument)
{
    uint32_t counter = 0;

    for(;;)
    {
        // Give semaphore (signal LED toggle event)
        osSemaphoreRelease(ledSemaphoreHandle);
        LOG_INFO("Producer", "Event generated - count=%lu", counter++);

        osDelay(300);  // Generate events every 300ms
    }
}

/**
 * @brief Consumer task - waits for events and toggles LEDs
 */
void Example4_LEDConsumerTask(void *argument)
{
    uint32_t counter = 0;

    for(;;)
    {
        // Wait for semaphore (wait for LED toggle event)
        if (osSemaphoreAcquire(ledSemaphoreHandle, osWaitForever) == osOK)
        {
            // Toggle LEDs
            HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);  // Orange
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);  // Green

            LOG_INFO("Consumer", "Event processed - count=%lu", counter++);
        }
    }
}

/**
 * @brief Create tasks for Example 4
 */
void Example4_CreateTasks(void)
{
    example4_producerHandle = osThreadNew(Example4_LEDProducerTask, NULL, &example4_producer_attributes);
    example4_consumerHandle = osThreadNew(Example4_LEDConsumerTask, NULL, &example4_consumer_attributes);

    if (example4_producerHandle == NULL || example4_consumerHandle == NULL) {
        Error_Handler();
    }

    LOG_INFO("Example4", "Counting Semaphore LED Control - Tasks created");
}

#endif // EXAMPLE_4_SEMAPHORE_LED


/* =============================================================================
 * Example 5: vTaskDelay vs vTaskDelayUntil
 * =============================================================================
 * Demonstrates:
 * - vTaskDelay: Relative delay (timing drift)
 * - vTaskDelayUntil: Absolute delay (precise timing)
 * - Comparison of timing precision
 */
#ifdef EXAMPLE_5_DELAY_COMPARISON

// Task handles
static osThreadId_t example5_delayHandle;
static osThreadId_t example5_delayUntilHandle;

// Task attributes
const osThreadAttr_t example5_delay_attributes = {
    .name = "TaskDelay",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t example5_delayuntil_attributes = {
    .name = "DelayUntil",
    .stack_size = 512 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

/**
 * @brief Task using vTaskDelay (relative delay)
 */
void Example5_TaskDelayTask(void *argument)
{
    TickType_t startTick, endTick, executionTime;
    uint32_t counter = 0;

    for(;;)
    {
        startTick = xTaskGetTickCount();

        // Simulate work
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);  // Orange LED

        // Calculate execution time
        endTick = xTaskGetTickCount();
        executionTime = endTick - startTick;

        LOG_INFO("TaskDelay", "Exec=%lu ticks, count=%lu (DRIFTS over time)",
                 executionTime, counter++);

        // Relative delay - timing will drift due to execution time
        osDelay(1000);
    }
}

/**
 * @brief Task using vTaskDelayUntil (absolute delay)
 */
void Example5_TaskDelayUntilTask(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    TickType_t startTick, endTick, executionTime;
    uint32_t counter = 0;

    for(;;)
    {
        startTick = xTaskGetTickCount();

        // Simulate work
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);  // Red LED

        // Calculate execution time
        endTick = xTaskGetTickCount();
        executionTime = endTick - startTick;

        LOG_INFO("DelayUntil", "Exec=%lu ticks, count=%lu (PRECISE timing)",
                 executionTime, counter++);

        // Absolute delay - compensates for execution time
        osDelayUntil(xLastWakeTime + pdMS_TO_TICKS(1000));
        xLastWakeTime += pdMS_TO_TICKS(1000);
    }
}

/**
 * @brief Create tasks for Example 5
 */
void Example5_CreateTasks(void)
{
    example5_delayHandle = osThreadNew(Example5_TaskDelayTask, NULL, &example5_delay_attributes);
    example5_delayUntilHandle = osThreadNew(Example5_TaskDelayUntilTask, NULL, &example5_delayuntil_attributes);

    if (example5_delayHandle == NULL || example5_delayUntilHandle == NULL) {
        Error_Handler();
    }

    LOG_INFO("Example5", "vTaskDelay vs vTaskDelayUntil - Tasks created");
}

#endif // EXAMPLE_5_DELAY_COMPARISON


/* =============================================================================
 * Common API Implementation
 * =============================================================================
 */

/**
 * @brief Create RTOS synchronization objects for selected example
 */
void Examples_CreateSyncObjects(void)
{
#ifdef EXAMPLE_1_LOGGER_MUTEX
    // Create mutex for logger protection
    extern osMutexId_t loggerMutexHandle;
    const osMutexAttr_t loggerMutex_attributes = {
        .name = "loggerMutex"
    };
    loggerMutexHandle = osMutexNew(&loggerMutex_attributes);
    if (loggerMutexHandle == NULL) {
        Error_Handler();
    }
    LOG_INFO("Example1", "Logger mutex created");
#endif

#ifdef EXAMPLE_4_SEMAPHORE_LED
    // Create counting semaphore (max 5 events, start empty)
    extern osSemaphoreId_t ledSemaphoreHandle;
    const osSemaphoreAttr_t ledSemaphore_attributes = {
        .name = "ledSemaphore"
    };
    ledSemaphoreHandle = osSemaphoreNew(5, 0, &ledSemaphore_attributes);
    if (ledSemaphoreHandle == NULL) {
        Error_Handler();
    }
    LOG_INFO("Example4", "LED counting semaphore created");
#endif
}

/**
 * @brief Initialize and create tasks for selected example
 */
void Examples_Init(void)
{
#ifdef EXAMPLE_1_LOGGER_MUTEX
    Example1_CreateTasks();
#endif

#ifdef EXAMPLE_2_TASK_CREATION
    Example2_CreateTasks();
#endif

#ifdef EXAMPLE_3_PRIORITY_SCHEDULING
    Example3_CreateTasks();
#endif

#ifdef EXAMPLE_4_SEMAPHORE_LED
    Example4_CreateTasks();
#endif

#ifdef EXAMPLE_5_DELAY_COMPARISON
    Example5_CreateTasks();
#endif
}
