#include <freertos/FreeRTOS.h>
#include <stdint.h>
#include <unistd.h>

#include <lwip/err.h>
#include <lwip/udp.h>
#include <lwip/sockets.h>

#include "transmission.h"

#define SERVER_IP "192.168.100.1"
#define PORT 4321

struct sockaddr_in dest_addr;
int32_t sock = -1;

void
transmission_init(void)
{
    uint8_t addr_family = AF_INET;
    char address[32];

    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    dest_addr.sin_family = addr_family;
    dest_addr.sin_port = htons(PORT);
    inet_ntoa_r(dest_addr.sin_addr, (char *)address, sizeof(address) - 1);

    while (sock < 0)
    {
        sock = socket(addr_family, SOCK_DGRAM, IPPROTO_IP);
    }
}

void
transmission_shutdown(void)
{
    shutdown(sock, 0);
    close(sock);
}

int32_t
transmission_send(char *data)
{
    int32_t err = sendto(sock, data, sizeof(data), 0,
                         (struct sockaddr *)&dest_addr,
                         sizeof(dest_addr));
    return err;
}
