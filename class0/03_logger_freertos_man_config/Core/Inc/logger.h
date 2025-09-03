/*
 * logger.h
 *
 *  Created on: Aug 20, 2025
 *      Author: Orr
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "main.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// Log levels
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_NONE
} log_level_t;

// Public functions
void logger_init(void);
void logger_set_level(log_level_t level);
void logger_log(log_level_t level, const char* tag, const char* format, ...);

// Convenience macros
#define LOG_DEBUG(tag, ...) logger_log(LOG_LEVEL_DEBUG, tag, __VA_ARGS__)
#define LOG_INFO(tag, ...)  logger_log(LOG_LEVEL_INFO, tag, __VA_ARGS__)
#define LOG_WARN(tag, ...)  logger_log(LOG_LEVEL_WARNING, tag, __VA_ARGS__)
#define LOG_ERROR(tag, ...) logger_log(LOG_LEVEL_ERROR, tag, __VA_ARGS__)

#endif /* INC_LOGGER_H_ */
