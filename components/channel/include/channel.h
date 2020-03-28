// Channels are objects which enable iteration over other Channel objects with
// the same name/identifier.
// This implementation resembles a map, mapping identifiers to lists. 
// Channels are implemented as an item of two separate lists.
// The "unique" list holds items with different identifiers,
// the "same" list holds items that share the same identifier. 
// Each list item itself is a valid list containing only itself;
// multiple items can be combined to a larger list.
// 
// This implementation constructs a single valid "unique" list,
// starting with a globally defined root node. All "same" lists of all items
// are considered valid.
//
// Adding to this structure implies iterating the "unique" list, until either
// the same identifier is found (and the new item is added to the "same" list
// of the found item) or the end of the "unique" list reached (and it's
// appended as new "unique" list item).
//
// Each added Channel object is now part of a "same" list which can be directly
// iterated to find other items.
//
// exemplary structure:
// 
//              root node---.  O        O
//                          v  |        |
// unique identifier list-> O--O--O--O--O  
//                             |     |
//                             O     O
//                             ^
//      same identifier list---'                    
//
// Each Channel object contains a context pointer, a callback function and
// an additional flags field. A broadcast function is provided to iterate
// through all "same" list items of a Channel object. The callback is issued
// with a data pointer and a timeout value by the caller and the callee
// specific context and flags. This interface is intended to enable dynamic
// management of FreeRTOS queues, where a queue can be registered as a context
// of a Channel and therefore fed by the Channels callback.

#ifndef __CHANNEL__
#define __CHANNEL__

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "list.h"

typedef BaseType_t (*Channel_callback) (void *ctx, const void * const data, TickType_t timeout, const BaseType_t flags);

typedef struct channel {
    const char *identifier;
    struct list_head same;
    struct list_head unique;
    void *ctx;
    BaseType_t flags;
    Channel_callback callback;
} Channel;

typedef struct broadcast {
    Channel *ch;
    Channel *pos;
    void *data;
    TickType_t timeout;
} Broadcast;

#ifndef STR
#define STR(X) _STR(X)
#define _STR(X) #X
#endif

/**
 * CHANNEL_INIT - Macro to provide initialization for channel object
 */
#define CHANNEL_INIT(name, identifier, ctx, flags, callback) { "" identifier "", LIST_HEAD_INIT(name.same), LIST_HEAD_INIT(name.unique), ctx, flags, callback }

/**
 * CHANNEL - Macro to create and initialize channel object
 */
#define CHANNEL(name, identifier, ctx, flags, callback) Channel name = CHANNEL_INIT(name, "" identifier "", ctx, flags, callback)

/*
 * OUTPUT - Macro to create and initialize channel object used by producer
 */
#define OUTPUT(name) CHANNEL(name, STR(name), NULL, 0, NULL)

/*
 * INPUT - Macro to create and initialize channel object used by consumer
 */
#define INPUT(name, ctx, flags, callback) CHANNEL(name, STR(name), ctx, flags, callback)

/*
 * INPUT_QUEUE - Macro to create and initialize channel object used by consumer for queues
 */
#define INPUT_QUEUE(name, queue) CHANNEL(name, STR(name), queue, queueSEND_TO_BACK, &xQueueGenericSend)

/**
 * channel_init - helper function to initialize channel object
 * @ch: pointer to channel object to initialize
 * @identifier: identifier string of the channel
 * @ctx: pointer to context of the callback
 * @flags: additional flags for the callback to modify behaviour
 * @callback: function to call on new data  
 */ 
static
void
inline __attribute__((always_inline))
channel_init 
(Channel * const ch, const char * const identifier, void * const ctx, const BaseType_t flags, Channel_callback callback) {
    ch->identifier = identifier;
    INIT_LIST_HEAD(&ch->unique);
    INIT_LIST_HEAD(&ch->same);
    ch->ctx = ctx;
    ch->flags = flags;
    ch->callback = callback;
}

/**
 * channel_setContext - helper function to change the context the callback acts on
 * @ch: pointer to channel object to modify
 * @ctx: pointer to new context
 */
static
void
inline __attribute__((always_inline))
channel_setContext 
(Channel * ch, void * const ctx) {
    ch->ctx = ctx;
}

/**
 * channel_setCallback - helper function to change the callback
 * @ch: pointer to channel object to modify
 * @callback: function to register as callback
 */
static
void 
inline __attribute__((always_inline))
channel_setCallback
(Channel * ch, const Channel_callback callback) {
    ch->callback = callback;
}

/**
 * channel_register - register a channel
 * @ch: channel to register
 * 
 * This function adds channel to the message passing structure. 
 * It doesn't make a difference whether this is a message
 * provider or consumer. The difference is only made by the channels
 * callback implementation.
 */
void
channel_register
(Channel * const ch);

/**
 * unregister_channel - removes a channel input or output from the message structure
 * @ch: channel element to remove
 */
void
channel_unregister
(Channel *ch);

/**
 * channel_send - sends a message to a single channel object
 * @ch: the channel structure that holds informations for this channel
 * @data: the pointer to the message
 * @timeout: how many time-slices the callback may block before aborting
 * @return: returns pdPass (=1) if send was successful, if non-one value is returned, callback failed
 * 
 * Calls the callback function on the channel.
 */
BaseType_t
channel_send
(const Channel *ch, const void *data, const TickType_t timeout);

/**
 * channel_broadcast_init - helper function to initialize broadcast handler
 * @handle: pointer to broadcast object
 * @ch: pointer to channel object initiating the broadcast
 * @data: pointer to data to be transferred
 * @timeout: number of time-slices each receiver may block 
 */
static
void
inline __attribute__((always_inline))
channel_broadcast_init
(Broadcast *handle, const Channel * ch, const void *data, const TickType_t timeout)
{
    handle->ch = (void*) ch;
    handle->pos = (void*) ch;
    handle->data = (void*) data;
    handle->timeout = timeout;
}

/**
 * channel_broadcast_finished - helper function to determine if broadcast finished
 * @ch: pointer to channel object
 * @pos: location of pointer where broadcast stopped 
 */
static
bool
inline __attribute__((always_inline))
channel_broadcast_finished
(const Broadcast *handle)
{
    return handle->pos == NULL;
}

/**
 * channel_broadcast - send a message to all who registered on this channels identifier
 * @handle: the broadcast handle to act on
 * @return: returns callback return value each time iteration was stopped 
 * 
 * Iterates over all elements in the same list, that registered on this channels identifier
 * and calls the callback function on it. May return with error_code before all elements were notified, allowing for custom error handling
 * Example for skipping failing callbacks:
 *  Channel ch;
 *  Channel *pos;
 *  Broadcast handle;
 *  channel_broadcast_init(&handle,&ch,data,timeout);
 *  while(!channel_broadcast_finished(&handle)) {
 *    channel_broadcast(&handle);
 *  }
 */
BaseType_t
channel_broadcast
(Broadcast *handle);

#endif
