#ifndef LIFESENSOR_COMMON_LOGGER_H
#define LIFESENSOR_COMMON_LOGGER_H

#include <stddef.h>

enum lifesensor_logger_scope_type {
    LIFESENSOR_LOGGER_SCOPE_NONE,
    LIFESENSOR_LOGGER_SCOPE_LIST,
    LIFESENSOR_LOGGER_SCOPE_MAP
};

struct lifesensor_logger_scope {
    int (*printf)(const char *fmt, ...);
    enum lifesensor_logger_scope_type type;
    size_t entries;
    size_t level;
};
typedef struct lifesensor_logger_scope Lifesensor_logger_scope;

struct lifesensor_logger {
    void (*enter_map)(
        struct lifesensor_logger_scope* scope,
        struct lifesensor_logger_scope* subscope,
        char *name
    );
    void (*enter_list)(
        struct lifesensor_logger_scope* scope,
        struct lifesensor_logger_scope* subscope,
        char *name
    );
    void (*exit)(
        struct lifesensor_logger_scope* scope,
        struct lifesensor_logger_scope* subscope
    );

    void (*log_uint)(
        struct lifesensor_logger_scope* scope,
        unsigned long int value,
        char *name
    );
    void (*log_int)(
        struct lifesensor_logger_scope* scope,
        long int value,
        char *name
    );
    void (*log_float)(
        struct lifesensor_logger_scope* scope,
        float value,
        char *name
    );
    void (*log_char)(
        struct lifesensor_logger_scope* scope,
        char value,
        char *name
    );
    void (*log_str)(
        struct lifesensor_logger_scope* scope,
        char *value,
        char *name
    );
    void (*log_ptr)(
        struct lifesensor_logger_scope* scope,
        void *value,
        char *name
    );
};
typedef struct lifesensor_logger Lifesensor_logger;


#endif
