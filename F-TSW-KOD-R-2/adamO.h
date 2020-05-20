//3350
//program: adamO.cpp
//author: adam ostlund
//date: february 14, 2019
//
//This program is an individual source file containing a name and profile picture.
//
//


#ifndef ADAMO_H
#define ADAMO_H


//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <time.h>
//#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#include <X11/keysym.h>
#include <GL/glx.h>
//#include "log.h"
////#include "ppm.h"
#include "fonts.h"
#include <string>


using namespace std;

enum stateID
{
    END_TURN, CHAR_MENU, BATTLE, ALLY, BOX
};

struct vec9
{
    float x, y;
};

struct vec10
{
    float x, y;
};

struct vec11
{
    float x, y;
};

struct Boxes
{
    stateID ID;
    float width, height;
    vec9 center;
    Boxes();
    Boxes(stateID);
};

struct BattleBoxes
{
    stateID ID;
    float width, height;
    vec10 center;
    BattleBoxes();
    BattleBoxes(stateID);
};

struct AllyBoxes
{
    stateID ID;
    float width, height;
    vec11 center;
    AllyBoxes();
    AllyBoxes(stateID);
};

struct AOglobal
{
    Boxes box[10];
    BattleBoxes bbox[3];
    AllyBoxes abox[4];
    GLuint endTurnTex;
    AOglobal();
};

class uiboxes
{
    private:
        int size;
        Boxes boxes[10];
        float xres, yres;
    public:
        uiboxes(Boxes[10], float, float);
        ~uiboxes(){}
        void procMouseInput(int, int);
        void procKeyInput(int key);
        void drawBoxes();
        void posBoxes();
        void check_mouse(int, int, int);
};

class battleboxes
{
    private:
        int size;
        BattleBoxes btboxes[3];
        float xres, yres;
    public:
        battleboxes(BattleBoxes[3], float, float);
        ~battleboxes(){}
        void procMouseInput(int, int);
        void procKeyInput(int key);
        void drawBattleBoxes(int);
        void posBattleBoxes(int);
        void check_mouse(int, int, int);
};

class allyboxes
{
    private:
        int size;
        AllyBoxes alboxes[4];
        float xres, yres;
    public:
        allyboxes(AllyBoxes[4], float, float);
        ~allyboxes(){}
        void procMouseInput(int, int);
        void procKeyInput(int key);
        void drawAllyBoxes();
        void posAllyBoxes(int);
        void check_mouse(int, int, int);
};

void showAdamPicture(int x, int y, GLuint textid);

#endif
