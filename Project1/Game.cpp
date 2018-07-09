#include "Game.h"
#include "Debug.h"
#include "GameSrc/LevelState.h"
#include "GameSrc/BossState.h"
#include "TestState.h"
#include "VideoState2.h"

#include "MenuState.h"

#define FRAMETIME 16

Game* Game::instance = NULL;

Game::Game(string title, int width, int height) : dt(0), frameStart(SDL_GetTicks()) , WIDTH(width), HEIGHT(height) {

	newState = NULL;


	if (instance == nullptr)
		this->instance = this;


	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) != 0) {
		//SDL_GetError());
	}

	if ((window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL)) == nullptr) {
		//SDL_GetError());
	}

	if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) != 0) {
		cout << SDL_GetError() << endl;
	}

	// desabilitando deprecated code
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// seta versao para 3.2 do opengl
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//dessenha num buffer pra depois desenhar na tela
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
//    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);


	//cria contexto onde vai renderizar as parada
	if ((context = SDL_GL_CreateContext(window)) == nullptr) {
		//SDL_GetError());
	}

	SDL_GL_SetSwapInterval(0);

	/* Inicializa glew*/
	glewExperimental = GL_TRUE;

	if (GLenum res = glewInit() != GLEW_OK) {
		//glewGetErrorString(res));		
	}
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	ilInit();
	iluInit();
	ilutInit();
	ilutEnable(ILUT_OPENGL_CONV);
	ilutRenderer(ILUT_OPENGL);

	AudioSystem::Instance().Init();
}

float Game::GetAspectRatio() {
	return (float)WIDTH / (float)HEIGHT;
}

Game::~Game() {
	AudioSystem::Instance().Shutdown();
	aiDetachAllLogStreams();
	// Delete our OpengL context
	SDL_GL_DeleteContext(context);
	// Destroy our window
	SDL_DestroyWindow(window);
	// Shutdown SDL 2
	SDL_Quit();
}

SDL_GLContext Game::GetContext() {
	return context;
}

void Game::AddState(State * toAdd)
{
	newState = toAdd;
}

State * Game::GetState()
{
	return states.top();
}

Game* Game::GetInstance() {
	return instance;
}

void Game::Run() {
	//VideoState2 -> PS
	states.push(new VideoState2());



	while (!states.empty()) {
		CalculateDeltaTime();

		InputManager::GetInstance().Update();
		AudioSystem::Instance().Update(dt);

		if (InputManager::GetInstance().ControllerButtonPress(0))
//            std::cout << "Pau no cu do  lg\n\a";

		if (InputManager::GetInstance().KeyPress(SDLK_F1)) {
			//"FPS", 1 / dt);
		}

		states.top()->Update(dt);
		

		states.top()->Render();


		if (newState != NULL) {
			states.push(newState);
			newState = NULL;
		}

		if (states.top()->ShouldRemove()) {
			states.pop();
		}
		
		//Renders GL on SDL window
		SDL_GL_SwapWindow(window);
		
		
		int currentFrameTime = (SDL_GetTicks() - frameStart);
		if (currentFrameTime < FRAMETIME) {
			//double tEspera = (frameTime - dt) * 1000;
			SDL_Delay(FRAMETIME - currentFrameTime);
		}
	}
		

}

void Game::CalculateDeltaTime() {
	dt = (float)(SDL_GetTicks() - frameStart) / 1000;
	dt *= constant;
	frameStart = SDL_GetTicks();
}

float Game::GetDeltaTime() {
	return dt;
}

