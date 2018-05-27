#include "Particle.h"

#include "MasterRenderer.h"
#include <math.h>

Particle::Particle() : PoolObject(), gravity(0), position(vec3(0)), velocity(vec3(0)), gravityEffect(0), lifeLength(0), rotation(0), scale(1), texture(ParticleTexture(0,0,false)){
	distance = 0;
	texOffset1 = vec2();
	texOffset2 = vec2();
}

Particle::Particle(ParticleTexture &texture, vec3 pos, vec3 vel, float gravity, float gravityEffect, float lifeLength, float rotation, float scale)
	: PoolObject(), texture(texture), gravity(gravity), position(pos), velocity(vel), gravityEffect(gravityEffect), lifeLength(lifeLength), rotation(rotation), scale(scale)
{
	distance = 0;
	texOffset1 = vec2();
	texOffset2 = vec2();
	//MasterRenderer::GetInstance().AddParticle(*this);
}

bool Particle::Update(float dt, Camera camera)
{
	velocity.y += gravity * gravityEffect * dt;
	position = velocity * dt + position;
	distance = distance2(camera.position , position);
	
	UpdateTextureCoordInfo();
	elapsedTime += dt;
	return elapsedTime < lifeLength;
}

PoolObject * Particle::clone() const
{
	return (new Particle(*this));
}

void Particle::reset()
{
	elapsedTime = 0;
	distance = 0;
	texOffset1 = vec2();
	texOffset2 = vec2();
}

void Particle::UpdateTextureCoordInfo()
{
	float lifeFactor = elapsedTime / lifeLength;

	int stageCount = texture.numberOfRows * texture.numberOfRows;

	float atlasProgression = lifeFactor * stageCount;

	int index1 = glm::floor(atlasProgression);
	int index2 = (index1 < stageCount - 1) ? index1 + 1 : index1;
	blend = fmod(atlasProgression, 1);
	setTextureOffset(texOffset1, index1);
	setTextureOffset(texOffset2, index2);

	int column = index1 % texture.numberOfRows;
	int row = index1 / texture.numberOfRows;

	texOffset1.x = (float)column / (float)texture.numberOfRows;
	texOffset1.y = (float)row / (float)texture.numberOfRows;

	column = index2 % texture.numberOfRows;
	row = index2 / texture.numberOfRows;

	texOffset2.x = (float)column / (float)texture.numberOfRows;
	texOffset2.y = (float)row / (float)texture.numberOfRows;
}

void Particle::setTextureOffset(vec2 offset, int index)
{
	
}

Particle::~Particle()
{
}
