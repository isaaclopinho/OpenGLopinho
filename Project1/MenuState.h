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
#include "VideoState.h"
#include "ParticleSystem.h"
#include "AudioSource.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;
using namespace std;

class MenuState : public State {


	vector<GLuint> texturesID;
	vector<GLuint> textureStart;
	vector<GLuint> textureExit;
	vector<GLuint> textureIcone;



	vector<GUITexture> guiMenu;
	GUIRenderer guirenderer = GUIRenderer();

	float time = 0;
	float time2 = 0;
	int contador = 0;
	int contadorText = 0;
	float frames = 201;

	int op = 0;
	int opMax = 2;

	bool bloqueado = false;



	float timeStartButton = 0;
	int contadorFrameStartButton = 0;
	int contadorFrameExitButton = 0;
	int contadorIcone = 0;

	float timeIcone = 0;

	bool startButton = true;

	float timeFadeout = 0;

	int c = 0;

	std::unique_ptr<AudioSource> fireSource, cicadaSource, forestSource;

public:
	MenuState() {

		for (int i = 1; i <= frames; i++) {
			std::cout << "res/menu/animation/sp (" + to_string(i) + ").png" << endl;
			GLuint g = Loader::LoadTexture("res/menu/animation/sp (" + to_string(i) + ").png");
			texturesID.emplace_back(g);
		}

		for (int i = 1; i <= 12; i++) {
			std::cout << "res/menu/start/sp (" + to_string(i) + ").png" << endl;
			GLuint g = Loader::LoadTexture("res/menu/start/sp (" + to_string(i) + ").png");
			GLuint g2 = Loader::LoadTexture("res/menu/exit/sp (" + to_string(i) + ").png");
			textureStart.emplace_back(g);
			textureExit.emplace_back(g2);
		}


		for (int i = 1; i <= 60; i++) {
			std::cout << "res/menu/icone2/sp (" + to_string(i) + ").png" << endl;
			GLuint g = Loader::LoadTexture("res/menu/icone2/sp (" + to_string(i) + ").png");
			textureIcone.emplace_back(g);
		}



		float offset = 0.0f;


		guiMenu.emplace_back(GUITexture(texturesID[0], vec2(0,0), vec2(1,1)));
		guiMenu.emplace_back(GUITexture(textureStart[0], vec2(0.57f, -0.10 - offset), vec2(125.0f / 1280.0f, 88.0f/720.0f)*1.1f ));
		guiMenu.emplace_back(GUITexture(textureExit[0], vec2(0.57f, -0.40 - offset), vec2(93.0f / 1280.0f, 83.0f / 720.0f)* 1.1f ));
		guiMenu.emplace_back(GUITexture(textureIcone[0], vec2(0.57f, 0.35f - offset), vec2(277.0f / 1280.0f, 143.0f / 720.0f) * 1.5f));

		fireSource = make_unique<AudioSource>("res/sounds/fireplace.wav", true); fireSource->SetGain(10);
		cicadaSource = make_unique<AudioSource>("res/sounds/cicada.wav", true); cicadaSource->SetGain(10);
		forestSource = make_unique<AudioSource>("res/sounds/forest.wav", true); forestSource->SetPosition(0, -10, 0);
		
	};

	~MenuState() {
	}

	void Update(float dt) {
		
		if (c == 0) {
			if (fireSource->IsPlaying())
				fireSource->Play(); cicadaSource->Play(); forestSource->Play();
			c++;
		}

		timeIcone += dt;
		if (timeIcone >= 2.0f / (float)textureIcone.size()) {
			contadorIcone++;

			if (contadorIcone >= textureIcone.size())
				contadorIcone = 0;

			guiMenu[3].textureID = textureIcone[contadorIcone];


			timeIcone = 0;
		}

		if (!bloqueado) {

			

			time += dt;



			if (time >= 1.0/30.0) {
				contador++;

				if (contador > frames - 1)
					contador = 0;

				guiMenu[0].textureID = texturesID[contador];
				time = 0;
			}


			if (InputManager::GetInstance().KeyPress(SDLK_UP)) {

				op++;

				if (op > opMax - 1)
					op = 0;

			}
			else if (InputManager::GetInstance().KeyPress(SDLK_DOWN)) {

				op--;

				if (op < 0)
					op = opMax - 1;
			}

			switch (op) {
			case 0:
				startButton = true;
				break;
			case 1:
				startButton = false;
				break;
			}


			if (startButton) {

				timeStartButton += dt;

				if (timeStartButton >= 0.5f/12.0f) {
					guiMenu[1].textureID = textureStart[contadorFrameStartButton];
					guiMenu[2].textureID = textureExit[contadorFrameExitButton];
					timeStartButton = 0;
					contadorFrameStartButton = glm::clamp(contadorFrameStartButton + 1, 0, 11);
					contadorFrameExitButton = glm::clamp(contadorFrameExitButton - 1, 0, 11);
	
				}

			} else {

				timeStartButton += dt;

				if (timeStartButton >= 0.5f/12.0f) {
					guiMenu[1].textureID = textureStart[contadorFrameStartButton];
					guiMenu[2].textureID = textureExit[contadorFrameExitButton];
					timeStartButton = 0;
					contadorFrameStartButton = glm::clamp(contadorFrameStartButton - 1, 0, 11);
					contadorFrameExitButton = glm::clamp(contadorFrameExitButton + 1, 0, 11);
				}

			}

			
			// guiMenu[1].textureID = op == 0 ? texturesButtonNormal[0] : texturesButtonSelected[0];
			//guiMenu[2].textureID = op == 1 ? texturesButtonNormal[1] : texturesButtonSelected[1];


			if (InputManager::GetInstance().KeyPress(SDLK_SPACE)) {

				switch (op) {
				case 0:
					bloqueado = true;
					break;
				case 1:
					exit(1);
					break;
				}
			}
		}
		else {
			time += dt;
			timeFadeout += dt;

			float t = timeFadeout / 2.0f;

			fireSource->SetGain((1 - t) * 10);
			cicadaSource->SetGain((1 - t) * 10);
			forestSource->SetGain((1-t));

			for(int i=1; i<4; i++)
				guiMenu[i].constant = lerp(1.0f, 0.0f, t);


			if (time >= 1.0f / (float)30) {
				contador++;

				if (contador > frames - 1) {
					contador = frames - 1;

					if (t >= 1) {
						Game::GetInstance()->AddState(new VideoState());
						fireSource->Stop();
						cicadaSource->Stop();
						forestSource->Stop();
					}
				}

				guiMenu[0].textureID = texturesID[contador];
				time = 0;
			}
		}


	};

	void Render() {
		glClearColor(1, 1, 1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		guirenderer.render(guiMenu);

	};
};
