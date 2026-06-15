#include "counter.h"
#include "system_state.h" // Aquí debe de ser #include "system_state.h" y no #include "system_states.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static const char *TAG = "COUNTER"; // Acá también pasa algo raro, debe ser *TAG y no TAG

static void counters_step(void) // Aquí debe de ser counters_step y no counter_step
{
	/*TODO --> Si g_system.direction = COUNT_UP entonces g_system.value incrementa su valor*/
    if(g_system.direction == COUNT_UP)
	{
		g_system.value++;
    }
    else
    {
	/*TODO --> Si g_system.direction = COUNT_DOWN entonces g_system.value decrementa su valor*/
	g_system.value--;
    }
}

void counter_task(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        /*
           Si esta tarea está suspendida, no entra aquí.
           Cuando está corriendo, cuenta en función del periodo.
        */
        ESP_LOGI(TAG,"Valor=%u | Direccion=%s | Periodo=%lu ms",g_system.value,g_system.direction == COUNT_UP ? "UP" : "DOWN",(unsigned long)g_system.period_ms);

        vTaskDelay(pdMS_TO_TICKS(g_system.period_ms));

        counters_step();
    }
}