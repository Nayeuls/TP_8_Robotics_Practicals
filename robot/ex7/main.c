#include "config.h"
#include "robot.h"
#include "module.h"
#include "hardware.h"
#include "registers.h"
#include "constants.h"
#include "can.h"

const uint8_t MOTOR_ADDR[5] = {25, 22, 24, 26, 5 };

volatile static float freq = 1;
volatile static float amplitude = 0;
volatile static float phase_lag = 1;

/* Register callback function, handles some new registers on the radio.
 * All these registers are of course completely useless, but it demonstrates how
 * to implement a register callback function, and what it can do.
 */
static int8_t register_handler(uint8_t operation, uint8_t address, RadioData* radio_data)
{
  
  if(operation == ROP_WRITE_8) {
      if (address == REG8_FREQ) {
        freq = DECODE_PARAM_8(radio_data->byte, 0.0, FREQ_MAX); // Decode the frequency to a float value
        return TRUE;
      }
      if (address == REG8_AMPLITUDE) {
        amplitude = DECODE_PARAM_8(radio_data->byte, 0.0, AMPLITUDE_MAX); // Decode the amplitude to a float value
        return TRUE;
      }
      if (address == REG8_PHASE_LAG) {
        phase_lag = DECODE_PARAM_8(radio_data->byte, 0.5, 1.5); // Decode the amplitude to a float value
        return TRUE;
      }
  }
  return FALSE;
}

int main(void)
{
  hardware_init();
  registers_init();

  set_color(2);
  
  // Registers the register handler callback function
  radio_add_reg_callback(register_handler); // when a register is read or written, the function register_handler is called
  
  for(int i=0; i < 5; i++){
    init_body_module(MOTOR_ADDR[i]);
    start_pid(MOTOR_ADDR[i]);
    set_reg_value_dw(MOTOR_ADDR[i], MREG32_LED, 0);

  }

  uint32_t dt, cycletimer;
  float my_time, delta_t;

  cycletimer = getSysTICs();
  my_time = 0;

  // Keeps the LED blinking in green to demonstrate that the main program is
  // still running and registers are processed in background.
  while (1) {

    // Calculates the delta_t in seconds and adds it to the current time
    dt = getElapsedSysTICs(cycletimer);
    cycletimer = getSysTICs();
    delta_t = (float) dt / sysTICSperSEC;
    my_time += delta_t;

    // Calculates the sine wave
    for(int i = 0; i < 5; i++){
      int l = amplitude * sin(M_TWOPI * (freq * my_time + i * phase_lag / 5));
      bus_set(MOTOR_ADDR[4-i], MREG_SETPOINT, DEG_TO_OUTPUT_BODY((int8_t)l));
    }

    // Make sure there is some delay, so that the timer output is not zero
    pause(ONE_MS);
  }

  return 0;
}
