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
#include "nrf_delay.h"
#include "app_timer.h"
#include "microbit_v2.h"

#define MONITOR_TYPES 2
#define SIMULATOR 0
#define SENSOR 1
#define SETUP_NO SIMULATOR

#define AI __attribute__((always_inline)) inline

#define DEBUG_ON 1
#define DEBUG_PRINT if (DEBUG_ON) printf
