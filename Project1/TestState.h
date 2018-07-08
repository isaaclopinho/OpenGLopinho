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
#include "FlareManager.h"


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
	DirectionalLight direct = DirectionalLight(pos, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f);

	Camera camera = Camera(vec3(0, 0, 3));
	int i = 0;
	vector<GUITexture> guiTextures;
	GUIRenderer guirenderer = GUIRenderer(); 
	ParticleSystem *  ps;
	GUITexture shadowMap;

	ShadowFrameBuffer sfb;

	Fbo *fbo, *output;
	PostProcessing * pp;

	GameObjectTest *t;

	vector<FlareTexture> texturesFlare;

	FlareManager * lensFlare;

	FlareRenderer rendererF = FlareRenderer();

public:

	TestState() : shadowMap(0, vec2(0.75f, 0.75f), vec2(0.25f, 0.25f)), sfb(2048,2048) {
		MasterRenderer::GetInstance().usingShadow = true;
		fbo = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT);
		output = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT, 1);

		pp = new PostProcessing();
		pp->init();
		camera.pitch = 0;
		camera.angleAroundTarget = 0;
		camera.roll = 0;
		camera.distanceFromTarget = 2;
		FlareTexture flare = FlareTexture(Loader::LoadTexture("res/lensFlares/tex8.png"), 0.5f);
		flare.screenPos = vec2(0.5f);
		texturesFlare.emplace_back(flare);

		/*texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex4.png"), 0.23f));
		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex2.png"), 0.1f));
		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex7.png"), 0.05f));
		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex1.png"), 0.02f));

		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex3.png"), 0.06f));
		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex9.png"), 0.12f));
		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex5.png"), 0.07f));
		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex1.png"), 0.012f));

		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex9.png"), 0.1f));
		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex3.png"), 0.07f));
		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex5.png"), 0.3f));
		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex4.png"), 0.4f));

		texturesFlare.emplace_back(FlareTexture(Loader::LoadTexture("res/lensFlare/tex8.png"), 0.6f));*/


		lensFlare = new FlareManager(0.16f, texturesFlare);

		btBroadphaseInterface *itf = new btDbvtBroadphase();
		ParticleTexture*  pt = new ParticleTexture(Loader::LoadTexture("res/blood_hit.png"), 4, true);
		ps = new ParticleSystem(*pt, 10, 0.6f, 0.2f, 0.8f);

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/models/plane.dae"), glm::vec3(0, -0.5, 0), glm::vec3(-90, 0, 0), vec3(25,25,25), "", true)));
		
		guiTextures.emplace_back(shadowMap);

		t = new GameObjectTest(Entity(Loader::LoadModel("res/models/pet-01.dae"), glm::vec3(0 , -1, 0), glm::vec3(-90,0, 0), vec3(1, 1, 1)*0.15f, "IdleRight", true));
		t->animated = true;

		
		AddGameObject(t);
		guiTextures.emplace_back(GUITexture(Loader::LoadTexture("res/lensFlare/tex8.png"), vec2(0.75f, 0.90f), vec2(0.22f, 0.1f)));
		//direct = DirectionalLight(pos, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f);
	}


	void Update(float dt) {
		float delta = dt;
		ps->Update(dt, vec3(t->entity.position.x, t->entity.position.y + 0.375f, t->entity.position.z));
		
		MasterRenderer::GetInstance().updateAllParticles (dt, camera);

		camera.Update(dt, t->entity.position, t->entity.rotation);
		vec3 posi = camera.position;

		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Update(dt);
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_ESCAPE)) {
			remove = true;
		}

		
		
		if (InputManager::GetInstance().KeyPress(SDLK_SPACE)) {
			Movie::playfile("res/videos/video.ogv", Game::GetInstance()->window, Game::GetInstance()->renderer);
		}
		
	//pos = rotateY(pos, radians(5.0f));
		sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*1.0f, vec3(1, 1, 1), vec3(1, 1, 1));
	//direct = DirectionalLight(pos, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f); 

	}

	void Render() {
		
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Render();
		}

		sfb.renderSceneOnBuffer();
		sfb.bindShadowMap();

//        fbo->bindFrameBuffer();

		MasterRenderer::GetInstance().render(sl, pt, direct, camera);


//        fbo->unbindFrameBuffer();

//        fbo->resolveToFbo(*output);

//        pp->doPostProcessing(output->colourTexture);

		guiTextures[0].textureID = sfb.depthTexture;		

		//rendererF.render(texturesFlare, 0.8f);
	//	guirenderer.render(guiTextures);
	}

};
