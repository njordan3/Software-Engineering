// Author: Brandon Hernandez
// Spring 2019: CMPS 3350
// Group Project: F-TSW-KOD-R-2
// Last Updated: 5-7-2019

//====================[TO DO LIST]====================//
//               --------MAJOR--------
// - Finish CreditGS
//      - Currently displays a blank screen that can
//      check any key and return to TitleGS but 
//      causes a segfault when entering a new game
//          - FIXED: Segfault on New Game
// - Finish Cleanup
//      - Needs to set Done in main so the game can
//      perform a clean exit. Currently its case ?
//      in Game.cpp.
// -<N/A>Configuration Menu
//      - Need subset menu under controls which will
//      bring up a set of options. Not sure which
//      options to add here but sounds/screen size
//      etc.
//          - Wont make it in time.
//               --------MINOR--------
// -<COMPLETE> Unit Test 
//      - Case around my couts for testing
// -<N/A> Profiling
//      - Might not add in personal cpp, just in main
// -<COMPLETE> Refactor K&R
//      - Last thing to do
//====================================================//

#include "brandonH.h"

extern NJordGlobal njG;
extern NLarsGlobal * nlG;

Model pModel[2] = {
Model("models/characters/Tank.obj","models/characters/ModelTexturePlayer.png"),
Model("models/characters/Spearman.obj","models/characters/ModelTextureEnemy.png")};
//=================================================//
//--------------- START OF BHglobal ---------------//
//=================================================//

BHglobal::BHglobal() 
{
    
    GenerateGLTexture(MMTex, "images/title2.png", true);
    GenerateGLTexture(BHTex, "images/brandonH.png", false);
    GenerateGLTexture(NLTex, "images/nickLCreditPic.jpg", false);
    GenerateGLTexture(NJTex, "images/nicholasJo.png", false);
    GenerateGLTexture(MJTex, "images/marbienJPicture.png", false);
    GenerateGLTexture(AOTex, "images/aopic.png", false);
    
    GenerateGLTexture(logo, "images/FTSWKODR2_logo.png", true);
    static Button btn[5] = {
        Button("New Game", NEW_GAME),
        Button("Load Game",LOAD_GAME),
        Button("High Scores",HIGH_SCORES),
        Button("Controls", CONTROLS),
        Button("Exit",EXIT)
    };
    static Button btn1[5] = {
        Button("Archer", ARCHER),
        Button("Soldier", SOLDIER),
        Button("Tank", TANK),
        Button("Nick", NICK),
        Button("Main Menu", MAIN_MENU)
    };
    static Button btn2[5] = {
        Button("Save 1", SAVE1),
        Button("Save 2", SAVE2),
        Button("Save 3", SAVE3),
        Button("Save 4", SAVE4),
        Button("Main Menu", MAIN_MENU)
    };
    static Button pbtn[3] = {
        Button("Resume", RESUME),
        //Button("Save Game", SAVE_GAME),
        Button("Controls",  PCONTROLS),
        Button("Main Menu", PMAIN_MENU),
    };
    static Button bbtn[3] = {
        Button("Resume", RESUME),
        Button("Controls", CONTROLS),
        //Button("Save Game", SAVE_GAME),
        Button("Retreat", RETREAT),
    };
    static Button tbtn[3] = {
        Button("Resume", RESUME),
        Button("Save Game", SAVE_GAME),
        Button("Rest", REST),
    };
    //static Button ebtn[2] = { 
        //Button("Continue?", CONTINUE),
        //Button("Main Menu", EMAIN_MENU),
    //};
    // Derived Menu from PauseMenu. Select which save file to override
    // Needs implementation
    static Button pbtn2[5] = {
        Button("Save 1", SAVE1),
        Button("Save 2", SAVE2),
        Button("Save 3", SAVE3),
        Button("Save 4", SAVE4),
        Button("Return", PMAIN_MENU),
    };
    for (int i=0; i<5; i++) {
        menus[0][i] = btn[i];
        menus[1][i] = btn1[i];
        menus[2][i] = btn2[i];
    }
    for (int i=0; i<3; i++) { 
        pmenu[i] = pbtn[i];
        bmenu[i] = bbtn[i];
        tmenu[i] = tbtn[i];
    }
    for (int i=0; i<2; i++) {
        //emenu[i] = ebtn[i];
    }

    hs = false;
    ctrls = false;
    count = 0;
} BHglobal bhg;

//=================================================//
//---------------- START OF BUTTON ----------------//
//=================================================//

Button::Button() {}

Button::Button(string n, ButtonID _bid) 
{
    name = n;
    bid = _bid;
}

Button::Button(string n, pButtonID _bid) 
{
    name = n;
    pbid = _bid;
}

Button::~Button() {}

//=================================================//
//---------------- START OF PAUSEGS ---------------//
//=================================================//

PauseGS::PauseGS(Button b[3],int xres, int yres)
{
    buttons = new Button[3];
    this->xres = xres;
    this->yres = yres;
    for (int i=0; i<3; i++) {
        buttons[i] = b[i];
        buttons[i].width = 100;
        buttons[i].height = 20;
        buttons[i].center.x = xres/4;
        buttons[i].center.y = yres/1.4 - (i+1)*60;
        //buttons[i].center.y = 500 - (i+1)*60;
    }
}

PauseGS::~PauseGS()
{
    delete [] buttons;
}

int PauseGS::procMouseInput(int x, int y)
{
    pButtonID btn;
    for (int i=0;i<3;i++) {
        if (yres - y < buttons[i].center.y + buttons[i].height &&
                yres - y > buttons[i].center.y - buttons[i].height &&
                x < buttons[i].center.x + buttons[i].width &&
                x > buttons[i].center.x - buttons[i].width) {
            // Output which button and its corressponding ID to verify button clicks
#ifdef UNIT            
            cout << "Count: " << i << " Button: " << buttons[i].name << endl;
#endif
            btn = buttons[i].pbid;
        }
    }
    switch(btn) {
        case RESUME:
#ifdef SOUND
            alSourcePlay(njG.sound.menuSound); 
#endif
            return -1;
            break;
        case SAVE_GAME:
#ifdef SOUND
            alSourcePlay(njG.sound.menuSound); 
#endif
            njG.saveEntities(1);
            break;
        case PCONTROLS:
            if(bhg.ctrls == true) {
                bhg.ctrls = false;
            }    
            else {    
                bhg.ctrls = true;
            }
            break;
        case PMAIN_MENU:
#ifdef SOUND
            alSourcePlay(njG.sound.menuMusic);
            alSourceStop(njG.sound.ambientSound);
            alSourcePlay(njG.sound.menuSound); 
#endif
            njG.player->resetInstance();
            njG.resetAllies();
            njG.resetEnemies();
        case RETREAT:
            njG.resetTurns();
            njG.resetBPos();
#ifdef SOUND
            alSourceStop(njG.sound.battleSound);
            alSourcePlay(njG.sound.ambientSound);
#endif
            return -2;
            break;
        case REST:
            if (njG.player->score > 25) {
                njG.player->resetStats();
                for(int i=0; i<njG.allies->count;i++) {
                    njG.allies[i].resetStats();
                }
                njG.player->score -= 25;
            }
            break;
    }
    return 0;

}

int PauseGS::procKeyInput(int key)
{
    switch(key) {
        case XK_Escape:
            return -1;
    };
    return 0;
}

void PauseGS::drawGameState()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,xres,0,yres,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for (int i=0; i<3; i++) {
        //Draw Buttons
        Button *s;
        glColor3ub(130,130,130);
        s = &buttons[i];
        glPushMatrix();
        glTranslatef(s->center.x, s->center.y, s->center.z);
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

        //Draw Names
        int yres = 600;
        Rect r;
        //r.bot = yres - (170 +(i*60));
        //r.left = 200 ;
        r.bot = yres/.95 - (i+1)*60;
        r.left = xres/4 ;
        ggprint16(&r, 16, 0xFFFFFFFF, buttons[i].name.c_str());
    }
}

//=================================================//
//---------------- START OF MENUGS ----------------//
//=================================================//

MenuGS::MenuGS(int s, Button b[5][5], int xres, int yres)
{
    state = 0;
    size = s;
    buttons = new Button*[5];
    for (int i=0; i<5; i++)
        buttons[i] = new Button[5];
    this->xres = xres;
    this->yres = yres;
    for (int j=0; j<size; j++) {
        for (int i=0; i<size; i++) {
            buttons[j][i] = b[j][i];
            //Position Buttons
            buttons[j][i].width = 100;
            buttons[j][i].height = 20;
            buttons[j][i].center.x = xres/2;
            buttons[j][i].center.y = yres/1.7 - (i+1)*60;
        }
    }
}

MenuGS::~MenuGS()
{
    for (int i=0; i<5; i++)
        delete [] buttons[i];
    delete [] buttons;
}

int MenuGS::procMouseInput(int x, int y) 
{
    ButtonID btn;
    for (int i=0;i<size;i++) {
        if (yres - y < buttons[state][i].center.y + buttons[state][i].height &&
                yres - y > buttons[state][i].center.y - buttons[state][i].height &&
                x < buttons[state][i].center.x + buttons[state][i].width &&
                x > buttons[state][i].center.x - buttons[state][i].width) {
            // Output which button and its corressponding ID to verify button clicks
#ifdef UNIT
            cout << "Count: " << i << " Button: " << buttons[state][i].name << endl;
#endif
            btn = buttons[state][i].bid;
        }
    }
    switch (btn) {
        case MAIN_MENU:
#ifdef SOUND
            alSourcePlay(njG.sound.menuSound);
#endif
            state = MAIN_MENU;
            break;
        case NEW_GAME:
#ifdef SOUND
            alSourcePlay(njG.sound.menuSound); 
#endif
            state = NEW_GAME;
            break;
        case LOAD_GAME:
#ifdef SOUND
            alSourcePlay(njG.sound.menuSound);
#endif
            state = LOAD_GAME;
            break;
        case HIGH_SCORES:
#ifdef SOUND
            alSourcePlay(njG.sound.menuSound); 
#endif
            //state = HIGH_SCORES;
            if(bhg.hs == false)
                bhg.hs = true;
            else
                bhg.hs = false;
            break;
        case CONTROLS:
#ifdef SOUND
            alSourcePlay(njG.sound.menuSound); 
#endif
            if(bhg.ctrls == true) {
                bhg.ctrls = false;
            }    
            else {    
                bhg.ctrls = true;
            }
            //state = CONTROLS;
            break;
        case EXIT:
#ifdef SOUND
            alSourcePlay(njG.sound.menuSound); 
            njG.sound.clearSounds();
#endif
            //exit(0);
            return -3;
            break;
        case ARCHER:
            if(njG.name.size() == 3)
            {
#ifdef SOUND
                alSourcePlay(njG.sound.menuSound);
                alSourceStop(njG.sound.menuMusic);
                alSourcePlay(njG.sound.ambientSound);
#endif
                if (Player::count != 0) {
                    Player::resetInstance();
                }
                Player *player = Player::setInstance("Archer");
                njG.spawnAllies(3);
                njG.spawnEnemies(3);
                //stops 'unused variable' warning
                (void)player;
                state = MAIN_MENU;
                return 1;
            }
            break;
        case SOLDIER:
            if(njG.name.size() == 3)
            {
#ifdef SOUND
                alSourcePlay(njG.sound.menuSound); 
                alSourceStop(njG.sound.menuMusic);
                alSourcePlay(njG.sound.ambientSound);
#endif
                if (Player::count != 0) {
                    Player::resetInstance();
                }
                Player *player = Player::setInstance("Soldier");
                njG.spawnAllies(3);
                njG.spawnEnemies(3);
                //stops 'unused variable' warning
                (void)player;
                state = MAIN_MENU;
                return 1;
            }
            break;
        case TANK:
            if(njG.name.size() == 3)
            {
#ifdef SOUND
                alSourcePlay(njG.sound.menuSound); 
                alSourceStop(njG.sound.menuMusic);
                alSourcePlay(njG.sound.ambientSound);
#endif
                if (Player::count != 0) {
                    Player::resetInstance();
                }
                Player *player = Player::setInstance("Tank");
                njG.spawnAllies(3);
                njG.spawnEnemies(3);
                //stops 'unused variable' warning
                (void)player;
                state = MAIN_MENU;
                return 1;
            }
            break;
        case NICK:
            if(njG.name.size() == 3)
            {
#ifdef SOUND
                alSourcePlay(njG.sound.menuSound); 
                alSourceStop(njG.sound.menuMusic);
                alSourcePlay(njG.sound.ambientSound);
#endif
                if (Player::count != 0) {
                    Player::resetInstance();
                }
                Player *player = Player::setInstance("Nick");
                njG.spawnAllies(3);
                njG.spawnEnemies(3);
                //stops 'unused variable' warning
                (void)player;
                state = MAIN_MENU;
                return 1;
            }
            break;
        case SAVE1:
            {
#ifdef SOUND
                alSourcePlay(njG.sound.menuSound); 
                alSourceStop(njG.sound.menuMusic);
                alSourcePlay(njG.sound.ambientSound);
#endif
                if (njG.loadEntities(1)) {
                    state = MAIN_MENU;
                    return 1;
                }
                return 0;
            }
            break;
        case SAVE2:
            {
#ifdef SOUND
                alSourcePlay(njG.sound.menuSound); 
                alSourceStop(njG.sound.menuMusic);
                alSourcePlay(njG.sound.ambientSound);
#endif
                if (njG.loadEntities(2)) {
                    state = MAIN_MENU;
                    return 1;
                }
                return 0;
            }
            break;
        case SAVE3:
            {
#ifdef SOUND
                alSourcePlay(njG.sound.menuSound); 
                alSourceStop(njG.sound.menuMusic);
                alSourcePlay(njG.sound.ambientSound);
#endif
                if (njG.loadEntities(3)) {
                    state = MAIN_MENU;
                    return 1;
                }
                return 0;
            }
            break;
        case SAVE4:
            {
#ifdef SOUND
                alSourcePlay(njG.sound.menuSound); 
                alSourceStop(njG.sound.menuMusic);
                alSourcePlay(njG.sound.ambientSound);
#endif
                if (njG.loadEntities(4)) {
                    state = MAIN_MENU;
                    return 1;
                }
                return 0;
            }
            break;
    }
    return 0;
}

int MenuGS::procKeyInput(int key)
{
    switch (key) {
        case XK_Escape:
            if (state == 0) {
                    return -1;
            }
            break;
        case XK_a:
        case XK_A:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"A";
#ifdef UNIT_TEST
            cout << "Inserting A" << endl;
            cout << this->state << " " << njG.name.size() << endl;
#endif
        }
        break;
        case XK_b:
        case XK_B:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"B";
#ifdef UNIT_TEST
            cout << "Inserting B" << endl;
#endif
        }
        break;
        case XK_c:
        case XK_C:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"C";
#ifdef UNIT_TEST
            cout << "Inserting C" << endl;
#endif
        }
        break;
        case XK_d:
        case XK_D:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"D";
#ifdef UNIT_TEST
            cout << "Inserting D" << endl;
#endif
        }
        break;
        case XK_e:
        case XK_E:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"E";
#ifdef UNIT_TEST
            cout << "Inserting E" << endl;
#endif
        }
        break;
        case XK_f:
        case XK_F:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"F";
#ifdef UNIT_TEST
            cout << "Inserting F" << endl;
#endif
        }
        break;
        case XK_g:
        case XK_G:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"G";
#ifdef UNIT_TEST
            cout << "Inserting G" << endl;
#endif
        }
        break;
        case XK_h:
        case XK_H:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"H";
#ifdef UNIT_TEST
            cout << "Inserting H" << endl;
#endif
        }
        break;
        case XK_i:
        case XK_I:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"I";
#ifdef UNIT_TEST
            cout << "Inserting I" << endl;
#endif
        }
        break;
        case XK_j:
        case XK_J:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"J";
#ifdef UNIT_TEST
            cout << "Inserting J" << endl;
#endif
        }
        break;
        case XK_k:
        case XK_K:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"K";
#ifdef UNIT_TEST
            cout << "Inserting K" << endl;
#endif
        }
        break;
        case XK_l:
        case XK_L:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"L";
#ifdef UNIT_TEST
            cout << "Inserting L" << endl;
#endif
        }
        break;
        case XK_m:
        case XK_M:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"M";
#ifdef UNIT_TEST
            cout << "Inserting M" << endl;
#endif
        }
        break;
        case XK_n:
        case XK_N:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"N";
#ifdef UNIT_TEST
            cout << "Inserting N" << endl;
#endif
        }
        break;
        case XK_o:
        case XK_O:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"O";
#ifdef UNIT_TEST
            cout << "Inserting O" << endl;
#endif
        }
        break;
        case XK_p:
        case XK_P:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"P";
#ifdef UNIT_TEST
            cout << "Inserting P" << endl;
#endif
        }
        break;
        case XK_q:
        case XK_Q:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"Q";
#ifdef UNIT_TEST
            cout << "Inserting Q" << endl;
#endif
        }
        break;
        case XK_r:
        case XK_R:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"R";
#ifdef UNIT_TEST
            cout << "Inserting R" << endl;
#endif
        }
        break;
        case XK_s:
        case XK_S:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"S";
#ifdef UNIT_TEST
            cout << "Inserting S" << endl;
#endif
        }
        break;
        case XK_t:
        case XK_T:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"T";
#ifdef UNIT_TEST
            cout << "Inserting T" << endl;
#endif
        }
        break;
        case XK_u:
        case XK_U:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"U";
#ifdef UNIT_TEST
            cout << "Inserting U" << endl;
#endif
        }
        break;
        case XK_v:
        case XK_V:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"V";
#ifdef UNIT_TEST
            cout << "Inserting V" << endl;
#endif
        }
        break;
        case XK_w:
        case XK_W:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"W";
#ifdef UNIT_TEST
            cout << "Inserting W" << endl;
#endif
        }
        break;
        case XK_x:
        case XK_X:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"X";
#ifdef UNIT_TEST
            cout << "Inserting X" << endl;
#endif
        }
        break;
        case XK_y:
        case XK_Y:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"Y";
#ifdef UNIT_TEST
            cout << "Inserting Y" << endl;
#endif
        }
        break;
        case XK_z:
        case XK_Z:
        if (state == 1 && njG.name.size() < 3) {
            njG.name = njG.name +"Z";
#ifdef UNIT_TEST
            cout << "Inserting Z" << endl;
#endif
        }
        break;
        case XK_BackSpace:
        if (state == 1 && njG.name.size() > 0) {
            njG.name.pop_back();

        }
        break;
        default:
        break;
    };
    return 0;
}

void MenuGS::drawGameState()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,xres,0,yres,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, bhg.MMTex);
    glBegin(GL_QUADS);
   
        glTexCoord2f(0.0f, 0.0f);glVertex2i(0, 0);
	    glTexCoord2f(0.0f, 1.0f);glVertex2i(0, 900);
	    glTexCoord2f(1.0f, 1.0f);glVertex2i( 1200, 900);
    	glTexCoord2f(1.0f, 0.0f);glVertex2i( 1200, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0f, 1.0f, 1.0f);

    

    for (int i=0; i<size; i++) {
        //Draw Buttons
        Button *s;
        glColor3ub(130,130,130);
        s = &buttons[state][i];
        glPushMatrix();
        glTranslatef(s->center.x, s->center.y, s->center.z);
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

        //Draw Names
        int yres = 600;
        Rect r;
        //r.bot = yres - (170 +(i*60));
        r.bot = yres/1.155 - (i+1)*60;
        r.left = xres/2 ;
        ggprint16(&r, 16, 0xFFFFFFFF, buttons[state][i].name.c_str());
        //buttons[j][i].center.x = xres/2;
        //buttons[j][i].center.y = yres/1.4 - (i+1)*60;
    }
    if(state == 1) {
        Rect r;
        r.bot = yres/1.7;
        r.left = xres/2;
        ggprint16(&r, 16, 0xFFFFFFFF, "Enter Name:");
        r.bot = yres/1.8;
        ggprint16(&r, 16, 0xFFFFFFFF, njG.name.c_str());
    } 
}

//=================================================//
//---------------- START OF TOWNGS ----------------//
//=================================================//

TownGS::TownGS(Button b[3], int xres, int yres)
{
    buttons = new Button[3];
    this->xres = xres;
    this->yres = yres;
    for (int i=0; i<3; i++) {
        buttons[i] = b[i];
        buttons[i].width = 100;
        buttons[i].height = 20;
        buttons[i].center.x = xres/4;
        buttons[i].center.y = yres/1.4 - (i+1)*60;
        //buttons[i].center.y = 500 - (i+1)*60;
    }
}

TownGS::~TownGS()
{
    delete [] buttons;
}

int TownGS::procMouseInput(int x, int y)
{
    pButtonID btn;
    for (int i=0;i<3;i++) {
        if (yres - y < buttons[i].center.y + buttons[i].height &&
                yres - y > buttons[i].center.y - buttons[i].height &&
                x < buttons[i].center.x + buttons[i].width &&
                x > buttons[i].center.x - buttons[i].width) {
            // Output which button and its corressponding ID to verify button clicks
#ifdef UNIT
            cout << "Count: " << i << " Button: " << buttons[i].name << endl;
#endif
            btn = buttons[i].pbid;
        }
    }
    switch(btn) {
        case RESUME:
            return -1;
            break;
        case REST:
        case PCONTROLS:
            break;
            njG.player->resetStats();
            break;
        case SAVE_GAME:
            break;
        case PMAIN_MENU:
            break;
        case RETREAT:
            break;
    }
    return 0;

}

int TownGS::procKeyInput(int key)
{
    switch(key) {
        case XK_Escape:
            return -1;
        default:
            break;;
    }
    return 0;//read keys
}

void TownGS::drawGameState()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,xres,0,yres,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for (int i=0; i<3; i++) {
        //Draw Buttons
        Button *s;
        glColor3ub(90,140,90);
        s = &buttons[i];
        glPushMatrix();
        glTranslatef(s->center.x, s->center.y, s->center.z);
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

        //Draw Names
        int yres = 600;
        Rect r;
        //r.bot = yres - (170 +(i*60));
        //r.left = 200 ;
        r.bot = yres/.95 - (i+1)*60;
        r.left = xres/4 ;
        ggprint16(&r, 16, 0x00ffff00, buttons[i].name.c_str());
    }
}

//=================================================//
//--------------- START OF TITLEGS ----------------//
//=================================================//
TitleGS::TitleGS(int xres, int yres)
{
    this->xres = xres;
    this->yres = yres;
    yawL = 0;
    yawR = 180;
}

int TitleGS::procMouseInput(int,int)
{
    return 0;
}

int TitleGS::procKeyInput(int)
{
    return 6;
}

void TitleGS::drawGameState()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,xres,0,yres,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, bhg.logo);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);glVertex2i(0, 0);
	    glTexCoord2f(0.0f, 1.0f);glVertex2i(0, 900);
	    glTexCoord2f(1.0f, 1.0f);glVertex2i( 1200, 900);
    	glTexCoord2f(1.0f, 0.0f);glVertex2i( 1200, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0f, 1.0f, 1.0f);

    //swap to 3d
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,100.0f);
    
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//discussed futher in later tutorial
	//glShadeModel(GL_SMOOTH);//enables smooth shading

	// sets the depth buffer//stop elements from drawing over others
	glClearDepth(1.0f);//Depth buffer setup
	glEnable(GL_DEPTH_TEST);//Enables Depth Testing
	glDepthFunc(GL_LEQUAL);//The type of depth test to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//??makes the perspective view better??
    glClear(GL_DEPTH_BUFFER_BIT);
    pModel[0].drawTitleModel(-3.0f, -3.5f, -15.0f, yawR);
    pModel[1].drawTitleModel(3.0f, -3.5f, -15.0f, yawL);
    yawL += 0.65f;
    yawR -= 0.65f;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,xres,0,yres,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
     
    Rect r;   
    r.bot = yres/4;
    r.left = xres/2;
    bhg.count++;
    #ifdef UNIT
    cout << bhg.count << endl;
    #endif
    if ((bhg.count%80) > 40)
        ggprint16(&r, 16, 0xFFFFFFFF, "Press any key");
}

//=================================================//
//-------------- START OF CREDITGS ----------------//
//=================================================//
CreditGS::CreditGS(int xres, int yres)
{
    this->xres = xres;
    this->yres = yres;
}

int CreditGS::procMouseInput(int x, int y)
{
    return 0;
}

int CreditGS::procKeyInput(int key)
{
#ifdef SOUND
    alSourceStop(njG.sound.ambientSound);
    alSourcePlay(njG.sound.menuSound);
#endif
    njG.player->resetInstance();
    njG.resetAllies();
    njG.resetEnemies();	
    return -4;
}

void CreditGS::drawGameState()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,xres,0,yres,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    extern void Display_NicholasJordan(int,int,GLuint);
    extern void draw_nickLCredit(int,int,GLuint);
    extern void showAdamPicture(int,int, GLuint);
    extern void showMarbienPicture(int,int,GLuint);
    // SHOW PICTURE FUNCTIONS
    brandonHCredit(xres/2, yres-100, bhg.BHTex);
    Display_NicholasJordan(xres/2,yres-250, bhg.NJTex);
    showAdamPicture(xres/2,yres-400, bhg.AOTex);
    draw_nickLCredit(xres/2,yres-550,bhg.NLTex);
    showMarbienPicture(xres/2,yres-700,bhg.MJTex);
    
    Rect r;
    r.bot = yres/8;
    r.left = xres/2;
    ggprint8b(&r, 16, 0xFFFFFFFF, "Your High Scores are located at https://cs.csub.edu/~nlarson/3350/game/");
    
}


//=================================================//
//--------------- START OF ShowPic ----------------//
//=================================================//
void brandonHCredit(int x, int y, GLuint texture)
{
    Rect r;
    float wid = 50;
    glPushMatrix();
    glTranslatef(x,y,0);
    glColor3f(1.0f,1.0f,1.0f);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(wid,0,0);
    r.bot = 0;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0xFFFFFFFF, "Brandon Hernandez");
    glPopMatrix();
}


//=================================================//
//----------------- START OF GAME -----------------//
//=================================================//
Game::Game(int x, int y)
{
    xres = x;
    yres = y;
    done = false;
}

bool Game::getDone()
{
    return done;
}

void Game::init()
{
    states.push(new TitleGS(xres,yres));
}

void Game::cleanUp()
{
    while(!states.empty()) {
        states.pop();
    }
    done = true;
}

void Game::procMouseInput(int x, int y)
{
    int changeState = states.top()->procMouseInput(x,y);
    srand(time(0));
    int random;
    switch (changeState) {
        case -4:
            while(states.size()-1 > 0) {
                states.pop();
            }
            break;
        case -3:
            cleanUp();
            break;
        case -2:
            for (int i=0; i<2; i++)
                states.pop();
            break;
        case -1:
            states.pop();
            break;
        case 1:
            states.push(new WorldGS(nlG->MainMap, 25 ,25, 180, 7, 7, xres, yres));
            break;
        case 2:
            states.push(new PauseGS(bhg.pmenu,xres,yres));
            break;

        case 4:
                states.push(new PauseGS(bhg.bmenu,xres,yres));
            break;
        case 3:
            random = rand()%2;
            states.push(new BattleGS(random>0 ? nlG->BattleMap2 : nlG->BattleMap1,
             10 ,10, 180, 0, 0, xres, yres, 0));
                        
#ifdef SOUND
            alSourceStop(njG.sound.ambientSound);
            alSourcePlay(njG.sound.battleSound);
#endif
            break;
        case 5:
            random = rand()%2;
            states.push(new BattleGS(random>0 ? nlG->BattleMap2 : nlG->BattleMap1,
             10 ,10, 180, 0, 0, xres, yres, 1));
#ifdef SOUND
            alSourceStop(njG.sound.ambientSound);
            alSourcePlay(njG.sound.battleSound);
#endif
            break;
        case 7:
            random = rand()%2;
            states.push(new BattleGS(random>0 ? nlG->BattleMap2 : nlG->BattleMap1,
             10 ,10, 180, 0, 0, xres, yres, 2));
#ifdef SOUND
            alSourceStop(njG.sound.ambientSound);
            alSourcePlay(njG.sound.battleSound);
#endif
            break;
        case 6:
            states.push(new MenuGS(5, bhg.menus,  xres, yres));
            break;
        case 8:
            states.push(new CreditGS(xres, yres));
            break;
        case 9:
            states.push(new PauseGS(bhg.tmenu,xres,yres));
            break;
    }
}

void Game::procKeyInput(int key)
{
    int changeState = states.top()->procKeyInput(key);
    switch(changeState) {
        case -4:
            while(states.size()-1 > 0) {
                states.pop();
            }
            break;
        case -2:
            for (int i=0; i<2; i++)
            states.pop();
            break;
        case -1:
            states.pop();
            break;
        case 1:
            states.push(new WorldGS(nlG->MainMap,25 ,25, 180, 0, 0, xres, yres));
            break;
        case 2:
            states.push(new PauseGS(bhg.pmenu,xres,yres));
            break;
        //case 3:
            //states.push(new BattleGS(nlG->BattleMap1,10 ,10, 180, 0, 0, xres, yres));
            //break;
        case 4:
            states.push(new PauseGS(bhg.bmenu,xres,yres));
            break;
        case 5:
            //states.push(new PauseGS(bhg.tmenu,xres,yres));
            break;
        case 6:
            states.push(new MenuGS(5, bhg.menus,  xres, yres));
            break;
        case 8:
            sendScore(njG.name, njG.player->score);
            states.push(new CreditGS(xres, yres));
            break;
    }
}

void Game::drawGameState()
{
    if (states.size() > 0)
        states.top()->drawGameState();
}

