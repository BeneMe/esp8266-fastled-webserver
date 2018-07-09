#ifndef DEFINES_H
#define DEFINES_H

#define DATA_PIN      2
#define LED_TYPE      WS2811
#define COLOR_ORDER   GRB
#define NUM_LEDS      50

#define MILLI_AMPS         2000     // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#define FRAMES_PER_SECOND  120 // here you can control the speed. With the Access Point / Web Server the animations run a bit slower.

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]));

#endif