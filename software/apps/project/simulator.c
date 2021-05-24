/*
 * simulator.c
 *
 * Sensor simulator implementation 
 */ 

#include "simulator.h"


float a_terms[3] = { 0.05F, 0.10F, 0.15F } ;

float curr_a_term ;


/*
 * ---------- Simulation Methods ----------
 */ 
detection_mode select_random_detection_mode_to_simulate(void) ;

uint8_t select_random_seconds_to_simulate(void);

uint8_t switch_simulation_settings(simulator *sim);

uint8_t simulate_exponential_expansion(simulator *self);
{
    /*
     * TOP
     *
     * Simulate a new heartbeat by calculating along an 
     * exponential equation. This method makes different 
     * assumptions than linear expansion, as follows:
     *
     * - @self->heartbeat_change_val represents the next input 
     *   value to calculate with.
     * - The global "curr_a_term" is the a term used in the 
     *   function a * 2^x
     * - The function used here is as follows:
     *   curr_a_term * (2 ^ heartbeat_change_val)
     *
     * @self->heartbeat_change_val DOES change. It increments
     * based on the direction
     * @self->direction does NOT change. 
     *
     * Bound the expansion based on the maximum/minimum heartbeat 
     *
     * NOTE --- Very suspicious casting
     */ 

    /*
     * Calculate the expansion
     */ 
    uint8_t input = self->heartbeat_change_val ;
    bool change_pos = (self->direction == POS) ;
 
    uint32_t expansion = (uint32_t) ldexp(curr_a_term, input); /* a * 2^x */  

    uint32_t new_heartbeat = 
	(change_pos) ?
	(MIN(expansion, ((uint32_t) MAX_HEARTBEAT))) :
	(MAX(expansion, ((uint32_t) MIN_HEARTBEAT))) ;	


    /*
     * Increment @self->heartbeat_change_val to represent
     * incrementing the input value by @self->direction, if 
     * necessary
     *
     * No risk of overflowing b/c of the math
     */ 
    if (new_heartbeat == expansion)
    {
	uint8_t new_input = 
	    (change_pos) ? 
	    (input + 1) :
	    (input - 1) ;

	self->heartbeat_change_val = new_input ;
    }

		
    return ((uint8_t) new_heartbeat);
}


uint8_t simulate_quadratic_expansion(simulator *self);
{
    /*
     * TOP
     *
     * Simulate a new heartbeat by calculating along a 
     * quadratic equation. This method makes different 
     * assumptions than linear expansion, as follows:
     *
     * - @self->heartbeat_change_val represents the next input 
     *   value to calculate with.
     * - The global "curr_a_term" is the a term used in the 
     *   function ax^2
     * - The function used here is as follows:
     *   curr_a_term * (heartbeat_change_val ^ 2)
     *
     * @self->heartbeat_change_val DOES change. It increments
     * based on the direction
     * @self->direction does NOT change. 
     *
     * Bound the expansion based on the maximum/minimum heartbeat 
     *
     * NOTE --- Very suspicious casting
     */ 

    /*
     * Calculate the expansion
     */ 
    uint8_t input = self->heartbeat_change_val ;
    bool change_pos = (self->direction == POS) ;
 
    uint32_t sq_term = ((uint32_t) input) * ((uint32_t) input);
    uint32_t expansion = (uint32_t) (curr_a_term * ((float) sq_term)); 

    uint32_t new_heartbeat = 
	(change_pos) ?
	(MIN(expansion, ((uint32_t) MAX_HEARTBEAT))) :
	(MAX(expansion, ((uint32_t) MIN_HEARTBEAT))) ;	


    /*
     * Increment @self->heartbeat_change_val to represent
     * incrementing the input value by @self->direction, if 
     * necessary
     *
     * No risk of overflowing b/c of the math
     */ 
    if (new_heartbeat == expansion)
    {
	uint8_t new_input = 
	    (change_pos) ? 
	    (input + 1) :
	    (input - 1) ;

	self->heartbeat_change_val = new_input ;
    }

		
    return ((uint8_t) new_heartbeat);
}


uint8_t simulate_linear_expansion(simulator *self);
{
    /*
     * TOP
     *
     * Simulate a new heartbeat by calculating the following:
     * last_heartbeat_simulated += [direction heartbeat_change_val] 
     *
     * @self->heartbeat_change_val does NOT change
     * @self->direction does NOT change
     *
     * Bound the expansion based on the maximum/minimum heartbeat 
     */

    /*
     * Simply calculate the expansion
     */  
    uint8_t last = self->last_heartbeat_simulated;
    uint8_t change = self->heartbeat_change_val;
    bool change_pos = (self->direction == POS) ;

    uint8_t expansion = 
	(change_pos) ?
	(last + change) :
	(last - change) ;
	
    uint8_t new_heartbeat = 
	(change_pos) ?
	(MIN(expansion, MAX_HEARTBEAT)) :
	(MAX(expansion, MIN_HEARTBEAT)) ;	

		
    return new_heartbeat;
}


uint8_t simulate_no_expansion(simulator *self) 
{
    /*
     * Simply fetch @self->last_heartbeat_simulated
     */ 
    return self->last_heartbeat_simulated ; 
}


