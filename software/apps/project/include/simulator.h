/*
 * simulator.h
 *
 * Simulate sensor functionality
 */ 

#pragma once

#include "system_headers.h"
#include "monitor.h"


/*
 * Definitions, types
 */ 
#define RAPID_HEARTBEAT_CHANGE 20
#define HIGH_HEARTBEAT 210
#define LOW_HEARTBEAT 30


enum sign {

    POS=0,
    NEG=1

};


typedef struct {

    /*
     * Fields
     */ 
    detection_mode mode_to_simulate ;

    uint8_t num_seconds_to_simulate_mode ;

    uint8_t heartbeat_change_val ; /* Value to change the heartbeat by */

    sign heartbeat_change_direction ; /* Heartbeat decreasing or increasing */

    uint8_t (*heartbeat_change_modifier)(uint8_t curr_heartbeat_change_val) /* Optional modifier of .heartbeat_change_val over simulation */

} simulator ;


/*
 * ---------- Simulation Methods ----------
 */ 
detection_mode select_random_detection_mode_to_simulate(void) ;

uint8_t select_random_seconds_to_simulate(void);

void switch_simulation_settings(simulator *sim);

uint8_t simulate_exponential_backoff(uint8_t val);

uint8_t simulate_quadratic_backoff(uint8_t val);

uint8_t simulate_linear_backoff(uint8_t val);


/*
 * ---------- Simulator-Specific Monitor Functionality Methods ----------
 *
 * NOTE --- These methods are needed to extend the monitor
 * and implement a fully-functioning version 
 */ 
void simulator_sensor_handler_setup(monitor *self) ;

void simulator_sensor_handler_cleanup(monitor *self) ;

void simulator_heartbeat_timer_handler(void *state) ;

