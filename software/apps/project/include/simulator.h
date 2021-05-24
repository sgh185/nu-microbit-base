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
#define MAX_HEARTBEAT_CHANGE 30
#define MIN_HEARTBEAT_CHANGE 0
#define MAX_HEARTBEAT 210
#define MIN_HEARTBEAT 40

enum sign {

    POS=0,
    NEG=1

};


typedef struct {

    /*
     * Fields, NOTE --- Don't care about struct layout
     */ 
    detection_mode mode_to_simulate ;

    uint8_t num_seconds_to_simulate_mode ;

    uint8_t last_heartbeat_simulated ;

    uint8_t heartbeat_change_val ; /* Value to change the heartbeat by */

    sign direction ; /* Heartbeat decreasing or increasing */

    uint8_t (*heartbeat_change_modifier)(simulator *self) /* Modifier of .heartbeat_change_val over simulation */

} simulator ;


/*
 * For simulating exponential and quadratic expansions
 */ 
extern float a_terms[3] ;

extern float curr_a_term ;



/*
 * ---------- Simulation Methods ----------
 */ 
detection_mode select_random_detection_mode_to_simulate(void) ;

uint8_t select_random_seconds_to_simulate(void);

void switch_simulation_settings(simulator *sim);

uint8_t simulate_exponential_expansion(simulator *self);

uint8_t simulate_quadratic_expansion(simulator *self);

uint8_t simulate_linear_expansion(simulator *self);

uint8_t simulate_no_expansion(simulator *self);



/*
 * ---------- Simulator-Specific Monitor Functionality Methods ----------
 *
 * NOTE --- These methods are needed to extend the monitor
 * and implement a fully-functioning version 
 */ 
void simulator_sensor_handler_setup(monitor *self) ;

void simulator_sensor_handler_cleanup(monitor *self) ;

void simulator_heartbeat_timer_handler(void *state) ;

