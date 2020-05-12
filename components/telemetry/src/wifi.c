#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <esp_event_loop.h>
#include <esp_wifi.h>
#include <string.h>
#include <stdlib.h>
#include <nvs_flash.h>

#include "wifi.h"

#define CONNECTED_BIT BIT0

static EventGroupHandle_t wifi_event_group;

/**
 * Handler for WiFi events
 *
 * @ctx: context pointer
 * @event: Wifi event that occurred
 *
 * @return: esp_err_t 0 in every case
 */
static
esp_err_t
event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id)
    {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;

        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            break;

        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            break;

        default:
            break;
    }
    return ESP_OK;
}

void
wifi_init(void)
{
#ifdef CONFIG_TELEMETRY_WIFI_ENABLE
    // initialize NVS, required for wifi
    ESP_ERROR_CHECK(nvs_flash_init());

    // initialize wifi structures
    wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t wifi_config = {
    .sta = {
        .ssid = CONFIG_TELEMETRY_WIFI_SSID,
        .password = CONFIG_TELEMETRY_WIFI_PW
        },
    };

    // connect to the wifi network
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
#endif
}

void wifi_wait_connected(void)
{
#ifdef CONFIG_TELEMETRY_WIFI_ENABLE
    xEventGroupWaitBits(wifi_event_group,
                        CONNECTED_BIT,
                        false,
                        true,
                        portMAX_DELAY);
#endif
}
