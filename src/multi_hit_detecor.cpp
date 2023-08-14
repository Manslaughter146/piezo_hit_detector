#include <Arduino.h>
#include <algorithm>    // std::any_of

#define N_SENSOR 5                       // Number of used sensors
#define N_SAMPLE 10                      // Number of samples to average
#define HIT_RECOGNITION_DELAY 1000       // Minimal time [ms] delay between two hits
#define MULTIHIT_TIMER 500               // Maximum time for detecting a hit with multiple sensors

// Fill these with your project specific data
uint8_t Sensor_pins[N_SENSOR]   = {13, 14, 15, 16, 17};     // What pins are your sensors connected to
float threshold[N_SENSOR]       = {1023,512,512,1023,1023};     // What reading indicates a hit

// Timer used to ensure that no duplicate values for one hit get sent
elapsedMillis since_hit         = 0;

//Timer for multi-sensor detection
elapsedMillis since_first_hit   = 0;

//A place to store sensor data
float sensor_data[N_SENSOR]     = {0};
bool sensor_hit[N_SENSOR]       = {0};

void clear_hits(){
  for (int i = 0; i< N_SENSOR; i++){
    sensor_hit[i] = false;
  }
}

bool data_in_bounds(float* thresh_table, float* data_table, int table_len){
  
  // If a hit happenned in the last [HIT_RECOGNITION_DELAY] ms         AND
  // If the time from first sensor hit is greater than [MULTIHIT_TIMER] ms

  if (since_hit < HIT_RECOGNITION_DELAY && since_first_hit > MULTIHIT_TIMER){
    
    //If any sensor registered a hit <- should always be true in this case
    if(std::any_of(sensor_hit, sensor_hit + N_SENSOR,[](bool val){ return val;})) { 
      
      //react to getting hit
      char temp[N_SENSOR] = {0};
      for(int i = 0; i<N_SENSOR; i++){
        if(sensor_hit[i]){
          temp[i] = 48+i;                     //show sensor ID
        }
        else{
          temp[i] = ' ';
        }
        
      }
      Serial.print("HIT: ");
      Serial.println(temp);

      clear_hits();
      return false;                          //Inform that there was a hit in the last [HIT_RECOGNITION_DELAY] ms
      }
    }

  for(int i = 0; i< table_len;i++){
    //If there was a hit before check if second sensor registered it
    if (std::any_of(sensor_hit, sensor_hit + N_SENSOR,[](bool val){ return val;})){
       if (!sensor_hit[i] && data_table[i] > (thresh_table[i])){
        sensor_hit[i] = true;

        Serial.println(" Multiple hits, different times");
      }
    }
    //if there was no hits before - look for hits 
    else{
      if (data_table[i] > (thresh_table[i])){
        sensor_hit[i] = true;
        since_hit = 0;
        since_first_hit = 0;
      }
    }
  }
                                            //Inform that there was no hits in the last [HIT_RECOGNITION_DELAY] ms
  return true;                              //Even if there was, but we are still waiting to see if other sensors noticed the same hit
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
  data_in_bounds(threshold,sensor_data,N_SENSOR);
  
  delayMicroseconds(2000); // small delay
}
