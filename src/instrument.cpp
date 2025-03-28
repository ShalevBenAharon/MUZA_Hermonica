#include "instrument.hpp"

Instrument::Instrument() : synth(nullptr), settings(nullptr),
                           adriver(nullptr), soundFont(nullptr){
                           };
Instrument::~Instrument(){
    delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
};

void Instrument::PlayNote(int index,int chanelIndex ,int velocity ) {
    //std::cout << "Playing note..." << std::endl;
    fluid_synth_noteon(synth, chanelIndex, index, velocity);  // Channel 0, note 60 (C4), velocity 100
    
}

void Instrument::Stop(int index,int chanelIndex) {
  //std::cout << "stop played." << std::endl;
  fluid_synth_noteoff(synth, chanelIndex, index);  // Stop the note
}

void Instrument::StopAllNotes() {
  //std::cout << "stop all notes." << std::endl;
 fluid_synth_all_notes_off(synth, 0);
}

int Instrument::LoadSoundFont(const char *pathToSF) 
{
    std::cout << "Creating FluidSynth settings..." << std::endl;
    settings = new_fluid_settings();
    fluid_settings_setstr(settings, "audio.driver", "dsound");
    synth = new_fluid_synth(settings);
     if (!synth) 
     {
        std::cerr << "Failed to create FluidSynth synthesizer!" << std::endl;
        delete_fluid_settings(settings);
        return 1;
    }

    std::cout << "Loading SoundFont..." << std::endl;
    if (fluid_synth_sfload(synth,pathToSF, 1) == -1) 
    {
        std::cerr << "Failed to load SoundFont!" << std::endl;
        delete_fluid_synth(synth);
        delete_fluid_settings(settings);
        return 1;
    }

    std::cout << "Creating FluidSynth audio driver..." << std::endl;
    adriver = new_fluid_audio_driver(settings, synth);
    if (!adriver) 
    {
        std::cerr << "Failed to create FluidSynth audio driver!" << std::endl;
        delete_fluid_synth(synth);
        delete_fluid_settings(settings);
        return 1;
    }
    return 0;
}

/*********************************** Piano ****************************************/
// Piano class (inherits from Instrument)
Piano::Piano(){
    std::cout << "Piano created" << std::endl;
    LoadSoundFont(pathToPianoSoundFont);  // Load your SF2 soundfont file
}

Piano::~Piano() {
    std::cout << "Piano destroyed" << std::endl;
}

/*********************************** Harmonica ***********************************/

Harmonica::Harmonica(){
    cout << "Harmonica created" << endl;
    LoadSoundFont(pathToHarmonicaSoundFont);
}

Harmonica::~Harmonica(){
    cout << "Harmonica destroyed" << endl;
}

/********************************* Violin ***************************************/

Violin::Violin(){
    cout << "Violin created" << endl;
    LoadSoundFont(pathToViolinSoundFont);
}
Violin::~Violin(){
    cout << "Violin destroyed" << endl;
}