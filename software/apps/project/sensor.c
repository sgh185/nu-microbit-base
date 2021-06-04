/*
 * sensor.c
 *
 * Implementation of MAX30102 Pulse Oximeter sensor functionality and interface 
 */ 

#include "sensor.h"

NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

static const nrf_twi_mngr_t *i2c_manager = NULL;

sensor *the_sensor = NULL;


/*
 * ---------- Sensor Methods ----------
 */ 
/*
 * Helper function to perform a 1-byte I2C read of a given register
 *
 * i2c_addr - address of the device to read from
 * reg_addr - address of the register within the device to read
 *
 * returns 8-bit read value
 */ 
static uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) 
{
    uint8_t rx_buf = 0;
    nrf_twi_mngr_transfer_t const read_transfer[] = {
	NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
	NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0),
    };

    ret_code_t error_code = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
    APP_ERROR_CHECK(error_code);
    
    return rx_buf;
}


/*
 * Helper function to perform a 1-byte I2C write of a given register
 *
 * i2c_addr - address of the device to write to
 * reg_addr - address of the register within the device to write
 */
#define TRANSFER_SIZE 2
static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) 
{
    uint8_t buffer[TRANSFER_SIZE] = { reg_addr, data } ;
    nrf_twi_mngr_transfer_t const write_transfer[] = {
	NRF_TWI_MNGR_WRITE(i2c_addr, buffer, 2, 0)
    };

    ret_code_t error_code = nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
    APP_ERROR_CHECK(error_code);

    return;
}


/*
 * Helper function to perform a 6-byte I2C read of the MAX30102's
 * FIFO_DATA register 
 *
 * returns 32-bit reconstructed read value 
 */ 
#define FIFO_TRANSFER_SIZE 6
static uint32_t fifo_read(void) 
{
    /*
     * Perform the transfer
     */ 
    uint8_t rx_buf[FIFO_TRANSFER_SIZE] = {0};
    uint8_t fifo_data_reg = MAX30102_REG_FIFO_DATA;
    nrf_twi_mngr_transfer_t const read_transfer[] = {
	NRF_TWI_MNGR_WRITE(MAX30102_ADDR, &fifo_data_reg, 1, NRF_TWI_MNGR_NO_STOP),
	NRF_TWI_MNGR_READ(MAX30102_ADDR, &rx_buf, FIFO_TRANSFER_SIZE, 0),
    };
    
    ret_code_t error_code = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
    APP_ERROR_CHECK(error_code);
  

    /*
     * Reconstruct the read value
     */  
    uint32_t fifo_read = 0; 
    fifo_read |= rx_buf[0] & 0x03;
    fifo_read <<= 8;
    fifo_read |= rx_buf[1];
    fifo_read <<= 8;
    fifo_read |= rx_buf[2];


    return fifo_read;
}


void __sensor_query_callback(void *state)
{
    /*
     * TOP --- Callback for the sensor query timer
     * Performs the following steps:
     * 1) Fetch the next sample (NOTE --- not checking availability, 
     *    relying on the frequency of callbacks to avoid this)
     * 2) Check if there is a new heartbeat --- if so, process
     */ 

    uint32_t fifo_fetch = fifo_read();
    printf("fifo_fetch: %lu\n", fifo_fetch);
    return;
}



#define I2C_SCL_MINE NRF_GPIO_PIN_MAP(0, 26)
#define I2C_SDA_MINE NRF_GPIO_PIN_MAP(1, 0)
#define MAX30102_PART_MAGIC 0x15
#define MAX30102_SAMPLING_RATE 400.0F
void sensor_max30102_init(const nrf_twi_mngr_t *i2c)
{
    /*
     * Initialize I2C peripheral and driver 
     */  
    nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
    i2c_config.scl = I2C_SCL_MINE;
    i2c_config.sda = I2C_SDA_MINE;
    i2c_config.frequency = NRF_TWIM_FREQ_100K;
    i2c_config.interrupt_priority = 0;
    ret_code_t err_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
    APP_ERROR_CHECK(err_code);


    /*
     * Set global TWIM manager
     */
    i2c_manager = i2c;


    /*
     * Reset MAX30102
     */ 
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_MODE_CONF, 0x40);


    /*
     * Enable interrupt 1, unset interrupt 2
     */  
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_INT_ENABLE_1, 0xC0);
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_INT_ENABLE_2, 0);


    /*
     * Reset all pointer registers
     */  
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_FIFO_WR_PTR, 0);
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_OVERFLOW_CTR, 0);
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_FIFO_RD_PTR, 0);


    /*
     * - Configure sampling to represent average of 4 adjancent readings
     * - Skip "Almost Full Interrupt" with 15 samples remaining
     */ 
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_FIFO_CONF, 0x4F);
    
    
    /*
     * Configure MAX30102 to HR Mode, set up LEDs
     */ 
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_MODE_CONF, 0x02);
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_SPO2_CONF, 0x27);
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_LED1_PA, 0x3F);
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_LED2_PA, 0x3F);
    i2c_reg_write(MAX30102_ADDR, MAX30102_REG_PILOT_PA, 0x7F);


    /*
     * Assert part and revision registers
     */ 
    uint8_t ref = i2c_reg_read(MAX30102_ADDR, MAX30102_REG_REF_ID);
    uint8_t part = i2c_reg_read(MAX30102_ADDR, MAX30102_REG_PART_ID);
    
    if (DEBUG_ON) printf("REV_ID: %u\n", ref);
    
    assert(
	true
	&& (part == MAX30102_PART_MAGIC)
	&& "sensor_max30102_init: part register does not read 0x15!"
    );	


    return;
}


void sensor_check_for_new_heartbeat(sensor *sen)
{
    return;
} 


float sensor_perform_running_average_filter(
    float sample,
    sensor *sen
)
{
    /*
     * Adjust values and running sum 
     */ 
    uint8_t idx = sen->ra.curr_index;
    sen->ra.curr_running_sum += sample - sen->ra.values[idx];
    sen->ra.values[idx] = sample;


    /*
     * Increment index (of ring buffer) and count (if necessary)
     */ 
    sen->ra.curr_index = modulo_incr((sen->ra.curr_index), NUM_RUNNING_VALUES); 
    sen->ra.num_values = MIN((sen->ra.num_values + 1), NUM_RUNNING_VALUES);
    

    /*
     * Calculate the average
     */ 
    sen->ra.curr_running_avg = sen->ra.curr_running_sum / ((float) sen->ra.num_values);


    /*
     * Perform the filter
     */ 
    float filtered = sen->ra.curr_running_avg - sample;
    return filtered;
}


/*
 * ---------- Sensor-Specific Monitor Functionality Methods ----------
 */ 
void sensor_monitor_handler_setup(monitor *self)
{
    /*
     * Set up @self's modes 
     */ 
    self->mode = PERIODIC;
    self->status = NORMAL;


    /*
     * Set analysis methods
     */ 
    self->is_heartbeat_high = base_is_heartbeat_high;
    self->is_heartbeat_low = base_is_heartbeat_low;
    self->is_heartbeat_rising_rapidly = base_is_heartbeat_rising_rapidly;
    self->is_heartbeat_falling_rapidly = base_is_heartbeat_falling_rapidly;
    self->set_detection_status = base_set_detection_status;
	

    /*
     * Set functionality methods
     */ 
    self->get_new_heartbeat = sensor_get_new_heartbeat;
    self->display_last_heartbeat = base_display_last_heartbeat;
    self->display_detection_status = base_display_detection_status;
    self->change_monitoring_mode = base_change_monitoring_mode; 
    self->print_heartbeat_history = base_print_heartbeat_history;
    self->monitor_handler_cleanup = sensor_monitor_handler_cleanup; 
    self->heartbeat_timer_handler = sensor_heartbeat_timer_handler;
    self->monitor_handler_dump_state = sensor_monitor_handler_dump_state;


    /*
     * Allocate global sensor object
     */ 
    the_sensor = calloc(1, sizeof(sensor));
    assert(!!the_sensor && "sensor_monitor_handler_setup: malloc failed");


    /*
     * Initialize MAX30102 sensor 
     */ 
    sensor_max30102_init();


    /*
     * Initialize timer to query sensor for ADC counts 
     */ 
    app_timer_create(&sensor_query_timer, APP_TIMER_MODE_REPEATED, __sensor_query_callback);
    app_timer_start(sensor_query_timer, HZ(MAX30102_SAMPLING_RATE), NULL);


    /*
     * Debugging
     */ 
    if (DEBUG_ON) self->monitor_handler_dump_state(self);


    return;
}


void sensor_monitor_handler_cleanup(monitor *self)
{
    /*
     * Deallocate all memory
     */ 
    free(the_sensor);
    free(self);
    return;
}


void sensor_get_new_heartbeat(monitor *self)
{
    /*
     * TOP
     *
     * Fetch the current heart rate from "the_sensor"
     * global object --- this is already taking care
     * of queries and processing in the background,
     * so simply fetch the field
     *
     * Update @self->heartbeat_history with this info
     */ 
    rb_push(
	the_sensor->current_heart_rate,
	&(self->heartbeat_history)
    );


    return;
}


void sensor_heartbeat_timer_handler(void *state)
{
    /*
     * TOP
     *
     * We're at a 1-second interval. Follow these steps:
     * 1) Fetch the new heartbeat, add to history
     * 2) Display the right features and calculate detection
     *    status if necessary for the current monitoring mode
     */ 
    
    /*
     * <Step 1.>
     */ 
    the_monitor->get_new_heartbeat(the_monitor);
    uint8_t last_heartbeat = rb_get_latest(&(the_monitor->heartbeat_history)); 
    SERIAL_PRINT("BEAT %u\n", last_heartbeat); 


    /*
     * <Step 2.>
     */  
    switch (the_monitor->mode)
    {
	case PERIODIC:
	{
	    the_monitor->display_last_heartbeat(the_monitor);
	    break; 
	}
    	case QUERY: { break; }
	case DETECT:
	{
	    the_monitor->set_detection_status(the_monitor);
	    SERIAL_PRINT("RATE %c\n", detection_status_chars[the_monitor->status]);
	    the_monitor->display_detection_status(the_monitor);
	    break; 
	}
    }

   
    /*
     * Debugging
     */  
    if (DEBUG_ON) the_monitor->monitor_handler_dump_state(the_monitor);


    return; 
}


void sensor_monitor_handler_dump_state(monitor *self)
{
    return;
}




