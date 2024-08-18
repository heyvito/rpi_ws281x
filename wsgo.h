//
// Created by Vito Sartori on 18/08/24.
//

#ifndef RPI_WS281X_WSGO_H
#define RPI_WS281X_WSGO_H

#include <stdint.h>

int wsgo_init(int leds, int gpio, void **stateOut);
int wsgo_set_color(void *state, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
int wdgo_deinit(void *state);

#endif //RPI_WS281X_WSGO_H
