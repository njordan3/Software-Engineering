#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "fonts.h"
#include "log.h"
#include <unistd.h>
#include <stack>
#include <string>
#include <bits/stdc++.h>

#include "Image.h"
#include "maps.h"
#ifdef PROFILE
#include "profiling.h"
#endif

#include "Game.h"
#include "nickolasL.h"
#include "nicholasJo.h"
#include "marbienJ.h"
#include "brandonH.h"
#include "adamO.h"

using namespace std;

/******Image Class Definitions********/
Image::~Image() { delete [] data; }
Image::Image(const char *fname) {
    if (fname[0] == '\0')
        return;
    int ppmFlag = 0;
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    char ppmname[80];
    if (strncmp(name+(slen-4), ".ppm", 4) == 0)
        ppmFlag = 1;
    if (ppmFlag) {
        strcpy(ppmname, name);
    } else {
        name[slen-4] = '\0';
        sprintf(ppmname,"%s.ppm", name);
        char ts[100];
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
    }

    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
        char line[200];
        fgets(line, 200, fpi);
        fgets(line, 200, fpi);
        //skip comments and blank lines
        while (line[0] == '#' || strlen(line) < 2)
            fgets(line, 200, fpi);
        sscanf(line, "%i %i", &width, &height);
        fgets(line, 200, fpi);
        //get pixel data
        int n = width * height * 3;
        data = new unsigned char[n];

        for (int i=0; i<n; i++){
            data[i] = fgetc(fpi);
        }
        fclose(fpi);
    } else {
        printf("ERROR opening image: %s\n",ppmname);
        exit(0);
    }
    if (!ppmFlag)
        unlink(ppmname);

}
/*******************************************************/

void displayCharacterImage(GLuint image, float x, float y) {
    float wid = 50;
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, image);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

class Global {
    public:
        int xres, yres;
        int n;
        int count;
        bool ctrls = true;
        int done = 0;
        static Global * GetInstance()
        {
            if (!instance)
                instance = new Global();
            return instance;
        }
    private:
        static Global * instance;
        Global() {
            xres = 1200;
            yres = 900;
            count = 0;
        }
        Global(Global const& copy);
        Global & operator=(Global const& copy);

};
Global* Global::instance = 0;
Global * g = Global::GetInstance();

class X11_wrapper {
    private:
        Display *dpy;
        Window win;
        GLXContext glc;
    public:
        ~X11_wrapper() {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        X11_wrapper() {
            GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            int w = g->xres, h = g->yres;
            dpy = XOpenDisplay(NULL);
            if (dpy == NULL) {
                cout << "\n\tcannot connect to X server\n" << endl;
                exit(EXIT_FAILURE);
            }
            Window root = DefaultRootWindow(dpy);
            XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
            if (vi == NULL) {
                cout << "\n\tno appropriate visual found\n" << endl;
                exit(EXIT_FAILURE);
            } 
            Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
            XSetWindowAttributes swa;
            swa.colormap = cmap;
            swa.event_mask =
                ExposureMask | KeyPressMask | KeyReleaseMask |
                ButtonPress | ButtonReleaseMask |
                PointerMotionMask |
                StructureNotifyMask | SubstructureNotifyMask;
            win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
                    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
            set_title();
            glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
            glXMakeCurrent(dpy, win, glc);
        }
        void set_title() {
            //Set the window title bar.
            XMapWindow(dpy, win);
            XStoreName(dpy, win, "Fate: The Shadow Kingdom of Darkness: Two: The Second One");
        }
        bool getXPending() {
            //See if there are pending events.
            return XPending(dpy);
        }
        XEvent getXNextEvent() {
            //Get a pending event.
            XEvent e;
            XNextEvent(dpy, &e);
            return e;
        }
        void swapBuffers() {
            glXSwapBuffers(dpy, win);
        }
} x11;

//Function prototypes
void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void render();
Game game(g->xres, g->yres);
extern NJordGlobal njG;
//=====================================
// MAIN FUNCTION IS HERE
//=====================================
int main()
{
    logOpen();
    Log("main()\n");
    game.init(); 
    srand(time(NULL));
    init_opengl();
#ifdef SOUND
    njG.sound.initializeSounds();
    njG.sound.loadSounds();
    alSourcePlay(njG.sound.menuMusic);
#endif
    //Main animation loop

    while (!game.getDone()) {
        //Process external events.
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            check_mouse(&e);
            g->done = check_keys(&e);
        }
        render();
        x11.swapBuffers();
    }
    logClose();
#ifdef PROFILE
    printf("Program Complete\n\n");
    printf("Profiling Times:\n");
    printf("njG.sound.loadSounds(): %lf nanoseconds\n", njG.loadSoundTime / 10e9);
#endif
    return 0;
}

void init_opengl(void)
{
    Log("init_opengl()\n");
    //OpenGL initialization
    glViewport(0, 0, g->xres, g->yres);
    //Initialize matrices

    glOrtho(0, g->xres, 0, g->yres, -1, 1);

    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    //Insert Fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    //GenerateGLTexture(g.archerImage, "./images/nickLCreditPic.jpg", false);
    //GenerateGLTexture(g.soldierImage, "./images/nicholasJo.png", false);
    //GenerateGLTexture(g.tankImage, "./images/brandonH.png", false);
}

void init_opengl3D(void)
{
    //OpenGL initialization
    glViewport(0, 0, g->xres, g->yres);
    //Initialize matrices
    //3D perspective view

    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)g->xres/(GLfloat)g->yres,0.1f,100.0f);

    glShadeModel(GL_SMOOTH);//enables smooth shading

    // sets the depth buffer//stop elements from drawing over others
    glClearDepth(1.0f);//Depth buffer setup
    glEnable(GL_DEPTH_TEST);//Enables Depth Testing
    glDepthFunc(GL_LEQUAL);//The type of depth test to do

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//??makes the perspective view better??

    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    //Insert Fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    //GenerateGLTexture(g.archerImage, "./images/nickLCreditPic.jpg", false);
    //GenerateGLTexture(g.soldierImage, "./images/nicholasJo.png", false);
    //GenerateGLTexture(g.tankImage, "./images/brandonH.png", false);
}
void check_mouse(XEvent *e)
{
    static int savex;
    static int savey;
    //int flag;

    if (e->type != ButtonRelease &&
            e->type != ButtonPress) {
        //This is not a mouse event that we care about.
        return;
    }
    if (e->type == ButtonRelease) {
        return;
    }
    //----------------------------------------------------------
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button was pressed
            if(savex != e->xbutton.x || savey != e->xbutton.y) {
                savex = e->xbutton.x;
                savey = e->xbutton.y;
            }
            game.procMouseInput(savex, savey);
            //---------------------------------------------------------
            if (e->xbutton.button==3) {
                //Right button was pressed
                return;
            }
        }
    }
}

int check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0); 
    if (e->type == KeyPress) {
        game.procKeyInput(key);
    }
    return 0;
}

void render()
{
    // ONE LINE RENDER!!!!
    game.drawGameState();
    // Placeholder control display while in testing
    extern BHglobal bhg;
    if(bhg.ctrls) {
        Rect r;
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, g->xres, 0, g->yres, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        r.bot = g->yres - 20;
        r.left = 10;
        r.center = 0;
        ggprint8b(&r, 16, 0xFFFFFFFF, "-----Controls-----");
        ggprint8b(&r, 16, 0xFFFFFFFF, "W - Camera Up");
        ggprint8b(&r, 16, 0xFFFFFFFF, "A - Camera Left");
        ggprint8b(&r, 16, 0xFFFFFFFF, "S - Camera Down");
        ggprint8b(&r, 16, 0xFFFFFFFF, "D - Camera Right");
        ggprint8b(&r, 16, 0xFFFFFFFF, "C - Center On Player");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Esc - Pause Game");

        ggprint8b(&r, 16, 0xFFFFFFFF, "---World Controls---");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Left Click - Move/Interact");

        ggprint8b(&r, 16, 0xFFFFFFFF, "---Battle Controls---");
        ggprint8b(&r, 16, 0xFFFFFFFF, "E - End Turn");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Left Click - Move/Attack");

        ggprint8b(&r, 16, 0xFFFFFFFF, "-----Objective-----");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Defeat all enemies on the map");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Do not let your hero die.");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Heal after battle via towns.");

        glPopMatrix();
    }
}
