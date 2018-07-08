#include "AudioSystem.h"

AudioSystem & AudioSystem::Instance() {
	static AudioSystem audioSystem;
	return audioSystem;
}

void AudioSystem::Init() {
	// Initialize SDL Mizer
	Mix_Init(MIX_INIT_FLAC | MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_MOD);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);

	// Initialize OpenAL
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	
	// Initialize Listener
	SetListenerGain(1);
	SetListenerPosition(0, 0, 0);
	SetListenerVelocity(0, 0, 0);
}

void AudioSystem::Shutdown() {
	alcMakeContextCurrent(NULL);
	if (context)
		alcDestroyContext(context);
	if (device)
		alcCloseDevice(device);
	Mix_CloseAudio();
	Mix_Quit();
}

void AudioSystem::Update(float dt) {
	if (transitioning) {
		elapsedTime += dt;

		alSourcef(musicMap[currentMusic], AL_GAIN, 1 - (elapsedTime / transitionTime));
		alSourcef(musicMap[nextMusic], AL_GAIN, elapsedTime / transitionTime);

		if (elapsedTime > transitionTime) {
			alSourcef(musicMap[currentMusic], AL_GAIN, 0);
			alSourcef(musicMap[nextMusic], AL_GAIN, 1);
			currentMusic = nextMusic;
			nextMusic = "";
			transitioning = false;
			elapsedTime = 0;
		}
	}
}

void AudioSystem::SetListenerGain(float gain) {
	alListenerf(AL_GAIN, gain);
}

void AudioSystem::SetListenerPosition(float x, float y, float z) {
	alListener3f(AL_POSITION, x, y, z);
}

void AudioSystem::SetListenerVelocity(float x, float y, float z) {
	alListener3f(AL_VELOCITY, x, y, z);
}

void AudioSystem::AddMusic(std::string path) {
	if (musicMap.find(path) == musicMap.end()) {
		ALuint source;
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, AudioBufferManager::GetBuffer(path, false));
		alSourcei(source, AL_LOOPING, AL_TRUE);
		alSourcef(source, AL_GAIN, 0);
		alSourcef(source, AL_PITCH, 1);
		musicMap[path] = source;
	}
}

void AudioSystem::RemoveMusic(std::string path) {
	if (musicMap.find(path) != musicMap.end()) {
		alSourceStop(musicMap[path]);
		alDeleteSources(1, &musicMap[path]);
		AudioBufferManager::ReleaseBuffer(path);
		musicMap.erase(path);
	}
}

void AudioSystem::PlayAllOnMute() {
	for (auto it : musicMap) {
		alSourcePlay(it.second);
	}
}

void AudioSystem::SetCurrent(std::string path) {
	if (musicMap.find(path) != musicMap.end()) {
		alSourcef(musicMap[path], AL_GAIN, 1);
		currentMusic = path;
	}
}

void AudioSystem::StartTransitionTo(std::string path) {
	if (musicMap.find(path) != musicMap.end() and path != currentMusic) {
		transitioning = true;
		nextMusic = path;
	}
}

void AudioSystem::SetTransitionTime(float transitionTime) {
	this->transitionTime = transitionTime;
}
