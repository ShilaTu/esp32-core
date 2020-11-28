
#include "logger.h"

static inline void
_lifesensor_logger_json_comma(
    Lifesensor_logger_scope *scope)
{
    if (scope->entries++)
    {
        scope->printf(", ");
    }
}

static inline void
_lifesensor_logger_json_name(
    Lifesensor_logger_scope *scope,
    char *name)
{
    if (name)
    {
        scope->printf("\"%s\": ");
    }
}

static void
lifesensor_logger_json_enter_map(
    Lifesensor_logger_scope *scope,
    Lifesensor_logger_scope *subscope,
    char *name)
{
    if (scope) {
        subscope->entries = 0;
        subscope->level = scope->level + 1;
        subscope->printf = scope->printf;

        _lifesensor_logger_json_comma(scope);
        _lifesensor_logger_json_name(scope, name);
    }

    subscope->type = LIFESENSOR_LOGGER_SCOPE_MAP;
    subscope->printf("{ ");
}

static void
lifesensor_logger_json_enter_list(
    Lifesensor_logger_scope *scope,
    Lifesensor_logger_scope *subscope,
    char *name)
{
    if (scope) {
        subscope->entries = 0;
        subscope->level = scope->level + 1;
        subscope->printf = scope->printf;

        _lifesensor_logger_json_comma(scope);
        _lifesensor_logger_json_name(scope, name);
    }

    subscope->type = LIFESENSOR_LOGGER_SCOPE_LIST;
    subscope->printf("[ ");
}

static void
lifesensor_logger_json_exit(
    Lifesensor_logger_scope *scope,
    Lifesensor_logger_scope *subscope)
{
    switch (subscope->type)
    {
    case LIFESENSOR_LOGGER_SCOPE_MAP:
        subscope->printf(" }");
        break;
    case LIFESENSOR_LOGGER_SCOPE_LIST:
        subscope->printf(" ]");
        break;
    }
}

static void
lifesensor_logger_json_log_uint(
    Lifesensor_logger_scope *scope,
    unsigned long int value,
    char *name)
{
    _lifesensor_logger_json_comma(scope);
    _lifesensor_logger_json_name(scope, name);
    scope->printf("%+lu", value);
}

static void
lifesensor_logger_json_log_int(
    Lifesensor_logger_scope *scope,
    long int value,
    char *name)
{
    _lifesensor_logger_json_comma(scope);
    _lifesensor_logger_json_name(scope, name);
    scope->printf("%+ld", value);
}

static void
lifesensor_logger_json_log_float(
    Lifesensor_logger_scope *scope,
    float value,
    char *name)
{
    _lifesensor_logger_json_comma(scope);
    _lifesensor_logger_json_name(scope, name);
    scope->printf("%+G", value);
}

static void
lifesensor_logger_json_log_char(
    Lifesensor_logger_scope *scope,
    char value,
    char *name)
{
    _lifesensor_logger_json_comma(scope);
    _lifesensor_logger_json_name(scope, name);
    scope->printf("\"%c\"", value);
}

static void
lifesensor_logger_json_log_str(
    Lifesensor_logger_scope *scope,
    char *value,
    char *name)
{
    _lifesensor_logger_json_comma(scope);
    _lifesensor_logger_json_name(scope, name);
    scope->printf("\"%s\"", value);
}

static void
lifesensor_logger_json_log_ptr(
    Lifesensor_logger_scope *scope,
    void *value,
    char *name)
{
    _lifesensor_logger_json_comma(scope);
    _lifesensor_logger_json_name(scope, name);
    scope->printf("\"0x%x\"", value);
}

void lifesensor_logger_json_init(
    Lifesensor_logger *logger,
    Lifesensor_logger_scope *scope,
    int (*printf)(const char *fmt, ...))
{
    logger->enter_map = &lifesensor_logger_json_enter_map;
    logger->enter_list = &lifesensor_logger_json_enter_list;
    logger->exit = &lifesensor_logger_json_exit;
    logger->log_uint = &lifesensor_logger_json_log_uint;
    logger->log_int = &lifesensor_logger_json_log_int;
    logger->log_float = &lifesensor_logger_json_log_float;
    logger->log_char = &lifesensor_logger_json_log_char;
    logger->log_str = &lifesensor_logger_json_log_str;
    logger->log_ptr = &lifesensor_logger_json_log_ptr;

    scope->type = LIFESENSOR_LOGGER_SCOPE_NONE;
    scope->entries = 0;
    scope->level = 0;
    scope->printf = printf;
}
