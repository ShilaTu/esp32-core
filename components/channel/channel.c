#include "channel.h"

#include <string.h>
#include <freertos/projdefs.h>


// root channel of the unique channel list
static CHANNEL(root, "", NULL, 0, NULL);

void 
channel_register
(Channel *ch) 
{
    Channel *curr;
    list_for_each_entry(curr, &root.unique, unique) {
        if (strcmp(ch->identifier, curr->identifier) == 0) {
            list_add(&ch->same, &curr->same);
            return;
        }
    }

    list_add(&ch->unique, &root.unique);
}

BaseType_t
channel_broadcast
(Channel * const ch, Channel ** pos, const void * const data, const TickType_t timeout)
{
    Channel *curr = *pos;
    list_for_each_entry_continue(curr, &ch->same, same) {
        if (curr->callback) {
            BaseType_t status = curr->callback(curr->ctx, data, timeout, curr->flags);
            if (!status) {
                *pos = curr;
                return status;
            };
        }
    }
    return pdPASS;
}

BaseType_t
channel_send
(Channel * const ch, const void * const data, const TickType_t timeout, const BaseType_t flags)
{
    if (!ch->callback) {
        return pdPASS;
    }
    return ch->callback(ch->ctx, data, timeout, ch->flags);
}

void 
channel_unregister
(Channel *ch) 
{
    int unique = ! list_empty(&ch->unique);
    int same   = ! list_empty(&ch->same);

    //channel is not registered
    if (!unique && !same) {
        return;
    }
    //channel is not in the unique chain so just delete it
    if (!unique && same) {
        list_del(&ch->same);
        return;
    }
    //channel is in the unique list, but single element so also just delete it
    if (unique && !same) {
        list_del(&ch->unique);
        return;
    }
    //channel is in unique list and there are others in same list, so replace channel
    if (unique && same) {
        list_add(&ch->unique, &list_entry(ch->same.next, struct channel, same)->unique);
        list_del(&ch->unique);
        list_del(&ch->same);
        return;
    }
}
