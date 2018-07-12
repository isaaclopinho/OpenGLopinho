#pragma once
#include <glm/glm.hpp>
#include <iostream>

using namespace std;
using namespace glm;


class Camera
{
private:

	const float FOV = 70;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000;

public:

	mat4 viewMatrix;
	mat4 projectionMatrix;

	float distanceFromTarget = 10;
	float angleAroundTarget = 0;
	vec3 position = glm::vec3();
	float pitch = 0;
	float yaw = 0;
	float roll;

	vec3 targetPosition;
	vec3 targetRotation;

	float vDist = 0;

	void calculateCameraPosition(float hDistance, float vDistance);
	float calculateHorizontalDistance();
	float calculateVerticalDistance();
	void calculateZoom(float z);
	void calculatePitch(float p);
	void calculateAngleAroundPlayer(float a);
	void Update(float delta, vec3 pos, vec3 rot); 	
	void move(vec3 pos);
	Camera();	
	Camera(vec3 position);
	~Camera();
    
    void ApplyShake(vec2 noise, vec3 pos);
    void EnableShake();
    void DisableShake();
    vec2 PerlinShake();
};

