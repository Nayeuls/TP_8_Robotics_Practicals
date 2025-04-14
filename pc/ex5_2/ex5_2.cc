#include <iostream>
#include "remregs.h"
#include "robot.h"
#include "utils.h"
#include "regdefs.h"
#include "../../robot/ex5_2/modes.h"
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
    regs.set_reg_b(REG8_MODE, IMODE_SINMOTOR_DEMO); // Set mode to sin demo
    float freq = 1.0;
    float amplitude = 40.0;
    while (freq != 0) {
        std::cout << "Enter frequency (0 to stop): ";
        std::cin >> freq;
        if (freq != 0) {
            std::cout << "Enter amplitude: ";
            std::cin >> amplitude;
            regs.set_reg_b(REG8_FREQ, ENCODE_PARAM_8(freq, 0.0, FREQ_MAX));
            regs.set_reg_b(REG8_AMPLITUDE, ENCODE_PARAM_8(amplitude, 0.0, AMPLITUDE_MAX));
        }
    }

    regs.close();
    return 0;
}