#include "ParticleSystem.h"




ParticleSystem::ParticleSystem(ParticleTexture &texture, float pps, float speed, float gravityComplient, float lifeLength)
	:texture(texture), pps(pps), speed(speed), gravityComplient(gravityComplient), lifeLength(lifeLength)
{
	//"Psystem", texture.textureID);
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
		switch (type) {
		default:

			
			break;
		case 0:

			EmitParticle(center);
			break;
		case 1:
			EmitParticle2(center);
			break;
		}
	}

	if (timer >= 1) {
		pEmitidas = 0;
		timer = 0;
	}
}

void ParticleSystem::EmitParticle2(vec3 center)
{

	vec3 velocity = vec3(0, 1, 0);
	velocity = glm::normalize(velocity);
	velocity = velocity * (speed);
	
	Particle(texture, center + vec3((rand() % 15-7), 0, (rand() %15-7) ), velocity, -10, gravityComplient, lifeLength, 0, (rand()%50+1));
}

void ParticleSystem::EmitParticle(vec3 center)
{
	vec3 velocity = vec3(float(rand() % 30 - 10), float(rand() % 30 - 10), float(rand() % 30 - 10) ) ;
	velocity = glm::normalize(velocity);
	velocity = velocity * (speed);
	Particle(texture, center, velocity, -10 , gravityComplient, lifeLength, rand()%360, float(rand()%30+1)/100.0f);
}

ParticleSystem::~ParticleSystem()
{
}
