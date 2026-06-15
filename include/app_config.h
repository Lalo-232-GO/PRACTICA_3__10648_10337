#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define LED_B0      2
#define LED_B1      4
#define LED_B2      5
#define LED_B3      18 // Aquí debe de ser 18 y no 28 porque el 28 no exite xd

#define BTN_DIR     19
#define BTN_SPEED   21 // Aquí ya esta usado el pin 4 como salida por lo que debe de ser 21
#define BTN_START   22 // Aquí ya esta usado el pin 2 como salida por lo que debe de ser 22

#define SPEED_SLOW_MS   500
#define SPEED_FAST_MS   250

#define BUTTON_POLL_MS  20 //Aca debe se der 20 y no 20000

#endif