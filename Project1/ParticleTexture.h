#pragma once

#include <iostream>

using namespace std;


class ParticleTexture
{
public:
	int textureID;
	int numberOfRows;
	bool isAddictive;

	ParticleTexture(int textureID, int numberOfRows, bool isAdd);
	~ParticleTexture();


	bool operator==(const ParticleTexture &other) const
	{
		return (textureID == other.textureID
			&& numberOfRows == other.numberOfRows);
	}

	
};

namespace std
{
	template <>
	struct hash<ParticleTexture>
	{
		size_t operator()(const ParticleTexture& k) const
		{
			return ((hash<int>()(k.textureID) ^ (hash<int>()(k.numberOfRows)  << 1) ^ (hash<bool>()(k.isAddictive) << 1)) >> 1);
		}
	};
}
