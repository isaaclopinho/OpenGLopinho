#include "ParticleTexture.h"
#include "Debug.h"

ParticleTexture::ParticleTexture(int textureID, int numberOfRows, bool isAddictive) : textureID(textureID), numberOfRows(numberOfRows), isAddictive(isAddictive)
{
	Debug3("ParticleTexture", textureID, numberOfRows);
}

ParticleTexture::~ParticleTexture()
{
}
