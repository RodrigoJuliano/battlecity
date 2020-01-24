#include "SoundSystem.h"


bool SoundSystem::loadSounds(std::string folder, float volume)
{
    for (int i = 0; i < SFX::nSounds; i++) {
        if (soundBuffers[i].loadFromFile(folder + "\\" + std::to_string(i) + ".wav")) {
            sounds[i].setBuffer(soundBuffers[i]);
            sounds[i].setVolume(volume);
        }
        else
            return false;
    }
	return true;
}

void SoundSystem::play(SFX sound, bool loop)
{
    sounds[sound].play();
    sounds[sound].setLoop(loop);
}

void SoundSystem::stop(SFX sound)
{
    sounds[sound].stop();
}

void SoundSystem::pause(SFX sound)
{
    sounds[sound].pause();
}

bool SoundSystem::isPlaying(SFX sound)
{
    return sounds[sound].getStatus() == Sound::Status::Playing;
}

bool SoundSystem::isPaused(SFX sound)
{
    return sounds[sound].getStatus() == Sound::Status::Paused;;
}
