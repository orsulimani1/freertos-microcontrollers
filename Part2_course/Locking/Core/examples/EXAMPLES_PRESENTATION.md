# FreeRTOS Examples - Presentation Slides

---

## Slide 1: Examples Overview

### FreeRTOS Hands-On Examples
**STM32 Microcontroller Course**

**Today's Practical Demonstrations:**
- Example 1: Logger Coherency with Mutex Protection
- Example 2: Task Creation - Static vs Dynamic Allocation
- Example 3: Priority-Based Preemptive Scheduling
- Example 4: Counting Semaphore for Resource Control
- Example 5: Precise Timing with Delay APIs

**Easy Example Selection:**
- Single `#define` in `examples_config.h`
- Compile-time validation (only one active)
- Complete working code for each concept

---

## Slide 2: Example Selection System

### How to Switch Between Examples

**Step 1:** Open `Core/examples/examples_config.h`

**Step 2:** Uncomment ONE example:
```c
/* Uncomment ONE example at a time */

// Example 1: Logger Coherency Problem with Mutex
#define EXAMPLE_1_LOGGER_MUTEX

// Example 2: Task Creation - Static vs Dynamic
// #define EXAMPLE_2_TASK_CREATION

// Example 3: Priority-Based Scheduling
// #define EXAMPLE_3_PRIORITY_SCHEDULING

// Example 4: Counting Semaphore LED Control
// #define EXAMPLE_4_SEMAPHORE_LED

// Example 5: vTaskDelay vs vTaskDelayUntil
// #define EXAMPLE_5_DELAY_COMPARISON
```

**Step 3:** Build and flash

**Built-in Protection:** Compiler error if multiple examples selected!

---

## Slide 3: Example 1 - Logger Coherency Problem

### Problem: Race Condition on Shared UART

**The Issue:**
```
Task A logs: "Temperature: 25.5°C"
Task B logs: "Button pressed"

Actual Output: "TemperaButton presseture: 25.5°C"
                ^^^^^^^^^^^^^^^^^^^^^^^^
                     GARBLED!
```

**Root Cause:**
- Multiple tasks call `logger_log()` simultaneously
- UART peripheral is a **shared resource**
- No synchronization = interleaved/corrupted output

**Real-World Impact:**
- Debugging becomes impossible
- Log analysis tools fail
- Critical errors get masked

---

## Slide 4: Example 1 - Solution with Mutex

### Mutex Protection Pattern

**What We're Locking:**
```
┌─────────────────────────────────┐
│  UART Peripheral (Hardware)     │
├─────────────────────────────────┤
│  Logger Buffer (512 bytes)      │
├─────────────────────────────────┤
│  RTC Read Operations            │
└─────────────────────────────────┘
```

**How Mutex Solves It:**
```c
void logger_log(...) {
    // Acquire mutex - only ONE task proceeds
    if (osMutexAcquire(loggerMutexHandle, osWaitForever) == osOK) {

        // CRITICAL SECTION - Atomic operation
        char buffer[512];
        format_message(buffer);
        HAL_UART_Transmit(&huart2, buffer, strlen(buffer), 100);

        // Release mutex - next task can proceed
        osMutexRelease(loggerMutexHandle);
    }
}
```

---

## Slide 5: Example 1 - Demonstration

### What You'll See

**Two Tasks Running:**
1. **Logger Task** (Priority: Normal)
   - Logs count every 250ms
   - Blue LED indicator

2. **LED Task** (Priority: Normal)
   - Toggles all 4 LEDs every 500ms
   - Logs each toggle event

**Expected UART Output:**
```
[00:00:00][INFO][Logger] FreeRTOS running - count=0
[00:00:00][DEBUG][LED] LEDs toggled
[00:00:00][INFO][Logger] FreeRTOS running - count=1
[00:00:00][INFO][Logger] FreeRTOS running - count=2
[00:00:01][DEBUG][LED] LEDs toggled
```

**Key Observation:** Messages are **never garbled** or interleaved!

**Stack Size Fix:** Task stack increased to 4KB to accommodate 512-byte buffer

---

## Slide 6: Example 2 - Task Creation Methods

### Static vs Dynamic Allocation

**Dynamic Allocation (Heap):**
```c
const osThreadAttr_t dynamic_attributes = {
    .name = "Dynamic",
    .stack_size = 512 * 4,      // 2KB stack
    .priority = osPriorityNormal,
    // No pre-allocated buffers
};

osThreadId_t handle = osThreadNew(TaskFunction, NULL, &dynamic_attributes);
```
✅ Flexible - create/delete at runtime
✅ Simpler code
❌ Heap fragmentation risk
❌ Memory allocation can fail

**Static Allocation (Pre-allocated):**
```c
StackType_t xStaticTaskStack[512];      // At compile time
StaticTask_t xStaticTaskTCB;            // At compile time

const osThreadAttr_t static_attributes = {
    .name = "Static",
    .stack_mem = xStaticTaskStack,      // Pre-allocated!
    .stack_size = sizeof(xStaticTaskStack),
    .cb_mem = &xStaticTaskTCB,          // Pre-allocated!
    .cb_size = sizeof(xStaticTaskTCB),
    .priority = osPriorityNormal,
};
```
✅ Deterministic memory usage
✅ No heap fragmentation
✅ Suitable for safety-critical systems
❌ Memory allocated even if task not running

---

## Slide 7: Example 2 - Memory Layout Comparison

### Where Memory Lives

**Dynamic Allocation:**
```
┌──────────────────────────────────┐
│ FreeRTOS Heap                    │
│ (configTOTAL_HEAP_SIZE = 75KB)  │
│                                  │
│ ┌──────────────────────────────┐ │
│ │ Dynamic Task Control Block   │ │ ← Allocated at runtime
│ ├──────────────────────────────┤ │
│ │ Dynamic Task Stack (2KB)     │ │ ← Allocated at runtime
│ ├──────────────────────────────┤ │
│ │ Other RTOS Objects           │ │
│ ├──────────────────────────────┤ │
│ │ Free Space: ???              │ │ ← Varies
│ └──────────────────────────────┘ │
└──────────────────────────────────┘
```

**Static Allocation:**
```
┌──────────────────────────────────┐
│ Static Memory Section (.bss)    │
│                                  │
│ ├─ xStaticTaskStack[512]        │ ← Known at compile time
│ ├─ xStaticTaskTCB               │ ← Known at compile time
│ └─ Other static variables       │
└──────────────────────────────────┘

┌──────────────────────────────────┐
│ FreeRTOS Heap (Still 75KB)      │
│                                  │
│ ├─ Other Dynamic Objects        │
│ └─ Free Space: More Available!  │ ← Static tasks don't use heap
└──────────────────────────────────┘
```

---

## Slide 8: Example 2 - Demonstration

### What You'll See

**Three Tasks Running:**

1. **Dynamic Task** - Logs every 1 second
2. **Static Task** - Logs every 1 second
3. **Monitor Task** - Reports heap usage every 5 seconds

**Expected UART Output:**
```
[00:00:01][INFO][Dynamic] Task running (heap-allocated) - count=0
[00:00:01][INFO][Static] Task running (pre-allocated) - count=0
[00:00:02][INFO][Dynamic] Task running (heap-allocated) - count=1
[00:00:02][INFO][Static] Task running (pre-allocated) - count=1
[00:00:05][INFO][Monitor] Heap Free: 68234 bytes, Min Ever: 67890 bytes
```

**Key Observations:**
- Both task types work identically from functionality perspective
- Monitor shows heap consumption difference
- `xPortGetMinimumEverFreeHeapSize()` tracks lowest point
- Static allocation preserves more heap space

**Try This:** Comment out static task, rebuild, compare heap usage!

---

## Slide 9: Example 3 - Priority-Based Scheduling

### Preemptive Multitasking in Action

**FreeRTOS Scheduling Rule:**
> "Highest priority ready task ALWAYS runs"

**Our Test Configuration:**
```
Priority 4 (Highest) → High Priority Task   → Blue LED  → 500ms period
Priority 2 (Medium)  → Medium Priority Task → Green LED → 1000ms period
Priority 1 (Low)     → Low Priority Task    → Red LED   → 2000ms period
Priority 0 (Idle)    → Idle Task (system)
```

**What Happens:**
1. **High Priority Task** preempts others immediately when ready
2. **Medium Priority Task** only runs when High is blocked
3. **Low Priority Task** only runs when both High and Medium blocked
4. **Idle Task** runs when all application tasks are blocked

---

## Slide 10: Example 3 - Scheduling Timeline

### Visual Timeline of Preemption

```
Time (ms)      0    500   1000  1500  2000  2500  3000
               ↓     ↓     ↓     ↓     ↓     ↓     ↓
High (P4)      ███   ███   ███   ███   ███   ███   ███
               ↑     ↑     ↑     ↑     ↑     ↑     ↑
               Run   Run   Run   Run   Run   Run   Run

Medium (P2)    ███         ███         ███         ███
               ↑           ↑           ↑           ↑
               Run         Run         Run         Run

Low (P1)       ███                     ███
               ↑                       ↑
               Run                     Run

Idle (P0)      ─────────────────────────────────────────
               (runs in gaps between task delays)
```

**Legend:**
- `███` = Task executing
- Gap = Task blocked (in delay)

**Key Point:** Higher priority task gets CPU immediately when it wakes up!

---

## Slide 11: Example 3 - Demonstration

### What You'll See

**Visual Indicators:**
- **Blue LED** blinks fastest (High priority - 500ms)
- **Green LED** blinks at medium rate (Medium priority - 1000ms)
- **Red LED** blinks slowest (Low priority - 2000ms)

**Expected UART Output:**
```
[00:00:00][INFO][HighPrio] Executing (Priority 4) - count=0
[00:00:00][INFO][MediumPrio] Executing (Priority 2) - count=0
[00:00:00][INFO][LowPrio] Executing (Priority 1) - count=0
[00:00:00][INFO][HighPrio] Executing (Priority 4) - count=1    ← 500ms
[00:00:01][INFO][MediumPrio] Executing (Priority 2) - count=1  ← 1000ms
[00:00:01][INFO][HighPrio] Executing (Priority 4) - count=2    ← 500ms
[00:00:01][INFO][HighPrio] Executing (Priority 4) - count=3    ← 500ms
[00:00:02][INFO][MediumPrio] Executing (Priority 2) - count=2  ← 1000ms
[00:00:02][INFO][LowPrio] Executing (Priority 1) - count=1     ← 2000ms
```

**Experiment:** Set breakpoint in LowPrio task, notice it rarely executes!

---

## Slide 12: Example 4 - Counting Semaphore Concept

### Producer-Consumer Pattern

**What is a Counting Semaphore?**
- Counter that tracks available resources
- Range: 0 to N (configured max count)
- No ownership (unlike mutex)

**Our Configuration:**
```c
// Create counting semaphore: max=5, initial=0
osSemaphoreId_t ledSemaphoreHandle = osSemaphoreNew(5, 0, &attributes);
```

**Semaphore Operations:**
```
Operation           Effect                  Use Case
─────────────────────────────────────────────────────────
Give/Release        Counter++              Producer signals event
Take/Acquire        Counter--              Consumer waits for event
                    (blocks if count=0)
```

**Buffer Analogy:**
```
Semaphore = [0] [0] [0] [0] [0]    Max capacity = 5 events
             Empty slots           Current count = 0

Producer gives:
Semaphore = [✓] [0] [0] [0] [0]    Count = 1

Consumer takes:
Semaphore = [0] [0] [0] [0] [0]    Count = 0 (back to empty)
```

---

## Slide 13: Example 4 - Producer-Consumer Flow

### Event Signaling Between Tasks

**Architecture:**
```
┌─────────────────┐         Semaphore         ┌─────────────────┐
│  Producer Task  │────────────────────────────│  Consumer Task  │
│                 │                            │                 │
│  Every 300ms:   │   osSemaphoreRelease()    │  Waiting:       │
│  Generate event │  ────────────────────────> │  - Blocks       │
│                 │                            │  - Wakes up     │
│                 │                            │  - Toggles LEDs │
│                 │   osSemaphoreAcquire()    │                 │
│                 │  <──────────────────────── │                 │
└─────────────────┘                            └─────────────────┘
```

**Code Structure:**

**Producer:**
```c
void ProducerTask(void *argument) {
    for(;;) {
        osSemaphoreRelease(ledSemaphoreHandle);  // Signal event
        LOG_INFO("Producer", "Event generated");
        osDelay(300);  // Generate events every 300ms
    }
}
```

**Consumer:**
```c
void ConsumerTask(void *argument) {
    for(;;) {
        // Wait for event (blocks if semaphore count = 0)
        osSemaphoreAcquire(ledSemaphoreHandle, osWaitForever);

        // Process event
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);  // Orange LED
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);  // Green LED
        LOG_INFO("Consumer", "Event processed");
    }
}
```

---

## Slide 14: Example 4 - Demonstration

### What You'll See

**Task Behavior:**
- **Producer** generates events every 300ms
- **Consumer** processes each event (toggles LEDs)
- Events queue up if consumer is slower (up to max=5)

**Visual Indicators:**
- **Orange LED** toggles when event processed
- **Green LED** toggles when event processed

**Expected UART Output:**
```
[00:00:00][INFO][Producer] Event generated - count=0
[00:00:00][INFO][Consumer] Event processed - count=0
[00:00:00][INFO][Producer] Event generated - count=1
[00:00:00][INFO][Consumer] Event processed - count=1
[00:00:00][INFO][Producer] Event generated - count=2
[00:00:00][INFO][Consumer] Event processed - count=2
```

**Synchronization:** Consumer wakes immediately when producer signals!

**Experiment Ideas:**
- Increase producer delay → Consumer waits longer
- Decrease producer delay → Events may queue
- Add delays in consumer → See buffer filling effect

---

## Slide 15: Example 5 - Delay API Comparison

### vTaskDelay vs vTaskDelayUntil

**The Problem:**
Every task has execution time that causes timing drift!

**vTaskDelay() - Relative Delay:**
```c
void TaskFunction(void *argument) {
    for(;;) {
        do_work();        // Takes 5ms
        vTaskDelay(100);  // Delay 100ms from NOW
    }
}
// Actual period: 105ms (5ms work + 100ms delay)
// Over time: DRIFTS away from intended timing
```

**vTaskDelayUntil() - Absolute Delay:**
```c
void TaskFunction(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for(;;) {
        do_work();  // Takes 5ms

        // Delay until 100ms from LAST wake time
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}
// Actual period: EXACTLY 100ms
// Execution time is compensated automatically!
```

---

## Slide 16: Example 5 - Timing Drift Visualization

### Cumulative Effect Over Time

**Scenario:** Task should run every 1000ms, but takes 5ms to execute

**Using vTaskDelay():**
```
Iteration  Target Time  Actual Time  Drift
────────────────────────────────────────────
1          1000ms       1005ms       +5ms
2          2000ms       2010ms       +10ms
3          3000ms       3015ms       +15ms
4          4000ms       4020ms       +20ms
5          5000ms       5025ms       +25ms
...
100        100000ms     100500ms     +500ms (half second!)
```

**Using vTaskDelayUntil():**
```
Iteration  Target Time  Actual Time  Drift
────────────────────────────────────────────
1          1000ms       1000ms       0ms
2          2000ms       2000ms       0ms
3          3000ms       3000ms       0ms
4          4000ms       4000ms       0ms
5          5000ms       5000ms       0ms
...
100        100000ms     100000ms     0ms (perfect!)
```

**When to Use Each:**
- `vTaskDelay()`: Simple delays, non-critical timing, one-shot delays
- `vTaskDelayUntil()`: Periodic tasks, sensor sampling, control loops

---

## Slide 17: Example 5 - Demonstration

### What You'll See

**Two Tasks:**
1. **TaskDelay Task** - Uses `vTaskDelay()` (relative)
2. **DelayUntil Task** - Uses `vTaskDelayUntil()` (absolute)

Both configured for 1000ms period

**Visual Indicators:**
- **Orange LED** = vTaskDelay task
- **Red LED** = vTaskDelayUntil task

**Expected UART Output:**
```
[00:00:01][INFO][TaskDelay] Exec=5 ticks, count=0 (DRIFTS over time)
[00:00:01][INFO][DelayUntil] Exec=5 ticks, count=0 (PRECISE timing)
[00:00:02][INFO][TaskDelay] Exec=5 ticks, count=1 (DRIFTS over time)
[00:00:02][INFO][DelayUntil] Exec=5 ticks, count=1 (PRECISE timing)
```

**Key Observations:**
- Execution time reported in ticks (1 tick = 1ms with configTICK_RATE_HZ=1000)
- Over long runs, TaskDelay accumulates drift
- DelayUntil maintains exact 1000ms period

**Real-World Example:** Sensor sampling at exactly 100Hz for data acquisition

---

## Slide 18: Common Mistakes & Best Practices

### Lessons from Examples

**Mutex Usage (Example 1):**
❌ **Don't:** Hold mutex during long blocking operations
```c
osMutexAcquire(mutex, osWaitForever);
osDelay(1000);  // BAD! Blocks all other tasks
osMutexRelease(mutex);
```
✅ **Do:** Minimize critical section
```c
osMutexAcquire(mutex, osWaitForever);
quick_operation();  // GOOD! Fast operation only
osMutexRelease(mutex);
osDelay(1000);  // Delay OUTSIDE mutex
```

**Stack Sizing (Example 2):**
❌ **Don't:** Guess stack sizes
```c
.stack_size = 128 * 4,  // 512 bytes - too small for logger!
```
✅ **Do:** Calculate requirements + margin
```c
// 512-byte buffer + locals + function calls + 50% margin
.stack_size = 1024 * 4,  // 4KB - safe
```

**Priority Assignment (Example 3):**
❌ **Don't:** Make everything high priority
```c
all_tasks.priority = osPriorityHigh;  // Defeats the purpose!
```
✅ **Do:** Assign based on timing requirements
```c
critical_task.priority = osPriorityHigh;
periodic_task.priority = osPriorityNormal;
background_task.priority = osPriorityLow;
```

---

## Slide 19: Debugging Tips

### Tools & Techniques Demonstrated

**Stack Overflow Detection:**
```c
// In FreeRTOSConfig.h
#define configCHECK_FOR_STACK_OVERFLOW  2
```
Implement callback:
```c
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    LOG_ERROR("RTOS", "Stack overflow in task: %s", pcTaskName);
    while(1);  // Halt for debugging
}
```

**Heap Monitoring (Example 2):**
```c
size_t free_heap = xPortGetFreeHeapSize();
size_t min_heap = xPortGetMinimumEverFreeHeapSize();
LOG_INFO("Heap", "Free: %u, MinEver: %u", free_heap, min_heap);
```

**Task State Inspection:**
```c
// Set breakpoint in task, check:
// - Stack high water mark
// - Task state (running/blocked/ready)
// - Wait reason (delay/semaphore/mutex)
```

**LED Debugging:**
- Unique LED per task for visual state indication
- Blink patterns indicate execution frequency
- Color coding for priority levels

---

## Slide 20: Summary & Next Steps

### What We Learned

**Example 1: Synchronization**
✅ Mutex protects shared resources (UART/Logger)
✅ Critical sections must be minimal
✅ Proper locking prevents race conditions

**Example 2: Memory Management**
✅ Static allocation: deterministic, no fragmentation
✅ Dynamic allocation: flexible, easier to use
✅ Monitor heap usage in production

**Example 3: Scheduling**
✅ Priority-based preemption ensures responsiveness
✅ Higher priority always runs first
✅ Assign priorities based on timing requirements

**Example 4: Inter-Task Communication**
✅ Counting semaphores for resource pools
✅ Producer-consumer pattern for event handling
✅ Non-blocking task coordination

**Example 5: Timing Precision**
✅ vTaskDelay for simple delays (drift accumulates)
✅ vTaskDelayUntil for periodic tasks (compensates drift)
✅ Choose based on timing requirements

---

## Slide 21: Next Class Preview

### Advanced Topics Coming Up

**Zero-Copy Logging:**
- Non-blocking logger using message queues
- Circular buffer for high-throughput logging
- Task notification for lightweight signaling

**Advanced Synchronization:**
- Recursive mutexes
- Priority inheritance
- Deadlock prevention patterns

**Inter-Task Communication:**
- Message queues for data passing
- Event groups for multi-condition synchronization
- Stream buffers for byte streams

**Low Power Modes:**
- Tickless idle
- Sleep modes integration
- Power profiling

**Homework Assignment:**
Modify Example 4 to use a queue instead of semaphore - pass actual data!

---

## Slide 22: Questions & Resources

### Getting Help

**Repository Structure:**
```
Core/examples/
├── examples_config.h  ← Edit this to switch examples
├── examples.h         ← API reference
├── examples.c         ← Implementation (read for learning!)
└── README.md         ← Detailed documentation
```

**Switching Examples:**
1. Open `examples_config.h`
2. Uncomment ONE `#define EXAMPLE_X_...`
3. Build and flash
4. Observe via UART (115200 baud) and LEDs

**Resources:**
- 📖 Example README: Complete usage guide
- 📧 Email: Orr.Foxx@gmail.com
- 💬 WhatsApp Group: Quick questions
- 🐛 GitHub Issues: Bug reports
- 📚 FreeRTOS Docs: https://www.freertos.org/Documentation/

**Remember:** Code is heavily commented - read examples.c for implementation details!

---

## Appendix: Hardware Setup

### Required Connections

**STM32F4 Discovery Board:**
- **UART2** (PA2/PA3) → USB-Serial adapter (115200 baud)
- **LEDs** (built-in):
  - LD3 (Orange) - PD13
  - LD4 (Green) - PD12
  - LD5 (Red) - PD14
  - LD6 (Blue) - PD15
- **Power** via USB ST-LINK

**Serial Monitor Settings:**
- Baud Rate: 115200
- Data Bits: 8
- Parity: None
- Stop Bits: 1
- Flow Control: None

**Recommended Tools:**
- STM32CubeIDE for development
- PuTTY / Tera Term / `screen` for UART monitoring
- Logic analyzer for timing verification (optional)

