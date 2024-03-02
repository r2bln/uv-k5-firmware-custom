#include "randomapp.h"

#include "../bsp/dp32g030/gpio.h"
#include "../driver/bk4819.h"
#include "../driver/gpio.h"
#include "../driver/keyboard.h"
#include "../driver/st7565.h"
#include "../driver/system.h"

#include "ui/helper.h"
#include "ui/main.h"

static const uint8_t MAX_WIDTH = 128;
static const uint8_t MAX_HEIGHT = 56;

static const int w = 32;
static const int h = 32;

#define for_x for (int x = 0; x < w; x++)
#define for_y for (int y = 0; y < h; y++)
#define for_xy for_x for_y

static bool isInitialized = false;
static KeyboardState kbd = {KEY_INVALID, KEY_INVALID, 0};

static void PutPixel(uint8_t x, uint8_t y, bool fill) {
    UI_DrawPixelBuffer(gFrameBuffer, x, y, fill);
}

static void Seed(void* u) {
    unsigned (*univ)[w] = u;
    for_xy univ[x][y] = BK4819_GetRSSI() % 2 ? 1 : 0;
}

static void DrawBox() {
    for (uint8_t x = 0; x < MAX_WIDTH; ++x) {
        PutPixel(x, 0, true);
        PutPixel(x, MAX_HEIGHT - 1, true);
    }

    for (uint8_t y = 0; y < MAX_HEIGHT; ++y) {
        PutPixel(0, y, true);
        PutPixel(MAX_WIDTH - 1, y, true);
    }    
}

static void Render(void* u) {
    unsigned (*univ)[MAX_WIDTH] = u;
    UI_DisplayClear();

    DrawBox();

    for_xy PutPixel(x, y, univ[x][y]);

    ST7565_BlitFullScreen();
}

static void Evolve(void* u) {
	unsigned (*univ)[w] = u;
	unsigned new[h][w];

	for_y for_x {
		int n = 0;
		for (int y1 = y - 1; y1 <= y + 1; y1++)
			for (int x1 = x - 1; x1 <= x + 1; x1++)
				if (univ[(y1 + h) % h][(x1 + w) % w])
					n++;

		if (univ[y][x]) n--;
		new[y][x] = (n == 3 || (n == 2 && univ[y][x]));
	}
	for_y for_x univ[y][x] = new[y][x];
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
    uint8_t univ[MAX_WIDTH][MAX_HEIGHT];

    Seed(univ);
    while (isInitialized)
    {
        HandleUserInput();
        Evolve(univ);
        Render(univ);
    }
}