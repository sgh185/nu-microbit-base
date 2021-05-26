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
extern uint32_t __sum_bytes (
    uint8_t *values,
    uint8_t num_values
) ;


extern uint32_t __sum_words (
    uint32_t *values,
    uint32_t num_values
) ;


extern float __average_bytes (
    uint8_t *values,
    uint8_t num_values
) ;


extern float __lsr_slope_bytes (
    uint8_t *x_values,
    uint8_t *y_values,
    uint8_t N 
) ;


/*
 * Bitwise Operations 
 */ 
extern bool __get_bit (
    uint8_t octet,
    uint8_t index
) ;

