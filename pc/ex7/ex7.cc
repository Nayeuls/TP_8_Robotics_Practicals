#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <windows.h>
#include "trkcli.h"
#include "utils.h"
#include "remregs.h"
#include "robot.h"

using namespace std;

const char* TRACKING_PC_NAME = "biorobpc6";   ///< host name of the tracking PC
const uint16_t TRACKING_PORT = 10502;          ///< port number of the tracking PC

const uint8_t RADIO_CHANNEL = 126;         ///< robot radio channel
const char* INTERFACE = "COM3";            ///< robot radio interface

int main()
{
  CTrackingClient trk;
  CRemoteRegs regs;

  
  if (!init_radio_interface(INTERFACE, RADIO_CHANNEL, regs)) {
    return 1;
  }

  // Reboots the head microcontroller to make sure it is always in the same state
  reboot_head(regs);

  // Connects to the tracking server
  if (!trk.connect(TRACKING_PC_NAME, TRACKING_PORT)) {
    return 1;
  }

  std::cout << "Enter frequency: ";
  std::cin >> freq;
  std::cout << "Enter amplitude: ";
  std::cin >> amplitude;
  regs.set_reg_b(REG8_FREQ, ENCODE_PARAM_8(freq, 0.0, FREQ_MAX));
  regs.set_reg_b(REG8_AMPLITUDE, ENCODE_PARAM_8(0, 0.0, AMPLITUDE_MAX));


  std::cout << "Presser une touche pour commencer: ";
  while (!kbhit()) {
  }
  regs.set_reg_b(REG8_AMPLITUDE, ENCODE_PARAM_8(amplitude, 0.0, AMPLITUDE_MAX));

  string file_name = string("Amplitude_") + std::to_string(amplitude) + "_frequency_" + std::to_string(freq)


  
  while (!kbhit()) {
    uint32_t frame_time;
    // Gets the current position
    if (!trk.update(frame_time)) {
      return 1;
    }



    double x, y;
    cout.precision(2);
    
    // Gets the ID of the first spot (the tracking system supports multiple ones)
    int id = trk.get_first_id();
    
    // Reads its coordinates (if (id == -1), then no spot is detected)
    ofstream csv_file("output.csv");
    if csv_file.fail() {
      cout << "Error opening file" << endl;
      return 1;
    }

    if (id != -1 && trk.get_pos(id, x, y)) {
      cout << "(" << fixed << x << ", " << y << ")" << " m      \r";
      csv_file << x << "," << y << endl;
    } else {
      cout << "(not detected)" << '\r';
    }


    
    // Waits 10 ms before getting the info next time (anyway the tracking runs at 15 fps)
    Sleep(10);
  }
  
  // Clears the console input buffer (as kbhit() doesn't)
  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

  regs.close();
  return 0;
}
