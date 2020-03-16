#include "message.h"

static channel_group unique_head;

/**
 * init_message - Initializes dummy head of unique identifier chain
 */
void 
init_message() 
{
    unique_head.name = "";
    INIT_LIST_HEAD(&unique_head.same);
    INIT_LIST_HEAD(&unique_head.unique);
    unique_head.push = NULL;
    unique_head.state = NULL;
    unique_head.data_size = 0;
}

/**
 * register_channel - register in or output to a channel
 * @channel: channel to register
 * 
 * This function adds channel to the message passing structure. 
 * It doesn't make a difference whether this is a message
 * provider or consumer. The difference is only made by the channels
 * callback implementation.
 */
void 
register_channel
(channel_group *channel) 
{
    channel_group *curr;
    list_for_each_entry(curr, &unique_head.unique, unique) {
        if (strcmp(channel->name, curr->name) == 0) {
            list_add(&channel->same, &curr->same);
            return;
        }
    }

    list_add(&channel->unique, &unique_head.unique);
}

/**
 * publish - publish a message to all who registered on this channels identifier
 * @channel: the channel structure that holds informations for this channel
 * @data: the messages content
 * @size: the size of the message data
 * 
 * Iterates over all elements in the same list, that registered on this channels identifier
 * and calls the push callback function on it. 
 */
void 
publish
(channel_group *channel, void *data, unsigned int size) 
{
    channel_group *curr;
    list_for_each_entry(curr, &channel->same, same) {
        if (curr->push) {
            curr->push(curr, data, size);
        }
    }
}

/**
 * unregister_channel - removes a channel input or output from the message structure
 * @channel: channel to remove
 */
void 
unregister_channel
(channel_group *channel) 
{
    //channel is not in the unique chain so just delete it
    if (list_empty(&channel->unique)) {
        list_del(&channel->same);
        return;
    }
    //channel is in the unique list
    //but single element so also just delete it
    if (list_empty(&channel->same)) {
        list_del(&channel->unique);
        return;
    }
    //channel is in unique list and there are others in same list
    //so replace channel
    struct list_head *unique_prev = channel->unique.prev;
    list_del(&channel->unique);
    list_add(unique_prev, channel->same.next);
}