#include "channel.h"
#include "channel_internal.h"

#include <string.h>
#include <freertos/projdefs.h>

/**
 * CHANNEL_INIT - Macro to provide initialization for channel object
 */
#define CHANNEL_INIT(name, identifier, ctx, flags, callback) { "" identifier "", LIST_HEAD_INIT(name.same), LIST_HEAD_INIT(name.unique), ctx, flags, callback }

/**
 * CHANNEL - Macro to create and initialize channel object
 */
#define CHANNEL(name, identifier, ctx, flags, callback) Channel name = CHANNEL_INIT(name, "" identifier "", ctx, flags, callback)

static CHANNEL(root_channel, "", NULL, 0, NULL);

void
channel_init
(Channel *ch, const char *identifier, void *ctx, const BaseType_t flags, Channel_callback callback)
{
    channel_internal_init(ch, identifier, ctx, flags, callback);
    channel_internal_register(ch);
}

EXPORT
void
channel_internal_init 
(Channel *ch, const char *identifier, void *ctx, const BaseType_t flags, Channel_callback callback)
{
    ch->identifier = identifier;
    INIT_LIST_HEAD(&ch->unique);
    INIT_LIST_HEAD(&ch->same);
    ch->ctx = ctx;
    ch->flags = flags;
    ch->callback = callback;
}

void
channel_reset
(Channel *ch)
{
    channel_internal_unregister(ch);
}

EXPORT
void 
channel_internal_register
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

EXPORT
void 
channel_internal_unregister
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
(Channel_broadcast *handle)
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

EXPORT
void
__attribute__((unused))
channel_internal_resetRoot
(void)
{
    INIT_LIST_HEAD(&root_channel.unique);
    INIT_LIST_HEAD(&root_channel.same);
}