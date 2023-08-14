# piezo_hit_detector
A simple HIT detector built with piezoelectic discs. <br>
I'm using it to make my robot understand it was shot with a NERF blaster. <br>
More about the project on [hackaday](https://hackaday.io/project/192309-hit-detector)
![gif showing the usecase](https://i.imgur.com/qPffkhK.gif)

Comes in two different versions: <br>
single hit detector   -> triggers whenever any of the sensors returns value over threshold.  <br>
multiple hit detector -> same trigger condition, but this one can inform about multiple sensors registering value over threshold.

# How to use
## Prepare piezoelectic sensors 
This "system" can work with an arbitrary number of sensors. Chose as many as you'd like. Just remember that You are bound by the number of analog pins on your microcontroller.
### Wiring diagram for a simple piezoelectric sensor:
![piezo_sensor_wiring](https://i.imgur.com/PtlxOdU.png)

## Test the sensors
Using double-sided tape stick the sensors to your testing surface. <br>
Connect sensor GND wire to GND pin on your board and Signal wire to an analog input pin. <br>
Using eg. ![Analog Read Serial](https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial) display sensors reading. <br>
Hit your testing surface a few times and look at the readings. <br>
![readings](https://i.imgur.com/tuaofQa.png) <br>
Assume your threshold value for this sensor above the noise level and remember it for the next step. <br>
Do the same thing for all the other sensors.  <br>

## Change parameters
Fill the defines with values corresponding to your project setup. <br>
Choose pins for all your sensors and place them in Sensor_pins table. <br>
Fill threshold table with values gathered in previous step. <br>
```cpp
#define N_SENSOR 5                       // Number of used sensors
#define N_SAMPLE 10                      // Number of samples to average
#define HIT_RECOGNITION_DELAY 1000       // Minimal time [ms] delay between two hits
#define MULTIHIT_TIMER 500               // Maximum time for detecting a hit with multiple sensors

uint8_t Sensor_pins[N_SENSOR]   = {13, 14, 15, 16, 17};     // What pins are your sensors connected to
float threshold[N_SENSOR]       = {1023,512,512,1023,1023};     // What reading indicates a hit
```
## Upload the code to your board and test it.
With everything connected, the only thing left to do is to test it. <br>
With the threshold set up correctly you can expect to see the system reacting to hits by sending data through Serial.  <br>

```
HIT!: [Hit sensor ID] 
```
