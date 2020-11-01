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

typedef BaseType_t (*Channel_callback) (void *ctx, const void *data, const TickType_t timeout, const BaseType_t flags);

struct channel {
    const char *identifier;
    struct list_head same;
    struct list_head unique;
    void *ctx;
    BaseType_t flags;
    Channel_callback callback;
};

typedef struct channel Channel;
typedef struct channel Channel_consumer;
typedef struct channel Channel_producer;

typedef struct broadcast {
    Channel *ch;
    Channel *pos;
    void *data;
    TickType_t timeout;
    TickType_t elapsed;
} Channel_broadcast;

#ifndef STR
#define STR(X) _STR(X)
#define _STR(X) #X
#endif

/**
 * channel_init - initialize & register channel object
 * @ch: pointer to channel object to initialize
 * @identifier: identifier string of the channel
 * @ctx: pointer to context of the callback
 * @flags: additional flags for the callback to modify behaviour
 * @callback: function to call on new data  
 */ 
void
channel_init 
(Channel *ch, const char *identifier, void *ctx, const BaseType_t flags, Channel_callback callback);

/**
 * channel_init_consumer - helper function to initialize channel object as consumer
 * @ch: pointer to channel object to initialize
 * @identifier: identifier string of the channel
 * @queue: queue handle to send input to 
 */
static
void
inline
channel_init_consumer
(Channel_consumer *ch, const char *identifier, QueueHandle_t queue)
{
    return channel_init(ch, identifier, queue, queueSEND_TO_BACK, &xQueueGenericSend);
}

/**
 * channel_init_producer - helper function to initialize channel object as producer
 * @ch: pointer to channel object to initialize
 * @identifier: identifier string of the channel
 */
static
void
inline
channel_init_producer
(Channel_producer *ch, const char *identifier)
{
    return channel_init(ch, identifier, NULL, 0, NULL);
}

/**
 * channel_setContext - helper function to change the context the callback acts on
 * @ch: pointer to channel object to modify
 * @ctx: pointer to new context
 */
static
void
inline
channel_setContext 
(Channel *ch, void *ctx)
{
    ch->ctx = ctx;
}

/**
 * channel_setCallback - helper function to change the callback
 * @ch: pointer to channel object to modify
 * @callback: function to register as callback
 */
static
void 
inline
channel_setCallback
(Channel *ch, const Channel_callback callback)
{
    ch->callback = callback;
}

/**
 * reset_channel - removes a channel input or output from the message structure
 * @ch: channel element to reset
 */
void
channel_reset
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
(const Channel_consumer *ch, const void *data, const TickType_t timeout);

/**
 * channel_broadcast_init - helper function to initialize broadcast handler
 * @handle: pointer to broadcast object
 * @ch: pointer to channel object initiating the broadcast
 * @data: pointer to data to be transferred
 * @timeout: number of time-slices each receiver may block 
 */
static
void
inline
channel_broadcast_init
(Channel_broadcast *handle, const Channel_producer *ch, const void *data, const TickType_t timeout)
{
    handle->ch = (void*) ch;
    handle->pos = (void*) ch;
    handle->data = (void*) data;
    handle->timeout = timeout;
    handle->elapsed = 0;
}

/**
 * channel_broadcast_timeout - helper function to determine if broadcast timed out
 * @handle: pointer to broadcast object
 */
static
bool
inline
channel_broadcast_timeout
(Channel_broadcast *handle)
{
    if (handle->timeout == portMAX_DELAY) {
        return false;
    }
    if (handle->elapsed >= handle->timeout) {
        return true;
    }
    return false;
}

/**
 * channel_broadcast_finished - helper function to determine if broadcast finished
 * Does not provide information if broadcast was successful!
 * @ch: pointer to channel object
 */
static
bool
inline
channel_broadcast_finished
(const Channel_broadcast *handle)
{
    return handle->pos == NULL;
}

/**
 * channel_broadcast - send a message to all who registered on this channels identifier
 * @handle: the broadcast handle to act on
 * @return: returns callback return value each time iteration was stopped 
 * 
 * Iterates over all elements in the same list, that registered on this channels identifier
 * and calls the callback function on it. May return with error_code before all elements were notified,
 * allowing for custom error handling.
 */
BaseType_t
channel_broadcast
(Channel_broadcast *handle);

#endif
