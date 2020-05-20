//program: marbienJ.h
//author: Marbien Jimeno
//
//This program is a header file containing function prototype

//Modified By: Nickolas Larson for review
//changed all functions that take in cel[][COL] to a 2d pointer
 
#ifndef MARBIEN_J_H
#define MARBIEN_J_H

#include <GL/glx.h>
#include "fonts.h"
#include <iostream>
//#include <bits/stdc++.h>
#include <stack>
#include <math.h>
#include <float.h>
#include <set>
#include <vector>
#include <string.h>
#include "nicholasJo.h"
#include "nickolasL.h"

#include <memory>
using namespace std;

/*int Movement(int);
bool inRange(int, int);
void displayMap(int, int, int[][6]);*/

#define ROW 25 
#define COL 25 
  
// shortcut for int, int pair type 
typedef pair<int, int> Pair; 
  
// shortcut for pair<int, pair<int, int>> type 
typedef pair<double, pair<int, int>> pPair; 

//nick larson added

   
struct cell 
{ 
    // Row and Column index of its parent 
    int parent_i, parent_j; 
    // f = g + h 
    double f, g, h; 
};
typedef vector<vector<cell>> CellGroup;

bool isValid(int, int);
bool isUnBlocked(Pair, Tile **, int, int);
bool isDestination(int, int, Pair);
double calculateHValue(int, int, Pair);

//stack<Pair> tracePath(cell[][25], Pair dest);//changed
stack<Pair> tracePath(Pair, CellGroup, Pair dest);
//stack<Pair> aStarSearch(int[][25], Pair src, Pair dest);//changed
stack<Pair> aStarSearch(Pair, Tile **, Pair src, Pair dest);

void displayMap(int[][25]);

stack<Pair> Movement(Pair, Tile **, int x, int z, vec2); 
void showMarbienPicture(int x, int y, GLuint textid);

#endif
