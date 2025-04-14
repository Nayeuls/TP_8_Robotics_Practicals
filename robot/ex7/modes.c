#include "config.h"
#include "modes.h"
#include "robot.h"
#include "module.h"
#include "registers.h"
#include "hardware.h"

volatile static float freq = 1;
volatile static float amplitude = 40;


void main_mode_loop()
{
  reg8_table[REG8_MODE] = IMODE_IDLE;
  // radio_add_reg_callback(register_handler);

  while (1)
  {
    reg8_table[4] = reg8_table[3];
    // switch(reg8_table[REG8_MODE])
    // {
    //   case IMODE_IDLE:
    //     break;
    //   case IMODE_SINMOTOR_DEMO:
    //     sine_motor_mode();
    //     break;
    //   default:
    //     reg8_table[REG8_MODE] = IMODE_IDLE;
    // }
  }
}
