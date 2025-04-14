#ifndef __MODES_H
#define __MODES_H

#define REG8_FREQ 2
#define REG8_AMPLITUDE 3
#define FREQ_MAX 2.0
#define AMPLITUDE_MAX 60.0

#define MOTOR_ADDRESSES {21, 72} // Addresses of the motors
#define N_MOTORS sizeof(MOTOR_ADDRESSES) / sizeof(MOTOR_ADDRESSES[0])

#define REG_FREQ ( I ) (2 * I + 1)
#define REG_AMPLITUDE ( I ) (2 * I + 2)

/// Idle mode: do nothing
#define IMODE_IDLE          0

/// Sine wave demo
#define IMODE_SINMOTOR_DEMO     1

/// The main loop for mode switching
void main_mode_loop(void);

#endif // __MODES_H
