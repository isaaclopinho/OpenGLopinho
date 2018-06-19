#include "Camera.h"
#include "InputManager.h"
#include "Maths.h"

void Camera::calculateCameraPosition(float hDistance, float vDistance) {
	float theta = targetRotation.y + angleAroundTarget;
	float offsetX = hDistance * sin(radians(theta));
	float offsetZ = hDistance * cos(radians(theta));
	position.x = targetPosition.x + offsetX;
	position.y = targetPosition.y - vDistance;
	position.z = targetPosition.z + offsetZ;
}

float Camera::calculateHorizontalDistance() {
	return distanceFromTarget * glm::cos(radians(pitch));
}

float Camera::calculateVerticalDistance() {
	return distanceFromTarget * glm::sin(radians(pitch));
}



void Camera::calculateZoom(float z) {
	distanceFromTarget -= z;
	cout << "distance: "<<distanceFromTarget << endl;
}

void Camera::calculatePitch(float p) {
	pitch -= p;
	cout <<"pitch: "<< pitch << endl;
}

void Camera::calculateAngleAroundPlayer(float a) {
	angleAroundTarget += a;

	cout << "angle: " << angleAroundTarget << endl;
}


void Camera::Update(float delta,vec3 pos, vec3 rot) {

	targetPosition = pos;
	targetRotation = rot;

	if (InputManager::GetInstance().IsKeyDown(SDLK_F5)) {
		calculateAngleAroundPlayer(-0.3f);
	}

	if (InputManager::GetInstance().IsKeyDown(SDLK_F6)) {
		calculateAngleAroundPlayer(0.3f);
	}

	if (InputManager::GetInstance().IsKeyDown(SDLK_F7)) {
		calculatePitch(0.1f);
	}

	if (InputManager::GetInstance().IsKeyDown(SDLK_F8)) {
		calculatePitch(-0.1f);
	}

	if (InputManager::GetInstance().IsKeyDown(SDLK_F3)) {
		calculateZoom(0.3f);
	}

	if (InputManager::GetInstance().IsKeyDown(SDLK_F4)) {
		calculateZoom(-0.3f);
	}

	float horizontalDistance = calculateHorizontalDistance();
	float verticalDistance = calculateVerticalDistance();

	calculateCameraPosition(horizontalDistance, verticalDistance + vDist);
	yaw = - (targetRotation.y + angleAroundTarget);

	viewMatrix = Maths::createViewMatrix(*this);
}

void Camera::move(vec3 pos) { 
	position += pos; 
}

Camera::Camera() : position(vec3(0)) {
	projectionMatrix = Maths::createProjectionMatrix(FOV, NEAR_PLANE, FAR_PLANE);
	viewMatrix = Maths::createViewMatrix(*this);
}

Camera::Camera(vec3 position) : position(position) {
	projectionMatrix = Maths::createProjectionMatrix(FOV, NEAR_PLANE, FAR_PLANE);
	viewMatrix = Maths::createViewMatrix(*this);
}

Camera::~Camera() {

}