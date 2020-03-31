/* Example test application for testable component.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "esp_system.h"

static void print_banner(const char* text)
{
    printf("#### %s #####\n", text);
}

void app_main()
{
    print_banner("START TESTS");
    UNITY_BEGIN();
    unity_run_all_tests();
    UNITY_END();
    print_banner("END TESTS");

    //restart esp so qemu can exit
    esp_restart();
}

