#include "MasterRenderer.h"
#include "Debug.h"
#include <algorithm>

void MasterRenderer::enableCulling()
{

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void MasterRenderer::disableCulling()
{
	glDisable(GL_CULL_FACE);
}

void MasterRenderer::render(SpotLight spotLight, PointLight * pointLight, DirectionalLight directionalLight, Camera camera)
{
	
	prepare();

	animatedShader->start();
	animatedShader->loadViewPos(camera.position);
	animatedShader->materialProperties(5);
	animatedShader->SetDirectionalLightProperties(directionalLight.direction, directionalLight.ambient, directionalLight.diffuse, directionalLight.specular);
	animatedShader->lightSpace(lightSpaceMatrix);

	usingShadow ? animatedShader->usingShadow(1) : animatedShader->usingShadow(0);


	for (int i = 0; i < 4; i++) {
		animatedShader->SetPointLightProperties(i, pointLight[i].position, pointLight[i].ambient, pointLight[i].diffuse, pointLight[i].specular, pointLight[i].distance);
	}

	animatedShader->SetSpotLightProperties(spotLight.position, spotLight.direction, spotLight.ambient, spotLight.diffuse, spotLight.specular, spotLight.distance, spotLight.cutOff, spotLight.outerCutOff);

	animatedShader->loadViewMatrix(camera);
	
	renderer->render(entities);

	animatedShader->stop();

	sbRenderer->render(camera);
	
	pRenderer->render(particless, camera);

	entities.clear();
}


void MasterRenderer::prepare()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(color.x, color.y, color.z, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::processEntity(Entity * go)
{
	entities.emplace_back(go);
}


void MasterRenderer::cleanUp()
{
}

void  MasterRenderer::createProjectionMatrix()
{
	projectionMatrix = Maths::createProjectionMatrix(FOV, NEAR_PLANE, FAR_PLANE);
}

void MasterRenderer::updateAllParticles(float dt, Camera camera)
{

	vector<pair<ParticleTexture, vector<Particle>>> deleteCandidates;

	for (auto&& it : particless) {

		for (int i = 0; i < it.second.size(); i++) {
			bool stillAlive = it.second[i].Update(dt, camera);
			
			if (!stillAlive) {
				it.second.erase(it.second.begin() + i);
			}

		}

		/*sort(it.second.begin(), it.second.end(), [](const Particle& lhs, const Particle& rhs)
		{
			return lhs.distance > rhs.distance;
		});*/

		if (it.second.empty()) {
			deleteCandidates.emplace_back(it);
		}
	}


	for (auto i : deleteCandidates) {

		particless.erase(i.first);
	}
	
}

void MasterRenderer::AddParticle(Particle & particle)
{
	particless[particle.texture].emplace_back(particle);
	
}



MasterRenderer::MasterRenderer() : color(vec3(.1f,.1f,.1f))
{
	enableCulling();
	animatedShader = new AnimatedShader();
	skyboxShader = new SkyboxShader();
	particleShader = new ParticleShader();

	createProjectionMatrix();

	renderer = new Renderer(animatedShader, projectionMatrix);	
	sbRenderer = new SkyboxRenderer(skyboxShader, projectionMatrix);
	pRenderer = new ParticleRenderer(particleShader, projectionMatrix);
}


MasterRenderer::~MasterRenderer()
{
}
