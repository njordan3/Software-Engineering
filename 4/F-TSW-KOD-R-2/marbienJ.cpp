//program: marbienJ.cpp
//author: Marbien Jimeno
//
//This program contains function that will display picture

#ifndef MARBIEN_J_CPP
#define MARBIEN_J_CPP

#include "marbienJ.h" 
void showMarbienPicture(int x, int y, GLuint textid) {
    

  
    static int wid = 50;

    glPushMatrix();

    glTranslatef(x, y, 0);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, textid);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid); 
    glEnd();
    glTranslatef(wid,0,0);
    Rect r; 
    r.bot = 0; 
    r.left = 10; 
    r.center = 0;
    ggprint8b(&r, 16, 0x00000000, "Marbien Jimeno");
    glPopMatrix();

}

#endif
