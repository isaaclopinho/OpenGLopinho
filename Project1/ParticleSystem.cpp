#include "ParticleSystem.h"




ParticleSystem::ParticleSystem(ParticleTexture &texture, float pps, float speed, float gravityComplient, float lifeLength)
	:texture(texture), pps(pps), speed(speed), gravityComplient(gravityComplient), lifeLength(lifeLength)
{
	Debug2("Psystem", texture.textureID);
}

void ParticleSystem::Update(float dt, vec3 center)
{
	timer += dt;
	

	float particlesToCreate = pps * dt;


	int count = glm::floor(particlesToCreate);
	float partialParticle = glm::mod(particlesToCreate, 1.0f);
	
	pEmitidas += partialParticle;

	if (pEmitidas >= 1) {
		count++;
		pEmitidas - 1;
	}

	for (int i = 0; i < count; i++) {
		EmitParticle(center);
	}

	if (timer >= 1) {
		pEmitidas = 0;
		timer = 0;
	}
}

void ParticleSystem::EmitParticle(vec3 center)
{
	vec3 velocity = vec3(0, 1, 0);
	velocity = glm::normalize(velocity);
	velocity = velocity * (speed);
	Particle(texture, center + vec3((float)(rand()%80-40)/100.0, 0, (float)(rand() % 80 - 40) / 100.0), velocity, -10 , gravityComplient, lifeLength, rand()%360, float(rand()%15+1));
}

ParticleSystem::~ParticleSystem()
{
}
