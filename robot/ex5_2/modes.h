#ifndef __MODES_H
#define __MODES_H

#define REG8_FREQ 2
#define REG8_AMPLITUDE 3

/// Idle mode: do nothing
#define IMODE_IDLE          0

/// Sine wave demo
#define IMODE_SINMOTOR_DEMO     1

/// The main loop for mode switching
void main_mode_loop(void);

#endif // __MODES_H
