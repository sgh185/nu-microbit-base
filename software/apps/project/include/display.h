/*
 * display.h
 *
 * Utilities to display characters onto MicroBit's LED matrix 
 */ 

#pragma once

#include "system_headers.h"

/*
 * External/"public display handlers 
 */ 
void set_up_new_display (
    uint8_t val, 
    uint8_t len,
    bool convert_to_ascii,
    bool clear_after_display
) ;

void set_up_leds(void) ;


