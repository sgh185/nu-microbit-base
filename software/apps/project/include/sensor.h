/*
 * sensor.h
 *
 * Sensor functionality for the MAX30102 Pulse Oximeter
 */ 

#pragma once

#include "system_headers.h"
#include "monitor.h"


/*
 * Definitions, types
 */ 
static const uint8_t MAX30102_ADDR = 0x57;

typedef enum {
    MAX30102_REG_INT_STATUS_1 = 0x00,
    MAX30102_REG_INT_STATUS_2 = 0x01,
    MAX30102_REG_INT_ENABLE_1 = 0x02,
    MAX30102_REG_INT_ENABLE_2 = 0x03,
    MAX30102_REG_FIFO_WR_PTR = 0x04,
    MAX30102_REG_OVERFLOW_CTR = 0x05,
    MAX30102_REG_FIFO_RD_PTR = 0x06,
    MAX30102_REG_FIFO_DATA = 0x07,
    MAX30102_REG_FIFO_CONF = 0x08,
    MAX30102_REG_MODE_CONF = 0x09,
    MAX30102_REG_SPO2_CONF = 0X0A,
    MAX30102_REG_LED1_PA = 0x0C,
    MAX30102_REG_LED2_PA = 0x0D,
    MAX30102_REG_PILOT_PA = 0x10,
    MAX30102_REG_MULTILED_1 = 0x11,
    MAX30102_REG_MULTILED_2 = 0x12,
    MAX30102_REG_TEMP_INTR = 0x1F,
    MAX30102_REG_TEMP_FRAC = 0x20,
    MAX30102_REG_TEMP_CONF = 0x21,
    MAX30102_REG_PROX_INT_TH = 0x30,
    MAX30102_REG_REF_ID = 0xFE,
    MAX30102_REG_PART_ID = 0xFF
} max30102_regs_t ;


#define NUM_SIGNS 2
typedef enum {

    POS=0,
    NEG=1

} sign ;


#define DEPRECATED 1
#ifndef DEPRECATED
#define DC_FILTER_ALPHA 0.95F
typedef struct {
    
    /*
     * State for the following system:
     *
     * w(t) = x(t) + α·w(t-1)
     * y(t) = w(t) - w(t-1)
     */ 

    float alpha;
    float w;

} dc_filter ;
#endif


#define NUM_RUNNING_VALUES 16
typedef struct {
    
    /*
     * State to track running average
     */ 
    float values[NUM_RUNNING_VALUES];
    float curr_running_sum;
    float curr_running_avg;
    uint8_t num_values;
    uint8_t curr_index;

} running_average ;  


typedef struct {

    /*
     * Fields, NOTE --- Don't care about struct layout
     */ 
    uint32_t num_queries_since_last_heartbeat;

    uint8_t current_heart_rate;

    running_average ra;
    
} sensor ;


/*
 * Global sensor object
 */ 
extern sensor *the_sensor ;


/*
 * ---------- Sensor Methods ----------
 */ 
void sensor_max30102_init(const nrf_twi_mngr_t *i2c);

bool sensor_check_for_new_heartbeat(sensor *sen);

float sensor_perform_running_average_filter(
    float sample,
    sensor *sen
);



/*
 * ---------- Sensor-Specific Monitor Functionality Methods ----------
 *
 * NOTE --- These methods are needed to extend the monitor
 * and implement a fully-functioning version 
 */ 
void sensor_monitor_handler_setup(monitor *self) ;

void sensor_monitor_handler_cleanup(monitor *self) ;

void sensor_get_new_heartbeat(monitor *self) ;

void sensor_heartbeat_timer_handler(void *state) ;

void sensor_monitor_handler_dump_state(monitor *self) ;

