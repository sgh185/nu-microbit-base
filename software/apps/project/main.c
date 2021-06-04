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
#if 0
    while (1) {
	printf("MODE 0\n");
	printf("DATA 108\n");
	printf("RATE F\n");

	nrf_delay_ms(1000);
    }
#endif
    the_monitor = 
	bootstrap_monitor(
	    monitor_setups[SETUP_NO],
	    &monitor_timer
	);

    while (1) nrf_delay_ms(1000);
}

