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

const argv = require('minimist')(process.argv.slice(2));
const path = argv["d"];
console.log(path);

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline')

const device = new SerialPort(path, { 
    baudRate: 38400 
});

const parser = device.pipe(new Readline({ delimiter: '\n' }))
parser.on('data', console.log)

device.on('error', (err) => {
    console.log("Error!");
    console.error(err);
    process.exit(1);
});

