# FreeRTOS Examples for STM32

This directory contains organized FreeRTOS examples for the course. Each example demonstrates different concepts and can be easily selected using `#ifdef` directives.

## How to Use

1. Open `examples_config.h`
2. Uncomment **ONE** example at a time
3. Build and flash the project
4. Observe the behavior via UART output and LEDs

## Available Examples

### Example 1: Logger Coherency with Mutex Protection
**File**: `examples_config.h` - `#define EXAMPLE_1_LOGGER_MUTEX`

**Demonstrates**:
- Problem: Multiple tasks logging simultaneously causes garbled output
- Solution: Mutex protects UART/logger from concurrent access
- Key Concepts: Mutual exclusion, critical sections, resource protection

**Expected Behavior**:
- Two tasks (Logger and LED) log messages concurrently
- Messages are properly synchronized and don't overlap
- LEDs blink while logging

**UART Output**:
```
[00:00:01][INFO][Logger] FreeRTOS running - count=0
[00:00:01][DEBUG][LED] LEDs toggled
[00:00:01][INFO][Logger] FreeRTOS running - count=1
```

---

### Example 2: Task Creation - Static vs Dynamic Allocation
**File**: `examples_config.h` - `#define EXAMPLE_2_TASK_CREATION`

**Demonstrates**:
- Dynamic allocation: Memory from FreeRTOS heap
- Static allocation: Pre-allocated memory at compile time
- Memory monitoring and comparison

**Expected Behavior**:
- Dynamic and Static tasks run periodically
- Monitor task reports heap usage every 5 seconds
- Observe difference in heap consumption

**UART Output**:
```
[00:00:01][INFO][Dynamic] Task running (heap-allocated) - count=0
[00:00:01][INFO][Static] Task running (pre-allocated) - count=0
[00:00:05][INFO][Monitor] Heap Free: 68234 bytes, Min Ever: 67890 bytes
```

---

### Example 3: Priority-Based Scheduling
**File**: `examples_config.h` - `#define EXAMPLE_3_PRIORITY_SCHEDULING`

**Demonstrates**:
- Preemptive priority-based scheduling
- Higher priority tasks preempt lower priority
- Visual indication via different LEDs

**Task Priorities**:
- High Priority (4): Blue LED - runs most frequently (500ms)
- Medium Priority (2): Green LED - runs periodically (1000ms)
- Low Priority (1): Red LED - only runs when others blocked (2000ms)

**Expected Behavior**:
- Blue LED toggles fastest (high priority preempts others)
- Green LED toggles at medium rate
- Red LED toggles slowly (lowest priority)

**UART Output**:
```
[00:00:00][INFO][HighPrio] Executing (Priority 4) - count=0
[00:00:00][INFO][MediumPrio] Executing (Priority 2) - count=0
[00:00:01][INFO][HighPrio] Executing (Priority 4) - count=1
[00:00:02][INFO][LowPrio] Executing (Priority 1) - count=0
```

---

### Example 4: Counting Semaphore LED Control
**File**: `examples_config.h` - `#define EXAMPLE_4_SEMAPHORE_LED`

**Demonstrates**:
- Counting semaphore for resource pool management
- Producer-consumer pattern
- Semaphore signaling between tasks

**Expected Behavior**:
- Producer generates events every 300ms (gives semaphore)
- Consumer waits for events and toggles LEDs (takes semaphore)
- Orange and Green LEDs toggle when events are processed

**UART Output**:
```
[00:00:00][INFO][Producer] Event generated - count=0
[00:00:00][INFO][Consumer] Event processed - count=0
[00:00:00][INFO][Producer] Event generated - count=1
[00:00:00][INFO][Consumer] Event processed - count=1
```

---

### Example 5: vTaskDelay vs vTaskDelayUntil
**File**: `examples_config.h` - `#define EXAMPLE_5_DELAY_COMPARISON`

**Demonstrates**:
- `vTaskDelay()`: Relative delay (timing drift)
- `vTaskDelayUntil()`: Absolute delay (precise timing)
- Comparison of timing precision

**Expected Behavior**:
- Both tasks execute periodically every 1000ms
- TaskDelay shows timing drift due to execution time
- DelayUntil maintains precise timing by compensating for execution

**Visual Indicators**:
- Orange LED: vTaskDelay task
- Red LED: vTaskDelayUntil task

**UART Output**:
```
[00:00:01][INFO][TaskDelay] Exec=5 ticks, count=0 (DRIFTS over time)
[00:00:01][INFO][DelayUntil] Exec=5 ticks, count=0 (PRECISE timing)
```

---

## File Structure

```
examples/
├── examples_config.h    # Example selection (edit this to switch examples)
├── examples.h           # Header file with function prototypes
├── examples.c           # Implementation of all examples
└── README.md           # This file
```

## Key Functions

### From main.c
```c
// Called after osKernelInitialize() to create mutexes/semaphores
Examples_CreateSyncObjects();

// Called before osKernelStart() to create tasks
Examples_Init();
```

### Validation
The system automatically validates that exactly **one** example is selected:
- If no example is selected: Compile error
- If multiple examples are selected: Compile error

## LED Mapping (STM32F4 Discovery)

- **LD3** (Orange): GPIO PD13
- **LD4** (Green): GPIO PD12
- **LD5** (Red): GPIO PD14
- **LD6** (Blue): GPIO PD15

## Troubleshooting

### Multiple Definition Errors
- Ensure only ONE example is uncommented in `examples_config.h`

### No Output on UART
- Check UART connection (115200 baud, 8N1)
- Verify logger is initialized before examples

### Stack Overflow Errors
- Increase `configTOTAL_HEAP_SIZE` in FreeRTOSConfig.h
- Check task stack sizes in examples.c

### LEDs Not Blinking
- Verify GPIO initialization in main.c
- Check LED pins match your board

## Course Notes

These examples correspond to the FreeRTOS Class #4 presentation:
- Slide 161-296: Logger Coherency Problem
- Slide 23-56: Task Creation and Memory Management
- Slide 64-159: Priority-Based Scheduling
- Slide 209-242: Synchronization Primitives (Semaphores)
- Slide 311-346: Task Delay API

## Next Steps

After completing these examples, you'll learn:
- Advanced inter-task communication (queues)
- Zero-copy logging patterns
- Direct-to-task notifications
- Software timers
