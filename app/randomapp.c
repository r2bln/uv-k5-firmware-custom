#include "randomapp.h"

#include "../bsp/dp32g030/gpio.h"
#include "../driver/gpio.h"
#include "../driver/keyboard.h"
#include "../driver/st7565.h"
#include "../driver/system.h"


#include "debugging.h"
#include "ui/helper.h"
#include "ui/main.h"

static bool isInitialized = false;
static KeyboardState kbd = {KEY_INVALID, KEY_INVALID, 0};

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

static void DeInitRandomapp(void) {
     isInitialized = false;
}

static void OnKeyDown(uint8_t key) {
    switch (key)
    {
    case KEY_EXIT:
        DeInitRandomapp();
        break;
    
    default:
        break;
    }
}

static KEY_Code_t GetKey() {
  KEY_Code_t btn = KEYBOARD_Poll();
  if (btn == KEY_INVALID && !GPIO_CheckBit(&GPIOC->DATA, GPIOC_PIN_PTT)) {
    btn = KEY_PTT;
  }
  return btn;
}

static bool HandleUserInput() {
    kbd.prev = kbd.current;
    kbd.current = GetKey();

    if (kbd.current != KEY_INVALID && kbd.current == kbd.prev) {
        if (kbd.counter < 16)
        kbd.counter++;
        else
        kbd.counter -= 3;
        SYSTEM_DelayMs(20);
    } else {
        kbd.counter = 0;
    }

    if (kbd.counter == 3 || kbd.counter == 16) {
        OnKeyDown(kbd.current);
    }

    return true;
}

void APP_RunRandomapp(void) {
    isInitialized = true;
    LogUart("hello zadrostan from uart!");

    while (isInitialized)
    {
        HandleUserInput();
        Render();
    }
}