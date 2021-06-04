/*
 * system_headers.h
 *
 * Basic includes, setup for system
 */ 

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h> 

#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "nrf_twi_mngr.h"
#include "nrf_delay.h"
#include "app_timer.h"
#include "microbit_v2.h"

#define MONITOR_TYPES 2
#define SIMULATOR 0
#define SENSOR 1
#define SETUP_NO SENSOR 

#define AI __attribute__((always_inline)) inline

#define DEBUG_ON 0
#define DEBUG_PRINT if (DEBUG_ON) printf

#define SERIAL_ON 1
#define SERIAL_PRINT if (SERIAL_ON) printf
