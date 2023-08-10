#include <Arduino.h>
#include <algorithm>    // std::any_of

#define N_SENSOR 5                       // Number of used sensors
#define N_SAMPLE 10                      // Number of samples to average
#define HIT_RECOGNITION_DELAY 1000       // Minimal time [ms] delay between two hits

// Fill these with your project specific data
uint8_t Sensor_pins[N_SENSOR]   = {13, 14, 15, 16, 17};      // What pins are your sensors connected to
float threshold[N_SENSOR]       = {140,260,340,260,340};     // What reading indicates a hit

// Timer used to ensure that no duplicate values for one hit get sent
elapsedMillis since_hit         = 0;

//A place to store sensor data
float sensor_data[N_SENSOR]     = {0};

bool data_in_bounds(float* thresh_table, float* data_table, int table_len){
    if (since_hit < HIT_RECOGNITION_DELAY){
        return true;
    }

  for(int i = 0; i< table_len;i++){
    if (data_table[i] > (thresh_table[i])){
        since_hit = 0;
        return false;
    }
  }                                   
  return true;
}


void setup()
{
  Serial.begin(9600);

  for(int i = 0; i < N_SENSOR; i++){
    pinMode(Sensor_pins[i], INPUT);
  }
}

void loop()
{

  //Clear the sensor value buffer
  for(int i = 0; i < N_SENSOR; i++){
    sensor_data[i] = 0;
  }

  //Add values of N Samples
  for(int i = 0; i < N_SAMPLE;i++){
    for(int i = 0; i<N_SENSOR;i++){
    sensor_data[i] += analogRead(Sensor_pins[i]);
    }
  }

  //Get AVG
  for(int i = 0; i<N_SENSOR;i++){
    sensor_data[i] = sensor_data[i]/N_SAMPLE;
  }

  // Check if sensor data is lower than the threshold set for it
  // React accordingly if not
  if(!data_in_bounds(threshold,sensor_data,N_SENSOR)){
    Serial.println("HIT DETECTED");
  }
  
  delayMicroseconds(2000); // small delay
}