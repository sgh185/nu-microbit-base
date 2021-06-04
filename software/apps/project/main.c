/*
 * main.c 
 *
 * Driver for heart monitor system
 */ 

#include "monitor.h"
#include "simulator.h"
#include "sensor.h"
#include "utils.h"

/*
 * Setup functions
 */ 
typedef void (*monitor_setup)(monitor *);

monitor_setup monitor_setups[MONITOR_TYPES] = {
    simulator_monitor_handler_setup,
    sensor_monitor_handler_setup 
};


/*
 * Global timer for monitor
 */ 
APP_TIMER_DEF(monitor_timer);


int main(void) 
{
    /*
     * Setup board and monitor
     */
    printf("Board started!\n");
    
    the_monitor = 
	bootstrap_monitor(
	    monitor_setups[SETUP_NO],
	    &monitor_timer
	);

    while (1) nrf_delay_ms(1000);
}

