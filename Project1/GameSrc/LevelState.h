#pragma once

#include <SDL.h>
#include <glm\glm.hpp>
#include "../Game.h"
#include "../State.h"
#include "../GameCamera.h"
#include "../Loader.h"
#include "../MasterRenderer.h"
#include "../GUITexture.h"
#include "../GUIRenderer.h"
#include "../Mesh.h"
#include "../Movie.h"
#include "../GameObject.h"
#include "../Renderer.h"
#include "../Light.h"
#include "../InputManager.h"
#include <glm\gtx\rotate_vector.hpp>
#include "../ParticleSystem.h"
#include "Player.h"
#include "../BulletDebugDrawer.h"
#include <btBulletDynamicsCommon.h>
#include "../GameObjectTest.h"
#include "../ShadowFrameBuffer.h"
#include "../Fbo.h"
#include "../PostProcessing.h"
using namespace glm;
using namespace std;

class LevelState : public State {

	BulletDebugDrawer* debugDrawer;

	ShadowFrameBuffer sfb;

	Fbo * fbo;
	Fbo * output;
	PostProcessing * pp;

	PointLight pt[4] = {
		PointLight(vec3(-4, 0, 20),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(1, 0, -10),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -1, -10),	13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -2, 10),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f)
	};



	vec3 pos = vec3(-2.0, 4.0, -1.0);

	DirectionalLight direct = DirectionalLight(pos, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f);

	Camera camera = Camera();

	SpotLight sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f);

	btDiscreteDynamicsWorld* phyWorld;
	Player* player;
	Player* obj2;
	vector<btRigidBody> rigidBodies;


public:
	LevelState() : sfb(){ 
		fbo = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT);
		output = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT, 1);
		pp = new PostProcessing();
		pp->init();
		//Initialize Physics
		btBroadphaseInterface* broadphase = new btDbvtBroadphase();
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;


		MasterRenderer::GetInstance().usingShadow = true;
		phyWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		phyWorld->setGravity(btVector3(0, -9.8, 0));

		//temporary physics ground for testing purposes
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	
		btDefaultMotionState* groundState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
		btRigidBody::btRigidBodyConstructionInfo groundRBCI(0, groundState, groundShape, btVector3(0, 0, 0));
		btRigidBody* groundRB = new btRigidBody(groundRBCI);

		phyWorld->addRigidBody(groundRB);
		
		//debugDrawer = new BulletDebugDrawer();
		phyWorld->setDebugDrawer(debugDrawer);

		player = Player::getInstance();
		AddGameObject(player);
		camera.distanceFromTarget = 3;
		camera.pitch = 30;
		camera.vDist = -5;
		camera.angleAroundTarget = 180;
		phyWorld->addRigidBody(player->getRB());

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/models/plane.dae"), glm::vec3(0, 0, 0), glm::vec3(-90, 0, 0), 25, "", true)));


		//debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);


	};

	~LevelState();

	void Update(float dt) {
		float delta = dt;

		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Update(dt);
		}

		//Physics
		phyWorld->stepSimulation(dt);
		
		if (InputManager::GetInstance().IsKeyDown(SDLK_ESCAPE)) {
			remove = true;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_p)) {
			cout << "p" << endl;
			debugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
		}

		camera.Update(dt, player->entity.position, player->entity.rotation);
	};

	void Render() {
	
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Render();
		}


		sfb.renderSceneOnBuffer();
		sfb.bindShadowMap();

		fbo->bindFrameBuffer();


		MasterRenderer::GetInstance().render(sl, pt, direct, camera);

		fbo->unbindFrameBuffer();

		fbo->resolveToFbo(*output);

		pp->doPostProcessing(output->colourTexture);
	};
};