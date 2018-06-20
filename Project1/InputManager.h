//X360 CONTROLLER
//BOTÕES
#define MAXAXIS 32768
#define X360_A 0
#define X360_B 1
#define X360_X 2
#define X360_Y 3
#define X360_BACK 4
#define X360_HOME 5
#define X360_START 6
#define X360_LS 7
#define X360_RS 8
#define X360_LB 9
#define X360_RB 10
#define X360_DPADUP 11
#define X360_DPADDOWN 12
#define X360_DPADLEFT 13
#define X360_DPADRIGHT 14
//EIXOS
#define X360_LEFTSTICK_X 0
#define X360_LEFTSTICK_Y 1
#define X360_RIGHTSTICK_X 2
#define X360_RIGHTSTICK_Y 3
#define X360_LT 4
#define X360_RT 5


#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <unordered_map>


class InputManager
{
private:
	bool mouseState[6];
	int mouseUpdate[6];

	bool controllerButtonState[15];
    int controllerButtonUpdate[15];
    
    float controllerAxisValues[6];

	bool quitRequested;
	int mouseX;
	int mouseY;
	int updateCounter;
	std::unordered_map<int, bool> keyState;
	std::unordered_map<int, int> keyUpdate;
	SDL_GameController *controle;

public:
	
	InputManager() : quitRequested(false), mouseX(0), mouseY(0), updateCounter(0) {
		for (int i = 0; i < 6; i++)
			mouseState[i] = mouseUpdate[i] = controllerAxisValues[x] = 0;
		for(int x = 0; x < 15; x++){
        	controllerButtonState[x] = 0;
        	controllerButtonUpdate[x] = 0;
    	}

    	controle = SDL_GameControllerOpen(0);
	}

	~InputManager() {
	}

	static InputManager& GetInstance() {
		static InputManager inputManager;
		return inputManager;
	}

	void Update() {

		SDL_Event event;
		SDL_GetMouseState(&mouseX, &mouseY);
		quitRequested = false;
		updateCounter++;

		// SDL_PollEvent retorna 1 se encontrar eventos, zero caso contrário
		while (SDL_PollEvent(&event)) {
			// Se o evento for quit, setar a flag para terminação
			if (event.type == SDL_QUIT) {
				quitRequested = true;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				mouseState[event.button.button] = true;
				mouseUpdate[event.button.button] = updateCounter;
			}
			if (event.type == SDL_MOUSEBUTTONUP) {
				mouseState[event.button.button] = false;
				mouseUpdate[event.button.button] = updateCounter;
			}
			if (!event.key.repeat) {
				if (event.type == SDL_KEYDOWN) {
					keyState[event.key.keysym.sym] = true;
					keyUpdate[event.key.keysym.sym] = updateCounter;
				}
			}
			if (event.type == SDL_KEYUP) {
				keyState[event.key.keysym.sym] = false;
				keyUpdate[event.key.keysym.sym] = updateCounter;
			}
		}
	}

	bool KeyPress(int key) {
		return keyUpdate[key] == updateCounter && keyState[key];

	}

	bool KeyRelease(int key) {
		return keyUpdate[key] == updateCounter && !keyState[key];
	}

	bool IsKeyDown(int key) {
		return  keyState[key];
	}

	bool MousePress(int key) {
		return mouseUpdate[key] == updateCounter && mouseState[key];
	}

	bool MouseRelease(int key) {
		return mouseUpdate[key] == updateCounter && !mouseState[key];
	}

	bool IsMouseDown(int key) {
		return mouseState[key];
	}

	int GetMouseX() {
		return mouseX;
	}

	int GetMouseY() {
		return mouseY;
	}

	bool QuitRequested() {
		return quitRequested;
	}

	bool ControllerButtonPress(int key){
    if(controllerButtonUpdate[key] == updateCounter){
        return controllerButtonState[key];
    }
    else{
        return false;
    }
	}

	bool ControllerButtonRelease(int key){
	    if(controllerButtonUpdate[key] == updateCounter){
	        return !controllerButtonState[key];
	    }
	    else{
	        return false;
	    }
	}

	bool IsControllerButtonDown(int key){
	    return controllerButtonState[key];
	}

	float ControllerAxisValue(int value){
	    return controllerAxisValues[value];
	}

	void UpdateController(){
	    
	//    if(SDL_GameControllerGetAttached(controle) == SDL_TRUE){
	//        cout << "controle conectado" << endl;
	//    }
	//    else{
	//        cout << "controle não conectado" << endl;
	//    }

	    
	    //Percorre os botões
	    for (int i = 0; i < 15; i++){
	        if(SDL_GameControllerGetButton(controle, (SDL_GameControllerButton)i)){
	            if(!controllerButtonState[i]){
	                controllerButtonState[i] = true;
	                controllerButtonUpdate[i] = updateCounter;
	            }
	        }
	        else{
	            if(controllerButtonState[i]){
	                controllerButtonState[i] = false;
	                controllerButtonUpdate[i] = updateCounter;
	            }
	        }
	    }
	    
	    //Percorre os eixos
	    for(int i = 0; i < 6; i++){
	        controllerAxisValues[i] = SDL_GameControllerGetAxis(controle, (SDL_GameControllerAxis)i) / (float)MAXAXIS;
	    }
	    
	}

};

