#include <iostream>
#include "remregs.h"
#include "robot.h"
#include "utils.h"
#include "regdefs.h"
#include "../../robot/ex4/modes.h"
#include <math.h>

const uint8_t RADIO_CHANNEL = 201;         ///< robot radio channel
const char* INTERFACE = "COM1";            ///< robot radio interface

int main()
{
    CRemoteRegs regs;

    if (!init_radio_interface(INTERFACE, RADIO_CHANNEL, regs)) {
        return 1;
    }

    // Reboots the head microcontroller to make sure it is always in the same state
    reboot_head(regs);
  
    regs.set_reg_b(REG8_MODE, IMODE_IDLE); // Set mode to idle
    while (!kbhit()){
        // Mode demo
        regs.set_reg_b(REG8_MODE, IMODE_MOTOR_DEMO); // Set mode to motor demo
    }
    ext_key();
    while (!kbhit()){
        // Mode sin
        regs.set_reg_b(REG8_MODE, IMODE_MOTOR_SIN); // Set mode to sin demo
        regs.set_reg_b(2, 40*(sin(2*M_PI*time_d())));  // sine wave at 1 Hz (takes rad)
    }
    
    regs.set_reg_b(REG8_MODE,0); // Set mode to idle
    regs.close();
    return 0;
}