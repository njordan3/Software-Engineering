//3350
//program: adamO.cpp
//author: adam ostlund
//date: february 14, 2019
//
//This program is an individual source file containing a name and profile picture.
//
//

#include "adamO.h"

void showAdamPicture(int x, int y, GLuint textid) {
    

    
    
    glColor3ub(255,255,255);
    static int wid = 50;
    //float fx = (float)x + 100;
    //float fy = (float)y + 100;;
    glPushMatrix();
    glTranslatef(x,y,0);
    glBindTexture(GL_TEXTURE_2D, textid);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glTranslatef(wid,0,0);
    Rect r;
    r.bot = 0;
    r.left = 10;
    r.center = 0;
    ggprint8b (&r, 16, 0x00000000, "Adam Ostlund");
    glPopMatrix();
}
