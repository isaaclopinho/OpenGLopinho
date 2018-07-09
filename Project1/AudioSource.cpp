#include "AudioSource.h"

AudioSource::AudioSource(std::string path, bool loop, bool mono) {
	this->path = path;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, AudioBufferManager::GetBuffer(path, mono));
	SetLooping(loop);
	SetGain(1);
	SetPitch(1);
	SetPosition(0, 0, 0);
	SetVelocity(0, 0, 0);
}

AudioSource::~AudioSource() {
	Stop();
	alDeleteSources(1, &source);
	AudioBufferManager::ReleaseBuffer(path);
}

void AudioSource::Play() {
	alSourcePlay(source);
}

void AudioSource::Pause() {
	alSourcePause(source);
}

void AudioSource::Stop() {
	alSourceStop(source);
}

bool AudioSource::IsPlaying() {
	ALint result;
	alGetSourcei(source, AL_SOURCE_STATE, &result);
	return result == AL_PLAYING;
}

void AudioSource::SetGain(float gain) {
	alSourcef(source, AL_GAIN, gain);
}

void AudioSource::SetPosition(float x, float y, float z) {
	alSource3f(source, AL_POSITION, x, y, z);
}

void AudioSource::SetVelocity(float x, float y, float z) {
	alSource3f(source, AL_VELOCITY, x, y, z);
}

void AudioSource::SetLooping(bool loop) {
	alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void AudioSource::SetPitch(float pitch) {
	alSourcef(source, AL_PITCH, pitch);
}

void AudioSource::Update(float dt) {
	if (fading) {
		elapsedTime += dt;
		if (elapsedTime > fadeTime) {
			elapsedTime = fadeTime;
			fading = false;
		}
		float percentage = elapsedTime / fadeTime;
		SetGain((finalGain - initialGain) * percentage + initialGain);
	}
}

void AudioSource::FadeIn(float targetGain, float time) {
	if (!IsPlaying())
		Play();
	initialGain = 0;
	finalGain = targetGain;
	elapsedTime = 0;
	fadeTime = time;
	fading = true;
}

void AudioSource::FadeOut(float time) {
	alGetSourcef(source, AL_GAIN, &initialGain);
	finalGain = 0;
	elapsedTime = 0;
	fadeTime = time;
	fading = true;
}
