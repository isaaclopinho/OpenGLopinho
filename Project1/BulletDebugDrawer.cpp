#include "BulletDebugDrawer.h"

#include <stdio.h> //printf debugging

BulletDebugDrawer::BulletDebugDrawer()
:m_debugMode(0)
{
    
}

void    BulletDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
    //      if (m_debugMode > 0)
    {
        float tmp[ 6 ] = { from.getX(), from.getY(), from.getZ(),
            to.getX(), to.getY(), to.getZ() };
        
        glPushMatrix();
        {
            glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);
            glVertexPointer( 3,
                            GL_FLOAT,
                            0,
                            &tmp );
            
            glPointSize( 5.0f );
            glDrawArrays( GL_POINTS, 0, 2 );
            glDrawArrays( GL_LINES, 0, 2 );
        }
        glPopMatrix();
    }
    // Versão PDF LG
//    {
//        // draws a simple line of pixels between points.
//        // use the GL_LINES primitive to draw lines
//        glBegin(GL_LINES);
//        glColor3f(color.getX(), color.getY(), color.getZ());
//        glVertex3f(from.getX(), from.getY(), from.getZ());
//        glVertex3f(to.getX(), to.getY(), to.getZ());
//        glEnd();
//        Chapter 4
//        Debug rendering
//    }
    
}

void    BulletDebugDrawer::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

void    BulletDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
    //glRasterPos3f(location.x(),  location.y(),  location.z());
    //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void    BulletDebugDrawer::reportErrorWarning(const char* warningString)
{
    printf(warningString);
}

void    BulletDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
    {
        //btVector3 to=pointOnB+normalOnB*distance;
        //const btVector3&from = pointOnB;
        //glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);
        
        //BulletDebugDrawer::drawLine(from, to, color);
        
        //glRasterPos3f(from.x(),  from.y(),  from.z());
        //char buf[12];
        //sprintf(buf," %d",lifeTime);
        //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
    }
}
