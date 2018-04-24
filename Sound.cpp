#include "Sound.h"

#include <map>
#include "Rendering.h"

int sfx_volume, music_volume;

std::map<std::string,Mix_Chunk*> loaded_sounds;
Mix_Chunk* load_sound(std::string s)
{
    if (!loaded_sounds.count(s))
    {
        loaded_sounds[s] = Mix_LoadWAV((std::string("Data")+PATH_SEPARATOR+"Sounds"+PATH_SEPARATOR+s+".wav").c_str());
        Mix_VolumeChunk(loaded_sounds[s], sfx_volume);
    }
    return loaded_sounds[s];
}

std::map<std::string,Mix_Music*> loaded_music;
Mix_Music* load_music(std::string s)
{
    if (!loaded_music.count(s)) loaded_music[s] = Mix_LoadMUS((std::string("Data")+PATH_SEPARATOR+"Music"+PATH_SEPARATOR+s+".wav").c_str());
    return loaded_music[s];
}

void sound_init()
{
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_VolumeMusic(music_volume);

    play_music(load_music("Barjazz"));
}

void sound_init_update()
{
    Mix_VolumeMusic(music_volume);
    for (auto s: loaded_sounds)
    {
        Mix_VolumeChunk(s.second, sfx_volume);
    }
}

void play_sound(Mix_Chunk* s)
{
    if (s!= nullptr) Mix_PlayChannel(-1, s, 0);
}

void play_music(Mix_Music* s)
{
    Mix_PlayMusic(s,-1);
}


