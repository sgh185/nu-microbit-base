/*
 * utils.h
 *
 * Utilities for heartbeat/rate sensor system
 */ 

#pragma once

#include "system_headers.h"


/*
 * Calculation Methods
 */ 
AI uint32_t __sum (
    uint8_t *values,
    uint8_t num_values
) ;


AI float __average (
    uint8_t *values,
    uint8_t num_values
) ;


AI float __lsr_slope (
    uint8_t *x_values,
    uint8_t *y_values,
    uint8_t N 
) ;

