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
	vector<GLuint> texturesText;
	vector<GUITexture> guiMenu;
	GUIRenderer guirenderer = GUIRenderer();

	float time = 0;
	float time2 = 0;
	int contador = 0;
	int contadorText = 0;
	float frames = 13;

public:
	MenuState()  {

		for (int i = 1; i <= frames; i++) {
			std::cout << "res/menu/animation/sp (" + to_string(i) + ").png" << endl;
			GLuint g = Loader::LoadTexture("res/menu/animation/sp (" + to_string(i) + ").png");
			texturesID.emplace_back(g);
		}

		GLuint g1 = Loader::LoadTexture("res/menu/1.png");
		GLuint g2 = Loader::LoadTexture("res/menu/2.png");

		texturesText.emplace_back(g1);
		texturesText.emplace_back(g2);


		guiMenu.emplace_back(GUITexture(texturesID[0], vec2(0,0), vec2(1,1)));
		guiMenu.emplace_back(GUITexture(texturesText[0], vec2(0, 0), vec2(1, 1)));


	};

	~MenuState() {
	}

	void Update(float dt) {
		
		time += dt;
		time2 += dt;


		if (time >= 1.0f / (float)frames) {
			contador++;

			if (contador > frames - 1)
				contador = 0;

			guiMenu[0].textureID = texturesID[contador];
			time = 0;
		}


		if (time2 >= 0.5f) {
			contadorText++;

			if (contadorText > 1)
				contadorText = 0;

			guiMenu[1].textureID = texturesText[contadorText];

			time2 = 0;
		}
		
		if (InputManager::GetInstance().KeyPress(SDLK_SPACE)) {
			Movie::playfile("res/videos/video.ogv", Game::GetInstance()->window, Game::GetInstance()->renderer);
			InputManager::GetInstance().ResetState();
			Game::GetInstance()->AddState(new LevelState());
		}


		if (InputManager::GetInstance().IsKeyDown(SDLK_ESCAPE)) {
			remove = true;
		}

	};

	void Render() {
		guirenderer.render(guiMenu);

	};
};
