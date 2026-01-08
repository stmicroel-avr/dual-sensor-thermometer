#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool has_time;
    bool has_temp1;
    bool has_temp2;
    uint32_t minutes_since;
    uint32_t last_touch_time;
    int16_t temp1_x100;
    int16_t temp2_x100;
} frame_state_t;

extern volatile frame_state_t t_state_frame;