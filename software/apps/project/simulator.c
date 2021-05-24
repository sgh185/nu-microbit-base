/*
 * simulator.c
 *
 * Sensor simulator implementation 
 */ 

#include "simulator.h"


float a_terms[3] = { 0.05F, 0.10F, 0.15F } ;

float curr_a_term ;

expander expanders[NUM_EXPANSION_MODES] = {
    simulate_exponential_expansion,
    simulate_quadratic_expansion, 
    simulate_linear_expansion,
    simulate_no_expansion    
} ;


/*
 * ---------- Simulation Methods ----------
 */ 
static uint8_t _calculate_exponential_start_input(simulator *sim)
{
    /*
     * TOP
     *
     * Using @self->last_heartbeat_simulated, calculate
     * the input that would have generated that last 
     * heartbeat as an output. Return this input.
     *
     * The function here looks like y = a * 2^x.
     *
     * x (input) will be = log_2(y / a)
     *
     * No overflows back to uint8_t because of the math
     */ 
    float y = ((float) @self->last_heartbeat_simulated) ;
    float y_div_a = y / curr_a_term ;
    float x = log2f(y_div_a) ;
    return ((uint8_t) x);
}


static uint8_t _calculate_quadratic_start_input(simulator *sim)
{
    /*
     * TOP
     *
     * Using @self->last_heartbeat_simulated, calculate
     * the input that would have generated that last 
     * heartbeat as an output. Return this input.
     *
     * The function here looks like y = ax^2.
     *
     * x (input) will be = sqrt(y / a)
     *
     * No overflows back to uint8_t because of the math
     */ 
    float y = ((float) @self->last_heartbeat_simulated) ;
    float y_div_a = y / curr_a_term ;
    float x = sqrtf(y_div_a) ;
    return ((uint8_t) x);
}


void switch_simulation_settings(simulator *sim)
{
    /*
     * Randomly set the new settings that @sim should
     * simulate. Set the following metrics:
     * - Sign/@self->direction
     * - Expansion mode/@self->heartbeat_expander 
     * - Time/@self->num_seconds_to_simulate_curr_settings
     * - Change val/input/@self->heartbeat_change_val based
     *   on the expansion mode chosen
     */

    uint32_t random;

    /*
     * Select sign
     */
    random = lrand48();
    @self->direction = random % NUM_SIGNS ; 


    /*
     * Select expansion mode and the method to use
     */ 
    random = lrand48();
    expansion_mode mode = (random % NUM_EXPANSION_MODES);
    @self->heartbeat_expander = expanders[mode] ;


    /*
     * Select number of seconds to simulate
     */ 
    random = lrand48();
    @self->num_seconds_to_simulate_curr_settings = random % MAX_SIMULATION_INTERVAL;


    /*
     * Select change modifer/a_term/etc.
     */ 
    random = lrand48();
    if (false
	|| (mode == NONE)
	|| (mode == LINEAR))
    {
	@self->heartbeat_change_val = random % MAX_HEARTBEAT_LINEAR_CHANGE; 
    }
    else if (mode == QUADRATIC)
    {
	curr_a_term = a_terms[(random % NUM_A_TERMS)] ;	
	@self->heartbeat_change_val = _calculate_quadratic_start_input(self);
    }
    else if (mode == EXPONENTIAL)
    {
	curr_a_term = a_terms[(random % NUM_A_TERMS)] ;	
	@self->heartbeat_change_val = _calculate_exponential_start_input(self);
    }
}


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


