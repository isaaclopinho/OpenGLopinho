#pragma once

#include <string>
#include "AudioBufferManager.h"

class AudioSource {
public:
	AudioSource(std::string path, bool loop=false, bool mono=true);
	~AudioSource();

	void Play();
	void Pause();
	void Stop();
	bool IsPlaying();
	void SetGain(float gain);
	void SetPosition(float x, float y, float z);
	void SetVelocity(float x, float y, float z);
	void SetLooping(bool loop);
	void SetPitch(float pitch);
	void Update(float dt);
	void FadeIn(float targetGain, float time=1);
	void FadeOut(float time=1);

private:
	std::string path;
	ALuint source;
	float elapsedTime;
	float fadeTime;
	float fading;
	float initialGain, finalGain;
};
