/**
 *   @file RACM600.h
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


#ifndef RACM600_H
#define RACM600_H

#include <Arduino.h>
#include <Wire.h>

#define RACM600_DEFAULT_ADDR  0x27 // Default I2C address

// PMBus Commands for RACM600
#define RACM600_PAGE                    0x00  // R/W, 1 Byte - Selects power output page (Page 0: Main, Page 1: AUX)
#define RACM600_OPERATION               0x01  // R/W, 1 Byte - Controls the operational state (ON/OFF via Bit 7)
#define RACM600_CLEAR_FAULTS            0x03  // Send-Byte, 0 Bytes - Clears all fault bits in all status registers
#define RACM600_CAPABILITY              0x19  // R-Byte, 1 Byte - Reports device capabilities (See CAPABILITY bitmap)
#define RACM600_QUERY                   0x1A  // Block W-Block R, W: 1, R: 1 - Checks if a command is supported and its format
#define RACM600_VOUT_MODE               0x20  // R-Byte, 1 Byte - Defines the output voltage data format
#define RACM600_VOUT_OV_FAULT_LIMIT     0x40  // R/W-Word, 2 Bytes - Output overvoltage fault threshold
#define RACM600_IOUT_OC_FAULT_LIMIT     0x46  // R/W-Word, 2 Bytes - Output overcurrent fault threshold
#define RACM600_IOUT_OC_WARN_LIMIT      0x4A  // R/W-Word, 2 Bytes - Output overcurrent warning threshold
#define RACM600_OT_FAULT_LIMIT          0x4F  // R-Word, 2 Bytes - Overtemperature fault threshold
#define RACM600_OT_WARN_LIMIT           0x51  // R-Word, 2 Bytes - Overtemperature warning threshold
#define RACM600_STATUS_BYTE             0x78  // R-Byte, 1 Byte - Summary of most critical faults
#define RACM600_STATUS_WORD             0x79  // R-Word, 2 Bytes - Summary of fault conditions (See STATUS_WORD bitmap)
#define RACM600_STATUS_VOUT             0x7A  // R-Byte, 1 Byte - Output voltage warning and fault status
#define RACM600_STATUS_IOUT             0x7B  // R-Byte, 1 Byte - Output current warning and fault status
#define RACM600_STATUS_INPUT            0x7C  // R-Byte, 1 Byte - Input voltage warning and fault status
#define RACM600_STATUS_TEMPERATURE      0x7D  // R-Byte, 1 Byte - Temperature warning and fault status
#define RACM600_STATUS_CML              0x7E  // R-Byte, 1 Byte - Communication status
#define RACM600_STATUS_OTHER            0x7F  // R-Byte, 1 Byte - Other status
#define RACM600_STATUS_MFR_SPECIFIC     0x80  // R-Byte, 1 Byte - Manufacturer-specific status
#define RACM600_READ_VIN                0x88  // R-Word, 2 Bytes - Reads input voltage in Volts
#define RACM600_READ_VCAP               0x8A  // R-Word, 2 Bytes - Reads voltage on the energy storage capacitor
#define RACM600_READ_VOUT               0x8B  // R-Word, 2 Bytes - Reads actual output voltage (same format as VOUT_MODE)
#define RACM600_READ_IOUT               0x8C  // R-Word, 2 Bytes - Reads measured output current in Amperes
#define RACM600_READ_TEMPERATURE_1      0x8D  // R-Word, 2 Bytes - Reads ambient temperature in degrees Celsius
#define RACM600_READ_TEMPERATURE_2      0x8E  // R-Word, 2 Bytes - Reads PFC temperature in degrees Celsius
#define RACM600_READ_TEMPERATURE_3      0x8F  // R-Word, 2 Bytes - Reads LLC temp (Page 0) or VSB temp (Page 1) in Celsius
#define RACM600_READ_POUT               0x96  // R-Word, 2 Bytes - Reads output power in Watts
#define RACM600_PMBUS_REVISION          0x98  // R-Byte, 1 Byte - Reads the PMBus revision compliance

// Manufacturer-Specific Commands
#define RACM600_MFR_VIN_MIN             0xA0  // R-Word, 2 Bytes - Minimum rated input voltage in Volts
#define RACM600_MFR_VIN_MAX             0xA1  // R-Word, 2 Bytes - Maximum rated input voltage in Volts
#define RACM600_MFR_IIN_MAX             0xA2  // R-Word, 2 Bytes - Maximum rated input current in Amperes
#define RACM600_MFR_PIN_MAX             0xA3  // R-Word, 2 Bytes - Maximum rated input power in Watts
#define RACM600_MFR_VOUT_MIN            0xA4  // R-Word, 2 Bytes - Minimum rated output voltage in Volts
#define RACM600_MFR_VOUT_MAX            0xA5  // R-Word, 2 Bytes - Maximum rated output voltage in Volts
#define RACM600_MFR_IOUT_MAX            0xA6  // R-Word, 2 Bytes - Maximum rated output current in Amperes
#define RACM600_MFR_POUT_MAX            0xA7  // R-Word, 2 Bytes - Maximum rated output power in Watts
#define RACM600_MFR_TAMBIENT_MAX        0xA8  // R-Word, 2 Bytes - Maximum rated ambient temperature in Celsius
#define RACM600_MFR_TAMBIENT_MIN        0xA9  // R-Word, 2 Bytes - Minimum rated ambient temperature in Celsius



class RACM600 {
public:
    RACM600(uint8_t i2c_address = RACM600_DEFAULT_ADDR);
    void begin();
    
    // Command Functions
    void enableOutput();
    void disableOutput();
    void clearFaults();
    uint16_t readFaults();
    float readVoltage();
    float readCurrent();
    float readAmbientTemperature();
    float readACINPUTTemperature();
    float readDCOUTPUTTemperature();
    
private:
    uint8_t _address;

    // Helper Functions
    void writeCommand(uint8_t cmd, uint16_t value);
    uint16_t readCommand(uint8_t cmd);
    void readDetailedFault(uint8_t faultRegister, const char* faultType);
};

#endif
