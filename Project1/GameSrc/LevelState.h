#pragma once

#include <SDL.h>
#include <glm\glm.hpp>
#include "../Game.h"
#include "../State.h"
#include "../Camera.h"
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

using namespace glm;
using namespace std;

class LevelState : public State {

	BulletDebugDrawer* debugDrawer;

	PointLight pt[4] = {
		PointLight(vec3(-4, 0, 20),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(1, 0, -10),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -1, -10),	13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -2, 10),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f)
	};
	DirectionalLight direct = DirectionalLight(vec3(0, 0, -1), vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*0.6f, vec3(1, 1, 1)*10.0f);


	Camera camera = Camera(vec3(0, 2, 0));

	SpotLight sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f, vec3(1, 1, 1)*0.0f);

	btDiscreteDynamicsWorld* phyWorld;
	Player* player;
	Player* obj2;
	vector<btRigidBody> rigidBodies;


public:
	LevelState() { 

		//Initialize Physics
		btBroadphaseInterface* broadphase = new btDbvtBroadphase();
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

		phyWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		phyWorld->setGravity(btVector3(0, -9.8, 0));

		//temporary physics ground for testing purposes
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
		btDefaultMotionState* groundState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
		btRigidBody::btRigidBodyConstructionInfo groundRBCI(0, groundState, groundShape, btVector3(0, 0, 0));
		btRigidBody* groundRB = new btRigidBody(groundRBCI);

		phyWorld->addRigidBody(groundRB);
		
		//debugDrawer = new BulletDebugDrawer();
		phyWorld->setDebugDrawer(debugDrawer);

		player = Player::getInstance();
		AddGameObject(player);

		phyWorld->addRigidBody(player->getRB());

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

		if (InputManager::GetInstance().IsKeyDown(SDLK_UP)) {
			camera.position -= vec3(0, 0, 5)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_DOWN)) {
			camera.position += vec3(0, 0, 5)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_w)) {
			camera.position += vec3(0, 5, 0)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_s)) {
			camera.position -= vec3(0, 5, 0)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_LEFT)) {
			camera.position -= vec3(5, 0, 0)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_RIGHT)) {
			camera.position += vec3(5, 0, 0)*delta;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_p)) {
			cout << "p" << endl;
			debugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
		}
	};

	void Render() {
	
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Render();
		}

		phyWorld->debugDrawWorld();

		MasterRenderer::GetInstance().render(sl, pt, direct, camera);
	};
};