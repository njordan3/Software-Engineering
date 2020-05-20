//Author: Nickolas Larson
//Date: 2/14/2019
//Modified By: Nickolas Larson
//Modified: 5/5/2019

#ifndef NICKOLAS_L_CPP
#define NICKOLAS_L_CPP

#include "nickolasL.h"
#include "nicholasJo.h"
#include "marbienJ.h"


#define RAD(degree) degree * PI / 180

#define XOFFSET -2.0f
#define ZOFFSET -1.70710378118f

extern int mainMap[][25];
extern int battleMap1[][10];
extern int battleMap2[][10];
extern NJordGlobal njG;
extern AOglobal aog;

/*============LARSON'S GLOBALS==========*/
NLarsGlobal* NLarsGlobal::instance = 0;
NLarsGlobal* NLarsGlobal::GetInstance()
{
	if (!instance)
		instance = new NLarsGlobal();
	return instance;
	
}
NLarsGlobal * nlG = NLarsGlobal::GetInstance();
NLarsGlobal::NLarsGlobal()
{
	MainMap = new int*[25];
	for ( int i = 0 ; i < 25 ; i++) {
		MainMap[i] = new int[25];
		for (int j = 0 ; j < 25 ; j++) {
			MainMap[i][j] = mainMap[i][j];
		}
	}
	BattleMap1 = new int*[10];
	for ( int i = 0 ; i < 10 ; i++) {
		BattleMap1[i] = new int[10];
		for (int j = 0 ; j < 10 ; j++) {
			BattleMap1[i][j] = battleMap1[i][j];
		}
	}
	BattleMap2 = new int*[10];
	for ( int i = 0 ; i < 10 ; i++) {
		BattleMap2[i] = new int[10];
		for (int j = 0 ; j < 10 ; j++) {
			BattleMap2[i][j] = battleMap2[i][j];
		}
	}

}
/*
NLarsGlobal& NLarsGlobal::getInstance()
{
	static NLarsGlobal instance;
	return instance;
}*/
/*=======================================*/
/*========VECTORE MATH(vec2/vec3)========*/
//vec2-----------------------------------
vec2::vec2() 
{
	x = y = 0;
}
vec2::vec2(float _x,float _y)
{
	x = _x;
	y = _y;
}

vec2 vec2::operator = (const vec2& right)
{
	x = right.x;
	y = right.y;
	return *this;
}
vec2 vec2::operator +(const vec2& right)
{
	vec2 ret;
	ret.x = x + right.x;
	ret.y = y + right.y;
	return ret;

}
vec2 vec2::operator +=(const vec2& right)
{
	return *this + right;

}
//---------------------------------------
//vec3-----------------------------------
vec3::vec3()
{
	x = y = z = 0;
}

vec3::vec3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void vec3::operator()(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}
vec3 vec3::operator = (const vec3& right)
{	
	x = right.x;
	y = right.y;
	z = right.z;
	return *this;
}
vec3 vec3::operator +(const vec3& right)
{
	vec3 ret;
	ret.x = x + right.x;
	ret.y = y + right.y;
	ret.z = z + right.z;
	return ret;
}
vec3 vec3::operator +=(const vec3& right)
{
	this->x += right.x;
	this->y += right.y;
	this->z += right.z;
	return *this;
}
vec3 vec3::operator -(const vec3& right)
{
	vec3 ret;
	ret.x = x - right.x;
	ret.y = y - right.y;
	ret.z = z - right.z;
	return ret;
}
vec3 vec3::operator -=(const vec3& right)
{
	this->x -= right.x;
	this->y -= right.y;
	this->z -= right.z;
	return *this;
}
vec3 vec3::operator*(float scale)
{
	return vec3(x*scale, y*scale, z*scale);
}
vec3 vec3::operator/(float scale)
{
	return vec3(x/scale, y/scale, z/scale);
}
vec3 vec3::operator=(const vec4 & right)
{
	return vec3(right.x, right.y, right.z);
}
vec3 vec3::crossProd(const vec3& left, const vec3& right)
{
	vec3 ret;
	ret.x = left.y*right.z - left.z*right.y;
	ret.y = left.z*right.x - left.x*right.z;
	ret.z = left.x*right.y - left.y*right.x;
	return ret;
}
float vec3::Magnitude(vec3& vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}
vec3 vec3::Normalize(const vec3& vec)
{
	vec3 ret = vec;
	return (ret/Magnitude(ret));
}
//---------------------------------------
//----------------vec4-------------------
vec4::vec4(float _x, float _y, float _z, float _w)
{
	x = _x; y = _y; z = _z; w = _w;
}
vec4 operator*(Matrix & mat, vec4 vec)
{
	if (mat.N != 4)
		return vec4(0,0,0,0);

	return vec4(
			mat(0,0)*vec.x + mat(0,1)*vec.y + mat(0,2)*vec.z + mat(0,3)*vec.w,
			mat(1,0)*vec.x + mat(1,1)*vec.y + mat(1,2)*vec.z + mat(1,3)*vec.w,
			mat(2,0)*vec.x + mat(2,1)*vec.y + mat(2,2)*vec.z + mat(2,3)*vec.w,
			mat(3,0)*vec.x + mat(3,1)*vec.y + mat(3,2)*vec.z + mat(3,3)*vec.w
	);
}

/*=======================================*/

/*==============MATRIX===================*/

Matrix::Matrix()
{
	m_arr = NULL;
	m_size = 0;
#ifdef UNIT_TEST
	std::cout << "Created Matrix" << std::endl;

#endif
}
//identity
Matrix::Matrix(int size)
{
	if (size == 1) throw -1;
	N = size;
	m_size = size*size;
	m_arr = new float[m_size];
	for (int i = 0; i < m_size; i++)
		m_arr[i] = i/size == i%size ? 1 : 0;
}

Matrix::Matrix(int size, float  arr[])
{
	if (size == 1) throw -1;
	N = size;
	m_size = size*size;
	m_arr = new float[m_size];
	if (arr)
	{
		for (int i = 0; i < m_size; i++)
			m_arr[i] = arr[i];
	}
}

//copy constructor
Matrix::Matrix(const Matrix & temp)
{
	N = temp.N;
	m_size = temp.m_size;
	m_arr = new float[m_size];

	for (int i = 0; i < m_size; i++)
		m_arr[i] = temp.m_arr[i];
}

Matrix::~Matrix()
{
	delete[] m_arr;
}
float Matrix::operator()(int row, int col)
{
	if (row > m_size - 1 || col > m_size -1 ) throw -1;
	if (row < 0 || col < 0) throw -1;
	return m_arr[(N * row) + col];
}
Matrix Matrix::operator =(Matrix right)
{
	if (m_arr)
		delete[] m_arr;
	N = right.N;
	m_size = right.m_size;
	m_arr = new float[m_size];
	for (int i = 0; i < m_size; i++)
		m_arr[i] = right.m_arr[i];
	return right;
}
Matrix Matrix::operator+(Matrix & right)
{
	float * newArr = new float[m_size];
	if (canMath(m_size, right.m_size))
	{
		for (int i = 0; i < m_size; i++)
		{
			newArr[i] = m_arr[i] + right.m_arr[i];
		}
		Matrix newMatrix(N, newArr);
		delete[] newArr;
		return newMatrix;
	}
	// handle error
	return Matrix();
}
Matrix Matrix::operator -(Matrix & right)
{
	float * newArr = new float[m_size];
	if (canMath(m_size, right.m_size))
	{
		for (int i = 0; i < m_size; i++)
		{
			newArr[i] = m_arr[i] - right.m_arr[i];
		}
		Matrix newMatrix(N, newArr);
		delete[] newArr;
		return newMatrix;
	}
	return Matrix();
}
Matrix Matrix::operator *(Matrix & right)
{
	if (canMath(m_size, right.m_size))
	{
		float * newArr = new float[m_size];
		float elementHold;
		int pos1, pos2;
		int index = 0;
		//loop to run through each row of the left matrix
		for (int y1 = 0; y1 < N; y1++)
		{
			//loop to run through each col of the right matrix
			for (int x2 = 0; x2 < N; x2++)
			{
				elementHold = 0;
				//loop through each of the col for left matrix and rows for right matrix
				for (int x1 = 0; x1 < N; x1++) {
					pos1 = x1 + (y1*N);
					pos2 = x2 + (x1*N);
					elementHold += m_arr[pos1] * right.m_arr[pos2];
				}
				newArr[index++] = elementHold;
			}
		}
		Matrix newMatrix(N, newArr);
		delete[] newArr;
		return newMatrix;
	}
	return Matrix();
}
Matrix Matrix::operator *(float num)
{
	float * newArr = new float[m_size];

	for (int i = 0; i < m_size; i++)
		newArr[i] = num * m_arr[i];

	Matrix newMatrix(N, newArr);
	delete[] newArr;
	return newMatrix;
}
void Matrix::transpose()
{
	float * transpose = new float[m_size];
	int col = 0, row = 0;
	for (int i = 0; i < m_size; i++)
	{
		transpose[i] = m_arr[(N * row++) + col];
		if (row == N) {
			row = 0;
			col++;
		}
	}
	delete[] m_arr;
	m_arr = transpose;
};
Matrix Matrix::matrixMinor(int col, int row)
{
	
	if (row >= N || row < 0) throw -1;
	if (col >= N || col < 0) throw -1;

	float * matVals;
	//number of elements of n-1 x n-1 matrix
	int newMatSize = (N - 1) * (N - 1);
	matVals = new float[ newMatSize ];
	int matInd = 0;
	for (int j = 0 ; j < m_size ; j++ ) {
			//(nxn) columns   && skip first row(Just in case)
			if (  j/N != col && j%N != row) {
				matVals[matInd++] = m_arr[j];
			}
	}
	
	Matrix ret( N-1 , matVals);
	delete[] matVals;
	return ret;
}
void Matrix::matrixCofactor(Matrix & mat)
{
	bool neg = false;
	for ( int i = 0 ; i < mat.m_size ; i++ ) {
		
		//even rows = false
		if (i%mat.N == 0) {
			if ((i/mat.N) % 2 == 0 )
				neg = false;
			else
				neg = true;
		}
		
		mat.m_arr[i] *= neg? -1 : 1 ;
		neg = !neg;
	}
}
float Matrix::determinant(Matrix & mat)
{
	//mat.print();
	if (mat.N < 2 ) throw -1;
	// determinant of a 2x2 matrix
	if (mat.N == 2)
		return mat(0,0)*mat(1,1) - mat(1,0)*mat(0,1);
	
	//det of nxn | n > 2
	float det = 0;
	Matrix * minorMat;
	for ( int i = 0; i < mat.N ; i++ ) {
		minorMat = new Matrix(mat.matrixMinor(0,i));
		det += mat.m_arr[i]*determinant(*minorMat) * (i%2 == 0 ? 1 : -1);
		delete minorMat;
	}

	return det;
}

Matrix Matrix::inverse(Matrix & mat)
{
	Matrix inverse(mat.N);
	for (int i = 0 ; i < mat.m_size ; i++) {
		Matrix matMinor( mat.matrixMinor(i/mat.N, i%mat.N));
		inverse.m_arr[i] = determinant(matMinor);
	}
	matrixCofactor(inverse);
	inverse.transpose();
	inverse = inverse * (1.0f/determinant(mat));
	return inverse;
}

bool Matrix::canMath(int lm_size, int rm_size)
{
	return (lm_size == rm_size);
}

void Matrix::print()
{
	int index = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cout << m_arr[index++] << "  ";
		}
		std::cout << std::endl;
	}
}

/*=======================================*/
/*=====Function added to Image Class=====*/
void Image::invertY()
{
	unsigned char flip[height][width*3];
	int iter = 0, i, j;
	for ( i = 0 ; i < height ; i++ ) {
		for ( j = 0 ; j < width * 3 ; j++ ) {
			flip[i][j] = data[iter++];
		}
	}

	iter = 0;
	for ( i = height - 1 ; i >= 0 ; i-- ) {
		for ( j = 0 ; j < width * 3 ; j++ ) {
			data[iter++] = flip[i][j];
		}
	}
}
/*=======================================*/
/*==============Draw Credits=============*/

void draw_nickLCredit(int x, int y, GLuint texture)
{
	Rect r;
	float wid = 50;
	glPushMatrix();
	glTranslatef(x,y,0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);


	glTranslatef(wid,0,0);
	unsigned int c = 0xffffffff;
	r.bot = 0;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, c, "Nickolas Larson");


	glPopMatrix();
}

/*=======================================*/
/*============MODEL STUCTURE=============*/
Model::Model() {
	const char * objFile = "models/characters/Tank.obj";
	const char * texFile = "models/characters/ModelTexturePlayer.png";
	if (!GenerateModel(objFile)) {
		printf("Failed to generate model\n");
	}
	if (!GenerateTexture(texFile)) {
		printf("Failed to load texture\n");
	}
}

Model::Model( const char * objFile, const char * texFile )
{
	if (!GenerateModel(objFile)) {
		printf("Failed to generate model\n");
	}
	if (!GenerateTexture(texFile)) {
		printf("Failed to load texture\n");
	}
}

void Model::draw(int x, int z, float y, float yaw) 
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	
	float posx = x * XOFFSET;
	float posz = z * ZOFFSET;
	float posy = y; 
	
	if (z%2 == 0)
		posx -= 1.0f;
	glTranslatef(posx, posy, posz);
	
	if ( yaw > 0 ) {
		glRotatef(yaw, 0, 1.0f, 0);
	}	
	glBegin(GL_TRIANGLES);

	for ( unsigned int i = 0 ; i < vIndices.size() ; i+=3 ) {
		glTexCoord2f(vertTex[vtIndices.at(i)-1].x,
				vertTex[vtIndices.at(i)-1].y);
		glVertex3f(vert[vIndices.at(i)-1].x ,
				vert[vIndices.at(i)-1].y,
				vert[vIndices.at(i)-1].z);
		glTexCoord2f(vertTex[vtIndices.at(i+1)-1].x,
				vertTex[vtIndices.at(i+1)-1].y);
		glVertex3f(vert[vIndices.at(i+1)-1].x,
				vert[vIndices.at(i+1)-1].y,
				vert[vIndices.at(i+1)-1].z);
		glTexCoord2f(vertTex[vtIndices.at(i+2)-1].x,
				vertTex[vtIndices.at(i+2)-1].y);
		glVertex3f(vert[vIndices.at(i+2)-1].x,
				vert[vIndices.at(i+2)-1].y,
				vert[vIndices.at(i+2)-1].z);
	}
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Model::drawTitleModel(float x, float y, float z, float yaw) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(x, y, z);
	glRotatef(yaw, 0, 1.0f, 0);
		
	glBegin(GL_TRIANGLES);

	for ( unsigned int i = 0 ; i < vIndices.size() ; i+=3 ) {
		glTexCoord2f(vertTex[vtIndices.at(i)-1].x,
				vertTex[vtIndices.at(i)-1].y);
		glVertex3f(vert[vIndices.at(i)-1].x ,
				vert[vIndices.at(i)-1].y,
				vert[vIndices.at(i)-1].z);
		glTexCoord2f(vertTex[vtIndices.at(i+1)-1].x,
				vertTex[vtIndices.at(i+1)-1].y);
		glVertex3f(vert[vIndices.at(i+1)-1].x,
				vert[vIndices.at(i+1)-1].y,
				vert[vIndices.at(i+1)-1].z);
		glTexCoord2f(vertTex[vtIndices.at(i+2)-1].x,
				vertTex[vtIndices.at(i+2)-1].y);
		glVertex3f(vert[vIndices.at(i+2)-1].x,
				vert[vIndices.at(i+2)-1].y,
				vert[vIndices.at(i+2)-1].z);
	}
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

static Model tiles[8] = {  
	Model( "models/tiles/waterTile.obj" , "models/tiles/waterTexture.png" ),
	Model( "models/tiles/grassTile.obj" , "models/tiles/grassTexture.png" ),
	Model( "models/tiles/mountain.obj" , "models/tiles/mountainTex.png" ),
	Model( "models/tiles/forestTile.obj" , "models/tiles/forestTexture.png" ),
	Model( "models/tiles/waterTile_smallBoat.obj" , "models/tiles/waterTile_smallBoat.png" ),
	Model( "models/tiles/waterTile_medBoat.obj" , "models/tiles/waterTile_medBoat.png" ),
	Model( "models/tiles/waterTile_lgBoat.obj" , "models/tiles/waterTile_lgBoat.png" ),
	Model( "models/tiles/CastleTile.obj" , "models/tiles/CastleTile.png" )

};

bool Model::GenerateModel( const char * objFile) {
	FILE * inFile= fopen(objFile,"r");
	if (!inFile){
#ifdef UNIT_TEST
		cout<<"Could Not Locate File Specified\n";
#endif
		return 0;
	}
#ifdef UNIT_TEST
	cout << "File opened succesfully\n";
#endif

	while (1){

		char line[128];
		if (fscanf(inFile,"%s",line)==EOF) {
			break;
		}

		//v=vertices vt=textureCoords vn=normals f=faces(holds multiple datas
		if (strcmp(line,"v")==0){
			vec3 vertex;
			fscanf(inFile,"%f %f %f\n",
					&vertex.x,&vertex.y,&vertex.z);
			vert.push_back(vertex);
		} else if (strcmp(line,"vt")==0){
			vec2 texture;
			fscanf(inFile,"%f %f\n",
					&texture.x,&texture.y);
			vertTex.push_back(texture);
		} else if (strcmp(line,"vn")==0){
			vec3 norm;
			fscanf(inFile,"%f %f %f\n", &norm.x, &norm.y, &norm.z);
			vertNorm.push_back(norm);
		} else if (strcmp(line,"f")==0){
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3],
			textureIndex[3],normalIndex[3];
			int matches = fscanf(inFile,
					"%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &textureIndex[0], &normalIndex[0],
					&vertexIndex[1], &textureIndex[1], &normalIndex[1],
					&vertexIndex[2], &textureIndex[2], &normalIndex[2] );
			if (matches != 9){
#ifdef UNIT_TEST
				printf("%d Failed to read\n",matches);
#endif
				return false;
			}
			//save all the data where it needs to be stored
			vIndices.push_back(vertexIndex[0]);
			vIndices.push_back(vertexIndex[1]);
			vIndices.push_back(vertexIndex[2]);
			vtIndices.push_back(textureIndex[0]);
			vtIndices.push_back(textureIndex[1]);
			vtIndices.push_back(textureIndex[2]);
			vnIndices.push_back(normalIndex[0]);
			vnIndices.push_back(normalIndex[1]);
			vnIndices.push_back(normalIndex[2]);
		}
	}
	fclose(inFile);
	return true;
}

bool Model::GenerateTexture ( const char * texFile ) {
	GenerateGLTexture(texture, texFile, true);
	return glIsTexture(texture);
}
/*=======================================*/

bool Tile::collisionDetect(float x, float z) {
	float width = (this->x * -2.0f);
	float height = (this->z * -1.70710378118f);
	if (this->z%2 == 0) width -= 1.0f;
	width -= x;
	height -= z;
	float distance = sqrt((width*width) + (height*height));
#ifdef UNIT_TEST
	printf("distance: %f thisX:%d thisZ:%d\n",distance, this->x, this->z);
#endif
	return distance > radius ? false : true;
}


/*=======================================*/
Map::Map(int ** map, int _width, int _height){
	mapW = _width;
	mapH = _height;
	tile = new Tile *[mapH];
	for (int i = 0; i<mapW;i++){
		tile[i] = new Tile[mapH];
	}
	int count = 0;
	for (int i = 0; i < mapH; i++){

		for (int j = 0; j < mapW ; j++){
			tile[i][j].modelID = map[i][j];
			tile[i][j].x = i;
			tile[i][j].z = j;
			tile[i][j].occ = false;

			count++;
		}
	}
}
Map::~Map()
{
	for (int i = 0 ; i < mapH ; i++)
		delete[] tile[i];
	delete[] tile;
}
void Map::draw(){
	for (int i = -10; i < mapH + 10; i++){
		for (int j = -11; j< mapW + 11; j++){
			if( i < 0 || j < 0) {
				tiles[0].draw(i,j);
			} else if ( i > mapH-1 || j > mapW -1) {
				tiles[0].draw(i,j);
			} else {
				if (tile[i][j].modelID == 8) {
					tiles[7].draw(i, j, 0, 120);
				} else {
					tiles[tile[i][j].modelID].draw(i,j);
				}
			}
			glColor3f(1.0f,1.0f,1.0f);
		}
	}
}
vec2 Map::checkCollision( float x, float z) {
	int xPos, yPos;
	vec2 ret(-1,-1);
	yPos = abs(z / 1.70710378118f);
	xPos = abs(x / 2);/*
	if (yPos%2 == 0)
		xPos -= 1.0f;*/
#ifdef UNIT_TEST
	printf("X:%d Y:%d\n", xPos , yPos);
#endif
	for (int i = -1; i < 2 ; i++) {
		for (int j = -1; j < 2 ; j++) {
			if ( xPos+i >= 0 && xPos+i < mapH && yPos+j >= 0 && yPos+j < mapW) {
#ifdef UNIT_TEST
				printf("i:%d j:%d\n", i , j);
#endif
				if (tile[xPos+i][yPos+j].collisionDetect( x, z)) {
					ret = vec2(xPos+i, yPos+j);
#ifdef UNIT_TEST
				    printf("Collided with x:%d y:%d\n",xPos+i,yPos+j);
#endif
					return ret;
				}
			}
		}
	}
	return ret;
}





Camera::Camera()
{	
	yaw = 180.0f;
	pitch = 180 - 51.34019175;
	radius = 10.0f;
	camPos(
		sin(yaw * PI / 180)*radius,
		8.0f, 
		cos(yaw * PI / 180)*radius
	);
	front( 0, 0, -1);
	wUp( 0, 1.0f, 0);
	view((camPos.x + front.x),
			(camPos.y + front.y),
			(camPos.z + front.z));
	wPos(0,0,0);
}

Camera::Camera(float rot, int posx, int posz):
	wPos(posx*XOFFSET, 0.0f, posz*ZOFFSET), wUp(0, 1.0f, 0)
{	
	radius = 10.0f;
	yaw = 0;//rot;
	pitch = 180-51.34019175;
	updateVectors();
}

void Camera::update()
{
	updateVectors();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		wPos.x - camPos.x,  8.0f,  wPos.z - camPos.z,
		wPos.x, wPos.y, wPos.z,
		wUp.x, wUp.y, wUp.z
	);
}

void Camera::rotate(float direction)
{
	yaw -= direction;
	if (yaw < 0)
		yaw = 360;
	else if (yaw > 360)
		yaw = 0;
	camPos.x = sin(yaw * PI / 180)*radius;	
	camPos.z = cos(yaw * PI / 180)*radius;
}
void Camera::translate(int key, int size)
{
	vec3 direction(camPos.x/radius, 0, camPos.z/radius);
	if ( key == XK_w )
		wPos += direction * 0.8f;
	else if ( key == XK_s )
		wPos -= direction * 0.8f;
	else if ( key == XK_a )
		wPos -= vec3::Normalize(vec3::crossProd(direction, wUp)) * 0.8f;
	else if ( key == XK_d )
		wPos += vec3::Normalize(vec3::crossProd(direction, wUp)) * 0.8f;

	if(wPos.z < (size+1) * ZOFFSET)
		wPos.z = (size+1) * ZOFFSET;
	if(wPos.z > -ZOFFSET)
		wPos.z = -ZOFFSET;
	if(wPos.x < (size+1) * XOFFSET)
		wPos.x = (size+1) * XOFFSET;
	if(wPos.x > -XOFFSET)
		wPos.x = -XOFFSET;
}
void Camera::updateVectors()
{
	vec3 direction(camPos.x/radius, 0, camPos.z/radius);
	//camPos
	camPos(
		sin(yaw * PI / 180)*radius,
		8.0f, 
		cos(yaw * PI / 180)*radius
	);
	//front
	front = vec3(
		camPos + direction
	);
	//view
	vec3::Normalize(front);
	view = vec3(front+wPos);
	//right
	right = vec3(vec3::Normalize(vec3::crossProd(view, wUp)));
	//up
	up = vec3(vec3::Normalize(vec3::crossProd(right, view)));
	

	
}
void Camera::setCameraPosition(vec3 newPos)
{
	wPos = newPos;
}
vec3 Camera::getPos()
{
	return vec3(
		wPos.x - camPos.x,
		8.0f,
		wPos.z - camPos.z
	);
}
Matrix Camera::getViewMatrix()
{
	
	float viewMat2[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMat2);
	return Matrix(4, viewMat2);

}

/*=======================================*/
/*===============PICKER==================*/

Picker::Picker(){}

void Picker::update(Matrix & projMatrix, Camera camera, float xres, float yres, float mousex, float mousey)
{
	projectionMatrix = projMatrix;
	vec4 ray(
		(2.0f * mousex) / xres - 1.0f,
		1.0f - (2.0f * mousey) / yres,
		1.0f,
		1.0f
	);
	
	vec4 ray_clip = vec4(ray.x, ray.y, -1.0f, 1.0f);
	Matrix invProj = Matrix::inverse(projectionMatrix);
	vec4 ray_eye = invProj * ray_clip;
	ray_eye.z = -1.0f; ray_eye.w = 0;

	viewMatrix = camera.getViewMatrix();
	Matrix invView = Matrix::inverse(viewMatrix);
	vec4 ray_world = invView * ray_eye;
	
	
	curRay = vec3(ray_world.x, ray_world.y, ray_world.z);
	
	curRay = vec3::Normalize(curRay);
	

}
vec3 Picker::getCurrentRay()
{
	return curRay;
}
void Picker::rotate(float deg)
{
	curRay = vec3(
		cos(RAD(deg)) * curRay.x + sin(RAD(deg) * curRay.z),
		curRay.y,
		-sin(RAD(deg)) * curRay.x + cos(RAD(deg)) * curRay.z

	);
}
/*=======================================*/
/*===============WORLDGS=================*/
WorldGS::WorldGS(int ** mapArr,int sizex,int sizey,
		float camRot, int posx, int posz,
		float xres, float yres) :
	map(mapArr, sizex, sizey), camera(camRot, posx, posz), UI(aog.box, xres, yres), AB(aog.abox, xres, yres)
{	
	this->xres = xres;
	this->yres = yres;

	initializedPos = false;
	
	initWGS_GL();
}

void WorldGS::initWGS_GL()
{
	//OpenGL initialization
	glViewport(0, 0, xres, yres);
	//Initialize matrices    
	//3D perspective view
	
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)xres/(GLfloat)yres,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//discussed futher in later tutorial
	glShadeModel(GL_SMOOTH);//enables smooth shading

	// sets the depth buffer//stop elements from drawing over others
	glClearDepth(1.0f);//Depth buffer setup
	glEnable(GL_DEPTH_TEST);//Enables Depth Testing
	glDepthFunc(GL_LEQUAL);//The type of depth test to do

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//??makes the perspective view better??
	
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glEnable(GL_TEXTURE_2D);

	

}
void WorldGS::pick(vec3 ray)
{
	int count = 0;
	ray.y -= 0.01f;
	vec3 pos = camera.getPos();
	while (pos.y > 0.5f  && ++count != 1000) {
		
		pos.x += ray.x;
		pos.y += ray.y;
		pos.z += ray.z;
	}
	pickPos = vec3(pos.x, 0, pos.z);
}

int WorldGS::procMouseInput(int x, int y)
{
	glMatrixMode(GL_MODELVIEW);
	pkr.update(projMatrix, camera, xres, yres, x, y);
	pick(pkr.getCurrentRay());
	vec2 chkPath = map.checkCollision(pickPos.x, pickPos.z);
#ifdef UNIT_TEST
	printf("chkPath %f %f\n", chkPath.x, chkPath.y);
#endif
	pair<int,int> size(25,25);
	if ( chkPath.x > -1 && chkPath.y > -1) {
		stack<pair<int,int>> pathStack = 
            Movement(size, map.tile, njG.player->wPos.x, 
                     njG.player->wPos.z, chkPath);
		path.clear();
		while (!pathStack.empty()) {
			//pair<int,int> temp = pathStack.pop();
            path.push_back(pathStack.top());
            //denies player from moving through entities
            //if (njG.checkWorldCollision(pathStack.top().first, pathStack.top().second) > 1) {
            //    if (pathStack.size() > 1)
            //        return 20;
            //}
			pathStack.pop();
		}
        /*********NicholasJ addition************/
        while ((int)path.size() > njG.player->moveRange+1) {
            path.pop_back();
        }
        int collision = njG.checkWorldCollision(path.back().first,
                        path.back().second, map.tile);
        switch (collision) {
            case 0:
            {
				map.tile[(int)njG.player->wPos.x][(int)njG.player->wPos.z].occ = false;
                njG.player->wPos.x = path.back().first;
                njG.player->wPos.z = path.back().second;
				map.tile[(int)njG.player->wPos.x][(int)njG.player->wPos.z].occ = true;
            #ifdef SOUND
                alSourcePlay(njG.sound.moveSound);
            #endif
            }
            case 1:
                break;
            case 2:
                break;
            case 3:
                //use enemies[0] as enemies for battle state
                return 3;
            case 4:
                //use enemies[1] as enemies for battle state
                return 5;
            case 5:
                //use enemies[2] as enemies for battle state
                return 7;
            case 6:
                return 9;
        }
        /***************************************/
	}
#ifdef UNIT_TEST
	printf("X: %f  Z: %f\n", pickPos.x, pickPos.z);
#endif
	return 0;
	//picking/UI
}
int WorldGS::procKeyInput(int key)
{
	switch (key) {
		case XK_1:
			//Key 1 was pressed
			break;
		case XK_a: case XK_d: case XK_w: case XK_s:
			camera.translate(key, map.mapH);
			break;
		case XK_q:
			//camera.rotate(-4.0f);
			break;
		case XK_e:
			//camera.rotate(4.0f);
			break;
		case XK_c:
			camera.setCameraPosition(vec3(
				njG.player->wPos.x * XOFFSET,
				0.0f,
				njG.player->wPos.z * ZOFFSET
				));
			break;
        case XK_v:
            return 8;
            break;
		case XK_Escape:
			return 2;
            break;
		case XK_t:
			return 5;
            break;
	}
	return 0;
}
void WorldGS::drawPath() {
	if (path.empty()) return;

	for (int i = 0; i < (int)path.size(); i++) {
		float posx = path[i].first * XOFFSET;
		float posy = path[i].second * ZOFFSET;
		if (path[i].second%2 == 0)
			posx -= 1.0f;
		glBegin(GL_QUADS);
		glColor3f(1.0f, 0, 0);
		glVertex3f(posx + 0.25f, 0.5f , posy + 0.25f);
		glVertex3f(posx + 0.25f, 0.5f , posy - 0.25f);
		glVertex3f(posx - 0.25f, 0.5f , posy - 0.25f);
		glVertex3f(posx - 0.25f, 0.5f , posy + 0.25f);
		glColor3f(1.0f,1.0f,1.0f);
		glEnd();
	}
}
void WorldGS::drawGameState()
{
	if ( !initializedPos && njG.player) {
		for ( int i = 0; i < map.mapW; i++) {
			for ( int j = 0; j < map.mapH; j++) {
				if (njG.player->wPos.x == i && njG.player->wPos.z == j) {
					map.tile[i][j].occ = true;
				}
				/*for ( int k = 0; k < njG.allies->count; k++) {
					if ( njG.allies[k].wPos.x == i && njG.allies[k].wPos.z == j) {
						map.tile[i][j].occ = true;
					}
				}*/
				for ( int k = 0; k < njG.enemyArrayCount; k++) {
                    for (int l = 0; l < njG.enemyCount/njG.enemyArrayCount; l++) {
					    if ( njG.enemies[k][l].wPos.x == i && njG.enemies[k][l].wPos.z == j)
						    map.tile[i][j].occ = true;
				    }
                }
			}
		}
		initializedPos = true;
	}
	initWGS_GL();
	//gluPerspective(45.0f, xres/yres, 0.1f, 100.0f);
	// glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	float  projMat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projMat);
	projMatrix = Matrix( 4, projMat);

	glLoadIdentity();
	camera.update();
	map.draw();
	//drawPath();
    njG.player = Player::getInstance();
    if (njG.player->count != 0) {
        njG.player->drawWorld();
    }
    /*if (njG.allies->count != 0) {
        njG.allies[0].drawWorld();
    }*/
    for (int i = 0; i < njG.enemyArrayCount; i++) {
        if (!njG.enemiesAreDead(njG.enemies[i]))
            njG.enemies[i][0].drawWorld();
    }

	glPushMatrix();/*
	glBegin(GL_QUADS);
		glColor3f(1.0f, 0, 0);
		glTexCoord2f(1, 1);
		glVertex3f(pickPos.x+1, 0.5f , pickPos.z+1);
		glTexCoord2f(1, 0);
		glVertex3f(pickPos.x+1, 0.5f , pickPos.z-1);
		glTexCoord2f(0, 0);
		glVertex3f(pickPos.x-1, 0.5f , pickPos.z-1);
		glTexCoord2f(0, 1);
		glVertex3f(pickPos.x-1, 0.5f , pickPos.z+1);
		glColor3f(1.0f,1.0f,1.0f);
	glEnd();*/
	glBegin(GL_LINES);
		glColor3f(1.0f, 0, 0);
		glVertex3f(pickPos.x, 5.0f, pickPos.z);
		
		glVertex3f(pickPos.x, pickPos.y, pickPos.z);
		glColor3f(1.0f, 0, 0);
		glEnd();
	glPopMatrix();

	//set ortho

	//draw UI
	UI.drawBoxes();
	glColor3ub(255, 255, 255);
    AB.drawAllyBoxes();
    glColor3ub(255, 255, 255);
}
/*=======================================*/
/*===============BATTLEGS================*/

BattleGS::BattleGS(int ** mapArr,int sizex,int sizey,
		float camRot, int posx, int posz,
		float xres, float yres, int target) :
		WorldGS(mapArr, sizex, sizey, camRot, posx, posz, xres, yres),
        BT(aog.bbox, xres, yres), AB(aog.abox, xres, yres)
{

    turns = njG.allies->count + njG.player->count;
    count = 0;
    enemy = target;
}

int BattleGS::procMouseInput(int x, int y)
{   
    if (count % turns == 0) {
        if (njG.player->moveRange > 0) {
			if (njG.player->current_health > 0) {
				glMatrixMode(GL_MODELVIEW);
				pkr.update(projMatrix, camera, xres, yres, x, y);
				pick(pkr.getCurrentRay());
				vec2 chkPath = map.checkCollision(pickPos.x, pickPos.z);
				pair<int,int> size(10, 10);
				if ( chkPath.x > -1 && chkPath.y > -1) {
						for (int i = 0; i < njG.enemyArrayCount; i++) {
							for (int j = 0; j < njG.enemyCount; j++) {
							if (chkPath.x == njG.enemies[i][j].bPos.x &&
								chkPath.y == njG.enemies[i][j].bPos.z) {
								if (njG.player->inBattleRange(&njG.enemies[i][j])) {
									njG.player->dealDamage(&njG.enemies[i][j], map.tile);
									njG.player->moveRange--;
								#ifdef SOUND
									njG.sound.playRandomGrunt();
								#endif
									return 0;
								}
							}
						}
					}
					stack<pair<int,int>> pathStack = 
						Movement(size, map.tile, njG.player->bPos.x, 
								njG.player->bPos.z, chkPath);
					path.clear();
					while (!pathStack.empty()) {
						//pair<int,int> temp = pathStack.pop();
						path.push_back(pathStack.top());
						pathStack.pop();
					}
					while ((int)path.size()-1 > njG.player->moveRange) {
						path.pop_back();
					}
					if (njG.checkBattleCollision(path.back().first,
						path.back().second, 0, enemy, 0)) {
						njG.controlTurns(njG.player, 
										path.back().first, path.back().second, 
										(int)path.size(), map.tile);
					#ifdef SOUND
						alSourcePlay(njG.sound.moveSound);
					#endif
					}
				}
			}
			else {
				
			}
		}
    } else {
        if (njG.allies[count-1].moveRange > 0) {
	        glMatrixMode(GL_MODELVIEW);
	        pkr.update(projMatrix, camera, xres, yres, x, y);
	        pick(pkr.getCurrentRay());
	        vec2 chkPath = map.checkCollision(pickPos.x, pickPos.z);
#ifdef UNIT_TEST
	        printf("chkPath %f %f\n", chkPath.x, chkPath.y);
#endif
	        pair<int,int> size(10, 10);
	        if ( chkPath.x > -1 && chkPath.y > -1) {
                for (int i = 0; i < njG.enemyArrayCount; i++) {
                    for (int j = 0; j < njG.enemyCount; j++) {
                        if (chkPath.x == njG.enemies[i][j].bPos.x &&
                            chkPath.y == njG.enemies[i][j].bPos.z) {
                            if (njG.allies[count-1].inBattleRange(&njG.enemies[i][j])) {
                                njG.allies[count-1].dealDamage(&njG.enemies[i][j], map.tile);
                                njG.allies[count-1].moveRange--;
                            #ifdef SOUND
                                njG.sound.playRandomGrunt();
                            #endif
                                return 0;
                            }
                        }
                    }
                }
		        stack<pair<int,int>> pathStack = 
                    Movement(size, map.tile, njG.allies[count-1].bPos.x, 
                            njG.allies[count-1].bPos.z, chkPath);
		        path.clear();
		        while (!pathStack.empty()) {
			        //pair<int,int> temp = pathStack.pop();
			        path.push_back(pathStack.top());
			        pathStack.pop();
                }
                while ((int)path.size()-1 > njG.allies[count-1].moveRange) {
                    path.pop_back();
                }
                if (njG.checkBattleCollision(path.back().first,
                    path.back().second, count-1, enemy, 1)) {
                    njG.controlTurns(&njG.allies[count-1], 
                                    path.back().first, path.back().second, 
                                    (int)path.size(), map.tile);
                #ifdef SOUND
                    alSourcePlay(njG.sound.moveSound);
                #endif
                }
	        }
        }
    }
#ifdef UNIT_TEST
	printf("X: %f  Z: %f\n", pickPos.x, pickPos.z);
#endif
	return 0;
	//picking/UI
}
int BattleGS::procKeyInput(int key)
{
	switch (key) {
		case XK_1:
			//Key 1 was pressed
			break;
		case XK_a: case XK_d: case XK_w: case XK_s:
			camera.translate(key, map.mapH);
			break;
		case XK_q:
			//camera.rotate(-4.0f);
			break;
		case XK_e:
			//camera.rotate(4.0f);
			if(njG.player->current_health <= 0) {	
			    njG.player->resetInstance();
    			njG.resetAllies();
    			njG.resetEnemies();	
				return -4;
			} 
            if (njG.enemiesAreDead(njG.enemies[enemy])) {
                njG.resetTurns(njG.enemies[enemy]);
                njG.resetBPos();
                njG.enemies[enemy][0].wPos.x = 0;
                njG.enemies[enemy][0].wPos.z = 0;
                njG.player->score += 100;
                if (njG.allEnemiesAreDead()) {
                	njG.player->score += 500;
                	//sendScore(njG.name, njG.player->score);
                	return 8;
            	}
            #ifdef SOUND
        		alSourceStop(njG.sound.battleSound);
            	alSourcePlay(njG.sound.ambientSound);
            #endif
                return -1;
            }
            count++;
            if (count % turns == 0) {
                count = 0;
			    endTurn();
            }
            break;
		case XK_c:
			camera.setCameraPosition(vec3(
				njG.player->bPos.x * XOFFSET,
				0.0f,
				njG.player->bPos.z * ZOFFSET
				));
			break;
		case XK_Escape:
			return 4;
            break;

	}
	return 0;
}
void BattleGS::drawPath() {
	if (path.empty()) return;

	for (int i = 0; i < (int)path.size(); i++) {
		float posx = path[i].first * XOFFSET;
		float posy = path[i].second * ZOFFSET;
		if (path[i].second%2 == 0)
			posx -= 1.0f;
		glBegin(GL_QUADS);
		glColor3f(1.0f, 0, 0);
		glVertex3f(posx + 0.25f, 0.5f , posy + 0.25f);
		glVertex3f(posx + 0.25f, 0.5f , posy - 0.25f);
		glVertex3f(posx - 0.25f, 0.5f , posy - 0.25f);
		glVertex3f(posx - 0.25f, 0.5f , posy + 0.25f);
		glColor3f(1.0f,1.0f,1.0f);
		glEnd();
	}
}
void BattleGS::drawGameState()
{
	if ( !initializedPos) {
		for ( int i = 0; i < map.mapW; i++) {
			for ( int j = 0; j < map.mapH; j++) {
				if (njG.player->bPos.x == i && njG.player->bPos.z == j) {
					map.tile[i][j].occ = true;
				}
				for (int k = 0; k < njG.allies->count; k++) {
					if (njG.allies[k].bPos.x == i && njG.allies[k].bPos.z == j) {
						map.tile[i][j].occ = true;
					}
				}
				for (int k = 0; k < njG.enemyCount/njG.enemyArrayCount; k++) {
					if (njG.enemies[enemy][k].bPos.x == i &&
                        njG.enemies[enemy][k].bPos.z == j) {
						map.tile[i][j].occ = true;
					}
				}
			}
		}
		initializedPos = true;
	}
	initWGS_GL();
	//gluPerspective(45.0f, xres/yres, 0.1f, 100.0f);
	// glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	float  projMat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projMat);
	projMatrix = Matrix( 4, projMat);


	glLoadIdentity();
	camera.update();
	map.draw();
#ifdef UNIT_TEST
	drawPath();
#endif
	njG.player = Player::getInstance();
	if (njG.player->count != 0) {
		count == 0 ? glColor4f(0, 0, 0.75, 0.1) : glColor3f(255, 255, 255);
		njG.player->drawBattle();
    }
    for (int i = 0; i < njG.allies->count; i++) {
		if (njG.allies[i].current_health > 0) {
        	count == i+1 ? glColor4f(0, 0.75, 0, 0.1) : glColor3f(255, 255, 255);
        	njG.allies[i].drawBattle();
		}
    }
    for (int i = 0; i < njG.enemyCount/njG.enemyArrayCount; i++) {
			if (njG.enemies[enemy][i].current_health > 0) {
            	glColor3f(255, 255, 255);
				njG.enemies[enemy][i].drawBattle();
			}
    }
	glPushMatrix();
	glBegin(GL_LINES);
		glColor3f(1.0f, 0, 0);
		glVertex3f(pickPos.x, 5.0f, pickPos.z);
		
		glVertex3f(pickPos.x, pickPos.y, pickPos.z);
		glColor3f(1.0f, 0, 0);
		glEnd();
	glPopMatrix();


	//draw UI
	UI.drawBoxes();
    glColor3ub(255, 255, 255);
    BT.drawBattleBoxes(enemy);
	glColor3ub(255, 255, 255);
    AB.drawAllyBoxes();
    glColor3ub(255, 255, 255);
}

void BattleGS::endTurn()
{
    for (int i = 0; i < njG.enemyCount/njG.enemyArrayCount; i++) {
		if (njG.enemies[enemy][i].current_health > 0 ) {
			if (njG.enemies[enemy][i].inBattleRange(njG.player)) {
				njG.enemies[enemy][i].dealDamage(njG.player, map.tile);
				njG.enemies[enemy][i].moveRange--;
			#ifdef SOUND
				alSourcePlay(njG.sound.hitSound);
			#endif
			}
			vec2 destination(njG.player->bPos.x, njG.player->bPos.z);
			pair<int, int> size(10, 10);
			if (destination.x > -1 && destination.y > -1) {
				stack<pair<int, int>> pathStack =
                Movement(size, map.tile,
				        njG.enemies[enemy][i].bPos.x,
                        njG.enemies[enemy][i].bPos.z, destination);
				path.clear();
				while (!pathStack.empty()) {
					path.push_back(pathStack.top());
					pathStack.pop();
				}
				while ((int)path.size()-1 > njG.enemies[enemy][i].moveRange) {
					path.pop_back();
				}
				if (njG.checkBattleCollision(path.back().first,
                    path.back().second, i, enemy, 2)) {
					njG.controlTurns(&njG.enemies[enemy][i],
									path.back().first, path.back().second,
									(int)path.size(), map.tile);
				}
			}
		}
    }
	njG.resetTurns(njG.enemies[enemy]);
}

/*=======================================*/
/*=======================================*/

void GenerateGLTexture(GLuint & texture, const char * dataSrc, bool inverted)
{
	//IMAGE CLASS NEEDS DEFINITION	
	Image data(dataSrc);
	//flip image data because openGL is inverted on the Y
	if (inverted)
		data.invertY();

	int width = data.width;
	int height = data.height;
	glGenTextures( 1, &texture);
	glBindTexture( GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data.data);
	glBindTexture( GL_TEXTURE_2D, 0);
}

void sendScore(string name, int score)
{
	string requestStr = "./serverCon www.cs.csub.edu /~nlarson/3350/game/game.php\?";
	string dataStr = "n="+name;
	string scoreStr = to_string(score);
	requestStr += dataStr + "\\&s=" + scoreStr;
	const char * request;
	request = requestStr.c_str();
#ifdef UNIT_TEST
	printf("\n%s\n",request);
#endif
	system(request);
}

/*=======================================*/
#endif
