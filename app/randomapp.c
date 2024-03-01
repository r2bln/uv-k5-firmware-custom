#include "randomapp.h"

#include "../driver/st7565.h"

#include "driver/keyboard.h"
#include "ui/helper.h"
#include "ui/main.h"

static bool isInitialized = false;

static void PutPixel(uint8_t x, uint8_t y, bool fill) {
  UI_DrawPixelBuffer(gFrameBuffer, x, y, fill);
}

static void DrawBox() {
    for (uint8_t x = 0; x < 128; ++x) {
        PutPixel(x, 0, true);
        PutPixel(x, 55, true);
    }

    for (uint8_t y = 0; y < 56; ++y) {
        PutPixel(0, y, true);
        PutPixel(127, y, true);
    }    
}

static void Render() {
    UI_DisplayClear();

    UI_PrintString("HELLO", 2, 0, 0, 8);
    UI_PrintString("ZADROSTAN", 2, 0, 3, 8);

    DrawBox();

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