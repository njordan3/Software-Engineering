#ifndef ADAMO_CPP
#define ADAMO_CPP
// Adam Ostlund
// This file is for displaying the User Interface.
// Last Update: May 3, 2019
#include "adamO.h"
#include "nicholasJo.h"
#include "brandonH.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "fonts.h"
#include <string>

extern NJordGlobal njG;
extern BHglobal bhg;

AOglobal::AOglobal() {
    Boxes box[11] = {
        Boxes(BOX),
        Boxes(END_TURN),
        Boxes(BOX),
        Boxes(BOX),
        Boxes(BOX),
        Boxes(BOX),
        Boxes(BOX),
        Boxes(BOX),
        Boxes(BOX),
        Boxes(BOX),
        Boxes(BOX)
    };
        (void)box;
        BattleBoxes bbox[3] = {
        BattleBoxes(BATTLE),
        BattleBoxes(BATTLE),
        BattleBoxes(BATTLE)
    };
        (void)bbox;
        AllyBoxes abox[4] = {
        AllyBoxes(ALLY),
        AllyBoxes(ALLY),
        AllyBoxes(ALLY)
    };
        (void)abox;
}

AOglobal aog;

Boxes::Boxes() {
}

BattleBoxes::BattleBoxes() {
}

AllyBoxes::AllyBoxes() {
}

Boxes::Boxes(stateID sID) {
    ID = sID;
}

BattleBoxes::BattleBoxes(stateID sID) {
    ID = sID;
}

AllyBoxes::AllyBoxes(stateID sID) {
    ID = sID;
}

//================================//
//--------- BOX POSITIONS --------//
//================================//

void uiboxes::posBoxes() {

    // Character Frame Box
    boxes[0].width = (xres*0.0625) + 35;
    boxes[0].height = (yres/6);
    boxes[0].center.x = ((xres*11)/120);
    boxes[0].center.y = (yres/6);
    //

    // Character Pane Box
    boxes[1].width = (xres*0.0625) + 30;
    boxes[1].height = (yres/6) - 5;
    boxes[1].center.x = (xres*11)/120; 
    boxes[1].center.y = (yres/6);
    //

    // Character Image Box
    boxes[2].width = 75;
    boxes[2].height = 75;
    boxes[2].center.x = 110;
    boxes[2].center.y = 195;
    //

    // Character Info Background Box
    boxes[3].width = 75;
    boxes[3].height = 40;
    boxes[3].center.x = 110;
    boxes[3].center.y = 65;
    //

    // Character Name Box
    boxes[4].width = 70;
    boxes[4].height = 10;
    boxes[4].center.x = 110;
    boxes[4].center.y = 90;
    //

    // Character Health Bar Box
    boxes[5].width = 70;
    boxes[5].height = 10;
    boxes[5].center.x = 110;
    boxes[5].center.y = 65;
    //

    // Character Movements Left Box
    boxes[6].width = 70;
    boxes[6].height = 10;
    boxes[6].center.x = 110;
    boxes[6].center.y = 40;
    //

    // Character Gold Count Background
    boxes[7].width = 75;
    boxes[7].height = 27.5;
    boxes[7].center.x = xres-75;
    boxes[7].center.y = 27.5;
    //

    // Character GOLD
    boxes[8].width = 70;
    boxes[8].height = 10;
    boxes[8].center.x = xres-75;
    boxes[8].center.y = 40;
    //

    // Character Gold Count
    boxes[9].width = 70;
    boxes[9].height = 10;
    boxes[9].center.x = xres-75;
    boxes[9].center.y = 15;
    //
}

void battleboxes::posBattleBoxes(int x) {

    // Battle Mode Box
    btboxes[0].width = 75;
    btboxes[0].height = 27.5;
    btboxes[0].center.x = xres - x;
    btboxes[0].center.y = yres - 27.5;
    //

    // Enemy Name Box
    btboxes[1].width = 70;
    btboxes[1].height = 10;
    btboxes[1].center.x = xres - x;
    btboxes[1].center.y = yres - 15;
    //
       
    // Enemy Health Bar Box
    btboxes[2].width = 70;
    btboxes[2].height = 10;
    btboxes[2].center.x = xres - x;
    btboxes[2].center.y = yres - 40;
    //
}

void allyboxes::posAllyBoxes(int y) {

    // Ally Background Box
    alboxes[0].width = 75;
    alboxes[0].height = 40;
    alboxes[0].center.x = 75;
    alboxes[0].center.y = yres - y - 12.5;
    //

    // Ally Name Box
    alboxes[1].width = 70;
    alboxes[1].height = 10;
    alboxes[1].center.x = 75; 
    alboxes[1].center.y = yres - y + 12.5;
    //
       
    // Ally Health Bar Box
    alboxes[2].width = 70;
    alboxes[2].height = 10;
    alboxes[2].center.x = 75;
    alboxes[2].center.y = yres - y - 12.5;
    //

    // Ally Turns Box
    alboxes[3].width = 70;
    alboxes[3].height = 10;
    alboxes[3].center.x = 75;
    alboxes[3].center.y = yres - y - 37.5;
    //
}

//================================//
//-------- DRAW FUNCTIONS --------//
//================================//

void uiboxes::drawBoxes() {

   glPushMatrix(); 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, xres, 0, yres, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

    for (int i = 0; i < 10; i++) {
        Boxes *s;
        glColor3ub(90, 140, 90);
        if (i > 3 || i == 1)
            glColor3ub(40,40,40);
        if ( i == 2 || i == 3 || i == 0 || i == 7)
            glColor3ub(130,130,130);
        if (i == 4)
            glColor3ub(20, 20, 100);
        s = &boxes[i];
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(s->center.x, s->center.y, 0);
        float w, h;
        w = s->width;
        h = s->height;
        glBegin(GL_QUADS);
        glVertex2i(-w, -h);
        glVertex2i(-w,  h);
        glVertex2i( w,  h);
        glVertex2i( w, -h);
        glEnd();
        glPopMatrix();
    }
    
    Rect r;
    Player *player = Player::getInstance(); 
    r.bot = 78;
    r.left = 110;
    const char* pname = njG.name.c_str();
    ggprint16(&r, 16, 0xffffffff, pname);
    
    r.bot = 53;
    r.left = 90;
    string hc;
    hc = std::to_string((int)player->getCurrentHealth());
    if (player->getCurrentHealth() <= player->getMaxHealth() / 2)
        ggprint16(&r, 16, 0xfff00000, hc.c_str());
    else
        ggprint16(&r, 16, 0xffffffff, hc.c_str());
    
    r.bot = 53;
    r.left = 130;
    string hm;
    hm = std::to_string((int)player->getMaxHealth());
    if (player->getCurrentHealth() <= player->getMaxHealth() / 2)
        ggprint16(&r, 16, 0xfff00000, hm.c_str());
    else 
        ggprint16(&r, 16, 0xffffffff, hm.c_str());
    
    r.bot = 53;
    r.left = 110;
    ggprint16(&r, 16, 0xffffffff, "/");
    
    r.bot = 28;
    r.left = 110;
    string mv = to_string(player->moveRange);
    string mv2 = mv;
    ggprint16(&r, 16, 0xffffffff, mv2.c_str());

    r.bot = 28;
    r.left = xres - 75;
    ggprint16(&r, 16, 0xffffffff, "Gold");
    
    r.bot = 3;
    r.left = xres - 75;
    string gold = to_string(player->score);
    ggprint16(&r, 16, 0xffffff00, gold.c_str());
    
    glPushMatrix();
    glTranslatef(boxes[2].center.x, boxes[2].center.y, 0);
    glColor3ub(250,250,250);
    glBindTexture(GL_TEXTURE_2D, player->image);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-boxes[2].width+5,-boxes[2].width+5);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-boxes[2].width+5, boxes[2].width-5);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( boxes[2].width-5, boxes[2].width-5);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( boxes[2].width-5,-boxes[2].width+5);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    
    glPopMatrix();
}

void battleboxes::drawBattleBoxes(int enemy) {
   
    glPushMatrix(); 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, xres, 0, yres, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    for (int j = 0; j < njG.enemyCount/njG.enemyArrayCount; j++) {
        posBattleBoxes(75 + (75*j*2));
        if(1) {
        for (int i = 0; i < 3; i++) {
            BattleBoxes *s;
            glColor3ub(90, 140, 90);
            if (i == 0)    
                glColor3ub(130,130,130);
            if (i == 1)
                glColor3ub(100, 20, 20);
            if (i == 2)
                glColor3ub(40,40,40);
            s = &btboxes[i];
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(s->center.x, s->center.y, 0);
            float w, h;
            w = s->width;
            h = s->height;
            glBegin(GL_QUADS);
            glVertex2i(-w, -h);
            glVertex2i(-w,  h);
            glVertex2i( w,  h);
            glVertex2i( w, -h);
            glEnd();
            glPopMatrix();
        }
        }

        glPopMatrix();  
        
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, xres, 0, yres, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        Rect r;
        r.bot = yres - 27;
        r.left = xres - 75 - (150*(j));
        ggprint16(&r, 16, 0xffffffff, njG.enemies[enemy][j].combatType.c_str());
              
        r.bot = yres - 52;
        r.left = xres - 95 - (150*(j));
        string ehc;
        ehc = std::to_string((int)njG.enemies[enemy][j].getCurrentHealth());
        if (njG.enemies[enemy][j].getCurrentHealth() <= njG.enemies[enemy][j].getMaxHealth() / 2)
            ggprint16(&r, 16, 0xfff00000, ehc.c_str());
        else
            ggprint16(&r, 16, 0xffffffff, ehc.c_str());
    
        r.bot = yres - 52;
        r.left = xres - 55 - (150*(j));
        string ehm;
        ehm = std::to_string((int)njG.enemies[enemy][j].getMaxHealth());
        if (njG.enemies[enemy][j].getCurrentHealth() <= njG.enemies[enemy][j].getMaxHealth() / 2)
            ggprint16(&r, 16, 0xfff00000, ehm.c_str());
        else 
            ggprint16(&r, 16, 0xffffffff, ehm.c_str());
    
        r.bot = yres - 52;
        r.left = xres - 75 - (150*(j));
        ggprint16(&r, 16, 0xffffffff, "/");
        glPopMatrix();
    }
}

void allyboxes::drawAllyBoxes() {
   
    glPushMatrix(); 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, xres, 0, yres, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    for (int j = 0; j < njG.allies->count; j++) {

        posAllyBoxes(387.5 + (40*j*2));
        if(1) {
        for (int i = 0; i < 4; i++) {
            AllyBoxes *s;
            glColor3ub(90, 140, 90);
            if (i == 0)    
                glColor3ub(130,130,130);
            if (i == 1)
                glColor3ub(20, 100, 20);
            if (i >= 2)
                glColor3ub(40,40,40);
            s = &alboxes[i];
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(s->center.x, s->center.y, 0);
            float w, h;
            w = s->width;
            h = s->height;
            glBegin(GL_QUADS);
            glVertex2i(-w, -h);
            glVertex2i(-w,  h);
            glVertex2i( w,  h);
            glVertex2i( w, -h);
            glEnd();
            glPopMatrix();
        }
        }

        glPopMatrix();  
        
        glPushMatrix(); 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, xres, 0, yres, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        Rect r;
        r.bot = alboxes[1].center.y - 12.5;
        r.left = 75;
        ggprint16(&r, 16, 0xffffffff, njG.allies[j].combatType.c_str());
        
        r.bot = alboxes[2].center.y - 12.5;
        r.left = 55;
        string ahc;
        ahc = std::to_string((int)njG.allies[j].getCurrentHealth());
        if (njG.allies[j].getCurrentHealth() <= njG.allies[j].getMaxHealth() / 2)
            ggprint16(&r, 16, 0xfff00000, ahc.c_str());
        else
            ggprint16(&r, 16, 0xffffffff, ahc.c_str());
    
        r.bot = alboxes[2].center.y - 12.5;
        r.left = 95;
        string ahm;
        ahm = std::to_string((int)njG.allies[j].getMaxHealth());
        if (njG.allies[j].getCurrentHealth() <= njG.allies[j].getMaxHealth() / 2)
            ggprint16(&r, 16, 0xfff00000, ahm.c_str());
        else 
            ggprint16(&r, 16, 0xffffffff, ahm.c_str());
    
        r.bot = alboxes[2].center.y - 12.5;
        r.left = 75;
        ggprint16(&r, 16, 0xffffffff, "/");
        
        r.bot = alboxes[3].center.y - 12.5;
        r.left = 75;
        string amv = to_string(njG.allies[j].moveRange);
        string amv2 = amv;
        ggprint16(&r, 16, 0xffffffff, amv2.c_str());
        
        glPopMatrix();
    }
}
         
//=====================================//
//------------- UI BOXES --------------//
//=====================================//

uiboxes::uiboxes(Boxes b[10], float xres, float yres)
{
    size = 10;
    this->xres = xres;
    this->yres = yres;
    for(int i = 0; i < size; i++)
        boxes[i] = b[i];
    posBoxes();

}

//=====================================//
//----------- BATTLE BOXES ------------//
//=====================================//

battleboxes::battleboxes(BattleBoxes bb[3], float xres, float yres)
{
    size = 3;
    this->xres = xres;
    this->yres = yres;
    for(int i = 0; i < size; i++)
        btboxes[i] = bb[i];
    posBattleBoxes(75);
}

//=====================================//
//------------ ALLY BOXES -------------//
//=====================================//

allyboxes::allyboxes(AllyBoxes ab[4], float xres, float yres)
{
    size = 4;
    this->xres = xres;
    this->yres = yres;
    for(int i = 0; i < size; i++)
        alboxes[i] = ab[i];
    posAllyBoxes(75);
}

void showAdamPicture(int x, int y, GLuint textid) {
    glColor3ub(255,255,255);
    static int wid = 55;
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
    ggprint8b (&r, 16, 0xffffffff, "Adam Ostlund");
    glPopMatrix();
}

#endif
