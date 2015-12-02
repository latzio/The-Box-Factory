/* ***************************************************************
*
* File : SoundManager.h
*
* Author : Tiberiu Popa
*          J. Alexander Clarke
* Date   : June 18th, 2002
*
* Modified:
*
* Purpose: Header file for the sound manager
*
* ****************************************************************/
#ifndef _SOUND_MANAGER__H
#define _SOUND_MANAGER__H
#pragma interface

// include SDL header files
#include "/usr/include/SDL/SDL.h"
#include "/usr/include/SDL/SDL_mixer.h"
#include <string>

// if you want more than 100 sounds, change the MAX_SOUND
#define MAX_SOUNDS 100
#define MAX_MUSIC 100

// wrapper calss around SDL and SDL_mixer libraries
class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    // manipulating sound clips
    // sound clips can be played simultaneously
    int LoadSound(const std::string& file);

    // snd is the sound ID returned by LoadSound
    int PlaySound(int snd);
    int StopSound(int snd);
    int PauseSound(int snd);
    int ResumeSound(int snd);

    // manipulating music clips
    // should have only one music clip

    int LoadMusic(const std::string& file);

    int PlayMusic(int);
    int StopMusic(int);
    int PauseMusic(int);
    int ResumeMusic(int);
    int VolumeMusic(int);

private:

    // internal data

    // sound clips
    Mix_Chunk* chunks[MAX_SOUNDS];
    Mix_Music* music[MAX_MUSIC];

    // sound channels for active sounds
    int channel[MAX_SOUNDS];

    // number of sound clips loaded
    int nSounds;
    int nMusics;

    int ResetSound(int snd);
    int ResetMusic(int snd);
};


// the global instance of the sound manager
extern SoundManager SM;


#endif // _SOUND_MANAGER__H
