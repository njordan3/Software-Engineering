/*#ifndef GAME_H
#define GAME_H

#include <stack>
#include <vector>

#include "nickolasL.h"
#include "brandonH.h"
#include "nicholasJo.h"

class Game
{
	private:
        int xres, yres;
		stack<GameState*> states;
	public:
		Game(int x, int y);
        void init();
        void cleanUp();
		void procMouseInput(int x, int y);
		void procKeyInput(int key);
		void drawGameState();
};

#endif*/
