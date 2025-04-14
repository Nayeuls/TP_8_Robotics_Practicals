// #include "hardware.h"
// #include "module.h"
// #include "robot.h"

// const uint8_t MOTOR_ADDR = 21;

// int main(void)
// {
//   int8_t pos;

//   hardware_init();
  
//   // Changes the color of the led (red) to show the boot
//   set_color_i(4, 0);

//   // Initialises the body module with the specified address (but do not start
//   // the PD controller)
//   init_body_module(MOTOR_ADDR);
  
//   // And then... do this
//   while (1) { // set the leds depending on the position of the motor
//     pos = bus_get(MOTOR_ADDR, MREG_POSITION); // get the MREG_POSITION register value of the motor that is at the address MOTOR_ADDR on the bus
//     if (pos > 0) {
//       set_rgb(pos, 32, 0);
//     } else {
//       pos = -pos;
//       set_rgb(0, 32, pos);
//     }
//   }

//   return 0;
// }


#include "hardware.h"
#include "module.h"
#include "robot.h"
#include "registers.h"

#define NB_DOF 4
const uint8_t MOTOR_ADDR[NB_DOF] = {21U, 72U, 73U, 74U}; // addresses of the motors
static int8_t pos[NB_DOF]; // array to store the position of the motors

static int8_t register_handler(uint8_t operation, uint8_t address, RadioData* radio_data)
{
  if(operation == ROP_READ_MB) { // copy-paste from ex2
    if (address == 0) {
      radio_data->multibyte.size = NB_DOF;
      for (int i = 0; i < NB_DOF; i++) {
        radio_data->multibyte.data[i] = pos[i];
      }
      return TRUE;
    }
  } else {
    return FALSE;
  }
}


int main(void)
{

  hardware_init();
  radio_add_reg_callback(register_handler);

  // Changes the color of the led (red) to show the boot
  set_color_i(4, 0);

  // Initialises the body module with the specified address (but do not start
  // the PD controller)
  init_body_module(MOTOR_ADDR[0]);
  init_body_module(MOTOR_ADDR[1]);
  init_limb_module(MOTOR_ADDR[2]);
  init_limb_module(MOTOR_ADDR[3]);
  
  while (1) {
    for (int i = 0; i < NB_DOF; i++) {
      pos[i] = bus_get(MOTOR_ADDR[i], MREG_POSITION);
    }     
  }

  return 0;
}
