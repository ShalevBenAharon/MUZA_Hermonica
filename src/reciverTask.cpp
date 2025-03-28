#include <iostream>
#include <string>
#include <vector>
#include <windows.h> // For serial port communication
#include <algorithm>  // For remove
#include "reciverTask.hpp"
#include <chrono>
#include <atomic>

void parseStrintoMSge(std::string str, SeHarmonicaMsg &msg)
{

    std::string  byte =  str.substr(0, 2);;
    uint8_t number = static_cast<char>(std::stoi(byte, nullptr, 16));
    std::vector<uint16_t> notesBuff;
    uint16_t val = 0; 
    msg.packetSize = ( number >> 3) & 0xff ; 
    msg.btn1 = ( number >> 2) & 0x01 ; 
    msg.btn2 = ( number >> 1) & 0x01 ; 
    msg.btn3 = ( number) & 0x01 ; 

    
    for(size_t i(2); i <= 30; )
    {
        byte = str.substr(i,3);
        val = static_cast<uint16_t>(std::stoi(byte, nullptr, 16));
        notesBuff.push_back(val);
        i = i + 3;
    }

    msg.hole1  = notesBuff[0] & 0x0FFF; 
    msg.hole2  = notesBuff[1] & 0x0FFF;
    msg.hole3  = notesBuff[2] & 0x0FFF;
    msg.hole4  = notesBuff[3] & 0x0FFF;
    msg.hole5  = notesBuff[4] & 0x0FFF;
    msg.hole6  = notesBuff[5] & 0x0FFF;
    msg.hole7  = notesBuff[6] & 0x0FFF;
    msg.hole8  = notesBuff[7] & 0x0FFF;
    msg.hole9  = notesBuff[8] & 0x0FFF;
    msg.hole10 = notesBuff[9] & 0x0FFF;

    

} 


HANDLE openSerialPort(const char* portName) {
    // Open the serial port
    HANDLE hSerial = CreateFile(
        portName,                        // Port name, e.g., "COM3"
        GENERIC_READ | GENERIC_WRITE,    // Read and Write access
        0,                               // No sharing
        NULL,                            // Default security
        OPEN_EXISTING,                   // Open existing port only
        0,                               // No special attributes
        NULL);                           // No template

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening COM port" << std::endl;
        return INVALID_HANDLE_VALUE;
    }

    // Set up the device control block (DCB) structure
    DCB dcbSerialParams = { 0 };           // DCB structure to configure the serial port
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);  // Set the size of the DCB

    // Get the current serial port settings
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting COM port state" << std::endl;
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    // Set the baud rate, parity, stop bits, and byte size
    dcbSerialParams.BaudRate = 115200;      // Set baud rate to 9600 (or other value as required)
    dcbSerialParams.ByteSize = 8;             // 8 data bits
    dcbSerialParams.StopBits = ONESTOPBIT;    // One stop bit
    dcbSerialParams.Parity   = NOPARITY;      // No parity

    // Apply the settings
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting COM port state" << std::endl;
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    // Set timeouts for the serial port
    COMMTIMEOUTS timeouts = { 0 };
    
    timeouts.ReadIntervalTimeout = MAXDWORD; 
    timeouts.ReadTotalTimeoutConstant = 0;  
    timeouts.ReadTotalTimeoutMultiplier = 0; 

    timeouts.WriteTotalTimeoutConstant   = 50;   // Constant timeout for writing
    timeouts.WriteTotalTimeoutMultiplier = 10;   // Multiplier for the total write timeout (per byte)

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting timeouts for COM port" << std::endl;
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    return hSerial;
}

// Function to read data from the serial port
void readSerialPort(HANDLE hSerial, SeHarmonicaMsg& hermonicaMsg,bool isInCalibration) {
    char buf[1024];
    DWORD bytesRead;
    std::vector<uint8_t> byteArray;
    bool isRunning(true);
    // int counter = 0 ; 
    //std::thread timerThread(setTimeFlagAfterDelay, 1); 
    while (isRunning) {
        // Clear buffer and read data
        memset(buf, 0, sizeof(buf));    
        if (ReadFile(hSerial, buf, sizeof(buf), &bytesRead, NULL)) {  
            if (bytesRead > 0) {
                //++counter;
                parseStrintoMSge(buf, hermonicaMsg);
                if(isInCalibration)
                {
                   if(clbCount >= 100)
                   { 
                    
                        isRunning = false;
                   }
                   clbCount++;
                }
                
            }
        } else {
            std::cerr << "Error reading from COM port" << std::endl;
            break;
        }
        Sleep(10);  // Add a short delay to prevent CPU overuse
    }

}

void callibration(int32_t *notesAvg,SeHarmonicaMsg& hermonicaMsg ,HANDLE hSerial ){

    uint32_t prevClb = 0;
     // Start a thread to read data from the serial port
     std::thread caliReadSerial(readSerialPort, hSerial, std::ref(hermonicaMsg),true);
    while(clbCount <= 100)
    {
        // need to apply condition for making sure the value recivied is real .
        
        if(prevClb + 1 == clbCount)
        {
            prevClb = clbCount;
            
            std::cout << " note: "  << notesAvg[0] << " hole1: " << hermonicaMsg.hole1 << std::endl;
            notesAvg[0] += hermonicaMsg.hole1;
            
            // notesAvg[1] += hermonicaMsg.hole2;
            // notesAvg[2] += hermonicaMsg.hole3;
            // notesAvg[3] += hermonicaMsg.hole4;
            // notesAvg[4] += hermonicaMsg.hole5;
            // notesAvg[5] += hermonicaMsg.hole6;
            // notesAvg[6] += hermonicaMsg.hole7;
            // notesAvg[7] += hermonicaMsg.hole8;
            // notesAvg[8] += hermonicaMsg.hole9;
            // notesAvg[9] += hermonicaMsg.hole10;
    
        }
        
    }
    for (int i(0); i <= 0; ++i )
    {
        notesAvg[i] =  notesAvg[i] / 100 ;
    } 
    
    std::cout <<  notesAvg[0] << std::endl;
    clbCount = 0 ;
    caliReadSerial.join();
}