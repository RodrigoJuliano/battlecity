#pragma once

#include <SFML/Audio.hpp>
#include <string>

using namespace sf;

enum SFX : unsigned int {
	shoot = 0,
	bulletHit,
	tankExplode,
	tankMove,
	tankIdle,
	bonusSpawn,
	getBonus,
	getLife,
	nSounds
};

class SoundSystem
{
private:
	SoundBuffer soundBuffers[SFX::nSounds];
	Sound sounds[SFX::nSounds];
public:
	bool loadSounds(std::string folder, float volume = 20.f);
	void play(SFX sound, bool loop = false);
	void stop(SFX sound);
	void pause(SFX sound);
};

