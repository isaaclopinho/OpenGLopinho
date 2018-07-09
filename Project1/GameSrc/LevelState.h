#define MAXBARRA 0.2
#define BARRAX -0.5
#define BARRAY 0.87

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
#include "Enemy.h"
#include <glm/gtx/vector_angle.hpp>
#include "Hitbox.h"
using namespace glm;
using namespace std;

//bool callBackFunc(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1){
//
//    // colisões de objetos com flag 4 são tratadas aqui.
//
//    return false;
//}

class LevelState : public State {

	float h = 10;

	vector<vec3> posEnemies = {
		vec3(20, h, -100),
		
		vec3(0, h, 0),
		vec3(-30, h, 20),

		vec3(26, h, 160),
		vec3(-34, h, 280),
		
		vec3(30, h, 360),
		vec3(0, h, 420),
		vec3(-30, h, 520),

	};


	int enemiesCount = posEnemies.size();
	bool portal = false;

	float distanciaPerigo = 0;


	ShadowFrameBuffer sfb;

	Fbo * fbo;
	Fbo * output;
	PostProcessing * pp;
    
    vector<GUITexture> GUITextures;
    GUIRenderer guirenderer = GUIRenderer();

	PointLight pt[4] = {
		PointLight(vec3(-4, 0, 20),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(1, 0, -10),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -1, -10),	13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f),
		PointLight(vec3(0, -2, 10),		13,		vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f,	vec3(1, 1,1)*0.0f)
	};



	vec3 pos = vec3(-2.0, 400.0, -1.0);

	DirectionalLight direct = DirectionalLight(pos, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*0.6f, vec3(1, 1, 1)*0.6f);

	Camera camera = Camera();

	SpotLight sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*0.0f);

	
	Player* player;
	vector<btRigidBody> rigidBodies;
	PhysicsObject* ground;

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
		//temporary physics ground for testing purposes
        
        //(mass, shape, position, rotation, scale, inercia, entity);
    	ground = new PhysicsObject(0, PhysicsShape::Box, btVector3(0, 0, 0), btVector3(0, 0, 0), btVector3(200, 1, 1000), btVector3(0, 0, 0), new Entity(Loader::LoadModel("res/Models/plane.dae"), glm::vec3(0, 1, 0), glm::vec3(-90, 0, 0), vec3(1, 1, 1) * 900.0f, "", true));

		AddGameObject(ground);
		
		
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_degrau/casa_degrau.dae"), glm::vec3(73, -2, -51), glm::vec3(0, 90, 0), 10.5f *vec3(1.35f, 1, 1), "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(70, 29.5f, 0), glm::vec3(0, -90, 0), vec3(1, 1.4f, 1) *  26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(71, 0, 74), glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_quarto/casa_quarto.dae"), glm::vec3(86, -3, 153), glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(70, 29.5f, 221), glm::vec3(0, -90, 0), vec3(1, 1.4f, 1) * 26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(71, 0, 288), glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(70, 29.5f, 356), glm::vec3(0, -90, 0), vec3(1, 1.4f, 1) *  26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_quarto/casa_quarto.dae"), glm::vec3(86, -3, 418), glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(69, 29.5f, 485), glm::vec3(0, -90, 0), vec3(1, 1.4f, 1) * 26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(71, 0, 549), glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(-71, 0, -37), glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(-70, 29.5f, 30), glm::vec3(0, 90, 0), vec3(1, 1.4f, 1) * 26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_quarto/casa_quarto.dae"), glm::vec3(-86, -3, 110), glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(-70, 29.5f, 175), glm::vec3(0, 90, 0), vec3(1, 1.4f, 1) *  26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_degrau/casa_degrau.dae"), glm::vec3(-73, -2, 226), glm::vec3(0, -90, 0), 10.5f *vec3(1.5f, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(-71, 0, 292), glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_quarto/casa_quarto.dae"), glm::vec3(-86, -3, 372), glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(-70, 29.5f, 435), glm::vec3(0, 90, 0), vec3(1, 1.4f, 1) *  26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(-71, 0, 503.8f), glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(-70, 29.5f, 568), glm::vec3(0, 90, 0), vec3(1, 1.4f, 1) * 26.0f, "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(65, 0, 29.95), glm::vec3(-90, 0, -90), vec3(1, 1, 1) , "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(61, 0, 188.76), glm::vec3(-90, 0, -90), vec3(1, 1, 1) , "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(60, 0, 325.656), glm::vec3(-90, 0, -90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(60, 0, 456.175), glm::vec3(-90, 0, -90), vec3(1, 1, 1), "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(65, 0, 598.802), glm::vec3(-90, 0, -90), vec3(1, 1, 1), "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-65, 0, 1.4), glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-57, 0, 144.7), glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-57, 0, 248.821), glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-58, 0, 408.41), glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-63, 0, 536.892), glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));

		vec3 offset = vec3(0,0,680);
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/muro2.dae"), glm::vec3(0, 0, -74) - offset, glm::vec3(0, 0, 90), vec3(50, 100, 20), "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_degrau/casa_degrau.dae"), glm::vec3(73, -2, -51)-offset, glm::vec3(0, 90, 0), 10.5f *vec3(1.35f, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(70, 29.5f, 0) - offset, glm::vec3(0, -90, 0), vec3(1, 1.4f, 1) *  26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(71, 0, 74) - offset, glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_quarto/casa_quarto.dae"), glm::vec3(86, -3, 153) - offset, glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(70, 29.5f, 221) - offset, glm::vec3(0, -90, 0), vec3(1, 1.4f, 1) * 26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(71, 0, 288) - offset, glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(70, 29.5f, 356) - offset, glm::vec3(0, -90, 0), vec3(1, 1.4f, 1) *  26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_quarto/casa_quarto.dae"), glm::vec3(86, -3, 418) - offset, glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(69, 29.5f, 485) - offset, glm::vec3(0, -90, 0), vec3(1, 1.4f, 1) * 26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(71, 0, 549) - offset, glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(-71, 0, -37) - offset, glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(-70, 29.5f, 30) - offset, glm::vec3(0, 90, 0), vec3(1, 1.4f, 1) * 26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_quarto/casa_quarto.dae"), glm::vec3(-86, -3, 110) - offset, glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(-70, 29.5f, 175) - offset, glm::vec3(0, 90, 0), vec3(1, 1.4f, 1) *  26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_degrau/casa_degrau.dae"), glm::vec3(-73, -2, 226) - offset, glm::vec3(0, -90, 0), 10.5f *vec3(1.5f, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(-71, 0, 292) - offset, glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_quarto/casa_quarto.dae"), glm::vec3(-86, -3, 372) - offset, glm::vec3(0, 90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(-70, 29.5f, 435) - offset, glm::vec3(0, 90, 0), vec3(1, 1.4f, 1) *  26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_estrutura/casa_estrutura.dae"), glm::vec3(-71, 0, 503.8f) - offset, glm::vec3(0, -90, 0), vec3(1, 1, 1) * 13.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casa_01/casa-01.dae"), glm::vec3(-70, 29.5f, 568) - offset, glm::vec3(0, 90, 0), vec3(1, 1.4f, 1) * 26.0f, "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(65, 0, 29.95) - offset, glm::vec3(-90, 0, -90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(61, 0, 188.76) - offset, glm::vec3(-90, 0, -90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(60, 0, 325.656) - offset, glm::vec3(-90, 0, -90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(60, 0, 456.175) - offset, glm::vec3(-90, 0, -90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(65, 0, 598.802) - offset, glm::vec3(-90, 0, -90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-65, 0, 1.4) - offset, glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-57, 0, 144.7) - offset, glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-57, 0, 248.821) - offset, glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-58, 0, 408.41) - offset, glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));
		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/poste.dae"), glm::vec3(-63, 0, 536.892) - offset, glm::vec3(-90, 0, 90), vec3(1, 1, 1), "", true)));

		AddGameObject(new GameObjectTest(Entity(Loader::LoadModel("res/casas/p/piramide.obj"), glm::vec3(0, 195, 850), glm::vec3(0, 180, 0), vec3(1, 1, 1) * 2.0f, "", true)));


        ground->type = "Floor";
        phyWorld.addPhysicsObject(ground, COL_FLOOR);
		player = Player::getInstance();
		player->limitZ = vec2(-700, 720);
		player->limitX = vec2(-50, 50);

		AddGameObject(player);
		
		camera.distanceFromTarget = 20;
		camera.pitch = 12;
		camera.vDist = -20;
		camera.angleAroundTarget = 180;
        phyWorld.addPhysicsObject(player, COL_PLAYER, COL_FLOOR | COL_WALL | COL_ENEMY);
        //(mass, shape, position, rotation, scale, inercia, entity);
        //BOX debugdraw
        Entity* box = new Entity(Loader::LoadModel("res/Models/cube.obj"), glm::vec3(0, 10, 40), glm::vec3(0, 0, 0), vec3(5,4,5), "", true);
        Hitbox* playerHitbox = new Hitbox(btVector3(0,10,40), btVector3(0,0,0), btVector3(5,4,5), box);
        playerHitbox->owner = player;
//        attackBoxPlayer = new PhysicsObject(0, PhysicsShape::Box, btVector3(0,10,40), btVector3(0,0,0), btVector3(5,4,5), btVector3());
        phyWorld.addPhysicsObject(playerHitbox, COL_WALL, COL_ENEMY);
        AddGameObject(playerHitbox);
//        phyWorld.addPhysicsObject(attackBoxPlayer, COL_WALL, COL_ENEMY);
//        AddGameObject(attackBoxPlayer);
//        attackBoxPlayer->toggleContact(false);
//        attackBoxPlayer->type = "Trigger";
        
		
		for(int i=0; i < posEnemies.size(); i++)
			InstantiateEnemy(posEnemies[i]);
//        InstantiateEnemy(vec3(10, 10, 20));

        //HUD
        
        GLuint teste2 = Loader::LoadTexture("res/GUI/barra_semEnergia.png");
        GUITextures.emplace_back(GUITexture(teste2, vec2(BARRAX,BARRAY), vec2(MAXBARRA,0.15)));
        
        GLuint teste = Loader::LoadTexture("res/GUI/barra_energia.png");
        GUITextures.emplace_back(GUITexture(teste, vec2(BARRAX,BARRAY), vec2(MAXBARRA,0.04)));
        
        GLuint teste3 = Loader::LoadTexture("res/GUI/lg3.png");
        GUITextures.emplace_back(GUITexture(teste3, vec2(-0.85,0.8), vec2(0.15,0.2)));
	};

	~LevelState();

	void Update(float dt) {

		distanciaPerigo = clamp(posEnemies[0].z - player->entity.position.z, 0.0f, 700.0f);

		if (enemiesCount <= 0 && !portal) {
			portal = true;
			cout << "ativaPortal" << endl;
		}
		if(portal)
			if (player->entity.position.z >= player->limitZ.y-10 && player->entity.position.x >= -10 && player->entity.position.x <= 10) {
				cout << "entrou" << endl;

			//chama proxima cena
		}


		if (InputManager::GetInstance().KeyPress(SDLK_l)) {
			int r = rand() % 360;
			cout << r << endl;
			ground->entity->rotation = vec3(r,0, 0);
		}

		if (InputManager::GetInstance().KeyPress(SDLK_o)) {

			for (int i = gameObjects.size() - 1; i >= 0; i--) {
				if (gameObjects[i]->type == "Enemy")
					((Enemy*)gameObjects[i])->remove = true;
			}
		}

		float delta = dt;
        for (int i = gameObjects.size()-1; i >= 0; i--){
            if (gameObjects[i]->Remove() == true) {
				if (gameObjects[i]->type == "Enemy")
					enemiesCount--;


                if (gameObjects[i]->type != "GameObject"){
                    phyWorld.removePhysicsObject((PhysicsObject*)gameObjects[i]);
                }
                //porque funciona com essas merda comentada?
//                GameObject* obj = gameObjects[i]; // guarda referencia pra poder deletar
                gameObjects.erase(gameObjects.begin()+i); // remove do vetor
//                delete obj; // deleta
            }
        }
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Update(dt);
        }

        phyWorld.updateWorld(dt);
        


		
		if (InputManager::GetInstance().IsKeyDown(SDLK_ESCAPE)) {
			remove = true;
		}

		if (InputManager::GetInstance().KeyPress(SDLK_p)) {
			cout << player->entity.position.x <<" " << player->entity.position.y << " " << player->entity.position.z << endl;
		}
        
		


//        glm::vec3 newPos = glm::vec3(player->entity.position.x, player->entity.position.y + 10, player->entity.position.z + 7);
        glm::vec3 newPos = Maths::bulletToGlm((player->getWorldRotation().normalized() * 3) + player->getWorldPosition());
//        glm::vec3 newPos = transform1.
        
        glm::vec3 newRot = glm::vec3(player->entity.rotation.x, player->entity.rotation.y, player->entity.rotation.z);
        
//        std::cout << "newrot: " << newRot.x << " " <<newRot.y << " " << newRot.z << endl;
        
//        attackBoxPlayer->setPosition((player->getWorldPosition()+btVector3(0,5,0)) + Maths::glmToBullet(player->getForwardVector()*10.0f));
//        attackBoxplayer->getWorldRotation().normalized();
		//attackBoxPlayer->SetRotation(Maths::glmToBullet(newRot));
		//attackBoxPlayer->SetRotation(player->getWorldRotation());
        //attackBoxPlayer->entity->position = newPos;
        //attackBoxPlayer->entity->rotation = newRot;
		//printf("%f, %f, %f\n", player->getPlayerRot().x, player->getPlayerRot().y, player->getPlayerRot().z);

		camera.Update(dt, player->entity.position, player->entity.rotation);
        
        if(InputManager::GetInstance().KeyPress(SDLK_h)){
            player->LoseHP(10, btVector3(0,0,0));
        }

		camera.angleAroundTarget = player->entity.rotation.z - 180;
        UpdateHP();
	};
    
    void UpdateHP(){
        
        GUITextures[1].scale = vec2((player->GetHP()<=0)?0:(float)(player->GetHP()) / (float)(player->GetMaxHP()) * MAXBARRA,(player->GetHP()<=0)?0:0.05);
        GUITextures[1].position = vec2(BARRAX - (MAXBARRA *(1 -((float)(player->GetHP()) / (float)(player->GetMaxHP())))),BARRAY);
    }
    
    void InstantiateEnemy(vec3 pos){
        Enemy *inimigo = new Enemy(100, PhysicsShape::Box, btVector3(0,0,0), new Entity(Loader::LoadModel("res/Models/pet-01.dae"), pos, glm::vec3(-90, 0, 0), vec3(1, 1, 1)*4.0f, "IdleRight", true));
        phyWorld.addPhysicsObject(inimigo, COL_ENEMY, COL_FLOOR | COL_WALL | COL_PLAYER);
        AddGameObject(inimigo);
        Entity* box = new Entity(Loader::LoadModel("res/Models/cube.obj"), glm::vec3(0, 10, 40), glm::vec3(0, 0, 0), vec3(5,4,5), "", true);
        Hitbox* enemyHitbox = new Hitbox(btVector3(0,10,40), btVector3(0,0,0), btVector3(5,4,5), box);
        enemyHitbox->owner = inimigo;
        phyWorld.addPhysicsObject(enemyHitbox, COL_ENEMY, COL_PLAYER);
        AddGameObject(enemyHitbox);
        
        
    }

	void Render() {
	
		for (unsigned int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Render();
		}
        
		sfb.renderSceneOnBuffer();
		sfb.bindShadowMap();
        fbo->bindFrameBuffer(); //comentar pra rodar no mac

        
        MasterRenderer::GetInstance().render(sl, pt, direct, camera);
        
        fbo->unbindFrameBuffer(); //comentar pra rodar no mac
        fbo->resolveToFbo(*output); //comentar pra rodar no mac
        pp->doPostProcessing(output->colourTexture); //comentar pra rodar no mac
        
        guirenderer.render(GUITextures);
	};
};
