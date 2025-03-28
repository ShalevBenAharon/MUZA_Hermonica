#pragma once 

#include <atomic>
#include <thread>
#define ESCAPE_PRESSED 0x1B
#define NOT_PRESSED -1
#define TOTAL_INSTRUMENT 3

class KeyboardListener {
public:
    KeyboardListener();
    ~KeyboardListener();
    void StartListening(int &currentPress, int &octaveShift, int &currentInstrument);

private:
    bool isRunning;
};
