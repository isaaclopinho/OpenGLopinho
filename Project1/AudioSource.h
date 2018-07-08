#pragma once

#include <string>
#include "AudioBufferManager.h"

class AudioSource {
public:
	AudioSource(std::string path, bool loop=false);
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

private:
	std::string path;
	ALuint source;
};
