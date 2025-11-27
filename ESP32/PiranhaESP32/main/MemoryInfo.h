#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_hidd_prf_api.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "driver/gpio.h"
#include "hid_dev.h"

void print_sram_summary(void) {
    printf("=== SRAM SUMMARY ===\n");
    printf("Free heap: %" PRIu32 "bytes\n", esp_get_free_heap_size());
    printf("Min block: %ubytes\n", heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT));
    printf("Max block: %ubytes\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
}


void print_sram_details(void) {
    printf("--- SRAM BY CATEGORY ---\n");
    
    // Обычная оперативная память (DRAM)
    printf("DRAM (general): %u bytes\n", 
           heap_caps_get_free_size(MALLOC_CAP_8BIT));
    
    // Память с поддержкой DMA (для периферии)
    printf("DRAM (DMA): %u bytes\n",
           heap_caps_get_free_size(MALLOC_CAP_DMA));
    
    // Внутренняя память, исполняющая код (IRAM)
    printf("IRAM (code): %u bytes\n",
           heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_EXEC));
    
    // Остальная внутренняя память (DRAM без DMA)
    printf("DRAM (internal) : %u bytes\n",
           heap_caps_get_free_size(MALLOC_CAP_INTERNAL & ~MALLOC_CAP_DMA));
}

void print_task_stack_usage(void) {
    TaskHandle_t current = xTaskGetCurrentTaskHandle();
    UBaseType_t high_water = uxTaskGetStackHighWaterMark(current);
    
    printf("Task '%s' stack: %u bytes free\n",
           pcTaskGetName(current),
           high_water * sizeof(StackType_t));
}
/*
void dump_sram_stats(void) {
    printf("\n***** SRAM USAGE REPORT *****\n");
    print_sram_summary();
    print_sram_details();

    // Статистика по задачам
    printf("--- TASK STACK USAGE ---\n");
    TaskStatus_t *task_status = NULL;
    UBaseType_t task_count = uxTaskGetNumberOfTasks();
    
    task_status = malloc(task_count * sizeof(TaskStatus_t));
    if (task_status) {
        task_count = uxTaskGetSystemState(task_status, task_count, NULL);
        for (UBaseType_t i = 0; i < task_count; i++) {
            printf("Task '%s': %" PRIu32 "bytes free (high water)\n",
                   task_status[i].pcTaskName,
                   task_status[i].usStackHighWaterMark * sizeof(StackType_t));
        }
        free(task_status);
    }

    printf("**************************\n\n");
}
*/