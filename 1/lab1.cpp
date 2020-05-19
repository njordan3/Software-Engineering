//
//modified by: Nicholas Jordan
//date: 1/24/2019
//
//3350 Spring 2018 Lab-1
//This program demonstrates the use of OpenGL and XWindows
//
//Assignment is to modify this program.
//You will follow along with your instructor.
//
//Elements to be learned in this lab...
// .general animation framework
// .animation loop
// .object definition and movement
// .collision detection
// .mouse/keyboard interaction
// .object constructor
// .coding style
// .defined constants
// .use of static variables
// .dynamic memory allocation
// .simple opengl components
// .git
//
//elements we will add to program...
//   .Game constructor
//   .multiple particles
//   .gravity
//   .collision detection
//   .more objects
//
#include <iostream>
using namespace std;
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"

const int MAX_PARTICLES = 2000;
const float GRAVITY = -0.1;

//some structures

struct Vec {
	float x, y, z;
};

struct Shape {
	float width, height;
	float radius;
	Vec center;
    Shape() {
        //define box shape
        width = 100.0;
        height = 10.0;
    }
};

struct Particle {
	Shape s;
	Vec velocity;
};

class Global {
public:
	int xres, yres;
	Shape box[5];
	Particle particle[MAX_PARTICLES];
	int n;
	Global() {
		xres = 800;
		yres = 600;
		n = 0;
	}
} g;

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
		int w = g.xres, h = g.yres;
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
		XStoreName(dpy, win, "3350 Lab1");
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
void movement();
void render();



//=====================================
// MAIN FUNCTION IS HERE
//=====================================
int main()
{
	srand(time(NULL));
	init_opengl();
	//Main animation loop
	int done = 0;
	while (!done) {
		//Process external events.
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			check_mouse(&e);
			done = check_keys(&e);
		}
		movement();
		render();
		x11.swapBuffers();
	}
    cleanup_fonts();
	return 0;
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//Set 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//Set the screen background color
	glClearColor(0.1, 0.1, 0.1, 1.0);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
}

void makeParticle(int x, int y)
{
	if (g.n >= MAX_PARTICLES)
		return;
	cout << "makeParticle() " << x << " " << y << endl;
	//position of particle
	Particle *p = &g.particle[g.n];
	p->s.center.x = x;
	p->s.center.y = y;
	p->velocity.y = -4.0;
	p->velocity.x =  1.0;
	++g.n;
}

void check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;

	if (e->type != ButtonRelease &&
		e->type != ButtonPress &&
		e->type != MotionNotify) {
		//This is not a mouse event that we care about.
		return;
	}
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button was pressed
			int y = g.yres - e->xbutton.y;
            makeParticle(savex, y);
			return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed
			return;
		}
	}
	if (e->type == MotionNotify) {
		//The mouse moved!
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			savex = e->xbutton.x;
			savey = e->xbutton.y;
			//int y = g.yres - e->xbutton.y;
            //makeParticle(savex, y);
		}
	}
}

int check_keys(XEvent *e)
{
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyPress) {
		switch (key) {
			case XK_1:
				//Key 1 was pressed
				break;
			case XK_a:
				//Key A was pressed
				break;
			case XK_Escape:
				//Escape key was pressed
				return 1;
		}
	}
	return 0;
}

void movement()
{
	if (g.n <= 0)
		return;
    for(int i=0; i<g.n; i++) {    
	    Particle *p = &g.particle[i];
	    p->s.center.x += p->velocity.x;
	    p->s.center.y += p->velocity.y;
        p->velocity.y += GRAVITY;
	    //check for collision with shapes...
        //Shape 1
        g.box[0].center.y = 500;
        g.box[0].center.x = 125;
	    if (p->s.center.y < g.box[0].center.y + g.box[0].height &&
	        p->s.center.y > g.box[0].center.y - g.box[0].height &&
            p->s.center.x < g.box[0].center.x + g.box[0].width &&
	        p->s.center.x > g.box[0].center.x - g.box[0].width) {
            //bounce
            p->velocity.y = -p->velocity.y * 0.65;
        }

        //Shape 2
        g.box[1].center.y = 400;
        g.box[1].center.x = 250;
	    if (p->s.center.y < g.box[1].center.y + g.box[1].height &&
	        p->s.center.y > g.box[1].center.y - g.box[1].height &&
            p->s.center.x < g.box[1].center.x + g.box[1].width &&
	        p->s.center.x > g.box[1].center.x - g.box[1].width) {
            //bounce
            p->velocity.y = -p->velocity.y * 0.65;
        }

        //Shape 3
        g.box[2].center.y = 300;
        g.box[2].center.x = 375;
	    if (p->s.center.y < g.box[2].center.y + g.box[2].height &&
	        p->s.center.y > g.box[2].center.y - g.box[2].height &&
            p->s.center.x < g.box[2].center.x + g.box[2].width &&
	        p->s.center.x > g.box[2].center.x - g.box[2].width) {
            //bounce
            p->velocity.y = -p->velocity.y * 0.65;
        }

        //Shape 4
        g.box[3].center.y = 200;
        g.box[3].center.x = 500;
	    if (p->s.center.y < g.box[3].center.y + g.box[3].height &&
	        p->s.center.y > g.box[3].center.y - g.box[3].height &&
            p->s.center.x < g.box[3].center.x + g.box[3].width &&
	        p->s.center.x > g.box[3].center.x - g.box[3].width) {
            //bounce
            p->velocity.y = -p->velocity.y * 0.65;
        }

        //Shape 5
        g.box[4].center.y = 100;
        g.box[4].center.x = 625;
	    if (p->s.center.y < g.box[4].center.y + g.box[4].height &&
	        p->s.center.y > g.box[4].center.y - g.box[4].height &&
            p->s.center.x < g.box[4].center.x + g.box[4].width &&
	        p->s.center.x > g.box[4].center.x - g.box[4].width) {
            //bounce
            p->velocity.y = -p->velocity.y * 0.65;
        }

	    //check for off-screen
	    if (p->s.center.y < 0.0) {
		    //cout << "off screen" << endl;
            //this code is optimized below
            //g.particle[i] = g.particle[g.n-1]
            //--g.n;
	        g.particle[i] = g.particle[--g.n];
	    }
    }
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);
	
    float w, h;

    glPushMatrix();
	glColor3ub(150,160,220);
    for(int i=0; i<g.n; i++) {
	    Vec *c = &g.particle[i].s.center;
    	w =
	    h = 2;
	    glBegin(GL_QUADS);
		    glVertex2i(c->x-w, c->y-h);
		    glVertex2i(c->x-w, c->y+h);
		    glVertex2i(c->x+w, c->y+h);
		    glVertex2i(c->x+w, c->y-h);
	    glEnd();
    }
	glPopMatrix();
	
    //Draw shapes...
	//
	//draw a box
	glColor3ub(90,140,90);
	//Shape 1
	glPushMatrix();
	glTranslatef(125, 500, 0);
	w = g.box[0].width;
	h = g.box[0].height;
	glBegin(GL_QUADS);
		glVertex2i(-w, -h);
		glVertex2i(-w,  h);
		glVertex2i( w,  h);
		glVertex2i( w, -h);
    glEnd();
    glPopMatrix();
    //Shape 2
	glPushMatrix();
	glTranslatef(250, 400, 0);
	w = g.box[1].width;
	h = g.box[1].height;
	glBegin(GL_QUADS);
		glVertex2i(-w, -h);
		glVertex2i(-w,  h);
		glVertex2i( w,  h);
		glVertex2i( w, -h);
	glEnd();
	glPopMatrix();
    //Shape3
	glPushMatrix();
	glTranslatef(375, 300, 0);
	w = g.box[2].width;
	h = g.box[2].height;
	glBegin(GL_QUADS);
		glVertex2i(-w, -h);
		glVertex2i(-w,  h);
		glVertex2i( w,  h);
		glVertex2i( w, -h);
	glEnd();
	glPopMatrix();
    //Shape4
	glPushMatrix();
	glTranslatef(500, 200, 0);
	w = g.box[3].width;
	h = g.box[3].height;
	glBegin(GL_QUADS);
		glVertex2i(-w, -h);
		glVertex2i(-w,  h);
		glVertex2i( w,  h);
		glVertex2i( w, -h);
	glEnd();
	glPopMatrix();
    //Shape5
	glPushMatrix();
	glTranslatef(625, 100, 0);
	w = g.box[4].width;
	h = g.box[4].height;
	glBegin(GL_QUADS);
		glVertex2i(-w, -h);
		glVertex2i(-w,  h);
		glVertex2i( w,  h);
		glVertex2i( w, -h);
	glEnd();
	glPopMatrix();
    //
	//Draw the particle here
	/*glPushMatrix();
	glColor3ub(150,160,220);
    for(int i=0; i<g.n; i++) {
	    Vec *c = &g.particle[i].s.center;
    	w =
	    h = 2;
	    glBegin(GL_QUADS);
		    glVertex2i(c->x-w, c->y-h);
		    glVertex2i(c->x-w, c->y+h);
		    glVertex2i(c->x+w, c->y+h);
		    glVertex2i(c->x+w, c->y-h);
	    glEnd();
    }
	glPopMatrix();*/
	//
	//Draw your 2D text here
    
    Rect r[5];

    unsigned int c = 0x00ffff44;
    
    r[0].bot = 495;
    r[0].left = 125;
    r[0].center = 125;
    ggprint8b(&r[0], 16, c, "Requirements");
    
    r[1].bot = 395;
    r[1].left = 250;
    r[1].center = 250;
    ggprint8b(&r[1], 16, c, "Design");
    
    r[2].bot = 295;
    r[2].left = 375;
    r[2].center = 375;
    ggprint8b(&r[2], 16, c, "Implementation");
    
    r[3].bot = 195;
    r[3].left = 500;
    r[3].center = 500;
    ggprint8b(&r[3], 16, c, "Test");
    
    r[4].bot = 95;
    r[4].left = 625;
    r[4].center = 625;
    ggprint8b(&r[4], 16, c, "Maintenance");

}
