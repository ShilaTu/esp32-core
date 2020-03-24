#ifndef _TRANSMISSION_H_
#define _TRANSMISSION_H_

#include <stdint.h>

#define SERVER_IP "192.168.100.1"
#define PORT 4321

/**
 * Initialise transmission environment
 */
void transmission_init(void);

/**
 * Shutdown the socket
 */
void transmission_shutdown(void);

/**
 * Send data to target
 *
 * @data: data to transmitt
 *
 * Return:
 *      err: < 0 on failure
 */
int32_t transmission_send(char *data, unsigned int size);

#endif
