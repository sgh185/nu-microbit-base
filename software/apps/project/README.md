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


To view the UI (on any browser):

```
http://localhost:3000
``` 
