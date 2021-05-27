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
    "H" : "HIGH",
    "L" : "LOW",
    "R" : "Rising Rapidly",
    "F" : "Falling Rapidly"
} ;

const status_action = {
    "N" : "You're good!",
    "H" : "Call 911!",
    "L" : "Call 911!",
    "R" : "Take it easy ...",
    "F" : "Take it easy ..."
} ;


const status_color = {
    "N" : ["green", "success"],
    "H" : ["red", "danger"],
    "L" : ["red", "danger"],
    "R" : ["red", "danger"],
    "F" : ["red", "danger"]
} ;


const monitor_state = {
    "mode" : "0",
    "data" : new CBuffer(history_size),
    "status" : "Normal",
    "action" : "You're good!",
    "action_color" : "green",
    "badge_color" : "success"
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
	const curr_status_no = data_str.substring(5);
	const curr_status = rate_status[curr_status_no];
	const curr_action = status_action[curr_status_no];
	const curr_action_color = status_color[curr_status_no][0];
	const curr_badge_color = status_color[curr_status_no][1];
	monitor_state["status"] = curr_status;
	monitor_state["action"] = curr_action;
	monitor_state["action_color"] = curr_action_color;
	monitor_state["badge_color"] = curr_badge_color;
    }


    /*
     * Debugging
     */
    if (DEBUG)
    {
	console.log("monitor_state[mode]: " + monitor_state["mode"]);
	console.log("monitor_state[data]: " + monitor_state["data"].toArray().toString()); 
	console.log("monitor_state[status]: " + monitor_state["status"]);
	console.log("monitor_state[action]: " + monitor_state["action"]);
	console.log("monitor_state[action_color]: " + monitor_state["action_color"]);
	console.log("monitor_state[badge_color]: " + monitor_state["badge_color"]);
    }

});


device.on('error', (err) => {
    console.log("Error!");
    console.error(err);
    process.exit(1);
});


/*
 * Express setup
 */
const express = require('express');
const ejs = require('ejs');

const app = express();
app.set('view engine', 'ejs');
app.use(express.static("public"));


/*
 * Routes setup
 */
app.get("/*", (req, res) => {
    const raw = monitor_state["data"].toArray();
    res.render("index", { 
	monitor : monitor_state,
	raw_data : raw
    });
});


/*
 * Port setup
 */
let port = process.env.PORT;
if (port == null || port == "") {
    port = 3000;
}

app.listen(port, () => {
    console.log("Server started.");
});
