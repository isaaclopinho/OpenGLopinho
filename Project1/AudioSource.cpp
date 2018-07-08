#include "AudioSource.h"

AudioSource::AudioSource(std::string path, bool loop) {
	this->path = path;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, AudioBufferManager::GetBuffer(path));
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
