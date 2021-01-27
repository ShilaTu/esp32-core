
#include "logger.h"

static inline void
_lifesensor_logger_yaml_prefix(
    Lifesensor_logger_scope *scope,
    char *name)
{
    scope->printf("\n%*s", 4 * scope->level, "");
    scope->entries++;
    switch (scope->type)
    {
    case LIFESENSOR_LOGGER_SCOPE_LIST:
        scope->printf("- ");
        break;
    case LIFESENSOR_LOGGER_SCOPE_MAP:
        if (name)
        {
            scope->printf("%s: ", name);
        }
        break;
    }
}

static void
lifesensor_logger_yaml_enter_map(
    Lifesensor_logger_scope *scope,
    Lifesensor_logger_scope *subscope,
    char *name)
{
    subscope->type = LIFESENSOR_LOGGER_SCOPE_MAP;
    
    if (!scope) {
        return;
    }

    subscope->entries = 0;
    subscope->level = scope->level + 1;
    subscope->printf = scope->printf;

    _lifesensor_logger_yaml_prefix(scope, name);
}

static void
lifesensor_logger_yaml_enter_list(
    Lifesensor_logger_scope *scope,
    Lifesensor_logger_scope *subscope,
    char *name)
{
    subscope->type = LIFESENSOR_LOGGER_SCOPE_LIST;

    if (!scope) {
        return;
    }

    subscope->entries = 0;
    subscope->level = scope->level + 1;
    subscope->printf = scope->printf;

    _lifesensor_logger_yaml_prefix(scope, name);
}

static void
lifesensor_logger_yaml_exit(
    Lifesensor_logger_scope *scope,
    Lifesensor_logger_scope *subscope)
{
    if (!subscope->entries)
    {
        switch (subscope->type)
        {
        case LIFESENSOR_LOGGER_SCOPE_MAP:
            subscope->printf("{}");
            break;
        case LIFESENSOR_LOGGER_SCOPE_LIST:
            subscope->printf("[]");
            break;
        }
    }
}

static void
lifesensor_logger_yaml_log_uint(
    Lifesensor_logger_scope *scope,
    unsigned long int value,
    char *name)
{
    _lifesensor_logger_yaml_prefix(scope, name);
    scope->printf("%+lu", value);
}

static void
lifesensor_logger_yaml_log_int(
    Lifesensor_logger_scope *scope,
    long int value,
    char *name)
{
    _lifesensor_logger_yaml_prefix(scope, name);
    scope->printf("%+ld", value);
}

static void
lifesensor_logger_yaml_log_float(
    Lifesensor_logger_scope *scope,
    float value,
    char *name)
{
    _lifesensor_logger_yaml_prefix(scope, name);
    scope->printf("%+G", value);
}

static void
lifesensor_logger_yaml_log_char(
    Lifesensor_logger_scope *scope,
    char value,
    char *name)
{
    _lifesensor_logger_yaml_prefix(scope, name);
    scope->printf("\"%c\"", value);
}

static void
lifesensor_logger_yaml_log_str(
    Lifesensor_logger_scope *scope,
    char *value,
    char *name)
{
    _lifesensor_logger_yaml_prefix(scope, name);
    scope->printf("\"%s\"", value);
}

static void
lifesensor_logger_yaml_log_ptr(
    Lifesensor_logger_scope *scope,
    void *value,
    char *name)
{
    _lifesensor_logger_yaml_prefix(scope, name);
    scope->printf("0x%x", value);
}

void lifesensor_logger_yaml_init(
    Lifesensor_logger *logger,
    Lifesensor_logger_scope *scope,
    int (*printf)(const char *fmt, ...))
{
    logger->enter_map = &lifesensor_logger_yaml_enter_map;
    logger->enter_list = &lifesensor_logger_yaml_enter_list;
    logger->exit = &lifesensor_logger_yaml_exit;
    logger->log_uint = &lifesensor_logger_yaml_log_uint;
    logger->log_int = &lifesensor_logger_yaml_log_int;
    logger->log_float = &lifesensor_logger_yaml_log_float;
    logger->log_char = &lifesensor_logger_yaml_log_char;
    logger->log_str = &lifesensor_logger_yaml_log_str;
    logger->log_ptr = &lifesensor_logger_yaml_log_ptr;

    scope->type = LIFESENSOR_LOGGER_SCOPE_NONE;
    scope->entries = 0;
    scope->level = 0;
    scope->printf = printf;
}
