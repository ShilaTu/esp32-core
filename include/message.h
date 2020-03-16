#ifndef __MESSAGE__
#define __MESAAGE

#include <string.h>

#include "list.h"

typedef enum {
    PUSH_OK, PUSH_ERROR, PUSH_BLOCK
} push_result;

typedef struct group {
    const char *name;
    struct list_head same;
    struct list_head unique;
    void *state;
    unsigned int data_size;
    push_result (*push) (struct group *, void *, unsigned int);
} channel_group;


void init_message();

void register_channel(channel_group *group);
void unregister_channel(channel_group *group);

void publish(channel_group *channel, void *data, unsigned int size);

#endif