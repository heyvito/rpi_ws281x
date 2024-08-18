//
// Created by Vito Sartori on 18/08/24.
//

#include <stdlib.h>
#include <string.h>
#include "wsgo.h"
#include "ws2811.h"

typedef struct wsgo_state {
    ws2811_t leds;
} wsgo_state_t;

int wsgo_init(int leds, int gpio, void **stateOut) {
    ws2811_t strip = {
            .freq = 800000,
            .dmanum = 10,
            .channel = {
                    [0] = {
                            .gpionum = gpio,
                            .invert = 0,
                            .count = leds,
                            .strip_type = WS2811_STRIP_RGB,
                            .brightness = 255,
                            .leds = calloc(leds, sizeof(ws2811_led_t)),
                    },
                    [1] =  {
                        .gpionum = 0,
                        .invert = 0,
                        .count = 0,
                        .brightness = 0,
                    }
            }
    };

    if (strip.channel[0].leds == NULL) {
        return -2;
    }

    wsgo_state_t *state = malloc(sizeof(wsgo_state_t));
    if (state == NULL) {
        free(strip.channel[0].leds);
        return -2;
    }

    memcpy(&state->leds, &strip, sizeof(ws2811_t));

    ws2811_return_t err;
    if ((err = ws2811_init(&state->leds)) != WS2811_SUCCESS) {
        return err;
    }

    *stateOut = (void *)state;
    return 0;
}

int wsgo_set_color(void *rawState, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
    wsgo_state_t *state = (wsgo_state_t *)rawState;
    state->leds.channel[0].brightness = brightness;
    ws2811_led_t color = 0x00;
    color |= r << 8;
    color |= g << 16;
    color |= b << 24;

    for (size_t i = 0; i < (size_t)state->leds.channel[0].count; i++) {
        state->leds.channel[0].leds[i] = color;
    }

    if (ws2811_render(&state->leds) != WS2811_SUCCESS) {
        return 1;
    }

    return 0;
}

int wdgo_deinit(void *rawState) {
    wsgo_state_t *state = (wsgo_state_t *)rawState;
    ws2811_fini(&state->leds);
    free(state);
    return 0;
}