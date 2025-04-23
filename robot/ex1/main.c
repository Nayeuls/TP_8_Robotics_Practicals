#include <stdint.h>
#include "hardware.h"
#include "registers.h"

#define PROGRAM 1

int main(void)
{
  int8_t i;

  hardware_init();
  reg32_table[REG32_LED] = LED_MANUAL;  // manual LED control

  while (PROGRAM == 0) {
    for (i = 0; i < 127; i++) {
      set_rgb(i, 0, 0);
      pause(TEN_MS);
    }
    for (i = 0; i < 127; i++) {
      set_rgb(127, i, 0);
      pause(TEN_MS);
    }
    for (i = 0; i < 127; i++) {
      set_rgb(127, 127, i);
      pause(TEN_MS);
    }
    for (i = 127; i >= 0; i--) {
      set_rgb(i, 127, 127);
      pause(TEN_MS);
    }
    for (i = 127; i >= 0; i--) {
      set_rgb(0, i, 127);
      pause(TEN_MS);
    }
    for (i = 127; i >= 0; i--) {
      set_rgb(0, 0, i);
      pause(TEN_MS);
    }
  }
  while (PROGRAM == 1) {
    set_rgb(0, 127, 0);   // green LED on
    pause(HALF_SEC);           // 500 ms pause
    set_rgb(0, 0, 0);     //LED off
    pause(HALF_SEC);           // 500 ms pause
  }
  return 0;
}