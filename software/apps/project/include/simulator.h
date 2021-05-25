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
struct simulator;


#define SEED 42
#define MAX_HEARTBEAT_LINEAR_CHANGE 30
#define MAX_HEARTBEAT 210
#define MIN_HEARTBEAT 40
#define MAX_SIMULATION_INTERVAL 10

#define NUM_SIGNS 2
typedef enum {

    POS=0,
    NEG=1

} sign ;


#define NUM_EXPANSION_MODES 4
typedef enum {

    NONE=0,
    LINEAR,
    QUADRATIC,
    EXPONENTIAL

} expansion_mode  ;

typedef uint8_t (*expander)(struct simulator *);

typedef struct simulator {

    /*
     * Fields, NOTE --- Don't care about struct layout
     */ 
    uint8_t num_seconds_to_simulate_curr_settings ;

    uint8_t num_seconds_simulated ;

    uint8_t last_heartbeat_simulated ;

    uint8_t heartbeat_change_val ; /* Value to change the heartbeat by */

    sign direction ; /* Heartbeat decreasing or increasing */

    expander heartbeat_expander ; /* Modifier of .heartbeat_change_val over simulation */

    expansion_mode mode ;

} simulator ;


/*
 * Global simulator object
 */ 
extern simulator *the_sim ;


/*
 * Global array of expander methods
 */ 
extern expander expanders[NUM_EXPANSION_MODES] ;


/*
 * For simulating exponential and quadratic expansions
 */ 
#define NUM_A_TERMS 3

extern float a_terms[NUM_A_TERMS] ;

extern float curr_a_term ;



/*
 * ---------- Simulation Methods ----------
 */ 
void simulate_new_heartbeat(simulator *sim) ;

void switch_simulation_settings(simulator *sim);

uint8_t simulate_exponential_expansion(struct simulator *self);

uint8_t simulate_quadratic_expansion(struct simulator *self);

uint8_t simulate_linear_expansion(struct simulator *self);

uint8_t simulate_no_expansion(struct simulator *self);


/*
 * ---------- Simulator-Specific Monitor Functionality Methods ----------
 *
 * NOTE --- These methods are needed to extend the monitor
 * and implement a fully-functioning version 
 */ 
void simulator_monitor_handler_setup(monitor *self) ;

void simulator_monitor_handler_cleanup(monitor *self) ;

void simulator_get_new_heartbeat(monitor *self) ;

void simulator_heartbeat_timer_handler(void *state) ;

void simulator_monitor_handler_dump_state(monitor *self) ;

