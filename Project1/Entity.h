#pragma once
#include <glm\glm.hpp>
#include "TexturedModel.h"
#include "Mesh.h"
#include "Animator.h"

using namespace glm;

class Entity
{
public:
	Mesh * mesh;
	vec3 position;
	vec3 rotation;
	//Animator anim;
	std::string currentAnimation;
	float scale;
	vector<mat4>transforms;
	float time;
	bool loopAnimation;

	//int textureIndex;

	Entity(Mesh* m, glm::vec3 p, glm::vec3 r, float s, std::string startAnimation, bool loopAnimation);
	//Entity(glm::vec3 position, glm::vec3 rotation, float scale, int textureIndex); //varias textura em um obj
	~Entity();

	void Translate(vec3 pos);
	void Rotate(vec3 rot);
	Mesh* GetMesh();
	
	void Update(float dt);
	vector<mat4> GetTransforms();

	void ChangeAnimation(std::string newAnimation, bool loops);
};

