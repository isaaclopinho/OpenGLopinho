//
//  Timer.h
//  OpenGLopinho2
//
//  Created by Thiago Dantas on 07/07/18.
//  Copyright © 2018 Thiago Dantas. All rights reserved.
//

#ifndef Timer_h
#define Timer_h

class Timer{
    float currentTime;
    float cooldown;
    
public:
    Timer(float cooldown);
    void Reset();
    bool IsInCooldown();
    void SetCooldown(float cooldown);
    void Update(float dt);
};

#endif /* Timer_h */
