#pragma once
#ifdef __APPLE__
#include <SDL2/SDL_opengl.h>
#else
#include <SDL_opengl.h>
#endif
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class BulletDebugDrawer : public btIDebugDraw {

public:

	BulletDebugDrawer() {};
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	void setDebugMode(int newDebugMode) override { debugMode = newDebugMode; };
	//int getDebugMode() override{ return debugMode; } ;

	void ToggleDebugFlag(int newFlag);

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {};

	void reportErrorWarning(const char* warningString) override {};

	void draw3dText(const btVector3& location, const char* textString) override {};

protected:

	int debugMode = 1;

};
