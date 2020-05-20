//Author: Nickolas Larson
//Date: 2/14/2019
//Modified By: Nickolas Larson
//Modified 3/20/2019

#ifndef NICKOLAS_L_H
#define NICKOLAS_L_H

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <stack>
#include <set>
#include <cmath>
#include <GL/glx.h>
#include <GL/glu.h>
#include "fonts.h"

#include "Image.h"
//#include "Game.h"
#include "adamO.h"


using namespace std;

#define PI 3.14159265359

class Image;

class NLarsGlobal {
public:
	int ** MainMap;
	int ** BattleMap1;
	int ** BattleMap2;
	static NLarsGlobal * GetInstance();
private:
	static NLarsGlobal * instance;
	NLarsGlobal();
	NLarsGlobal(NLarsGlobal const& copy);
	NLarsGlobal& operator=(NLarsGlobal const& copy);

	//static NLarsGlobal& getInstance();
};



struct vec2 {
	float x, y;
	vec2();
	vec2(float x, float z);
	vec2 operator = (const vec2& right);
	vec2 operator +(const vec2& right);
	vec2 operator +=(const vec2& right);
};

struct Matrix {
	float* m_arr;
	int m_size;
	int N;
	Matrix();
	Matrix(int size);//identity matrix
	Matrix(int size, float arr[]);
	Matrix(const Matrix & temp);
	~Matrix();
	float operator ()(int row, int col);
	Matrix operator =(Matrix right);
	Matrix operator +(Matrix & right);
	Matrix operator -(Matrix & right);
	Matrix operator *(Matrix & right);
	Matrix operator *(float num);
	void transpose();
	bool canMath(int lm_size, int rm_size);
	void print();
	Matrix matrixMinor(int row, int col);
	static void matrixCofactor(Matrix & mat);
	static float determinant(Matrix & mat);
	static Matrix inverse(Matrix & mat);
};
struct vec4 {
	float x, y, z, w;
	vec4(float _x, float _y, float _z, float _w);
	friend vec4 operator*(Matrix & mat, vec4 vec);
};

struct vec3 {
	float x, y, z;
	vec3();
	vec3(float x, float y, float z);
	void operator()(float _x, float _y, float _z);
	vec3 operator = (const vec3& right);
	vec3 operator +(const vec3& right);
	vec3 operator +=(const vec3& right);
	vec3 operator -(const vec3& right);
	vec3 operator -=(const vec3& right);
	vec3 operator*(float scale);
	vec3 operator/(float scale);
	vec3 operator=(const vec4& right);

	static vec3 crossProd(const vec3& left, const vec3& right);
	static float Magnitude(vec3& vec);
	static vec3 Normalize(const vec3& vec);
};




void GenerateGLTexture(GLuint & texture, const char * dataSrc, bool inverted);
void sendScore( string name, int score);
void draw_nickLCredit(int x, int y, GLuint texture);

struct Model {
	GLuint texture;
	vector<vec3> vert;
	vector<vec2> vertTex;
	vector<vec3> vertNorm;
	//connection values
	vector<unsigned int> vIndices, vtIndices, vnIndices;
	vec3 pos; //moves along x and z
	Model ();
	Model ( const char * objFile, const char * texFile);
	void draw(int x, int z, float y=0, float yaw = 0);
	void drawTitleModel(float x, float y, float z, float yaw);
	bool GenerateModel( const char * objFile );
	bool GenerateTexture( const char * texFile );
private:
	//bool GenerateModel( const char * objFile );
	//bool GenerateTexture( const char * texFile );
};

struct Tile {
	int modelID;
	bool occ;//occupied
	int x,z;
	float radius = 1;
	bool collisionDetect(float x, float z);
	
};

class Map {
public:
	int mapW, mapH;
	Tile ** tile;
	Map(){}
	Map(int ** map, int _width, int _height);
	~Map();
	void draw();
	vec2 checkCollision( float x, float z);
};


class Camera {
private:
	vec3 wPos;//world position
	vec3 camPos;
	vec3 front;
	vec3 wUp;
	vec3 up;
	vec3 right;

	//vec2 vel;
	float radius;
	float pitch;
	
public:
	float yaw;
	//view was private
	vec3 view;
	Camera();
	Camera(float rot, int posx, int posz);
	void update();
	
	void rotate(float direction);
	void translate(int key, int size);
	void updateVectors();
	void setCameraPosition(vec3 newPos);
	vec3 getPos();
	Matrix getViewMatrix();
};

class Picker {
private:
	vec3 curRay;

	Matrix projectionMatrix;
	Matrix viewMatrix;

public:
	Picker();
	void update(Matrix & projMatrix, Camera camera, float xres, float yres, float mousex, float mousey);
	vec3 getCurrentRay();
	void rotate(float deg);

};

class GameState {
private:

public:
	GameState() {};
	virtual int procMouseInput(int x, int y) = 0;
	virtual int procKeyInput(int key) = 0;
	virtual void drawGameState() = 0;
};

class WorldGS : public GameState {
protected:
	Map map;
	Camera camera;
	float xres, yres;
    uiboxes UI;
	allyboxes AB;
    Picker pkr;
	vec3 pickPos;
	Matrix projMatrix;
	vector<pair<int,int>> path;
	bool initializedPos;
public:
	WorldGS(int ** mapArr,int sizex,int sizey,
	float camRot, int posx, int posz,
	float xres, float yres);
	void initWGS_GL();
	void pick(vec3 ray);
	int procMouseInput(int x, int y);
	int procKeyInput(int key);
	void drawPath();
	void drawGameState();
};
class BattleGS : public WorldGS {
protected:
	battleboxes BT;
	allyboxes AB;
    int turns;
    int count;
    int enemy;
public:
	BattleGS(int ** mapArr,int sizex,int sizey,
	float camRot, int posx, int posz,
	float xres, float yres, int enemy);
	int procMouseInput(int x, int y);
	int procKeyInput(int key);
	void drawPath();
	void drawGameState();
    void endTurn();
};
#endif
