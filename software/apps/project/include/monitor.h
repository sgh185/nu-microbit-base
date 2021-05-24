/*
 * monitor.h
 *
 * Abstraction layer for heartbeat monitor 
 */ 

#pragma once

#include "system_headers.h"


/*
 * Declarations, types
 */ 
#define HIGH_HEARTBEAT 200
#define LOW_HEARTBEAT 50
#define RAPID_RISE 16.0F
#define RAPID_FALL -16.0F
#define RECENT 5

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


typedef struct monitor {

    /*
     * Fields
     */ 
    ring_buffer heartbeat_history ;

    monitoring_mode mode ;

    detection_status status ;


    /*
     * Analysis methods
     */
    bool (*is_heartbeat_high)(monitor *self) ; 
    
    bool (*is_heartbeat_low)(monitor *self) ; 
    
    bool (*is_heartbeat_rising_rapidly)(monitor *self) ; 
    
    bool (*is_heartbeat_falling_rapidly)(monitor *self) ; 

    void (*set_detection_status)(monitor *self) ;


    /*
     * Functionality methods
     */ 
    void (*get_new_heartbeat)(monitor *self) ;

    void (*change_monitoring_mode)(
	monitor *self, 
	monitoring_mode new_mode
    ) ;

    void (*print_heartbeat_history)(monitor *self) ;

    void (*monitor_handler_setup)(monitor *self) ;
    
    void (*monitor_handler_cleanup)(monitor *self) ;

    void (*heartbeat_timer_handler)(void *state) ;


} monitor ;


/*
 * Global monitor object
 */ 
extern monitor *the_monitor ;


/*
 * ----------- Base Analysis Methods ----------
 *
 * NOTE --- The following methods implemented here are 
 * base methods that will apply to any extension of the 
 * monitor abstraction.
 */ 

bool base_is_heartbeat_high(monitor *self) ;
    
bool base_is_heartbeat_low(monitor *self) ; 

bool base_is_heartbeat_rising_rapidly(monitor *self) ; 
    
bool base_is_heartbeat_falling_rapidly(monitor *self) ; 

void base_set_detection_status(monitor *self) ;


/*
 * ----------- Base Functionality Methods ----------
 *
 * NOTE --- The following methods implemented here are 
 * base methods that will apply to any extension of the 
 * monitor abstraction.
 */ 

void base_change_monitoring_mode (
    monitor *self, 
    monitoring_mode new_mode
) ;

void base_print_heartbeat_history(monitor *self) ;

monitor *bootstrap_monitor(void (*setup_func)(monitor *)) ;


/*
 * ---------- Base Stubs ----------
 *
 * NOTE --- These methods are stubbed, whichever 
 * modules end up implementing a full monitor must
 * implement these methods fully
 */ 
void base_monitor_handler_setup(monitor *self) ;

void base_monitor_handler_cleanup(monitor *self) ;

void base_get_new_heartbeat(monitor *self) ;

void base_heartbeat_timer_handler(void *state) ;





