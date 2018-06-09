#include "BulletDebugDrawer.h"

void BulletDebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{

	{

		glBegin(GL_LINES);
		glColor3f(color.getX(), color.getY(), color.getZ());
		glVertex3f(from.getX(), from.getY(), from.getZ());
		glVertex3f(to.getX(), to.getY(), to.getZ());
		glEnd();

	}
}

void BulletDebugDrawer::ToggleDebugFlag(int flag) {

	if (debugMode & flag)
		debugMode = debugMode & (~flag);
	else
		debugMode |= flag;
}
