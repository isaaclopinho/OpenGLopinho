#pragma once
// ÅEo debug iniciante
#include <iostream>
#include <glm/glm.hpp>
#include <string>

#ifndef DEBUGINICIANTE_H
#define DEBUGINICIANTE_H
//#define USEDEBUG 0

#ifdef USEDEBUG
#define //x) std::cout << "Debug: (" << x << ")\n"
#define //x, y) std::cout << "Debug: (" << x << ", " << y << ")\n"
#define //x, y, z) std::cout << "Debug: (" << x << ", " << y << ", " << z << ")\n"
#define //x, y, z, w) std::cout << "Debug: (" << x << ", " << y << ", " << z << ", " << w << ")\n"
#define Debugv(v) std::cout << v.x << " " << v.y << " " << v.z << "\n";
#define //x)
#define //x,y)
#define //x,y,z)
#define //x,y,z,w)
#else


#endif

#endif 



