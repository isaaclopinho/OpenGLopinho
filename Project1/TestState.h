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

using namespace glm;
using namespace std;

class TestState : public State {

	PointLight pt[4] = {
		PointLight(vec3(-4, 0, 20),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(1, 0, -10),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -1, -10),	13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -2, 10),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f)
	};

	vec3 pos = vec3(1, 0, 0);

	SpotLight sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f);
	DirectionalLight direct = DirectionalLight(vec3(0, 0, 0), vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f);

	Camera camera = Camera(vec3(0, 0, -4));
	int i = 0;
	vector<GUITexture> guiTextures;
	GUIRenderer guirenderer = GUIRenderer(); //agrupar no masterRenderer
	ParticleSystem *  ps;
public:

	TestState() {
		ParticleTexture*  pt = new ParticleTexture(Loader::LoadTexture("res/rain.png"), 1, true);
		ps = new ParticleSystem(*pt, 300, 60, 1, 1);
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/models/t.dae"), glm::vec3(0, -5, 0), glm::vec3(-90, 90, 0), 1, "", true)));
		guiTextures.emplace_back(GUITexture(Loader::LoadTexture("res/GUI/gui.png"), vec2(0.75f, 0.90f), vec2(0.22f, 0.1f)));
	}


	void Update(float dt) {
		float delta = dt;
		ps->Update(dt, vec3(camera.position.x, camera.position.y + 10, camera.position.z));
		
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
		
		/*if (camera.position.z >= -1 && i==0) {
			Movie::playfile("res/videos/Downloads.ogv", Game::GetInstance()->window, Game::GetInstance()->renderer);
			i++;
		}*/
		
		pos = rotateY(pos, radians(5.0f));
		//sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*0.6f, vec3(1, 1, 1), vec3(1, 1, 1));
		direct = DirectionalLight(pos, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f);

	}

	void Render() {
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Render();
		}
		MasterRenderer::GetInstance().render(sl, pt, direct, camera);
		guirenderer.render(guiTextures);
	}

};
