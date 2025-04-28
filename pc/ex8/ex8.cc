#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <windows.h>
#include "trkcli.h"
#include "utils.h"
#include "regdefs.h"
#include "remregs.h"
#include "robot.h"
#include <math.h>
#include <fstream>
#include "../../robot/ex8/constants.h"


using namespace std;

const char* TRACKING_PC_NAME = "biorobpc6";   ///< host name of the tracking PC
const uint16_t TRACKING_PORT = 10502;          ///< port number of the tracking PC

const uint8_t RADIO_CHANNEL = 126;         ///< robot radio channel
const char* INTERFACE = "COM3";            ///< robot radio interface

bool set_reg_b(CRemoteRegs& regs, const uint16_t addr, const uint8_t val){
  for(int i=0; i<10; i++){
    if (regs.set_reg_b(addr, val)) {
      return true;
    }
    Sleep(1);
  }
  std::cerr << "Unable to set register " << addr << " to " << (int) val << endl;
  return false;
}

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

  float freq = 1;
  float amplitude = 0;
  float phase_lag = 1;

  std::cout << "Enter frequency: ";
  std::cin >> freq;
  std::cout << "Enter amplitude: ";
  std::cin >> amplitude;
  std::cout << "Enter phase lag: ";
  std::cin >> phase_lag;
  set_reg_b(regs, REG8_FREQ, ENCODE_PARAM_8(freq, 0.0, FREQ_MAX));
  set_reg_b(regs, REG8_AMPLITUDE, ENCODE_PARAM_8(0, 0.0, AMPLITUDE_MAX));
  set_reg_b(regs, REG8_PHASE_LAG, ENCODE_PARAM_8(phase_lag, 0.5, 1.5));


  std::cout << "Presser une touche pour commencer: ";
  ext_key();
  set_reg_b(regs, REG8_AMPLITUDE, ENCODE_PARAM_8(amplitude, 0.0, AMPLITUDE_MAX));

  string file_name = string("Amplitude_") + std::to_string(amplitude) + "_frequency_" + std::to_string(freq);

  char read_key = '\0';
  float offset = 0;

  while (read_key != 27) { // ESC key) {
    uint32_t frame_time;
    // Gets the current position
    if (!trk.update(frame_time)) {
      return 1;
    }

    if (kbhit()) {
      read_key = ext_key();
      if (read_key == 27) { // ESC key
        break;
      }
      if (read_key == 'a') {
        offset += amplitude / 40;
      }
      if (read_key == 'd') {
        offset -= amplitude / 40;
      }
      if (read_key == 's') {
        offset = 0;
      }
      offset = fmin(fmax(offset, -OFFSET_ABS_MAX), OFFSET_ABS_MAX);
      set_reg_b(regs, REG8_, ENCODE_PARAM_8(amplitude, -OFFSET_ABS_MAX, OFFSET_ABS_MAX));

    }

    double x, y;
    cout.precision(2);
    
    // Gets the ID of the first spot (the tracking system supports multiple ones)
    int id = trk.get_first_id();
    
    // Reads its coordinates (if (id == -1), then no spot is detected)
    std::ofstream csv_file(file_name);
    if (csv_file.fail()) {
      cout << "Error opening file" << endl;
      return 1;
    }

    if (id != -1 && trk.get_pos(id, x, y)) {
      cout << "(" << fixed << x << ", " << y << ")" << " m      \r";
      csv_file << x << "," << y << endl;
      csv_file.flush();
    } else {
      cout << "(not detected)" << '\r';
    }


    
    // Waits 10 ms before getting the info next time (anyway the tracking runs at 15 fps)
    Sleep(10);
  }

  set_reg_b(regs, REG8_AMPLITUDE, ENCODE_PARAM_8(0, 0.0, AMPLITUDE_MAX));
  
  // Clears the console input buffer (as kbhit() doesn't)
  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

  regs.close();
  return 0;
}
