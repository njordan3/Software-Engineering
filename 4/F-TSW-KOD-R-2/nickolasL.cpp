
#ifndef NICKOLAS_L_CPP
#define NICKOLAS_L_CPP

#include "nickolasL.h"



GameState::GameState(){ CREDITS = false;}


void draw_nickLCredit(int x, int y, GLuint texture)
{
	
	Rect r;
	float wid = 50;
	glPushMatrix();
	glTranslatef(x,y,0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glTranslatef(wid,0,0);
	//unsigned int c = 0x00000000;
        r.bot = 0;
        r.left = 10;
        r.center = 0;
        ggprint8b(&r, 16, 0x00000000, "Nickolas Larson");


	glPopMatrix();
}



#endif
