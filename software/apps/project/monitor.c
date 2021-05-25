/*
 * monitor.c
 *
 * Basic abstraction layer implementation for heartbeat monitor 
 */ 
#include "monitor.h"
#include "utils.h"


monitor *the_monitor = NULL ;


/*
 * ----------- Base Analysis Methods ----------
 *
 * NOTE --- There is suspicious casting. Ignore
 */ 
static uint8_t _average_recent_heartbeat_history(monitor *self)
{
    /*
     * TOP
     *
     * Average the last 5 heartbeats (or less if not 
     * enough history is available) 
     */ 

    /*
     * Fetch recent heartbeat history
     */
    uint8_t entries_to_receive[RECENT];
    uint8_t num_recent_entries = RECENT;
    uint8_t num_recent_entries_received = 0;
    rb_get_last_n_entries(
	&(self->heartbeat_history),
	(uint8_t *) &entries_to_receive,
	num_recent_entries,
	(uint8_t *) &num_recent_entries_received	
    );


    /*
     * Calculate an average over the history 
     */ 
    uint8_t average = 
	(uint8_t) __average_bytes(
	    (uint8_t *) &entries_to_receive, 
	    num_recent_entries_received
	);


    return average;
}


static float _rate_recent_heartbeat_history(monitor *self)
{
    /*
     * TOP
     *
     * Average the last 5 heartbeats (or less if not 
     * enough history is available) 
     */ 

    /*
     * Fetch recent heartbeat history
     */
    uint8_t entries_to_receive[RECENT];
    uint8_t num_recent_entries = RECENT;
    uint8_t num_recent_entries_received = 0;
    rb_get_last_n_entries(
	&(self->heartbeat_history),
	(uint8_t *) &entries_to_receive,
	num_recent_entries,
	(uint8_t *) &num_recent_entries_received	
    );


    /*
     * Build the x-values for the rate calculation
     */
    uint8_t x_values[num_recent_entries_received];
    for (uint8_t i = 0 ; i < num_recent_entries_received ; i++) x_values[i] = i;

    
    /*
     * Calculate rate of change of the history 
     */ 
    float rate = 
	__lsr_slope_bytes (
	    (uint8_t *) &x_values, 
	    (uint8_t *) &entries_to_receive, 
	    num_recent_entries_received
	);


    return rate;
}


bool base_is_heartbeat_high(monitor *self)
{
    /*
     * TOP
     *
     * Average the last 5 hearbeats (or all heartbeats if
     * less than 5 have been recorded) from @self's history 
     * and judge if the heartbeat is high
     */ 

    /*
     * Calculate average of recent heartbeat history
     */ 
    uint8_t average = _average_recent_heartbeat_history(self);
    DEBUG_PRINT("average: %u\n", average);


    /*
     * Check if the averaged history is high
     */
    return (average >= HIGH_HEARTBEAT);  
} 
   

bool base_is_heartbeat_low(monitor *self)
{
    /*
     * TOP
     *
     * Average the last 5 hearbeats (or all heartbeats if
     * less than 5 have been recorded) from @self's history 
     * and judge if the heartbeat is low
     */ 

    /*
     * Calculate average of recent heartbeat history
     */ 
    uint8_t average = _average_recent_heartbeat_history(self);
    DEBUG_PRINT("average: %u\n", average);


    /*
     * Check if the averaged history is low
     */
    return (average <= LOW_HEARTBEAT);  
} 


bool base_is_heartbeat_rising_rapidly(monitor *self)
{
    /*
     * Check if the heartbeat is rising rapidly according
     * the last 5 heartbeats (or less if fewer entries 
     * exist) in @self's history.
     */ 

    /*
     * Calculate the rate of change of recent heartbeat history
     */ 
    float rate = _rate_recent_heartbeat_history(self);
    DEBUG_PRINT("rate: %f\n", rate);


    /*
     * Check if the rate is rapidly rising
     */
    return (rate >= RAPID_RISE);  
}
   

bool base_is_heartbeat_falling_rapidly(monitor *self)
{
    /*
     * Check if the heartbeat is falling rapidly according
     * the last 5 heartbeats (or less if fewer entries 
     * exist) in @self's history.
     */ 

    /*
     * Calculate the rate of change of recent heartbeat history
     */ 
    float rate = _rate_recent_heartbeat_history(self);
    DEBUG_PRINT("rate: %f\n", rate);


    /*
     * Check if the rate is rapidly falling 
     */
    return (rate <= RAPID_FALL);  
} 


void base_set_detection_status(monitor *self)
{
    /*
     * Calculate the status of the heartbeat/rate detected
     * according to @self's heartbeat history. Start with
     * detection_status::NORMAL and go from there.
     */ 

    /*
     * Check if there is an abnormal status 
     */ 
    detection_status local_status = NORMAL;
    if (base_is_heartbeat_high(self)) local_status = RATE_HIGH;
    else if (base_is_heartbeat_low(self)) local_status = RATE_LOW;
    else if (base_is_heartbeat_rising_rapidly(self)) local_status = RISING_RAPIDLY;
    else if (base_is_heartbeat_falling_rapidly(self)) local_status = FALLING_RAPIDLY;


    /*
     * Set the detection status for @self 
     */ 
    self->status = local_status;


    return;
}


/*
 * ----------- Base Functionality Methods ----------
 */ 

void base_change_monitoring_mode (
    monitor *self, 
    monitoring_mode new_mode
)
{
    /*
     * Set @self->mode
     */ 
    self->mode = new_mode;
}


void base_print_heartbeat_history(monitor *self)
{
    /*
     * Print out @self->heartbeat history using 
     * the prefix "MONITOR"
     */
    rb_print(
	"MONITOR",
      	&(self->heartbeat_history)
    );


    return;
}


monitor *bootstrap_monitor(
    void (*setup_func)(monitor *),
    app_timer_id_t const *timer
)
{
    /*
     * TOP
     *
     * Bootstrap the monitor setup --- allocate a monitor
     * and set @setup_func into the new monitor. Call the
     * newly set setup method from there
     */ 

    /*
     * Allocate a new monitor
     */ 
    monitor *new_monitor = calloc(1, sizeof(monitor));
    assert(!!new_monitor && "bootstrap_monitor: malloc failed");


    /*
     * Set "new_monitor"->monitor_handler_setup
     */ 
    new_monitor->monitor_handler_setup = setup_func;


    /*
     * Set up the monitor properly using @setup_func
     */
    new_monitor->monitor_handler_setup(new_monitor);


    /*
     * Set up timer functionality
     */ 
    app_timer_init();

    app_timer_create(
	timer, 
	APP_TIMER_MODE_REPEATED,
	new_monitor->heartbeat_timer_handler
    );

    app_timer_start(
	*timer,
	32768, /* 1 second */
	NULL
    );


    return new_monitor;
}


/*
 * ---------- Base Stubs ----------
 */ 
void base_monitor_handler_setup(monitor *self)
{
    assert(false && "base_monitor_handler_setup should not be used!");
    return;
}


void base_monitor_handler_cleanup(monitor *self) 
{
    assert(false && "base_monitor_handler_cleanup should not be used!");
    return;
}


void base_get_new_heartbeat(monitor *self)
{
    assert(false && "base_get_new_heartbeat: should not be used!");
    return;
}


void base_heartbeat_timer_handler(void *state)
{
    assert(false && "base_heartbeat_timer_handler: should not be used!");
    return;
}


void base_monitor_handler_dump_state(monitor *self)
{
    assert(false && "base_monitor_handler_dump_state: should not be used!");
    return;
}


