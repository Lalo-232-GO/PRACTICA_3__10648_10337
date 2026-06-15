#include "app_task.h"

#include "app_config.h"
#include "system_state.h"
#include "leds.h"
#include "buttons.h" // Aquí debe de ser #include "buttons.h" y no #include "button.h"
#include "counter.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h" // Aquí debe de ser #include "esp_log.h" y no #include "esp_logs.h"

static const char *TAG = "MANAGER";

static TaskHandle_t h_leds[4];
static TaskHandle_t h_btn_start;
static TaskHandle_t h_btn_dir;
static TaskHandle_t h_btn_speed;
static TaskHandle_t h_counter;
static TaskHandle_t h_manager;

static LedTaskParams_t led_params[4] =
{
    /*TODO*/
    [0].bit_position = 0,
    [0].gpio = LED_B0,
    [0].name = "L_1",

    [1].bit_position = 1,
    [1].gpio = LED_B1,
    [1].name = "L_2",

    [2].bit_position = 2,
    [2].gpio = LED_B2   ,
    [2].name = "L_3",

    [3].bit_position = 3,
    [3].gpio = LED_B3,
    [3].name = "L_4"

};

static ButtonTaskParams_t btn_start =
{
    /*TODO*/
    .gpio = BTN_START,
    .name = "BTN_S_P",
    .type = BUTTON_START_PAUSE
};

static ButtonTaskParams_t btn_dir =
{
    /*TODO*/
    .gpio = BTN_DIR,
    .name = "BTN_D",
    .type = BUTTON_DIRECTION
};

static ButtonTaskParams_t btn_speed =
{
    /*TODO*/
    .gpio = BTN_SPEED,
    .name = "BTN_S",
    .type = BUTTON_SPEED
};

static const char *state_to_string(eTaskState state)
{
    switch (state) // Aquí debe de ser state y no eTaskState
    {
        case eRunning:
            return "RUNNING"; // Acá debe de ser RUNNING y no SUSPENDED

        case eReady:
            return "READY";

        case eBlocked:
            return "BLOCKED";

        case eSuspended:
            return "SUSPENDED";// Acá debe de ser SUSPENDED y no RUNNING

        case eDeleted:
            return "DELETED";

        default:
            return "UNKNOWN";
    }
}

static void manager_pause_system(void)
{
    g_system.mode = SYSTEM_PAUSED;

    /*
       Se suspende el contador:
       - conserva su valor actual
       - no ejecuta más pasos
       - NO regresa solo; necesita vTaskResume()
    */
    /*TODO*/
    vTaskSuspend(h_counter);


    /*
       Se suspenden botones que NO deben funcionar en pausa.
       Solo queda activo START/PAUSE.
    */
	/*TODO*/
    vTaskSuspend(h_btn_dir);
    vTaskSuspend(h_btn_speed);
    vTaskResume(h_btn_start);

    ESP_LOGW(TAG, "Sistema PAUSADO");
}

static void manager_run_system(void)
{
    g_system.mode = SYSTEM_RUNNING;

    /*
       Primero reanudamos botones de control.
    */
    vTaskResume(h_btn_dir);
    vTaskResume(h_btn_speed);

    /*
       Luego reanudamos contador.
    */
    vTaskResume(h_counter);

    ESP_LOGW(TAG, "Sistema RUNNING");
}

static void manager_toggle_direction(void)
{
    if (g_system.direction == COUNT_UP)
    {
        g_system.direction = COUNT_DOWN;
    }
    else
    {
        g_system.direction = COUNT_UP; // Acá debe de ser COUNT_UP y no COUNT_DOWN
    }

    ESP_LOGI(TAG,"Nueva direccion: %s",g_system.direction == COUNT_UP ? "UP" : "DOWN");
}

static void manager_toggle_speed(void)
{
    if (g_system.period_ms == SPEED_SLOW_MS)
    {
        g_system.period_ms = SPEED_FAST_MS;
    }
    else
    {
        g_system.period_ms = SPEED_SLOW_MS; // Acá debe de ser SPEED_SLOW_MS y no SPEED_FAST_MS
    }

    ESP_LOGI(TAG,"Nueva velocidad: %lu ms",(unsigned long)g_system.period_ms);
}

static void manager_print_states(void)
{
    ESP_LOGI(TAG, "------ ESTADOS ------");

    ESP_LOGI(TAG,"COUNTER: %s",state_to_string(eTaskGetState(h_counter)));

    ESP_LOGI(TAG,"BTN_START: %s",state_to_string(eTaskGetState(h_btn_start)));

    ESP_LOGI(TAG,"BTN_DIR: %s",state_to_string(eTaskGetState(h_btn_dir)));

    ESP_LOGI(TAG,"BTN_SPEED: %s",state_to_string(eTaskGetState(h_btn_speed)));

    for (int i = 0; i < 4; i++)
    {
        ESP_LOGI(TAG,"%s: %s",led_params[i].name,state_to_string(eTaskGetState(h_leds[i])));
    }

    ESP_LOGI(TAG,"Valor=%u | Modo=%s | Direccion=%s | Periodo=%lu ms",
             g_system.value,
             g_system.mode == SYSTEM_RUNNING ? "RUNNING" : "PAUSED",
             g_system.direction == COUNT_UP ? "UP" : "DOWN",
             (unsigned long)g_system.period_ms);
}

static void task_manager(void *pvParameters)
{
    (void)pvParameters;

    TickType_t last_print;
    last_print = xTaskGetTickCount();

    while (1)
    {
        ManagerEvent_t events;

        events = g_system.pending_event;

        if (events != MANAGER_EVENT_NONE) // Aquí debe de ser events y no event
        {
            /*
               Consumimos el evento.
               Nota: todavía no usamos mutex; en esta práctica aceptamos
               esta variable simple para enseñar handles/estados.
            */
            g_system.pending_event = MANAGER_EVENT_NONE;

            switch (events) // Aquí debe de ser events y no event
            {
                case MANAGER_EVENT_START_PAUSE:// Aquí debe de ser MANAGER_EVENT_START_PAUSE y no MANAGER_EVENT_SPEED
                    if (g_system.mode == SYSTEM_RUNNING)
                    {
                        manager_pause_system(); // Aquí debe de ser manager_pause_system() y no manager_run_system()
                    }
                    else
                    {
                        manager_run_system();
                    }
                    break;

                case MANAGER_EVENT_DIRECTION:
                    if (g_system.mode == SYSTEM_RUNNING)
                    {
                        manager_toggle_direction();
                    }
                    else
                    {
                        ESP_LOGW(TAG,"Direccion ignorada: sistema pausado");
                    }
                    break; // Aquí faltaba un break

                case MANAGER_EVENT_SPEED:
                    if (g_system.mode == SYSTEM_RUNNING)
                    {
                        manager_toggle_speed();
                    }
                    else
                    {
                        ESP_LOGW(TAG,"Velocidad ignorada: sistema pausado");
                    }
                    break;

                default:
                    break;
            }
        }

        /*
           Log periódico de estados cada 2 segundos.
        */
        if ((xTaskGetTickCount() - last_print) >= pdMS_TO_TICKS(2000))
        {
            last_print = xTaskGetTickCount();
            manager_print_states();
        }

        vTaskDelay(pdMS_TO_TICKS(20)); // Si son 2 mili segundos entoces es de 20 y no 20000
    }
}

void app_tasks_create(void)
{
    
	/* TODO --> Creación de tareas */
	//xTaskCreate(task_manager,"Manager",);
	xTaskCreate(task_manager, "Manager", 2048, NULL, tskIDLE_PRIORITY + 1, &h_manager);
    xTaskCreate(button_task, "BTN_START/Stop", 2048, &btn_start, tskIDLE_PRIORITY + 1, &h_btn_start);
    xTaskCreate(button_task, "BTN_DIR", 2048, &btn_dir, tskIDLE_PRIORITY + 1, &h_btn_dir);
    xTaskCreate(button_task, "BTN_SPEED", 2048, &btn_speed, tskIDLE_PRIORITY + 1, &h_btn_speed);
    xTaskCreate(counter_task, "Counter", 2048, NULL, tskIDLE_PRIORITY + 1, &h_counter);
    xTaskCreate(led_task, "LED_1", 2048, &led_params[0], tskIDLE_PRIORITY + 1, &h_leds[0]);
    xTaskCreate(led_task, "LED_2", 2048, &led_params[1], tskIDLE_PRIORITY +1, &h_leds[1]);
    xTaskCreate(led_task, "LED_3", 2048, &led_params[2], tskIDLE_PRIORITY + 1, &h_leds[2]);
    xTaskCreate(led_task, "LED_4", 2048, &led_params[3], tskIDLE_PRIORITY + 1, &h_leds[3]);
    /*
       Estado inicial:
       - contador pausado
       - dirección y velocidad deshabilitados
       - start activo
       - leds activos mostrando valor inicial
    */
    manager_pause_system();
}