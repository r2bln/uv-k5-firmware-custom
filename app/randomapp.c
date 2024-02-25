#include "randomapp.h"

#include "../driver/st7565.h"

#include "driver/keyboard.h"
#include "ui/helper.h"
#include "ui/main.h"

static bool isInitialized = false;

static void Render() {
    UI_DisplayClear();

    UI_PrintString("HELLO", 2, 0, 0, 8);
    UI_PrintString("ZADROSTAN", 2, 0, 3, 8);

    ST7565_BlitFullScreen();
}

// static void OnKeyDown(uint8_t key) {
//     switch (key)
//     {
//     case KEY_EXIT:
//         DeInitRandomapp();
//         break;
//     
//     default:
//         break;
//     }
// }

// static void DeInitRandomapp(void) {
//     isInitialized = false;
// }

void APP_RunRandomapp(void) {
    isInitialized = true;

    while (isInitialized)
    {
        Render();
    }
}