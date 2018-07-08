#pragma once
#include "Particle.h"
#include "MasterRenderer.h"

class ParticleSystem
{
public:
	float pps;
	float speed;
	float gravityComplient;
	float lifeLength;

	float timer = 0;
	float pEmitidas = 0;

	int type = 0;

	ParticleTexture texture;

	ParticleSystem(ParticleTexture &texture, float pps, float speed, float gravityComplient, float lifeLength);

	void Update(float dt, vec3 center);
	void EmitParticle2(vec3 center);
	void EmitParticle(vec3 center);

	~ParticleSystem();
};

