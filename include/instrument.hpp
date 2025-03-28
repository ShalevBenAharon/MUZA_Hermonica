#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fluidsynth.h>  // Include FluidSynth for SF2 support

using namespace std;


class Instrument {
public:
    Instrument();
    ~Instrument();

    virtual void PlayNote(int index,int chanelIndex, int velocity);
    virtual void Stop(int index,int chanelIndex);
    virtual void StopAllNotes();
    virtual int  LoadSoundFont(const char *pathToSF);  // Load the SF2 soundfont

protected:
    fluid_synth_t* synth;  // FluidSynth synthesizer
    fluid_settings_t* settings;
    fluid_audio_driver_t* adriver;
    fluid_sfont_t* soundFont;  // Soundfont loaded in FluidSynth
    vector<std::string> noteFiles;  // List of soundfont files (C.sf2, D.sf2, etc.)
   

};

class Piano : public Instrument {
public:
    Piano(); 
    ~Piano();

private:
    const char *pathToPianoSoundFont = "..\\Soundfont\\piano\\nine-foot-grand-piano.sf2";

};

class Harmonica : public Instrument {
public:
    Harmonica();
    ~Harmonica();

private:
    const char *pathToHarmonicaSoundFont = "..\\Soundfont\\harmonica\\astral-harmonica.sf2";

};

class Violin : public Instrument{
public:
    Violin();
    ~Violin();

private:
    const char *pathToViolinSoundFont = "..\\Soundfont\\strings\\violin.sf2";

};

#endif // INSTRUMENT_HPP
