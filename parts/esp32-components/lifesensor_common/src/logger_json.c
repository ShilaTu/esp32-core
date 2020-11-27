
#include "logger.h"

static inline void
_lifesensor_logger_yaml_comma(
    Lifesensor_logger_scope *scope
){
    if (scope->entries++) {
        scope->printf(", ");
    }
}

static inline void
_lifesensor_logger_yaml_name(
    Lifesensor_logger_scope *scope,
    char *name
){
    if (name) {
        scope->printf("\"%s\": ");
    }
}

static void
lifesensor_logger_yaml_enter_map(
    Lifesensor_logger_scope *scope,
    Lifesensor_logger_scope *subscope,
    char *name)
{
    subscope->entries = 0;
    subscope->level = scope->level + 1;
    subscope->type = LIFESENSOR_LOGGER_SCOPE_MAP;
    subscope->printf = scope->printf;
    
    _lifesensor_logger_yaml_comma(scope);
    _lifesensor_logger_yaml_name(scope,name);
    scope->printf("{ ");
}

static void
lifesensor_logger_yaml_enter_list(
    Lifesensor_logger_scope *scope,
    Lifesensor_logger_scope *subscope,
    char *name)
{
    subscope->entries = 0;
    subscope->level = scope->level + 1;
    subscope->type = LIFESENSOR_LOGGER_SCOPE_LIST;
    subscope->printf = scope->printf;
    
    _lifesensor_logger_yaml_comma(scope);
    _lifesensor_logger_yaml_name(scope,name);
    scope->printf("[ ");
}

static void
lifesensor_logger_yaml_exit(
    Lifesensor_logger_scope *scope,
    Lifesensor_logger_scope *subscope)
{
    switch(subscope->type) {
        case LIFESENSOR_LOGGER_SCOPE_MAP:
            scope->printf(" }");
            break;
        case LIFESENSOR_LOGGER_SCOPE_LIST:
            scope->printf(" ]");
            break;
    }
}

static void
lifesensor_logger_yaml_log_uint(
    Lifesensor_logger_scope *scope,
    char *name,
    unsigned long int value)
{
    _lifesensor_logger_yaml_comma(scope);
    _lifesensor_logger_yaml_name(scope, name);
    scope->printf("%+lu", value);
}

static void
lifesensor_logger_yaml_log_int(
    Lifesensor_logger_scope *scope,
    char *name,
    long int value)
{
    _lifesensor_logger_yaml_comma(scope);
    _lifesensor_logger_yaml_name(scope, name);
    scope->printf("%+ld", value);
}

static void
lifesensor_logger_yaml_log_float(
    Lifesensor_logger_scope *scope,
    char *name,
    float value)
{
    _lifesensor_logger_yaml_comma(scope);
    _lifesensor_logger_yaml_name(scope, name);
    scope->printf("%+G", value);
}

static void
lifesensor_logger_yaml_log_char(
    Lifesensor_logger_scope *scope,
    char *name,
    char value)
{
    _lifesensor_logger_yaml_comma(scope);
    _lifesensor_logger_yaml_name(scope, name);
    scope->printf("\"%c\"", value);
}

static void
lifesensor_logger_yaml_log_str(
    Lifesensor_logger_scope *scope,
    char *name,
    char *value)
{
    _lifesensor_logger_yaml_comma(scope);
    _lifesensor_logger_yaml_name(scope, name);
    scope->printf("\"%s\"", value);
}

static void
lifesensor_logger_yaml_log_ptr(
    Lifesensor_logger_scope *scope,
    char *name,
    void *value)
{
    _lifesensor_logger_yaml_comma(scope);
    _lifesensor_logger_yaml_name(scope, name);
    scope->printf("\"0x%x\"", value);
}

void
lifesensor_logger_yaml_init(
    Lifesensor_logger *logger,
    Lifesensor_logger_scope *scope,
    void (*printf)(const char* fmt, ...))
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
