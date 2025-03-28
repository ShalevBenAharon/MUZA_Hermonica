#include "instrument.hpp"
#include "reciverTask.hpp"
#include "Loger.hpp"
#include <fluidsynth.h>
#include <chrono>
#include <memory>
#include <map>
#include <thread>
#include <chrono> 
#include <mutex>

#define NOTES_PLAYING_OFFSET 20
#define REQUIRED_CONSECUTIVE_PACKETS 5 // Number of consecutive packets needed to confirm

static int octaveShift = 0;
static int currentInstrument = 1; // 0 - Piano, 1 - Harmonica, 2 - Violin
static bool playingNote = false;  // To track if the note is already playing
static int playingNoteValue = 0;  // To store the note value that is being played.

std::map<int, std::shared_ptr<Instrument>> instrumentsMap;
int32_t notesAvg[10];

std::map<int, int> noteMapping = {
    //lower octave
    {'C', 48},  // C
    {'D', 50},  // D
    {'E', 52},  // E
    {'G', 55},  // G
    {'B', 59},  // B

    //Middle octave
    {'C2', 60},  // C
    {'D2', 62},  // D
    {'E2', 64},  // E
    {'F',  65},  // F
    {'G2', 67},  // G
    {'A',  69},  // A
    {'C3', 72},  // C
    {'B2', 71},  // B

    //Upper octave
    {'E3', 76},  // E
    {'D3', 74},  // D
    {'G3', 79},  // G
    {'F2', 77},  // F
    {'C4', 84},  // C
    {'A2', 81},  // A
};

std::mutex hermonicaMsgMutex; // Mutex for hermonicaMsg

int consecutiveHighPackets = 0;  // Counter for consecutive high packets
int consecutiveLowPackets = 0;   // Counter for consecutive low packets

int main() {
    Logger log("Musa_log.txt");
    SeHarmonicaMsg hermonicaMsg;

    instrumentsMap[0] = std::make_shared<Piano>();
    instrumentsMap[1] = std::make_shared<Harmonica>();
    instrumentsMap[2] = std::make_shared<Violin>();

    HANDLE hSerial = openSerialPort("COM4");

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open COM port" << std::endl;
        return -1;
    }

    callibration(notesAvg, hermonicaMsg, hSerial);

    // Start a thread to read data from the serial port
    std::thread serialReadThread(readSerialPort, hSerial, std::ref(hermonicaMsg), false);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    while (true) {
        int hole1Value;
        {
            std::lock_guard<std::mutex> lock(hermonicaMsgMutex);
            hole1Value = hermonicaMsg.hole1;
        }

        // Check if value is high
        if (!playingNote && hole1Value >= notesAvg[0] + NOTES_PLAYING_OFFSET) {
            consecutiveHighPackets++;
            consecutiveLowPackets = 0;  // Reset low packet counter

            // Only play the note if we have 5 consecutive high packets
            if (consecutiveHighPackets >= REQUIRED_CONSECUTIVE_PACKETS) {
                std::cout << "hole 1 value high: " << hole1Value << std::endl;
                std::cout << "note avg: " << notesAvg[0] << std::endl;
                instrumentsMap[currentInstrument]->PlayNote(noteMapping['C2'], 0, 100);
                playingNote = true;
                playingNoteValue = noteMapping['C2'];
            }
        }
        // Check if value is low
        else if (!playingNote && hole1Value <= notesAvg[0] - NOTES_PLAYING_OFFSET) {
            consecutiveLowPackets++;
            consecutiveHighPackets = 0;  // Reset high packet counter

            // Only play the note if we have 5 consecutive low packets
            if (consecutiveLowPackets >= REQUIRED_CONSECUTIVE_PACKETS) {
                std::cout << "hole 1 value low: " << hole1Value << std::endl;
                std::cout << "note avg: " << notesAvg[0] << std::endl;
                instrumentsMap[currentInstrument]->PlayNote(noteMapping['D2'], 0, 100);
                playingNote = true;
                playingNoteValue = noteMapping['D2'];
            }
        }
        // Check if value is within average range
        else if (hole1Value >= notesAvg[0] - NOTES_PLAYING_OFFSET && hole1Value <= notesAvg[0] + NOTES_PLAYING_OFFSET) {
            // Smooth the average using a moving average approach
            notesAvg[0] = 0.1 * hole1Value + (1 - 0.1) * notesAvg[0];

            consecutiveHighPackets = 0;  // Reset counters when values are within average range
            consecutiveLowPackets = 0;

            if (playingNote) {
                instrumentsMap[currentInstrument]->Stop(playingNoteValue, 0);  // Only stop the playing note
                playingNote = false;
                playingNoteValue = 0;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Small delay
    }

    // Close the serial port when done
    CloseHandle(hSerial);

    std::cout << "Program exited." << std::endl;
    return 0;
}
