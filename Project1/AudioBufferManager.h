#pragma once

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <SDL2/SDL_mixer.h>
#else
#include <al.h>
#include <alc.h>
#include <SDL_mixer.h>
#endif
#include <string>
#include <iostream>



#include <unordered_map>

class AudioBufferManager {
public:
	static ALuint GetBuffer(std::string path, bool mono=true);
	static void ReleaseBuffer(std::string path);

private:
	static void GenerateNewBuffer(std::string path, bool mono=true);

	static std::unordered_map<std::string, ALuint> bufferMap;
	static std::unordered_map<ALuint, int> userCount;
};
