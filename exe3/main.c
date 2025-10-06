#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"
QueueHandle_t xQueueData;

void data_task(void *p) {
    vTaskDelay(pdMS_TO_TICKS(400));

    int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
    for (int i = 0; i < data_len; i++) {
        xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void func_B(int buffer[5], int data) {
    buffer[0] = buffer[1];
    buffer[1] = buffer[2];
    buffer[2] = buffer[3];
    buffer[3] = buffer[4];
    buffer[4] = data;
}

void process_task(void *p) {
    int data;

    int buffer[5] = {0,0,0,0,0};
    int soma, media;

    while (true) {
        if (xQueueReceive(xQueueData, &data, 100) == pdTRUE)  {
            // implementar filtro aqui!
            func_B(buffer,data);
            soma = (buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4]);
            media = soma/5;
            printf("%d \n", media);

            // deixar esse delay!
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    xQueueData = xQueueCreate(64, sizeof(int));

    xTaskCreate(data_task, "Data task ", 4096, NULL, 1, NULL);
    xTaskCreate(process_task, "Process task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}