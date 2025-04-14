#include <iostream>
#include "remregs.h"
#include "robot.h"
#include "utils.h"
#include "regdefs.h"
#include "../../robot/ex5/modes.h"
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
    regs.set_reg_b(REG8_MODE, IMODE_LED_DEMO); // Set mode to motor demo
    ext_key();
    regs.set_reg_b(REG8_MODE, IMODE_SINMOTOR_DEMO); // Set mode to sin demo
    ext_key();
    regs.set_reg_b(REG8_MODE, IMODE_IDLE); // Set mode to idle
    regs.close();
    return 0;
}