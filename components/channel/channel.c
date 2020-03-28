#include "channel.h"

#include <string.h>
#include <freertos/projdefs.h>

static CHANNEL(root_channel, "", NULL, 0, NULL);

void 
channel_register
(Channel *ch) 
{
    Channel *curr;
    list_for_each_entry(curr, &root_channel.unique, unique) {
        if (strcmp(ch->identifier, curr->identifier) == 0) {
            list_add(&ch->same, &curr->same);
            return;
        }
    }

    list_add(&ch->unique, &root_channel.unique);
}

void 
channel_unregister
(Channel *ch) 
{
    int isUnique = ! list_empty(&ch->unique);
    int isSame   = ! list_empty(&ch->same);

    //channel is not registered
    if (!isUnique && !isSame) {
        return;
    }
    //channel is not in the unique chain so just delete it
    if (!isUnique && isSame) {
        list_del(&ch->same);
        INIT_LIST_HEAD(&ch->same);
        return;
    }
    //channel is in the unique list, but single element so also just delete it
    if (isUnique && !isSame) {
        list_del(&ch->unique);
        INIT_LIST_HEAD(&ch->unique);
        return;
    }
    //channel is in unique list and there are others in same list, so replace channel
    if (isUnique && isSame) {
        Channel *nextSame = list_entry(ch->same.next, struct channel, same);
        Channel *nextUniq = list_entry(ch->unique.next, struct channel, unique);
        list_del(&ch->same);
        INIT_LIST_HEAD(&ch->same);
        list_del(&ch->unique);
        INIT_LIST_HEAD(&ch->unique);
        list_add(&nextSame->unique, &nextUniq->unique);
        return;
    }
}

BaseType_t
channel_send
(const Channel *ch, const void *data, const TickType_t timeout)
{
    if (!ch->callback) {
        return pdPASS;
    }
    return ch->callback(ch->ctx, data, timeout, ch->flags);
}

BaseType_t
channel_broadcast
(Broadcast *handle)
{
    Channel *ch  = handle->ch;
    Channel *pos = handle->pos;
    void *data   = handle->data;
    TickType_t timeout = handle->timeout; 
    list_for_each_entry_continue(pos, (&ch->same), same) {
        BaseType_t status;
        if (pos->callback) {
            status = pos->callback(pos->ctx,
                                   data,
                                   timeout,
                                   pos->flags);
            if (!status) {
                handle->pos = pos;
                return status;
            };
        }
    }
    handle->pos = NULL;
    return channel_send(handle->ch, handle->data, handle->timeout);
}
