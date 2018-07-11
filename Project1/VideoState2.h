#pragma once
#pragma once
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
#include "gameSrc/LevelState.h"
#include "MenuState.h"
using namespace glm;
using namespace std;

class VideoState2 : public State {


public:
	VideoState2() {};

	~VideoState2() {}

	void Update(float dt) {
		Movie::playfile("res/videos/a.ogv", Game::GetInstance()->window, Game::GetInstance()->renderer);
		Game::GetInstance()->AddState(new MenuState());
		
	};

	void Render() {
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};
};
