/**
 * @file Accelerometer_Example.ino
 *
 * Example sketch for the RACM600 Power Supply module using the 
 * RACM600.h library.
 *
 * This example demonstrates how to initialize the Naviguider compass module,
 * and read accelerometer data over I2C. The example is designed to run on 
 * the SAMD21-Mini and uses `SerialUSB` for serial communication. If you are 
 * using a standard Arduino board, replace all instances of `SerialUSB` 
 * with `Serial`.
 *
 * Marine Applied Research & Exploration (MARE) develops and shares this 
 * code to support the exploration and documentation of deep-water 
 * ecosystems, contributing to their conservation and management. To 
 * sustain our mission and initiatives, please consider donating at 
 * https://maregroup.org/donate.
 *
 * Created by Isaac Assegai Travers for Marine Applied Research & Exploration.
 *
 * This example is distributed under the BSD license. Redistribution must
 * retain this notice and accompanying license text.
 */


#include <Wire.h>
#include "RACM600.h"

RACM600 psu;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    psu.begin();

    psu.enableOutput();
    delay(500);
    Serial.println("Power Output Enabled");
}

void loop() {
    float voltage = psu.readVoltage();
    float current = psu.readCurrent();
    float temperature = psu.readTemperature();

    Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
    Serial.print("Current: "); Serial.print(current); Serial.println(" A");
    Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
    
    delay(1000);
}