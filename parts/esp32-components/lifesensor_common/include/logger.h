#ifndef LIFESENSOR_COMMON_LOGGER_H
#define LIFESENSOR_COMMON_LOGGER_H

#include <stddef.h>

#define LOG_UINT(LOGGER,SCOPE,PTR,NAME) LOGGER->log_uint(SCOPE,#NAME,(PTR)->NAME) 
#define LOG_INT(LOGGER,SCOPE,PTR,NAME) LOGGER->log_int(SCOPE,#NAME,(PTR)->NAME) 
#define LOG_FLOAT(LOGGER,SCOPE,PTR,NAME) LOGGER->log_float(SCOPE,#NAME,(PTR)->NAME) 
#define LOG_CHAR(LOGGER,SCOPE,PTR,NAME) LOGGER->log_char(SCOPE,#NAME,(PTR)->NAME) 
#define LOG_STR(LOGGER,SCOPE,PTR,NAME) LOGGER->log_str(SCOPE,#NAME,(PTR)->NAME) 
#define LOG_PTR(LOGGER,SCOPE,PTR,NAME) LOGGER->log_ptr(SCOPE,#NAME,(PTR)->NAME) 

enum lifesensor_logger_scope_type {
    LIFESENSOR_LOGGER_SCOPE_NONE,
    LIFESENSOR_LOGGER_SCOPE_LIST,
    LIFESENSOR_LOGGER_SCOPE_MAP
};

struct lifesensor_logger_scope {
    void (*printf)(const char *fmt, ...);
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
        char *name,
        unsigned long int value
    );
    void (*log_int)(
        struct lifesensor_logger_scope* scope,
        char *name,
        long int value
    );
    void (*log_float)(
        struct lifesensor_logger_scope* scope,
        char *name,
        float value
    );
    void (*log_char)(
        struct lifesensor_logger_scope* scope,
        char *name,
        char value
    );
    void (*log_str)(
        struct lifesensor_logger_scope* scope,
        char *name,
        char *value
    );
    void (*log_ptr)(
        struct lifesensor_logger_scope* scope,
        char *name,
        void *value
    );
};
typedef struct lifesensor_logger Lifesensor_logger;


#endif
