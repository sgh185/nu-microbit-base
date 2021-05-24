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
extern uint32_t __sum (
    uint32_t *values,
    uint32_t num_values
) ;


extern float __average (
    uint32_t *values,
    uint32_t num_values
) ;


extern float __lsr_slope (
    uint32_t *x_values,
    uint32_t *y_values,
    uint32_t N 
) ;

