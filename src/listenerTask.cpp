#include "listenerTask.hpp"
#include <windows.h>  // For GetAsyncKeyState
#include <iostream>

KeyboardListener::KeyboardListener() : isRunning(true) {}
KeyboardListener::~KeyboardListener() {}

void KeyboardListener::StartListening(int &currentPress, int &octaveShift, int &currentInstrument)
{
    int PrevKeyPressed = NOT_PRESSED;
    bool isKeyPressed = false;

    while (isRunning)
    {
        // Loop through all possible keys (ASCII values)
        for (int key = 8; key <= 255; key++)
        {
            // Check if the key is currently pressed
            if (GetAsyncKeyState(key) & 0x8000)
            {
                if (key != PrevKeyPressed)
                {
                    PrevKeyPressed = key;
                    currentPress = key;
                    isKeyPressed = true;
                   // std::cout << "Key pressed: " << (char)key << std::endl;
                }
                break;  // Break once a key is detected to avoid multiple detection in one cycle
            }
        }

        if((GetAsyncKeyState(VK_SPACE) & 0x8000))
        {
           currentInstrument = (currentInstrument + 1) % TOTAL_INSTRUMENT;
           std::cout << currentInstrument <<" playing" << std::endl;; 
        }

         if ((GetAsyncKeyState(VK_UP) & 0x8000) && octaveShift < 48)
        {
            octaveShift += 12;  // Increase the octave by 12
          //  std::cout << "Octave shifted up by 12. Current octave shift: " << octaveShift << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Prevent rapid multiple shifts
        }

        // Check for Down Arrow key press (VK_DOWN = 0x28) to decrease octave
        if (GetAsyncKeyState(VK_DOWN) & 0x8000 && (octaveShift > - 48))
        {
            octaveShift -= 12;  // Decrease the octave by 12
         //   std::cout << "Octave shifted down by 12. Current octave shift: " << octaveShift << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Prevent rapid multiple shifts
        }
        
        // Check if the key is released
        if (!(GetAsyncKeyState(PrevKeyPressed) & 0x8000) && isKeyPressed)
        {
         //   std::cout << "Key released: " << (char)PrevKeyPressed << std::endl;
            currentPress = -1;  // Reset the current press to indicate no key pressed
            isKeyPressed = false;
            PrevKeyPressed = -1;  // Reset previous key
        }

        // If 27 is pressed, stop the listener
        if (currentPress == VK_ESCAPE)
        {
            isRunning = false;
        }

        // Introduce a small delay to avoid high CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
