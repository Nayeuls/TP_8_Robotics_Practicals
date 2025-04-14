#include "config.h"
#include "modes.h"
#include "robot.h"
#include "module.h"
#include "registers.h"
#include "hardware.h"

const uint8_t MOTOR_ADDR = 21;

volatile static float freq = 1;
volatile static float amplitude = 40;

void sine_motor_mode(void)
{
  init_body_module(MOTOR_ADDR);
  start_pid(MOTOR_ADDR);
  set_color(4);

  uint32_t dt, cycletimer;
  float my_time, delta_t, l;
  int8_t l_rounded;

  cycletimer = getSysTICs();
  my_time = 0;

  freq = 1;

  do {
    // Calculates the delta_t in seconds and adds it to the current time
    dt = getElapsedSysTICs(cycletimer);
    cycletimer = getSysTICs();
    delta_t = (float) dt / sysTICSperSEC;
    my_time += delta_t;

    // Calculates the sine wave
    l = amplitude * sin(M_TWOPI * freq * my_time);
    l_rounded = (int8_t) l;

    bus_set(MOTOR_ADDR, MREG_SETPOINT, DEG_TO_OUTPUT_BODY(l_rounded));

    // Make sure there is some delay, so that the timer output is not zero
    pause(ONE_MS);

  } while (reg8_table[REG8_MODE] == IMODE_SINMOTOR_DEMO);

  bus_set(MOTOR_ADDR, MREG_SETPOINT, DEG_TO_OUTPUT_BODY(0.0));
  pause(ONE_SEC);
  bus_set(MOTOR_ADDR, MREG_MODE, MODE_IDLE);

  // Back to the "normal" green
  set_color(2);
}


static int8_t register_handler(uint8_t operation, uint8_t address, RadioData* radio_data)
{
  
  if(operation == ROP_WRITE_8) {
      if (address == REG8_FREQ) {
        freq = DECODE_PARAM_8(radio_data->byte, 0.0, 2.0); // Decode the frequency to a float value
        return TRUE;
      }
      if (address == REG8_AMPLITUDE) {
        amplitude = DECODE_PARAM_8(radio_data->byte, 0.0, 60.0); // Decode the amplitude to a float value
        return TRUE;
      }
  }
  return FALSE;
}


void main_mode_loop()
{
  reg8_table[REG8_MODE] = IMODE_IDLE;
  radio_add_reg_callback(register_handler);

  while (1)
  {
    switch(reg8_table[REG8_MODE])
    {
      case IMODE_IDLE:
        break;
      case IMODE_SINMOTOR_DEMO:
        sine_motor_mode();
        break;
      default:
        reg8_table[REG8_MODE] = IMODE_IDLE;
    }
  }
}
