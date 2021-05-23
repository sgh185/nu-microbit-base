/*
 * sensor.h
 *
 * Abstraction layer for heartbeat monitor 
 */ 

#pragma once

#include "system_headers.h"


/*
 * Declarations, types
 */ 
#define MAX_HISTORY 64
#define HIGH_HEARTBEAT 200
#define LOW_HEARTBEAT 40

enum monitoring_mode {
    
    SHORT_TERM=0,
    LONG_TERM,
    DETECT

};

enum detection_status {

    NORMAL=0,
    RATE_HIGH,
    RATE_LOW,
    RISING_RAPIDLY,
    FALLING_RAPIDLY
    
} ;


typedef struct {

    /*
     * Fields
     */ 
    uint8_t heartbeat_history[MAX_HISTORY] ;

    monitoring_mode mode ;

    detection_status status ;


    /*
     * Analysis methods
     */
    void (*is_heartbeat_high)(void) ; 
    
    void (*is_heartbeat_low)(void) ; 
    
    void (*is_heartbeat_rising_rapidly)(void) ; 
    
    void (*is_heartbeat_falling_rapidly)(void) ; 


    /*
     * Functionality ethods
     */ 
    void (*change_monitoring_mode)(monitoring_mode new_mode) ;

    void (*print_heartbeat_history)(void) ;

    void (*sensor_handler_setup)(void) ;
    
    void (*sensor_handler_cleanup)(void) ;

    void (*heartbeat_timer_handler)(void *state) ;


} monitor ;




