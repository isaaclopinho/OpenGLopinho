#include "AudioBufferManager.h"

std::unordered_map<std::string, ALuint> AudioBufferManager::bufferMap;
std::unordered_map<ALuint, int> AudioBufferManager::userCount;

ALuint AudioBufferManager::GetBuffer(std::string path, bool mono) {
	if (bufferMap.find(path) == bufferMap.end())
		GenerateNewBuffer(path, mono);
	userCount[bufferMap[path]]++;
	return bufferMap[path];
}

void AudioBufferManager::ReleaseBuffer(std::string path) {
	userCount[bufferMap[path]]--;
	if (userCount[bufferMap[path]] == 0) {
		userCount.erase(bufferMap[path]);
		alDeleteBuffers(1, &bufferMap[path]);
		bufferMap.erase(path);
	}
}

void AudioBufferManager::GenerateNewBuffer(std::string path, bool mono) {
	printf("Gerando buffer %s com: %s\n", mono ? "mono" : "stereo" , path.c_str()); 
	ALuint newBuffer;
	alGenBuffers(1, &newBuffer);

	// Load sound with sdl mixer and transfer to buffer
	Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
	alBufferData(newBuffer, mono ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, sound->abuf, sound->alen, MIX_DEFAULT_FREQUENCY);
	Mix_FreeChunk(sound);

	bufferMap[path] = newBuffer;
	userCount[bufferMap[path]] = 0;
}
