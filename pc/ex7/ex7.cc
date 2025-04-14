#include <iostream>
#include "remregs.h"
#include "robot.h"
#include "utils.h"
#include "regdefs.h"
#include "../../robot/ex7/modes.h"
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

    float freq, amplitude;

    regs.set_reg_b(3, 123);
    Sleep(1000);
    std::cout << regs.get_reg_b(4) << std::endl;

    // while(!kbhit()) {
    //     // for(uint8_t i = 0; i < N_MOTORS; i++) {
    //     //     cin >> freq >> amplitude;
    //     //     // Set frequency and amplitude for each motor
    //     //     regs.set_reg_b(REG_FREQ(i), ENCODE_PARAM_8(freq, 0, FREQ_MAX));
    //     //     regs.set_reg_b(REG_AMPLITUDE(i), ENCODE_PARAM_8(amplitude, 0, AMPLITUDE_MAX));
    //     // }
    // }

    regs.set_reg_b(REG8_MODE, IMODE_IDLE); // Set mode to idle
    regs.close();
    return 0;
}