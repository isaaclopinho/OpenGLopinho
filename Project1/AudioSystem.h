#pragma once

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <SDL2/SDL_mixer.h>
//#include <SDL2/SDL.h>
#else
#include <al.h>
#include <alc.h>
#include <SDL_mixer.h>
#endif
#include <iostream>
#include <vector>
#include <cstdio>
#include <unordered_map>
#include "AudioSource.h"

class AudioSystem {
public:
	static AudioSystem& Instance();

	void Init();
	void Shutdown();
	void Update(float dt);

	void SetListenerGain(float gain);
	void SetListenerPosition(float x, float y, float z);
	void SetListenerVelocity(float x, float y, float z);

	void AddMusic(std::string path);
	void RemoveMusic(std::string path);
	void PlayAllOnMute();
	void SetCurrent(std::string path);
	void StartTransitionTo(std::string target);
	void SetTransitionTime(float transitionTime);

private:
	ALCdevice* device = NULL;
	ALCcontext* context = NULL;

	std::unordered_map<std::string, ALuint> musicMap;
	std::string currentMusic = "";
	std::string nextMusic = "";
	float transitionTime = 2;
	float elapsedTime = 0;
	bool transitioning = false;
};
