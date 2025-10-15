/*
 * logger.c
 *
 *  Created on: Aug 20, 2025
 *      Author: Orr
 * @brief Simple UART-based logging system implementation
 *
 */



#include "logger.h"
#include "cmsis_os.h"

// External UART handle (defined in main.c)
extern UART_HandleTypeDef huart2;

// External RTC handle (defined in main.c)
extern RTC_HandleTypeDef hrtc;

// External mutex handle (defined in main.c)
extern osMutexId_t loggerMutexHandle;

// Private variables
static log_level_t current_log_level = LOG_LEVEL_INFO;

// Log level strings
static const char* level_str[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
};

/**
 * @brief Initialize logger system
 */
void logger_init(void) {
    current_log_level = LOG_LEVEL_INFO;

    // Send startup message
    const char* startup_msg = "\r\n=== Logger System Initialized ===\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)startup_msg, strlen(startup_msg), 1000);
}

/**
 * @brief Set logger level
 * @param level New log level
 */
void logger_set_level(log_level_t level) {
    current_log_level = level;
}

/**
 * @brief Main logging function
 * @param level Log level
 * @param tag Module tag
 * @param format Printf-style format string
 * @param ... Variable arguments
 */
void logger_log(log_level_t level, const char* tag, const char* format, ...) {
    // Filter by log level
    if (level < current_log_level) {
        return;
    }

    char buffer[512];
    va_list args;

    // Get timestamp from RTC
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;

    if (HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN) == HAL_OK) {
        HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // Must read date after time

        // Build log message with timestamp
        int prefix_len = snprintf(buffer, sizeof(buffer),
                                  "[%02d:%02d:%02d][%s][%s] ",
                                  time.Hours, time.Minutes, time.Seconds,
                                  level_str[level], tag);

        va_start(args, format);
        vsnprintf(buffer + prefix_len, sizeof(buffer) - prefix_len, format, args);
        va_end(args);
    } else {
        // RTC not available, use simple format
        int prefix_len = snprintf(buffer, sizeof(buffer),
                                  "[--:--:--][%s][%s] ",
                                  level_str[level], tag);

        va_start(args, format);
        vsnprintf(buffer + prefix_len, sizeof(buffer) - prefix_len, format, args);
        va_end(args);
    }

    // Add newline
    strcat(buffer, "\r\n");

    // Acquire mutex to protect UART access (only if RTOS is running)
    if (loggerMutexHandle != NULL) {
        osMutexAcquire(loggerMutexHandle, osWaitForever);
    }

    // Output to UART
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    // Release mutex (only if RTOS is running)
    if (loggerMutexHandle != NULL) {
        osMutexRelease(loggerMutexHandle);
    }
}
