#include <iostream>
#include "remregs.h"
#include "robot.h"
#include "utils.h"
#include "regdefs.h"
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
  
    regs.set_reg_b(REG8_MODE,0); // Set mode to idle
    while (!kbhit()){
        regs.set_reg_b(REG8_MODE,1); // Set mode to motor demo
    }
    while (!kbhit()){
        regs.set_reg_b(0, 40*(sin(2*M_PI*time_d())));  // sine wave at 1 Hz (takes rad)
    }
    
    regs.set_reg_b(REG8_MODE,0); // Set mode to idle
    regs.close();
    return 0;
}