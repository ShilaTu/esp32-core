#ifndef CHANNEL_INTERNAL_H_
#define CHANNEL_INTERNAL_H_

#include "channel.h"

#ifndef CONFIG_CHANNEL_INTERNAL_EXPORT_SYMBOLS
#define EXPORT static
#else
#define EXPORT
#endif

/**
 * channel_internal_init - initialize channel object
 * @ch: pointer to channel object to initialize
 * @identifier: identifier string of the channel
 * @ctx: pointer to context of the callback
 * @flags: additional flags for the callback to modify behaviour
 * @callback: function to call on new data  
 */
EXPORT
void
channel_internal_init 
(Channel *ch, const char *identifier, void *ctx, const BaseType_t flags, Channel_callback callback);

/**
 * channel_internal_register - register a channel
 * @ch: channel to register
 * 
 * This function adds channel to the message passing structure. 
 * It doesn't make a difference whether this is a message
 * provider or consumer. The difference is only made by the channels
 * callback implementation.
 */
EXPORT
void 
channel_internal_register
(Channel *ch);

/**
 * channel_debug_unregister - removes a channel input or output from the message structure
 * @ch: channel element to unregister
 */
EXPORT
void 
channel_internal_unregister
(Channel *ch);

/**
 * channel_internal_resetRoot - reset global root node
 * ATTENTION: THIS ONLY RESETS THE GLOBAL ROOT NODE!
 *            ALL CHANNELS OBJECTS WILL BE BROKEN!  
 */
EXPORT
void
channel_internal_resetRoot
(void);

#endif