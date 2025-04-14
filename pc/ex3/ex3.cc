#include <iostream>
#include "remregs.h"
#include "robot.h"
#include "utils.h"

using namespace std;

const uint8_t RADIO_CHANNEL = 201;         ///< robot radio channel
const char* INTERFACE = "COM1";            ///< robot radio interface

// Displays the contents of a multibyte register as a list of bytes (copy-paste from ex2)
void display_multibyte_register(CRemoteRegs& regs, const uint8_t addr)
{
  uint8_t data_buffer[32], len;
  if (regs.get_reg_mb(addr, data_buffer, len)) {
    cout << (int) len << " bytes: ";
    for (unsigned int i(0); i < len; i++) {
      if (i > 0) cout << ", ";
      cout << (int8_t) data_buffer[i];
    }
    cout << endl;
  } else {
    cerr << "Unable to read multibyte register." << endl;
  }
}


int main()
{
  CRemoteRegs regs;

  if (!init_radio_interface(INTERFACE, RADIO_CHANNEL, regs)) {
    return 1;
  }

  // Reboots the head microcontroller to make sure it is always in the same state
  reboot_head(regs);
  
  while(!kbhit()){
    // Register display demo
    display_multibyte_register(regs, 0);
    Sleep(500); // wait 0.5 second
  }


  regs.close();
  return 0;
}
