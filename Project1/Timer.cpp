//
//  Timer.cpp
//  OpenGLopinho2
//
//  Created by Thiago Dantas on 07/07/18.
//  Copyright © 2018 Thiago Dantas. All rights reserved.
//

#include <stdio.h>
#include "Timer.h"

Timer::Timer(float cooldown) : cooldown(cooldown), currentTime(0){
    
}

/// brief: Recomeça o timer;
void Timer::Reset(){
    currentTime = 0;
}
/// brief: Verdadeiro se estiver em cooldown
bool Timer::IsInCooldown(){
    return currentTime < cooldown;
}

/// brief: altera o cooldown (tempo de espera)
void Timer::SetCooldown(float cooldown){
    this->cooldown = cooldown;
    currentTime = 0;
}
/// brief: atualiza a passagem do tempo;
void Timer::Update(float dt){
    currentTime += dt;
}
