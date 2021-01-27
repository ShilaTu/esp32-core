#ifndef LIFESENSOR_COMMON_LOGGER_JSON_H
#define LIFESENSOR_COMMON_LOGGER_JSON_H

#include "logger.h"

void
lifesensor_logger_json_init(
    Lifesensor_logger *logger,
    Lifesensor_logger_scope *scope,
    int (*printf)(const char *fmt, ...));

#endif