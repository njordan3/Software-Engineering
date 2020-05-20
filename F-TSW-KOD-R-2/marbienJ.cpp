//program: marbienJ.cpp
//author: Marbien Jimeno
//
//This program contains function that will display picture & movement function that will trace path of entities

#ifndef MARBIEN_J_CPP
#define MARBIEN_J_CPP

#include "marbienJ.h"
using namespace std;

bool isValid(int row, int col) 
{ 
    // in range? 
    return ((row >=  0) && (row < ROW) && (col >= 0) && (col < COL)); 
} 
  
bool isUnBlocked(Pair size, Tile ** grid, int row, int col) 
{ 
    // true if blocked else false
    if (grid[row][col].modelID == 7 || grid[row][col].modelID == 8 || grid[row][col].modelID == 1) 
        return (true);
    return (false); 
} 

bool isUnOccupied(Pair size, Tile** grid, int row, int col) {

    return grid[row][col].occ ? false : true;
}

bool isDestination(int row, int col, Pair dest) 
{ 
    if (row == dest.first && col == dest.second) 
        return (true); 
    else
        return (false); 
} 
  
// calculate the 'h' heuristics. 
double calculateHValue(int row, int col, Pair dest) 
{ 
    return ((abs(row-dest.first) + abs(row+col - dest.first-dest.second) + abs(col - dest.second))/2);
} 
  
// trace path from the source to destination
stack<Pair> tracePath(Pair size, CellGroup cellDetails, Pair dest) 
{ 
    int row = dest.first; 
    int col = dest.second; 
  
    stack<Pair> Path; 
  
    while (!(cellDetails[row][col].parent_i == row 
             && cellDetails[row][col].parent_j == col )) 
    { 
        Path.push (make_pair (row, col)); 
        int temp_row = cellDetails[row][col].parent_i; 
        int temp_col = cellDetails[row][col].parent_j; 
        row = temp_row; 
        col = temp_col; 
    } 
  
    Path.push (make_pair (row, col)); 
    return (Path); 
} 

// A* Search Algorithm 
stack<Pair> aStarSearch(Pair size, Tile ** grid, Pair src, Pair dest) 
{ 
    CellGroup cellDetails;

    for(int i = 0; i < size.first ; i++) {
        cellDetails.push_back(vector<cell>());
        for (int j = 0; j < size.second; j++) {
            cellDetails[i].push_back(cell());
        }
    }
  
    int i, j; 
  
    for (i=0; i<size.first; i++) { 
        for (j=0; j<size.second; j++) { 
            cellDetails[i][j].f = FLT_MAX; 
            cellDetails[i][j].g = FLT_MAX; 
            cellDetails[i][j].h = FLT_MAX; 
            cellDetails[i][j].parent_i = -1; 
            cellDetails[i][j].parent_j = -1; 
        } 
    } 
  
    // parameters of the starting node 
    i = src.first, j = src.second; 
    cellDetails[i][j].f = 0.0; 
    cellDetails[i][j].g = 0.0; 
    cellDetails[i][j].h = 0.0; 
    cellDetails[i][j].parent_i = i; 
    cellDetails[i][j].parent_j = j; 
    
    // source is out of range? 
    if (isValid (src.first, src.second) == false) { 
#ifdef UNIT_TEST
        printf ("Source is invalid\n");
#endif	
        return(tracePath(size, cellDetails, src)); 
    }  

    // destination is out of range? 
    if (isValid (dest.first, dest.second) == false) {
#ifdef UNIT_TEST 
        printf ("Destination is invalid\n");
#endif	
        return(tracePath(size, cellDetails, src)); 
    } 
  
    // source or the destination is blocked? 
    if (isUnBlocked(size, grid, src.first, src.second) == false || isUnBlocked(size, grid, dest.first, dest.second) == false) {
#ifdef UNIT_TEST 
        printf ("Source or the destination is blocked\n");
#endif	
        return(tracePath(size, cellDetails, src)); 
    } 
  
    // destination cell = source cell? 
    if (isDestination(src.first, src.second, dest) == true) { 
#ifdef UNIT_TEST
        printf ("We are already at the destination\n"); 
#endif
        return(tracePath(size, cellDetails, src)); 
    } 
  
    // closed list initialises to false 
    // no cell included yet 
    // implemented as boolean 2D array 
    bool closedList[ROW][COL]; 
    memset(closedList, false, sizeof (closedList)); 
  
    /* 
     open list 
     <f, <i, j>> 
     where f = g + h, 
     and i = row and j = column index 
     implenented set of pair of pair.*/
    set<pPair> openList; 
  
    // starting cell on open list 
    // 'f' = 0 
    openList.insert(make_pair (0.0, make_pair (i, j))); 
   
    bool foundDest = false; 
    while (!openList.empty()) { 
        pPair p = *openList.begin(); 
  
        openList.erase(openList.begin()); 
  
        // Add vertex closed list 
        i = p.second.first; 
        j = p.second.second; 
        closedList[i][j] = true; 
       
        double gNew, hNew, fNew; 
    
        //----------- (East) ------------ 
        if (isValid (i, j+1) == true) { 
            if (isDestination(i, j+1, dest) == true) { 
                cellDetails[i][j+1].parent_i = i; 
                cellDetails[i][j+1].parent_j = j; 
#ifdef UNIT_TEST
                printf("The destination cell is found\n");
#endif	
                foundDest = true; 
                return(tracePath (size, cellDetails, dest)); 
            }
            else if (closedList[i][j+1] == false && isUnBlocked (size, grid, i, j+1) == true && isUnOccupied ( size, grid, i, j+1)) { 
                gNew = cellDetails[i][j].g + 1.0; 
                hNew = calculateHValue (i, j+1, dest); 
                fNew = gNew + hNew; 
                if (cellDetails[i][j+1].f == FLT_MAX || cellDetails[i][j+1].f > fNew) { 
                    openList.insert(make_pair(fNew, make_pair (i, j+1))); 
                    cellDetails[i][j+1].f = fNew; 
                    cellDetails[i][j+1].g = gNew; 
                    cellDetails[i][j+1].h = hNew; 
                    cellDetails[i][j+1].parent_i = i; 
                    cellDetails[i][j+1].parent_j = j; 
                } 
            } 
        } 
  
        //----------- 4th Successor (West) ------------ 
        if (isValid(i, j-1) == true) { 
            if (isDestination(i, j-1, dest) == true) { 
                cellDetails[i][j-1].parent_i = i; 
                cellDetails[i][j-1].parent_j = j; 
#ifdef UNIT_TEST
                printf("The destination cell is found\n");
#endif	
                foundDest = true; 
                return(tracePath (size, cellDetails, dest)); 
            } 
            else if (closedList[i][j-1] == false && isUnBlocked(size, grid, i, j-1) == true && isUnOccupied ( size, grid, i, j+1)) { 
                gNew = cellDetails[i][j].g + 1.0; 
                hNew = calculateHValue(i, j-1, dest); 
                fNew = gNew + hNew; 
                if (cellDetails[i][j-1].f == FLT_MAX || cellDetails[i][j-1].f > fNew) { 
                    openList.insert(make_pair (fNew, make_pair (i, j-1))); 
                    cellDetails[i][j-1].f = fNew; 
                    cellDetails[i][j-1].g = gNew; 
                    cellDetails[i][j-1].h = hNew; 
                    cellDetails[i][j-1].parent_i = i; 
                    cellDetails[i][j-1].parent_j = j; 
                } 
            } 
        } 
  
        //----------- (North-East) ------------ 
  	if ((i % 2) == 0) {
            if (isValid(i-1, j+1) == true) { 
                if (isDestination(i-1, j+1, dest) == true) { 
                    cellDetails[i-1][j+1].parent_i = i; 
                    cellDetails[i-1][j+1].parent_j = j;
#ifdef UNIT_TEST 
                    printf ("The destination cell is found\n");
#endif 
                    foundDest = true; 
                    return(tracePath (size, cellDetails, dest)); 
                } 
                else if (closedList[i-1][j+1] == false && isUnBlocked(size, grid, i-1, j+1) == true && isUnOccupied ( size, grid, i, j+1)) { 
                    gNew = cellDetails[i][j].g + 1.414; 
                    hNew = calculateHValue(i-1, j+1, dest); 
                    fNew = gNew + hNew; 
                    if (cellDetails[i-1][j+1].f == FLT_MAX || cellDetails[i-1][j+1].f > fNew) { 
                        openList.insert(make_pair (fNew, make_pair(i-1, j+1))); 
                        cellDetails[i-1][j+1].f = fNew; 
                        cellDetails[i-1][j+1].g = gNew; 
                        cellDetails[i-1][j+1].h = hNew; 
                        cellDetails[i-1][j+1].parent_i = i; 
                        cellDetails[i-1][j+1].parent_j = j; 
                    } 
                } 
            } 
        }
        else {
            if (isValid(i-1, j) == true) { 
               if (isDestination(i-1, j, dest) == true) { 
                    cellDetails[i-1][j].parent_i = i; 
                    cellDetails[i-1][j].parent_j = j;
#ifdef UNIT_TEST 
                    printf ("The destination cell is found\n");
#endif 
                    foundDest = true; 
                    return(tracePath (size, cellDetails, dest)); 
                } 
                else if (closedList[i-1][j] == false && isUnBlocked(size, grid, i-1, j) == true && isUnOccupied ( size, grid, i, j+1)) { 
                    gNew = cellDetails[i][j].g + 1.414; 
                    hNew = calculateHValue(i-1, j, dest); 
                    fNew = gNew + hNew; 
                    if (cellDetails[i-1][j].f == FLT_MAX || cellDetails[i-1][j].f > fNew) { 
                        openList.insert(make_pair (fNew, make_pair(i-1, j))); 
                        cellDetails[i-1][j].f = fNew; 
                        cellDetails[i-1][j].g = gNew; 
                        cellDetails[i-1][j].h = hNew; 
                        cellDetails[i-1][j].parent_i = i; 
                        cellDetails[i-1][j].parent_j = j; 
                    } 
                } 
            } 
        }  

        //----------- (North-West) ------------ 
        if ((i % 2) == 0) {
            if (isValid (i-1, j) == true) { 
                if (isDestination (i-1, j, dest) == true) { 
                    cellDetails[i-1][j].parent_i = i; 
                    cellDetails[i-1][j].parent_j = j;
#ifdef UNIT_TEST 
                    printf ("The destination cell is found\n"); 
#endif
                    foundDest = true; 
                    return(tracePath (size, cellDetails, dest)); 
                } 
                else if (closedList[i-1][j] == false && isUnBlocked(size, grid, i-1, j) == true && isUnOccupied ( size, grid, i, j+1)) { 
                    gNew = cellDetails[i][j].g + 1.414; 
                    hNew = calculateHValue(i-1, j, dest); 
                    fNew = gNew + hNew; 
                    if (cellDetails[i-1][j].f == FLT_MAX || 
                        cellDetails[i-1][j].f > fNew) { 
                        openList.insert( make_pair (fNew, make_pair (i-1, j))); 
                        cellDetails[i-1][j].f = fNew; 
                        cellDetails[i-1][j].g = gNew; 
                        cellDetails[i-1][j].h = hNew; 
                        cellDetails[i-1][j].parent_i = i; 
                        cellDetails[i-1][j].parent_j = j; 
                    } 
                } 
            } 
        }
        else {
            if (isValid (i-1, j-1) == true) { 
                if (isDestination (i-1, j-1, dest) == true) { 
                    cellDetails[i-1][j-1].parent_i = i; 
                    cellDetails[i-1][j-1].parent_j = j;
#ifdef UNIT_TEST 
                    printf ("The destination cell is found\n"); 
#endif
                    foundDest = true; 
                    return(tracePath (size, cellDetails, dest)); 
                } 
                else if (closedList[i-1][j-1] == false && isUnBlocked(size, grid, i-1, j-1) == true && isUnOccupied ( size, grid, i, j+1)) { 
                    gNew = cellDetails[i][j].g + 1.414; 
                    hNew = calculateHValue(i-1, j-1, dest); 
                    fNew = gNew + hNew; 
                    if (cellDetails[i-1][j-1].f == FLT_MAX || 
                        cellDetails[i-1][j-1].f > fNew) { 
                        openList.insert( make_pair (fNew, make_pair (i-1, j-1))); 
                        cellDetails[i-1][j-1].f = fNew; 
                        cellDetails[i-1][j-1].g = gNew; 
                        cellDetails[i-1][j-1].h = hNew; 
                        cellDetails[i-1][j-1].parent_i = i; 
                        cellDetails[i-1][j-1].parent_j = j; 
                    } 
                } 
            } 
        }

        //----------- (South-East) ------------ 
        if ((i % 2) == 0) {
            if (isValid(i+1, j+1) == true) { 
                if (isDestination(i+1, j+1, dest) == true) { 
                    cellDetails[i+1][j+1].parent_i = i; 
                    cellDetails[i+1][j+1].parent_j = j;
#ifdef UNIT_TEST 
                    printf ("The destination cell is found\n");
#endif  
                    foundDest = true; 
                    return(tracePath (size, cellDetails, dest)); 
                } 
                else if (closedList[i+1][j+1] == false && isUnBlocked(size, grid, i+1, j+1) == true && isUnOccupied ( size, grid, i, j+1)) { 
                    gNew = cellDetails[i][j].g + 1.414; 
                    hNew = calculateHValue(i+1, j+1, dest); 
                    fNew = gNew + hNew; 
                    if (cellDetails[i+1][j+1].f == FLT_MAX || cellDetails[i+1][j+1].f > fNew) { 
                        openList.insert(make_pair(fNew, make_pair (i+1, j+1))); 
                        cellDetails[i+1][j+1].f = fNew; 
                        cellDetails[i+1][j+1].g = gNew; 
                        cellDetails[i+1][j+1].h = hNew; 
                        cellDetails[i+1][j+1].parent_i = i; 
                        cellDetails[i+1][j+1].parent_j = j; 
                    } 
                } 
            } 
        }
        else {
            if (isValid(i+1, j) == true) { 
                if (isDestination(i+1, j, dest) == true) { 
                    cellDetails[i+1][j].parent_i = i; 
                    cellDetails[i+1][j].parent_j = j;
#ifdef UNIT_TEST 
                    printf ("The destination cell is found\n"); 
#endif
                    foundDest = true; 
                    return(tracePath (size, cellDetails, dest)); 
                } 
                else if (closedList[i+1][j] == false && isUnBlocked(size, grid, i+1, j) == true && isUnOccupied ( size, grid, i, j+1)) { 
                    gNew = cellDetails[i][j].g + 1.414; 
                    hNew = calculateHValue(i+1, j, dest); 
                    fNew = gNew + hNew; 
                    if (cellDetails[i+1][j].f == FLT_MAX || cellDetails[i+1][j].f > fNew) { 
                        openList.insert(make_pair(fNew, make_pair (i+1, j))); 
                        cellDetails[i+1][j].f = fNew; 
                        cellDetails[i+1][j].g = gNew; 
                        cellDetails[i+1][j].h = hNew; 
                        cellDetails[i+1][j].parent_i = i; 
                        cellDetails[i+1][j].parent_j = j; 
                    } 
                } 
            }
        }

        //----------- (South-West) ------------ 
        if ((i % 2) == 0) {
            if (isValid (i+1, j) == true) { 
                if (isDestination(i+1, j, dest) == true) { 
                    cellDetails[i+1][j].parent_i = i; 
                    cellDetails[i+1][j].parent_j = j;
#ifdef UNIT_TEST 
                    printf("The destination cell is found\n"); 
#endif
                    foundDest = true; 
                    return(tracePath (size, cellDetails, dest)); 
                } 
                else if (closedList[i+1][j] == false && isUnBlocked(size, grid, i+1, j) == true && isUnOccupied ( size, grid, i, j+1)) { 
                    gNew = cellDetails[i][j].g + 1.414; 
                    hNew = calculateHValue(i+1, j, dest); 
                    fNew = gNew + hNew; 
                    if (cellDetails[i+1][j].f == FLT_MAX || cellDetails[i+1][j].f > fNew) { 
                        openList.insert(make_pair(fNew, make_pair(i+1, j))); 
                        cellDetails[i+1][j].f = fNew; 
                        cellDetails[i+1][j].g = gNew; 
                        cellDetails[i+1][j].h = hNew; 
                        cellDetails[i+1][j].parent_i = i; 
                        cellDetails[i+1][j].parent_j = j; 
                    } 
                } 
            }  
        } 
        else { 
            if (isValid (i+1, j-1) == true) { 
                if (isDestination(i+1, j-1, dest) == true) { 
                    cellDetails[i+1][j-1].parent_i = i; 
                    cellDetails[i+1][j-1].parent_j = j;
#ifdef UNIT_TEST 
                    printf("The destination cell is found\n"); 
#endif
                    foundDest = true; 
                    return(tracePath (size, cellDetails, dest)); 
                } 
                else if (closedList[i+1][j-1] == false && isUnBlocked(size, grid, i+1, j-1) == true && isUnOccupied ( size, grid, i, j+1)) { 
                    gNew = cellDetails[i][j].g + 1.414; 
                    hNew = calculateHValue(i+1, j-1, dest); 
                    fNew = gNew + hNew; 
                    if (cellDetails[i+1][j-1].f == FLT_MAX || cellDetails[i+1][j-1].f > fNew) { 
                        openList.insert(make_pair(fNew, make_pair(i+1, j-1))); 
                        cellDetails[i+1][j-1].f = fNew; 
                        cellDetails[i+1][j-1].g = gNew; 
                        cellDetails[i+1][j-1].h = hNew; 
                        cellDetails[i+1][j-1].parent_i = i; 
                        cellDetails[i+1][j-1].parent_j = j; 
                    } 
                } 
            } 
        }
    } 
    if (foundDest == false) {
#ifdef UNIT_TEST
        printf("Failed to find the Destination Cell\n");
#endif	
        stack<Pair> noMove;
        noMove.push(src);
        return noMove;
    }

  
    exit(1);
} 

stack<Pair> Movement(Pair size, Tile ** grid, int x, int z, vec2 dest) 
{
    bool error = false;
    (void)error;

     // Source is selected
    Pair source = make_pair(x, z); 
  
    // Destination is selected
    Pair destination = make_pair(dest.x, dest.y);
 
    // Stack containing path returned
    return (aStarSearch(size, grid, source, destination)); 

}

void showMarbienPicture(int x, int y, GLuint textid) 
{
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
    ggprint8b(&r, 16, 0xffffffff, "Marbien Jimeno");
    glPopMatrix();

}

#endif
