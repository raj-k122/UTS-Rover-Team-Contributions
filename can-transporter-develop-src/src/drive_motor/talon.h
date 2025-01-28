#ifndef TALON_H
#define TALON_H

#define MIN_SPEED 60

#define TALON 	0x02040000
#define CONTROL_3	0x00040080
#define STATUS_01	0x00041400
#define STATUS_02	0x00041440
#define STATUS_03	0x00041480
#define STATUS_04	0x000414C0
#define STATUS_13	0x00041700
#define PARAM_SET	0x041880

#define FLOAT_TO_FXP_10_22 (float)0x400000

#define KP	310
#define KI	311
#define KD	312
#define KF	313
#define StickyFaults	390
#define modeNoAction		0x00
#define modePercentOutput	0x01
#define modeServoPosition	0x02
#define modeMotionProfile	0x03

#endif