Heart Monitor System
=========
To build system:

```
cd ./web && \
    npm install && \
    cd ../ && \
    make flash ;
```


To run the web server: 

```
cd ./web && \
    node app.js -d /dev/tty.figure_out_your_microbit
```     
NOTE --- The front-end functionality will work best if 
the system is in DETECT mode. Simply press Button A on 
the MicroBit twice to switch to DETECT mode (should see
[N|H|L|R|F] displayed on the LED matrix if in DETECT mode).


To view the UI (on any browser):

```
http://localhost:3000
``` 
