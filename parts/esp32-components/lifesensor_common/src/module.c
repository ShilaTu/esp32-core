
#include "module.h"
#include "logger.h"

void lifesensor_module_init(Lifesensor_module *module)
{
    for (ptrdiff_t *offset = module->submodule_offsets; *offset; offset++)
    {
        Lifesensor_module *submodule = ((void *)module) + *offset;
        submodule->parent = module;
        if (submodule->init)
        {
            submodule->init(submodule);
        }
    }
    if (module->init)
    {
        module->init(module);
    }
}

void lifesensor_module_dump(Lifesensor_module *module, Lifesensor_logger *logger, Lifesensor_logger_scope *scope)
{
    Lifesensor_logger_scope subscope;
    Lifesensor_logger_scope subsubscope;

    logger->log_ptr(scope, module, "address");
    logger->log_ptr(scope, module->parent, "parent");
    logger->log_str(scope, module->type, "type");
    logger->log_str(scope, module->name, "name");
    logger->log_ptr(scope, module->init, "init");
    logger->log_ptr(scope, module->dump, "dump");

    logger->enter_list(scope, &subscope, "submodule_offsets");
    for (ptrdiff_t *offset = module->submodule_offsets; *offset; offset++)
    {
        logger->log_int(&subscope, *offset, NULL);
    }
    logger->exit(scope, &subscope);

    logger->enter_map(scope, &subscope, "locals");
    if (module->dump)
    {
        module->dump(module, logger, &subscope);
    }
    logger->exit(scope, &subscope);

    logger->enter_map(scope, &subscope, "submodules");
    for (ptrdiff_t *offset = module->submodule_offsets; *offset; offset++)
    {
        Lifesensor_module *submodule = ((void *)module) + *offset;
        logger->enter_map(&subscope, &subsubscope, submodule->name);
        lifesensor_module_dump(submodule, logger, &subsubscope);
        logger->exit(&subscope, &subsubscope);
    }
    logger->exit(scope, &subscope);
}
