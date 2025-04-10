#include <iostream>
#include "remregs.h"
#include "robot.h"

using namespace std;

const uint8_t RADIO_CHANNEL = 201;         ///< robot radio channel
const char* INTERFACE = "COM1";            ///< robot radio interface

// Displays the contents of a multibyte register as a list of bytes
void display_multibyte_register(CRemoteRegs& regs, const uint8_t addr)
{
  uint8_t data_buffer[32], len;
  if (regs.get_reg_mb(addr, data_buffer, len)) {
    cout << (int) len << " bytes: ";
    for (unsigned int i(0); i < len; i++) {
      if (i > 0) cout << ", ";
      cout << (int) data_buffer[i];
    }
    cout << endl;
  } else {
    cerr << "Unable to read multibyte register." << endl;
  }
}

void read_registers(CRemoteRegs& regs)
{
  cout << "get_reg_b(6)  = " << (int) regs.get_reg_b(6) << endl; //print 0
  cout << "get_reg_b(21) = " << (int) regs.get_reg_b(21) << endl; //print 0x42
  cout << "get_reg_b(21) = " << (int) regs.get_reg_b(21) << endl; //print 0x42
  cout << "get_reg_b(6)  = " << (int) regs.get_reg_b(6) << endl; //print 2
  cout << "get_reg_b(6)  = " << (int) regs.get_reg_b(6) << endl; //print 0
  
  cout << "get_reg_mb(2) = ";
  display_multibyte_register(regs, 2); // print 0 bytes

  uint8_t buffer[8];
  for (int i(0); i < 8; i++) {
    buffer[i] = 100 + i;
  }
  regs.set_reg_mb(2, buffer, sizeof(buffer)); // send 8 bytes

  cout << "get_reg_mb(2) = ";
  display_multibyte_register(regs, 2); // print 8 bytes : 104, 105, 106, 107, 108, 109, 110, 111
  
  regs.set_reg_b(2, 11); // set mb_buffer[0] = 11
  regs.set_reg_b(3, 22); // set mb_buffer[1] = 22
  
  cout << "get_reg_mb(2) = ";
  display_multibyte_register(regs, 2); // print 8 bytes : 11, 22, 106, 107, 108, 109, 110, 111
  
  regs.set_reg_w(7, 2121); // set datavar = 3*0 + 2121
  cout << "get_reg_dw(2) = " << regs.get_reg_dw(2) << endl; // print 2121
  regs.set_reg_w(7, 1765); // set datavar = 3*2121 + 1765 = 8128
  cout << "get_reg_dw(2) = " << regs.get_reg_dw(2) << endl; // print 8128
}

int main()
{
  CRemoteRegs regs;

  if (!init_radio_interface(INTERFACE, RADIO_CHANNEL, regs)) {
    return 1;
  }

  // Reboots the head microcontroller to make sure it is always in the same state
  reboot_head(regs);
  
  // Register display demo
  read_registers(regs);
  
  regs.close();
  return 0;
}
