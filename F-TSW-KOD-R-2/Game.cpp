/*#ifndef GAME_CPP
#define GAME_CPP
#include "Game.h"

extern BHglobal bhg;
extern NLarsGlobal * nlG;// = NLarsGlobal::GetInstance();
extern NJordGlobal njG;
Game::Game(int x, int y) 
{
    xres = x;
    yres = y;
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
}

void Game::procMouseInput(int x, int y)
{
	int changeState = states.top()->procMouseInput(x,y);

    swite (changeState) {
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
            states.push(new BattleGS(nlG->BattleMap1, 10 ,10, 180, 0, 0,
                        xres, yres, 0));
        #ifdef SOUND
            alSourceStop(njG.sound.ambientSound);
            alSourcePlay(njG.sound.battleSound);
        #endif
            break;
        case 5:
            states.push(new BattleGS(nlG->BattleMap1, 10 ,10, 180, 0, 0,
                        xres, yres, 1));
        #ifdef SOUND
            alSourceStop(njG.sound.ambientSound);
            alSourcePlay(njG.sound.battleSound);
        #endif
            break;
        case 7:
            states.push(new BattleGS(nlG->BattleMap1, 10 ,10, 180, 0, 0,
                        xres, yres, 2));
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
            states.push(new CreditGS(xres, yres));
            break;
    }
}

void Game::drawGameState()
{
	if(states.size() > 0)
		states.top()->drawGameState();
}

#endif*/
