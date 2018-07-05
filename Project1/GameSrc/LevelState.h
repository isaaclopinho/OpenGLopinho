#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <glm/glm.hpp>
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
#include <glm/gtx/rotate_vector.hpp>
#include "../ParticleSystem.h"
#include "Player.h"
#include <btBulletDynamicsCommon.h>
#include "../GameObjectTest.h"
#include "../ShadowFrameBuffer.h"
#include "../Fbo.h"
#include "../PostProcessing.h"
#include "../PhysicsObject.h"
#include "../PhysicsWorld.h"

using namespace glm;
using namespace std;

//bool callBackFunc(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1){
//
//    // colisões de objetos com flag 4 são tratadas aqui.
//
//    return false;
//}

class LevelState : public State {

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



	vec3 pos = vec3(-2.0, 400.0, -1.0);

	DirectionalLight direct = DirectionalLight(pos, vec3(1, 1, 1)*0.7f, vec3(1, 1, 1)*0.6f, vec3(1, 1, 1)*0.6f);

	Camera camera = Camera();

	SpotLight sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*0.0f);

	
	Player* player;
	vector<btRigidBody> rigidBodies;


public:
    PhysicsWorld phyWorld;
    
    
	LevelState() : sfb(4096,4096), phyWorld(){
		
		//Shadows and PostProcessing
		fbo = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT);
		output = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT, 1);
		pp = new PostProcessing();
		pp->init();
		MasterRenderer::GetInstance().usingShadow = true;


		//Initialize Physics
//        gContactAddedCallback=callBackFunc; // seta o ponteiro para nosso callback de contato;
		//temporary physics ground for testing purposes
        
        //(mass, shape, position, rotation, scale, inercia, entity);
        PhysicsObject* ground = new PhysicsObject(0, PhysicsShape::Box, btVector3(0,0,0), btVector3(0,0,0), btVector3(200,1,1000), btVector3(0,0,0), new Entity(Loader::LoadModel("res/casas/rua5.dae"), glm::vec3(0,1,0), glm::vec3(0,-90, 0), 700, "", true));
        
        
        AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/rua5.dae"), glm::vec3(0, 1, 0), glm::vec3(0, -90, 0), 700, "", true)));
		
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/casa degrau.dae"), glm::vec3(50, 26, 0), glm::vec3(0, -90, 0),26, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/muro2.dae"), glm::vec3(50, -10, 62), glm::vec3(0, -90, 0), 40, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/casa estrutura.dae"), glm::vec3(50, 0, 140), glm::vec3(0, -90, 0), 13, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/muro2.dae"), glm::vec3(50, -10, 190), glm::vec3(0, -90, 0), 40, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/casa quarto.dae"), glm::vec3(65, 0, 240), glm::vec3(0, -90, 0), 13, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/muro2.dae"), glm::vec3(50, -10, 320), glm::vec3(0, -90, 0), 40, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/casa simples.dae"), glm::vec3(40, 26, 380), glm::vec3(0, -90, 0), 27, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/muro2.dae"), glm::vec3(50, -10, 420), glm::vec3(0, -90, 0), 40, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/brise.dae"), glm::vec3(20,-10, 520), glm::vec3(0, -90, 0), 13, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(35, 0, 60), glm::vec3(-90, 0, -90), 1, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(35, 0, 195), glm::vec3(-90, 0, -90), 1, "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(35, 0, 310), glm::vec3(-90, 0, -90), 1, "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(35, 0, 425), glm::vec3(-90, 0, -90), 1, "", true)));


		//lado direito


		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/casa degrau.dae"), glm::vec3(-50, 26, 20), glm::vec3(0, 90, 0), 26, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/muro2.dae"), glm::vec3(-50, -10, 62), glm::vec3(0, 90, 0), 40, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/casa estrutura.dae"), glm::vec3(-50, 0, 140), glm::vec3(0, 90, 0), 13, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/muro2.dae"), glm::vec3(-50, -10, 190), glm::vec3(0, 90, 0), 40, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/casa quarto.dae"), glm::vec3(-65, 0, 260), glm::vec3(0, 90, 0), 13, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/muro2.dae"), glm::vec3(-50, -10, 320), glm::vec3(0, 90, 0), 40, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/casa simples.dae"), glm::vec3(-40, 26, 380), glm::vec3(0, 90, 0), 27, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/muro2.dae"), glm::vec3(-50, -10, 420), glm::vec3(0, 90, 0), 40, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/brise.dae"), glm::vec3(-20, -10, 460), glm::vec3(0, 90, 0), 13, "", true)));
		
		
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-35, 0, 60), glm::vec3(-90, 0, 90), 1, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-35, 0, 195), glm::vec3(-90, 0, 90), 1, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-35, 0, 310), glm::vec3(-90, 0, 90), 1, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-35, 0, 425), glm::vec3(-90, 0, 90), 1, "", true)));

        
//        AddGameObject(PhysicsObject());
        
        phyWorld.addPhysicsObject(ground);
        printf("cria player\n");
		player = Player::getInstance();
        printf("adiciona player\n");
		AddGameObject(player);
		camera.distanceFromTarget = 30;
		camera.pitch = 30;
		camera.vDist = -50;
		camera.angleAroundTarget = 180;
        printf("adicionando rigid body do player\n");
		phyWorld.addPhysicsObject(player);
        printf("adicionou\n");


	};

	~LevelState();

	void Update(float dt) {
		float delta = dt;

		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Update(dt);
		}
        phyWorld.updateWorld(dt);
        
		
		if (InputManager::GetInstance().IsKeyDown(SDLK_ESCAPE)) {
			remove = true;
		}

		if (InputManager::GetInstance().IsKeyDown(SDLK_p)) {
			cout << player->entity.position.x <<" " << player->entity.position.y << " " << player->entity.position.z << endl;
		}

		camera.Update(dt, player->entity.position, player->entity.rotation);
	};

	void Render() {
	
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Render();
		}
        
		sfb.renderSceneOnBuffer();
		sfb.bindShadowMap();
//        fbo->bindFrameBuffer(); //comentar pra rodar no mac

        
		MasterRenderer::GetInstance().render(sl, pt, direct, camera);
        phyWorld.debugDraw();
        
//        fbo->unbindFrameBuffer(); //comentar pra rodar no mac
//        fbo->resolveToFbo(*output); //comentar pra rodar no mac
//        pp->doPostProcessing(output->colourTexture); //comentar pra rodar no mac
        
	};
};
