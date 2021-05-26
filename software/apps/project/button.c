/*
 * button.c
 *
 * Implementation of button functionality and interrupt handlers
 */ 

#include "button.h"
#include "monitor.h"


static void _button_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) 
{
    /*
     * Generates functionality for the global monitor. 
     * - BUTTON 1 generates a toggle through monitoring modes 
     * - BUTTON 2 generates a heartbeat reading display on 
     *   demand (ONLY available with QUERY mode)
     */ 
    switch(pin) 
    {
	case BUTTON1: 
	{
	    /*
	     * Toggle modes
	     */
	    __disable_irq();
	    the_monitor->change_monitoring_mode(the_monitor);
	    __enable_irq();
	    break;
	}
	case BUTTON2: 
	{
	    /*
	     * Display the last heartbeat on demand 
	     */
	    if (the_monitor->mode == QUERY) the_monitor->display_last_heartbeat(the_monitor)
	    break;
	}
    }


    return;
}


static void _gpio_in_with_interrupt(
    nrfx_gpiote_pin_t pin, 
    nrf_gpio_pin_pull_t pull, 
    nrfx_gpiote_evt_handler_t handler
) 
{
    /*
     * Configure I/O pin
     * HITOLO: Sense high-to-low transition
     * Alternatives: LOTOHI or TOGGLE (for either)
     */
    nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = pull;
    ret_code_t err_code = nrfx_gpiote_in_init(pin, &in_config, handler);
    APP_ERROR_CHECK(err_code);


    /*
     * Enable the interrupts 
     */ 
    nrfx_gpiote_in_event_enable(pin, true);


    return;
}


void set_up_buttons(void) {
    /*
     * Initialize GPIO task/event (interrupts)
     */ 
    ret_code_t err_code = nrfx_gpiote_init();
    APP_ERROR_CHECK(err_code);

    gpio_in_with_interrupt(BUTTON1, NRF_GPIO_PIN_PULLUP, _button_handler);
    gpio_in_with_interrupt(BUTTON2, NRF_GPIO_PIN_PULLUP, _button_handler);
    gpio_in_with_interrupt(BUTTON3, NRF_GPIO_PIN_PULLUP, _button_handler);
    gpio_in_with_interrupt(BUTTON4, NRF_GPIO_PIN_PULLUP, _button_handler);


    return;
}

