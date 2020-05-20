// Author: Brandon Hernandez
// Spring 2019: CMPS 3350
// Group Project: F-TSW-KOD-R-2
// Last Updated: 5-7-2019

#ifndef BRANDON_H
#define BRANDON_H

#include "stack"
#include <vector>
#include "nickolasL.h"
#include "nicholasJo.h"

using namespace std;


enum ButtonID
{
    MAIN_MENU,NEW_GAME,LOAD_GAME,HIGH_SCORES,CONTROLS,
    EXIT,ARCHER,SOLDIER,TANK,NICK,
    SAVE1, SAVE2, SAVE3, SAVE4,
};

enum pButtonID
{
    RESUME,PCONTROLS,PMAIN_MENU,
    RETREAT,
    REST, SAVE_GAME,
    //CONTINUE,EMAIN_MENU
};


struct vec
{
    float x,y,z;
};

struct Button
{
    float height, width;
    vec center;
    string name;
    ButtonID bid; pButtonID pbid;
    Button();
    Button(string,ButtonID);
    Button(string,pButtonID);
    ~Button();
};

struct BHglobal
{
    Button menus[5][5];
    Button pmenu[3];
    Button bmenu[3];
    Button tmenu[3];
    Button emenu[2];
    bool ctrls;
    bool hs;
    int count;
    GLuint logo;
    GLuint MMTex;
    GLuint BHTex;
    GLuint NLTex;
    GLuint NJTex;
    GLuint MJTex;
    GLuint AOTex;
    BHglobal();
};

class Game
{
    private:
        int xres, yres;
        bool done;
        stack<GameState*> states;
    public:
        Game(int x, int y);
        void init();
        void cleanUp();
        bool getDone();
        void procMouseInput(int x, int y);
        void procKeyInput(int key);
        void drawGameState();
};

class TitleGS: public GameState {
    private:
        string title;
        string req;
        float xres, yres;
        float yawL, yawR;
    public:
        TitleGS(int, int);
        ~TitleGS();
        int procMouseInput(int, int);
        int procKeyInput(int);
        void drawGameState();
};
// Done During Lab hours
// Creates pause functionality by pressing esc while in game.
// Additionally added save game functionaliy
class PauseGS : public GameState {
    private:
        Button * buttons;
        float xres, yres;
    public:
        PauseGS(Button[3],int,int);
        ~PauseGS();
        int procMouseInput(int, int);
        int procKeyInput(int);
        void drawGameState();
};
// Updated During lab hours
class MenuGS : public GameState {
    private:
        int state;
        int size;
        bool clicked;
        Button ** buttons;
        Button select;
        float xres, yres;
    public:
        MenuGS(int, Button[5][5],int, int);
        ~MenuGS();
        int procMouseInput(int, int);
        int procKeyInput(int);
        void drawGameState();
};
// May not need implementation
class TownGS : public GameState {
    private:
        Button * buttons;
        float xres, yres;
    public:
        TownGS(Button[2], int, int);
        ~TownGS();
        int procMouseInput(int, int);
        int procKeyInput(int);
        void drawGameState();
};
 
class CreditGS : public GameState {
    private:
        float xres, yres;
    public:
        CreditGS(int, int);
        ~CreditGS();
        int procMouseInput(int, int);
        int procKeyInput(int);
        void drawGameState();
};

void brandonHCredit(int,int,GLuint);

#endif
