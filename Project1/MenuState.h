#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <glm/glm.hpp>
#include "Game.h"
#include "State.h"
#include "GameCamera.h"
#include "Loader.h"
#include "MasterRenderer.h"
#include "GUITexture.h"
#include "GUIRenderer.h"
#include "Mesh.h"
#include "Movie.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Light.h"
#include "InputManager.h"
#include "ParticleSystem.h"
using namespace glm;
using namespace std;

class MenuState : public State {


	vector<GLuint> texturesID;
	vector<GLuint> texturesButtonNormal;
	vector<GLuint> texturesButtonSelected;

	vector<GUITexture> guiMenu;
	GUIRenderer guirenderer = GUIRenderer();

	float time = 0;
	float time2 = 0;
	int contador = 0;
	int contadorText = 0;
	float frames = 8;

	int op = 0;
	int opMax = 2;

public:
	MenuState()  {

		for (int i = 1; i <= frames; i++) {
			std::cout << "res/menu/animation/sp (" + to_string(i) + ").png" << endl;
			GLuint g = Loader::LoadTexture("res/menu/animation/sp (" + to_string(i) + ").png");
			texturesID.emplace_back(g);
		}


		texturesButtonNormal.emplace_back(Loader::LoadTexture("res/menu/start.png"));
		texturesButtonNormal.emplace_back(Loader::LoadTexture("res/menu/exit.png"));

		texturesButtonSelected.emplace_back(Loader::LoadTexture("res/menu/start2.png"));
		texturesButtonSelected.emplace_back(Loader::LoadTexture("res/menu/exit2.png"));


		guiMenu.emplace_back(GUITexture(texturesID[0], vec2(0,0), vec2(1,1)));
		guiMenu.emplace_back(GUITexture(texturesButtonNormal[0], vec2(0, -0.4), vec2(0.35, 0.11)));
		guiMenu.emplace_back(GUITexture(texturesButtonNormal[1], vec2(0, -0.7), vec2(0.35, 0.11)));


	};

	~MenuState() {
	}

	void Update(float dt) {
		
		time += dt;




		if (time >= 1.0f / (float)frames) {
			contador++;

			if (contador > frames - 1)
				contador = 0;

			guiMenu[0].textureID = texturesID[contador];
			time = 0;
		}


		if (InputManager::GetInstance().KeyPress(SDLK_UP)) {

			op++;

			if (op > opMax-1)
				op = 0;
			
		}else if (InputManager::GetInstance().KeyPress(SDLK_DOWN)) {

			op--;

			if (op < 0)
				op = opMax-1;
		}

		guiMenu[1].textureID = op == 0 ? texturesButtonNormal[0] : texturesButtonSelected[0];
		guiMenu[2].textureID = op == 1 ? texturesButtonNormal[1] : texturesButtonSelected[1];
		
		
		if (InputManager::GetInstance().KeyPress(SDLK_SPACE)) {

			switch (op) {
				case 0: 
					Movie::playfile("res/videos/video.ogv", Game::GetInstance()->window, Game::GetInstance()->renderer);
					InputManager::GetInstance().ResetState();
					Game::GetInstance()->AddState(new LevelState());
					break;
				case 1: 
					remove = true;
					break;
			}
		}



	};

	void Render() {

		guirenderer.render(guiMenu);

	};
};
