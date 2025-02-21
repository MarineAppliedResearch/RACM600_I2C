/**
 *   @file RACM600.c
 *
 *  Library for interfacing with the Recom RACM600-SL Power Supply.
 *
 *  Marine Applied Research & Exploration (MARE) develops and shares this 
 *  code to support the exploration and documentation of deep-water 
 *  ecosystems, contributing to their conservation and management. To 
 *  sustain our mission and initiatives, please consider donating at 
 *  https://maregroup.org/donate.
 *
 *  Created by Isaac Assegai Travers for Marine Applied Research & Exploration.
 *
 *  This library is distributed under the BSD license. Redistribution must
 *  retain this notice and accompanying license text.
 */

#include "RACM600.h"

RACM600::RACM600(uint8_t i2c_address) {
    _address = i2c_address;
}

void RACM600::begin() {
    Wire.begin();
}

// Generic Read Function
uint16_t RACM600::readCommand(uint8_t cmd) {
    Wire.beginTransmission(_address);
    Wire.write(cmd);
    Wire.endTransmission(false);
    
    Wire.requestFrom(_address, (uint8_t)2);
    if (Wire.available() >= 2) {
        uint8_t low = Wire.read();
        uint8_t high = Wire.read();
        return (high << 8) | low;
    }
    return 0;
}

// Generic Write Function
void RACM600::writeCommand(uint8_t cmd, uint16_t value) {
    Wire.beginTransmission(_address);
    Wire.write(cmd);
    Wire.write(lowByte(value));
    Wire.write(highByte(value));
    Wire.endTransmission();
}

// Enable Power Output
void RACM600::enableOutput() {
    Wire.beginTransmission(_address);
    Wire.write(RACM600_OPERATION);  // OPERATION Command
    Wire.write(0x80);  // Bit 7: ON
    Wire.endTransmission();
}

// Disable Power Output
void RACM600::disableOutput() {
    Wire.beginTransmission(_address);
    Wire.write(RACM600_OPERATION);  // OPERATION Command
    Wire.write(0x00);  // Bit 7: OFF
    Wire.endTransmission();
}

// Clear Faults
void RACM600::clearFaults() {
    Wire.beginTransmission(_address);
    Wire.write(RACM600_CLEAR_FAULTS); // CLEAR_FAULTS Command
    Wire.endTransmission();
}

// Read Output Voltage
float RACM600::readVoltage() {
    uint16_t raw = readCommand(RACM600_READ_VOUT);  // READ_VOUT Command
    return raw * 0.01;  // Scale factor for Volts
}

// Read Output Current
float RACM600::readCurrent() {
    uint16_t raw = readCommand(RACM600_READ_IOUT);  // READ_IOUT Command
    return raw * 0.01;  // Scale factor for Amperes
}

// Read Temperature of the air around the power supply
float RACM600::readAmbientTemperature() {
    uint16_t raw = readCommand(RACM600_READ_TEMPERATURE_1);  // READ_TEMPERATURE_1 Command
    return raw;  // Already in °C
}

// Read Temperature of the power factor correction circuit
float RACM600::readACINPUTTemperature() {
    uint16_t raw = readCommand(RACM600_READ_TEMPERATURE_2);  // READ_TEMPERATURE_2 Command
    return raw;  // Already in °C
}

// Read Temperature of the LLC resonant converter stage
float RACM600::readDCOUTPUTTemperature(RACM600_READ_TEMPERATURE_3) {
    uint16_t raw = readCommand(0x8F);  // READ_TEMPERATURE_3 Command
    return raw;  // Already in °C
}

// Read basic faults from the fault register, and calls readDetailedFault as needed
uint16_t RACM600::readFaults() {
    uint16_t status = readCommand(RACM600_STATUS_WORD);
    
    Serial.print("Fault Status: 0x");
    Serial.println(status, HEX);

    if (status == 0) {
        Serial.println("No faults detected.");
        return status;
    }

    // Check each fault and print messages
    if (status & 0x0080) Serial.println("FAULT: Device Busy");
    if (status & 0x0040) Serial.println("FAULT: Power Output Off");
    if (status & 0x0020) {
        Serial.println("FAULT: Output Overvoltage");
        readDetailedFault(RACM600_STATUS_VOUT, "Output Voltage Fault");
    }
    if (status & 0x0010) {
        Serial.println("FAULT: Output Overcurrent");
        readDetailedFault(RACM600_STATUS_IOUT, "Output Current Fault");
    }
    if (status & 0x0008) {
        Serial.println("FAULT: Input Undervoltage");
        readDetailedFault(RACM600_STATUS_INPUT, "Input Fault");
    }
    if (status & 0x0004) {
        Serial.println("FAULT: Temperature Fault");
        readDetailedFault(RACM600_STATUS_TEMPERATURE, "Temperature Fault");
    }
    if (status & 0x0002) {
        Serial.println("FAULT: Communication Fault (CML)");
        readDetailedFault(RACM600_STATUS_CML, "Communication Fault");
    }
    if (status & 0x0001) Serial.println("FAULT: Unknown Fault");

    // Warnings
    if (status & 0x8000) Serial.println("WARNING: Output Voltage Issue");
    if (status & 0x4000) Serial.println("WARNING: Output Current or Power Issue");
    if (status & 0x2000) Serial.println("WARNING: Input Voltage or Power Issue");
    if (status & 0x1000) Serial.println("WARNING: Manufacturer-Specific Issue");
    if (status & 0x0800) Serial.println("WARNING: Power Good Signal Lost");
    if (status & 0x0200) Serial.println("WARNING: Fan or Airflow Issue");
    if (status & 0x0100) Serial.println("WARNING: Other Status Warning");

    return status;
}


// Reads the detailed fault register so we can figure out exactly what fault is occuring.
void RACM600::readDetailedFault(uint8_t faultRegister, const char* faultType) {
    uint8_t faultStatus = readCommand(faultRegister);

    Serial.print(faultType);
    Serial.print(" Details: 0x");
    Serial.println(faultStatus, HEX);

    if (faultRegister == RACM600_STATUS_VOUT) {
        if (faultStatus & 0x80) Serial.println(" - Output Overvoltage Fault");
        if (faultStatus & 0x40) Serial.println(" - Output Overvoltage Warning");
        if (faultStatus & 0x10) Serial.println(" - Output Undervoltage Warning");
        if (faultStatus & 0x08) Serial.println(" - Output Undervoltage Fault");
    }

    if (faultRegister == RACM600_STATUS_IOUT) {
        if (faultStatus & 0x80) Serial.println(" - Output Overcurrent Fault");
        if (faultStatus & 0x40) Serial.println(" - Critical Constant Current Mode Fault");
        if (faultStatus & 0x20) Serial.println(" - Output Overcurrent Warning");
    }

    if (faultRegister == RACM600_STATUS_INPUT) {
        if (faultStatus & 0x80) Serial.println(" - Input Overvoltage Fault");
        if (faultStatus & 0x40) Serial.println(" - Input Overvoltage Warning");
        if (faultStatus & 0x10) Serial.println(" - Input Undervoltage Warning");
        if (faultStatus & 0x08) Serial.println(" - Input Undervoltage Fault");
    }

    if (faultRegister == RACM600_STATUS_TEMPERATURE) {
        if (faultStatus & 0x80) Serial.println(" - Overtemperature Fault");
        if (faultStatus & 0x40) Serial.println(" - Overtemperature Warning");
    }

    if (faultRegister == RACM600_STATUS_CML) {
        if (faultStatus & 0x80) Serial.println(" - Invalid Command Received");
        if (faultStatus & 0x40) Serial.println(" - Invalid Data Received");
        if (faultStatus & 0x20) Serial.println(" - Packet Error Check Failed");
    }
}