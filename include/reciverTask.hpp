#include <iostream>
#include <thread>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <atomic>

#pragma pack(push, 1)
typedef struct SeHarmonicaMsg {
    uint8_t  packetSize : 5; 
    uint8_t  btn1       : 1; 
    uint8_t  btn2       : 1; 
    uint8_t  btn3       : 1; 

    uint16_t hole1      : 12; 
    uint16_t hole2      : 12; 
    uint16_t hole3      : 12; 
    uint16_t hole4      : 12; 
    uint16_t hole5      : 12; 
    uint16_t hole6      : 12; 
    uint16_t hole7      : 12; 
    uint16_t hole8      : 12; 
    uint16_t hole9      : 12; 
    uint16_t hole10     : 12; 

    uint16_t checkSum   : 12; 
} SeHarmonicaMsg;
#pragma pack(pop)


union UeharmonicaMsg {
    uint8_t buffer[17]; // 27 bytes buffer
    SeHarmonicaMsg data;    // Struct with 10 fields of 12 bits each
};

static std::atomic<int> clbCount(0);

//HANDLE static chooseSetting(HANDLE &hSerial);
HANDLE openSerialPort(const char* portName);
void readSerialPort(HANDLE hSerial ,SeHarmonicaMsg &hermonicaMsg, bool isPbit);
void callibration(int32_t *notesAvg,SeHarmonicaMsg& hermonicaMsg ,HANDLE hSerial );

