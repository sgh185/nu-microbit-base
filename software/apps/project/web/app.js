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
const DEBUG = 1;

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

const rate_status = {
    "N" : "Normal",
    "H" : "**Rate HIGH**",
    "L" : "**Rate LOW**",
    "R" : "**Rising Rapidly**",
    "F" : "**Falling Rapidly**"
} ;

const monitor_state = {
    "mode" : "0",
    "data" : new CBuffer(history_size),
    "status" : "Normal"
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
    else if (prefix == "BEAT")
    {
	/*
	 * Update history
	 */ 
	const new_mode = data_str.substring(5);
	monitor_state["data"].push(Number(new_mode));
    }
    else if (prefix == "RATE")
    {
	/*
	 * Update status based on rate evaluation
	 */ 
	const curr_status = rate_status[data_str.substring(5)];
	monitor_state["status"] = curr_status;
    }


    /*
     * Debugging
     */ 
    if (DEBUG) 
    { 
	console.log("monitor_state[mode]: " + monitor_state["mode"]);
	console.log("monitor_state[data]: " + monitor_state["data"].toArray().toString());
	console.log("monitor_state[status]: " + monitor_state["status"]);
    }

});


device.on('error', (err) => {
    console.log("Error!");
    console.error(err);
    process.exit(1);
});
