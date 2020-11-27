#ifndef CHANNEL_DEBUG_H_
#define CHANNEL_DEBUG_H_

#include "channel.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
#include <string.h>

/**
 * channel_debug_print - print channel attributes
 * @prefix: prefix string in debug output
 * @ch: channel object to print
 */ 
static
void
__attribute__((unused))
channel_debug_print
(const char *prefix, const Channel *ch)
{
    ESP_LOGD(prefix, "@%p identifier:%s",
             ch, ch->identifier);
    ESP_LOGD(prefix, "@%p same.next:%p",
             ch, list_entry(ch->same.next, struct channel, same));
    ESP_LOGD(prefix, "@%p same.prev:%p",
             ch, list_entry(ch->same.prev, struct channel, same));
    ESP_LOGD(prefix, "@%p uniq.next:%p",
             ch, list_entry(ch->unique.next, struct channel, unique));
    ESP_LOGD(prefix, "@%p uniq.prev:%p",
             ch, list_entry(ch->unique.prev, struct channel, unique));
}

/**
 * channel_debug_print_dot - print channel attributes in dot format
 * @prefix: prefix string in debug output
 * @ch: channel object to print
 */
static
void
__attribute__((unused))
channel_debug_print_dot
(const char *prefix, const Channel *ch)
{
    if (strcmp("",ch->identifier) == 0) {
        ESP_LOGD(prefix, "x%p [label=\"%p\\n%s\", shape=box];",
                 ch, ch, ch->identifier);
    } else {
        if (ch->ctx == NULL) {
            ESP_LOGD(prefix, "x%p [label=\"producer\\n%p\\n%s\" shape=house];",
                    ch, ch, ch->identifier);
        } else {
            ESP_LOGD(prefix, "x%p [label=\"consumer\\n%p\\n%s\" shape=invhouse];",
                    ch, ch, ch->identifier);
        }
    }
    ESP_LOGD(prefix, "x%p -> x%p [label=sn, color=red];",
             ch, list_entry(ch->same.next, struct channel, same));
    ESP_LOGD(prefix, "x%p -> x%p [label=sp, color=red];",
             ch, list_entry(ch->same.prev, struct channel, same));
    ESP_LOGD(prefix, "x%p -> x%p [label=un, color=blue];",
             ch, list_entry(ch->unique.next, struct channel, unique));
    ESP_LOGD(prefix, "x%p -> x%p [label=up, color=blue];",
             ch, list_entry(ch->unique.prev, struct channel, unique));
}

/**
 * channel_debug_printAll - print all reachable channel attributes
 * @prefix: prefix string in debug output
 * @ch: channel object to start search from 
 */
static
void
__attribute__((unused))
channel_debug_printAll
(const char *prefix, const Channel *ch)
{
    channel_debug_print(prefix, ch);
    Channel *chUniq;
    list_for_each_entry(chUniq, &ch->unique, unique) {
        channel_debug_print(prefix, chUniq);
        Channel *currSame;
        list_for_each_entry(currSame, &chUniq->same, same) {
            channel_debug_print(prefix, currSame);
        }
    }
    Channel *chSame;
    list_for_each_entry(chSame, &ch->same, same) {
        channel_debug_print(prefix, chSame);
        Channel *currUniq;
        list_for_each_entry(currUniq, &chSame->unique, unique) {
            channel_debug_print(prefix, currUniq);
            Channel *currSame;
            list_for_each_entry(currSame, &currUniq->same, same) {
                channel_debug_print(prefix, currSame);
            }
        }
    }
}

/**
 * channel_debug_printAll_dot - print all reachable channel attributes in dot format
 * @prefix: prefix string in debug output
 * @ch: channel object to start search from 
 */
static
void
__attribute__((unused))
channel_debug_printAll_dot
(const char *prefix, const Channel *ch)
{
    ESP_LOGD(prefix,"digraph G {");
    ESP_LOGD(prefix,"layout=neato;");
    ESP_LOGD(prefix,"overlap=false;");
    channel_debug_print_dot(prefix, ch);
    Channel *chUniq;
    list_for_each_entry(chUniq, &ch->unique, unique) {
        channel_debug_print_dot(prefix, chUniq);
        Channel *currSame;
        list_for_each_entry(currSame, &chUniq->same, same) {
            channel_debug_print_dot(prefix, currSame);
        }
    }
    Channel *chSame;
    list_for_each_entry(chSame, &ch->same, same) {
        channel_debug_print_dot(prefix, chSame);
        Channel *currUniq;
        list_for_each_entry(currUniq, &chSame->unique, unique) {
            channel_debug_print_dot(prefix, currUniq);
            Channel *currSame;
            list_for_each_entry(currSame, &currUniq->same, same) {
                channel_debug_print_dot(prefix, currSame);
            }
        }
    }
    ESP_LOGD(prefix,"}");
}

#endif
