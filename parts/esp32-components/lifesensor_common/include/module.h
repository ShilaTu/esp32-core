#ifndef LIFESENSOR_COMMON_MODULE_H
#define LIFESENSOR_COMMON_MODULE_H

#include "logger.h"
#include "macro/map.h"
#include <stddef.h>

#define INIT_LIFESENSOR_MODULE(TYPE, NAME, INIT, DUMP, /* SUBMODULES */...)                    \
    {                                                                                          \
        .parent = NULL,                                                                        \
        .type = #TYPE,                                                                         \
        .name = (NAME),                                                                        \
        .init = (INIT),                                                                        \
        .dump = (DUMP),                                                                        \
        .submodule_offsets = (ptrdiff_t[]){__VA_OPT__(MAP1(offsetof, TYPE, __VA_ARGS__), ) 0}, \
    }

struct lifesensor_module
{
    struct lifesensor_module *parent;
    char *name;
    char *type;
    void (*dump)(
        struct lifesensor_module *module,
        Lifesensor_logger *logger,
        Lifesensor_logger_scope *scope);
    void (*init)(
        struct lifesensor_module *module);
    ptrdiff_t *submodule_offsets;
};
typedef struct lifesensor_module Lifesensor_module;

void lifesensor_module_init(Lifesensor_module *module);
void lifesensor_module_dump(Lifesensor_module *module, Lifesensor_logger *logger, Lifesensor_logger_scope *scope);
#endif