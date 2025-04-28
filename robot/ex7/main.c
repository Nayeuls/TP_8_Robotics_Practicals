#include "hardware.h"
#include "registers.h"

static uint32_t rgb = 0;
static uint8_t r = 0;
static uint8_t g = 0;
static uint8_t b = 0;

/* Register callback function, handles some new registers on the radio.
 * All these registers are of course completely useless, but it demonstrates how
 * to implement a register callback function, and what it can do.
 */
static int8_t register_handler(uint8_t operation, uint8_t address, RadioData* radio_data)
{
  uint8_t i;
  
  switch (operation)
  {
    case ROP_WRITE_32:
      if (address == 0) {
        rgb = radio_data->dword;
        return TRUE;
      }
      break;
  }
  return FALSE;
}

int main(void)
{
  hardware_init();
  
  // Registers the register handler callback function
  radio_add_reg_callback(register_handler); // when a register is read or written, the function register_handler is called
  
  // Keeps the LED blinking in green to demonstrate that the main program is
  // still running and registers are processed in background.
  while (1) {
    r = (rgb >> 16) & 0xFF;
    g = (rgb >> 8) & 0xFF;
    b = rgb & 0xFF;
    set_rgb(r, g, b);
    pause(TEN_MS);
  }

  return 0;
}
