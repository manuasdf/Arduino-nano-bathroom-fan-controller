#include <avr/io.h>
// Include library for DHT11
#include "dht.h"

// Define pins
#define DHT11SENSOR_PIN 8
#define LIGHTSENSOR_PIN A6
#define RELAY_PIN 4

// Limits
const int light_on_value_limit = 200;
const int humidity_value_limit = 50;

// Times
const unsigned long minutes_to_wait_when_light_on = 3;
const unsigned long minutes_fan_on = 15;


dht DHT;

void setup() {
  Serial.begin(9600);
  
  pinMode(DHT11SENSOR_PIN, INPUT);
  pinMode(LIGHTSENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  // TURNS THE RELAY OFF:
  digitalWrite(RELAY_PIN, HIGH);
}

boolean check_humidity() {
  DHT.read11(DHT11SENSOR_PIN);
  //Fastest should be once every two seconds.
  delay(2000);
  if (DHT.humidity > humidity_value_limit) {
    return true;
  } else {
    return false;
  }
}

boolean check_light() {
  int light_sensor_value = analogRead(LIGHTSENSOR_PIN);
  if (light_sensor_value > light_on_value_limit) {
    return true;
  } else {
    return false;
  }
}

unsigned long return_minutes(unsigned long minutes) {
  // Minutes * (60 seconds) * (1000 milliseconds)
  return minutes * 60 * 1000;
}

void turn_fan_on () {
  // Turn on
  digitalWrite(RELAY_PIN,LOW);
  // Wait
  delay(return_minutes(minutes_fan_on));
  // Turn off if humidity is low and light is off
  if(check_humidity() == false && check_light() == false){
    digitalWrite(RELAY_PIN,HIGH);
  }
}

void loop() {
//   If humidity over limit, turn fan on for like 15 min
  if (check_humidity() == true) {
    turn_fan_on();
  }
  // If light on, check after three minutes if light is still on
  if (check_light() == true) {
    delay(return_minutes(minutes_to_wait_when_light_on));
    // If light is still on, turn fan on for like 15 min
    if (check_light() == true){
      turn_fan_on();
    }
  }


//  
//  Serial.print("light = ");
//  Serial.print(analogRead(LIGHTSENSOR_PIN));
//  Serial.print("\t dht11 = ");
//  Serial.println(DHT.read11(DHT11SENSOR_PIN));
//  Serial.print("\n");
//
//  delay(200);

}
