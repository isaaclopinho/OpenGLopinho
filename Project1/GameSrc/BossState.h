//
//  BossState.h
//  OpenGLopinho2
//
//  Created by Gabriel Mota Araújo on 08/07/18.
//  Copyright © 2018 Gabriel Mota Araújo. All rights reserved.
//

#define MAXBARRA 0.2
#define BARRAX -0.5
#define BARRAY 0.87
#define BARRABOSSX 0.5
#define BARRABOSSY 0.87

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
//#include "Enemy.h"
#include <glm/gtx/vector_angle.hpp>
#include "Hitbox.h"
#include "Boss.h"
using namespace glm;
using namespace std;

//bool callBackFunc(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1){
//
//    // colisões de objetos com flag 4 são tratadas aqui.
//
//    return false;
//}

class BossState : public State {
    
    ShadowFrameBuffer sfb;
    
    Fbo * fbo;
    Fbo * output;
    PostProcessing * pp;
    
    vector<GUITexture> GUITextures;
    GUIRenderer guirenderer = GUIRenderer();
    
	vector<PointLight> pt = {
        PointLight(vec3(-4, 0, 20),        13,        vec3(1, 1,1)*0.0f,    vec3(1, 1,1)*0.0f,    vec3(1, 1,1)*0.0f),
        PointLight(vec3(1, 0, -10),        13,        vec3(1, 1,1)*0.0f,    vec3(1, 1,1)*0.0f,    vec3(1, 1,1)*0.0f),
        PointLight(vec3(0, -1, -10),    13,        vec3(1, 1,1)*0.0f,    vec3(1, 1,1)*0.0f,    vec3(1, 1,1)*0.0f),
        PointLight(vec3(0, -2, 10),        13,        vec3(1, 1,1)*0.0f,    vec3(1, 1,1)*0.0f,    vec3(1, 1,1)*0.0f)
    };
    
    
    
    vec3 pos = vec3(-2.0, 400.0, -1.0);
    
    DirectionalLight direct = DirectionalLight(pos, vec3(1, 1, 1)*0.7f, vec3(1, 1, 1)*0.6f, vec3(1, 1, 1)*0.6f);
    
    Camera camera = Camera();
    
    SpotLight sl = SpotLight(camera.position, vec3(0, 0, -1), 13, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*1.0f, vec3(1, 1, 1)*0.0f);
    
    
    Player* player;
    vector<btRigidBody> rigidBodies;
    
	unique_ptr<AudioSource> bossMusic;
    
public:
    PhysicsWorld phyWorld;
    Boss* boss;
    
    BossState() : sfb(4096*3,4096*3), phyWorld(){
		MasterRenderer::GetInstance().resetParticles();
        //Shadows and PostProcessing
        fbo = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT);
        output = new Fbo(Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT, 1);
        pp = new PostProcessing();
        pp->init();
        MasterRenderer::GetInstance().usingShadow = true;
        
        
        //Initialize Physics
        //temporary physics ground for testing purposes
        
        PhysicsObject* ground = new PhysicsObject(0, PhysicsShape::Box, btVector3(0, 0, 0), btVector3(0, 0, 0), btVector3(200, 1, 1000), btVector3(0, 0, 0), new Entity(Loader::LoadModel("res/Models/plane.dae"), glm::vec3(0, 1, 0), glm::vec3(-90, 0, 0), vec3(1, 1, 1) * 900.0f, "", true));

        AddGameObject(ground);
        

        ground->type = "Floor";
        phyWorld.addPhysicsObject(ground, COL_FLOOR);
        player = Player::getInstance();
        player->setPosition(btVector3(0,1,0));
        AddGameObject(player);
        
        camera.distanceFromTarget = 20;
        camera.pitch = 12;
        camera.vDist = -20;
        camera.angleAroundTarget = 180;
        phyWorld.addPhysicsObject(player, COL_PLAYER, COL_FLOOR | COL_WALL | COL_ENEMY | COL_BOSS | COL_TRIGGER_ENEMY);
        //BOX debugdraw
//        Entity* box = new Entity(Loader::LoadModel("res/Models/cube.obj"), glm::vec3(0, 10, 40), glm::vec3(0, 0, 0), vec3(5,8,5), "", true);
        Hitbox* playerHitbox = new Hitbox(btVector3(0,10,40), btVector3(0,0,0), btVector3(5,8,5));
        playerHitbox->owner = player;
        phyWorld.addPhysicsObject(playerHitbox, COL_TRIGGER_PLAYER, COL_ENEMY | COL_BOSS);
        AddGameObject(playerHitbox);

//        player->limitZ = vec2(-700, 720);
//        player->limitX = vec2(-50, 50);
        
        
        boss = new Boss(0, PhysicsShape::Box, btVector3(0,0,0), new Entity(Loader::LoadModel("res/Models/boss.dae"), glm::vec3(0,10,200), glm::vec3(-90, 0, 0), vec3(1, 1, 1)*32.0f, "Main", true));
        boss->entity->scale/=8;
        phyWorld.addPhysicsObject(boss, COL_BOSS, COL_FLOOR | COL_WALL | COL_PLAYER | COL_TRIGGER_PLAYER | COL_ENEMY);
        
        AddGameObject(boss);
        
        //HUD
        
		GLuint teste = Loader::LoadTexture("res/GUI/barra_energia.png");
		GUITextures.emplace_back(GUITexture(teste, vec2(-0.40, 0.77), vec2(450.0 / 1280.0, 28.0 / 720.0)));

		GLuint teste2 = Loader::LoadTexture("res/GUI/Gui_Player.png");
		GUITextures.emplace_back(GUITexture(teste2, vec2(BARRAX, 0.77), vec2(620.0 / 1280.0, 168.0 / 720.0)));

		GLuint teste3 = Loader::LoadTexture("res/GUI/barra_energia_boss.png");
		GUITextures.emplace_back(GUITexture(teste3, vec2(0, -0.88), vec2(682.0 / 1280.0, 26.0 / 720.0)));

		GLuint teste4 = Loader::LoadTexture("res/GUI/Gui_Boss.png");
		GUITextures.emplace_back(GUITexture(teste4, vec2(0, -0.88), vec2(720.0 / 1280.0, 48.0 / 720.0)));
        
		bossMusic = make_unique<AudioSource>("res/music/boss.wav", true, false);
		bossMusic->Play();
    };
    
	~BossState() {}
    
    void Update(float dt) {
        float delta = dt;
        for (int i = gameObjects.size()-1; i >= 0; i--){
            if (gameObjects[i]->Remove() == true) {
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
        
        if (InputManager::GetInstance().KeyPress(SDLK_m)) {
            Shoot();
        }
        
        if (InputManager::GetInstance().KeyPress(SDLK_o)){
            //mata o boss
            boss->RecieveDamage(100);
        }
        
        if(boss->atira){
            Shoot();
        }
        
        if (InputManager::GetInstance().KeyPress(SDLK_LSHIFT)) {
            player->Dash();
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
		GUITextures[0].scale = vec2((player->GetHP() <= 0) ? 0 : (float)(player->GetHP()) / (float)(player->GetMaxHP()) * (450.0 / 1280.0), 28.0 / 720.0);
		GUITextures[0].position = vec2(-0.40 - ((450.0 / 1280.0)*(1 - ((float)(player->GetHP()) / (float)(player->GetMaxHP())))), 0.77);

		GUITextures[2].scale = vec2((boss->GetHP() <= 0) ? 0 : (float)(boss->GetHP()) / (float)(boss->GetMaxHP()) * (682.0 / 1280.0), 26.0 / 720.0);
		GUITextures[2].position = vec2( ((682.0 / 1280.0)*(1 - ((float)(boss->GetHP()) / (float)(boss->GetMaxHP())))), -0.88);

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
    
    void Shoot(){
        
        btVector3 ownerPos = boss->getWorldPosition();
        ownerPos.setY(ownerPos.getY()+10);
        
        PhysicsObject *projectile = new PhysicsObject(10, PhysicsShape::Box, btVector3(0,0,0), new Entity(Loader::LoadModel("res/Models/cube.obj"), Maths::bulletToGlm(Maths::glmToBullet(boss->getForwardVector()) * 40.0f + ownerPos), glm::vec3(0, 0, 0), vec3(1, 1, 1) * 5.0f, "", true));
        projectile->type = "Projectile";
        phyWorld.addPhysicsObject(projectile, COL_ENEMY, COL_FLOOR | COL_WALL | COL_PLAYER | COL_TRIGGER_PLAYER | COL_BOSS);
        AddGameObject(projectile);
        projectile->toggleContact(false);
        projectile->toggleGravity(false);
//        std::cout << "forward: " << boss->getForwardVector().x << " " << boss->getForwardVector().y << " " << boss->getForwardVector().z << endl;
        projectile->applyForce((player->getWorldPosition() - ownerPos).normalized() * 1000.0f);
    }
    
    void Render() {
        
        for (unsigned int i = 0; i < gameObjects.size(); i++) {
            gameObjects[i]->Render();
        }
        
        sfb.renderSceneOnBuffer();
        sfb.bindShadowMap();
        //fbo->bindFrameBuffer(); //comentar pra rodar no mac
        
        MasterRenderer::GetInstance().render(sl, pt, direct, camera);
        
        // fbo->unbindFrameBuffer(); //comentar pra rodar no mac
        // fbo->resolveToFbo(*output); //comentar pra rodar no mac
        // pp->doPostProcessing(output->colourTexture); //comentar pra rodar no mac
        
        guirenderer.render(GUITextures);
    };
};
