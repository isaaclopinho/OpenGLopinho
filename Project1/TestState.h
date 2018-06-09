#pragma once

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <glm/glm.hpp>
#include "Game.h"
#include "State.h"
#include "Camera.h"
#include "Loader.h"
#include "MasterRenderer.h"
#include "GUITexture.h"
#include "GUIRenderer.h"
#include "Mesh.h"
#include "Movie.h"
#include "GameObjectTest.h"
#include "Renderer.h"
#include "Light.h"
#include "InputManager.h"
#include "ParticleSystem.h"
#include "ParticleTexture.h"
#include <glm/gtx/rotate_vector.hpp>
#include "btBulletCollisionCommon.h"
#include "ShadowFrameBuffer.h"
#include "PostProcessing.h"

using namespace glm;
using namespace std;

class TestState : public State {

	PointLight pt[4] = {
		PointLight(vec3(-4, 0, 20),		13,		vec3(1, 1,1)*0.0f,	vec3(1,1,1) * 0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(1, 0, -10),		13,		vec3(1, 1,1)*0.0f,	vec3(1,1,1) * 0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -1, -10),	13,		vec3(1, 1,1)*0.0f,	vec3(1,1,1) * 0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -2, 10),		13,		vec3(1, 1,1)*0.0f,	vec3(1,1,1) * 0.0f,	vec3(1, 1,1)*0.0f)
	};

	vec3 pos = vec3(-2.0, 4.0, -1.0);

	SpotLight sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f);
	DirectionalLight direct = DirectionalLight(pos, vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f);

	Camera camera = Camera(vec3(0, 0, 3));
	int i = 0;
	vector<GUITexture> guiTextures;
	GUIRenderer guirenderer = GUIRenderer(); 
	ParticleSystem *  ps;
	GUITexture shadowMap;

	ShadowFrameBuffer sfb;

	Fbo *fbo, *output;
	PostProcessing * pp;

public:

	TestState() : shadowMap(0, vec2(0.75f, 0.75f), vec2(0.25f, 0.25f)), sfb(2048,2048) {

		fbo = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT);
		output = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT, 1);

		pp = new PostProcessing();
		pp->init();

		btBroadphaseInterface *itf = new btDbvtBroadphase();
		ParticleTexture*  pt = new ParticleTexture(Loader::LoadTexture("res/fire_002.png"), 5, true);
		ps = new ParticleSystem(*pt, 50, 3, 0, 1);
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/models/plane.dae"), glm::vec3(0, -0.5, 0), glm::vec3(-90, 0, 0), 25, "", true)));
		
		guiTextures.emplace_back(shadowMap);

		for (int i = 0; i < 5; i++) {
			GameObjectTest *g = new GameObjectTest(Entity(Loader::LoadModel("res/models/t.dae"), glm::vec3(-5+i*2, 0, 0), glm::vec3(-90, 0, 0), 1, "", true));
			g->animated = true;
			AddGameObject(g);
		}
		

		//guiTextures.emplace_back(GUITexture(Loader::LoadTexture("res/GUI/gui.png"), vec2(0.75f, 0.90f), vec2(0.22f, 0.1f)));
		direct = DirectionalLight(pos, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f);
	}


	void Update(float dt) {
		float delta = dt;
		ps->Update(dt, vec3(0, 0, -15));
		
		MasterRenderer::GetInstance().updateAllParticles (dt, camera);
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Update(dt);
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_ESCAPE)) {
			remove = true;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_UP)) {
			camera.position -= vec3(0, 0, 50)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_DOWN)) {
			camera.position += vec3(0, 0, 50)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_w)) {
			camera.position += vec3(0, 50, 0)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_s)) {
			camera.position -= vec3(0, 50, 0)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_LEFT)) {
			camera.position -= vec3(50, 0, 0)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_RIGHT)) {
			camera.position += vec3(50, 0, 0)*delta;
		}
		
		if (InputManager::GetInstance().KeyPress(SDLK_SPACE)) {
			Movie::playfile("res/videos/video.ogv", Game::GetInstance()->window, Game::GetInstance()->renderer);
		}
		
	//pos = rotateY(pos, radians(5.0f));
		//sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*0.6f, vec3(1, 1, 1), vec3(1, 1, 1));
	direct = DirectionalLight(pos, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f); 

	}

	void Render() {
		
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Render();
		}

		sfb.renderSceneOnBuffer();
		sfb.bindShadowMap();

		//fbo->bindFrameBuffer();

		MasterRenderer::GetInstance().render(sl, pt, direct, camera);

		//fbo->unbindFrameBuffer();

		//fbo->resolveToFbo(*output);

		//pp->doPostProcessing(output->colourTexture);

		guiTextures[0].textureID = sfb.depthTexture;		

		//guirenderer.render(guiTextures);
	}

};
