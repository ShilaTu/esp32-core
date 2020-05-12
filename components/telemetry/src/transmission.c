#include <freertos/FreeRTOS.h>
#include <stdint.h>
#include <unistd.h>

#include <lwip/err.h>
#include <lwip/udp.h>
#include <lwip/sockets.h>

#include "transmission.h"

static struct sockaddr_in dest_addr;
static int32_t sock = -1;

void
transmission_init(void)
{
#ifdef CONFIG_TELEMETRY_NETWORK_ENABLE
    uint8_t addr_family = AF_INET;
    char address[32];

    dest_addr.sin_addr.s_addr = inet_addr(CONFIG_TELEMETRY_NETWORK_SERVER_IP);
    dest_addr.sin_family = addr_family;
    dest_addr.sin_port = htons(CONFIG_TELEMETRY_NETWORK_SERVER_PORT);
    inet_ntoa_r(dest_addr.sin_addr, (char *)address, sizeof(address) - 1);

    while (sock < 0)
    {
        sock = socket(addr_family, SOCK_DGRAM, IPPROTO_IP);
    }
#endif
}

void
transmission_shutdown(void)
{
#ifdef CONFIG_TELEMETRY_NETWORK_ENABLE
    shutdown(sock, 0);
    close(sock);
#endif
}

int32_t
transmission_send(char *data, unsigned int size)
{
#ifdef CONFIG_TELEMETRY_NETWORK_ENABLE
    int32_t err = sendto(sock, data, size, 0,
                         (struct sockaddr *)&dest_addr,
                         sizeof(dest_addr));
    return err;
#else 
    return -1;
#endif
}
