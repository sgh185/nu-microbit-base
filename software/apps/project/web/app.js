/* jshint esversion: 6 */

/*
 * Server for Heartbeat Sensor Project 
 *
 * NOTE --- This is disorganized, there really should
 * be middleware, but who cares
 */

/*
 * Connection to serial output 
 */
const DEBUG = 0;

const argv = require('minimist')(process.argv.slice(2));
const path = argv["d"];
console.log(path);

const CBuffer = require('CBuffer');
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline')

const device = new SerialPort(path, { 
    baudRate: 38400 
});


/*
 * Parsing, Global state
 */ 
const history_size = 16;
const monitor_state = {
    "mode" : "0",
    "data" : new CBuffer(history_size)
} ;

const parser = device.pipe(new Readline({ delimiter: '\n' }));
parser.on('data', (data) => {

    /*
     * Setup
     */ 
    if (DEBUG) console.log("" + data);
    const data_str = "" + data; 
    

    /*
     * Handle mode changes
     */ 
    const prefix = data_str.substring(0,4);
    if (prefix == "MODE")
    {
	/*
	 * Update global mode
	 */ 
	const new_mode = data_str.substring(5, 6);
	monitor_state["mode"] = new_mode; 
    }
    else if (prefix == "DATA")
    {
	/*
	 * Update history
	 */ 
	const new_mode = data_str.substring(5);
	monitor_state["data"].push(Number(new_mode));
    }


    /*
     * Debugging
     */ 
    if (DEBUG) 
    { 
	console.log("monitor_state[mode]: " + monitor_state["mode"]);
	console.log("monitor_state[data]: " + monitor_state["data"].toArray().toString());
    }

});


device.on('error', (err) => {
    console.log("Error!");
    console.error(err);
    process.exit(1);
});
