// Fate: The Shadow Wizard: Kingdom of Darkness: Revived Two: The Second One
// Program: nicholasJo.cpp
// Author: Nicholas Jordan
// Date: 02/14/2019

#ifndef NICHOLAS_JO_CPP
#define NICHOLAS_JO_CPP

#include "nicholasJo.h"

NJordGlobal njG;

NJordGlobal::NJordGlobal()
{
#ifdef PROFILE
	loadSoundTime = 0.0;
#endif
	name = "";
}

void NJordGlobal::spawnEnemies(int amount)
{
	Log("spawnEnemies(int amount), amount = %i\n", amount);
	enemies = new Enemy*[amount];
	for (int i = 0; i < amount; i++) {
		enemies[i] = new Enemy[amount];
		Enemy::arrayCount++;
	}
	enemyArrayCount = Enemy::arrayCount;
	enemyCount = Enemy::count;
}

void NJordGlobal::spawnAllies(int amount)
{
	Log("spawnAllies(int amount), amount = %i\n", amount);
	allies = new Ally[amount];
}

void NJordGlobal::resetAllies()
{
	Log("Ally::resetAllies(), allies->count = %i\n", allies->count);
	if (allies->count != 0) {
		delete []allies;
		allies->count = 0;
	}
	return;
}

void NJordGlobal::resetEnemies()
{
	Log("enemyresetEnemies(), enemyArrayCount = %i\n", enemyArrayCount);
	if (enemyArrayCount != 0) {
		for (int i = 0; i < enemyArrayCount; i++)
			delete []enemies[i];
		delete []enemies;
		Enemy::count = 0;
		Enemy::arrayCount = 0;
		enemyCount = 0;
		enemyArrayCount = 0;
	}
	return;
}

void NJordGlobal::saveEntities(int save_number)
{
	Log("NJordGlobal::saveEntities(int save_number), save_number = %i\n", save_number);
	string file_name = "save" + to_string(save_number) + ".txt";
	ofstream file(file_name);
	if (file) {
		player->saveInstance(file);
		allies->saveAllies(file);
		saveEnemies(file);
	} else {
#ifdef UNIT
		printf("Could not locate file specified\n");
#endif
	}
	file << name << endl;
	file.close();
	return;
}

bool NJordGlobal::loadEntities(int save_number)
{
	Log("NJordGlobal::loadEntities(int save_number), save_number = %i\n", save_number);
	string file_name = "save" + to_string(save_number) + ".txt";
	ifstream file(file_name);
	if (file) {
		player->loadInstance(file);
		loadAllies(file);
		loadEnemies(file);
	} else {
#ifdef UNIT
		printf("Could not locate file specified\n");
#endif
		return false;
	}
	string line;
	getline(file, line);
	name = line.c_str();
	file.close();
	return true;
}

void NJordGlobal::loadAllies(ifstream& file)
{
	Log("NJordGlobal::loadAllies(ifstream& file)\n");
	string line;
	if (file) {
		//load count
		getline(file, line);
		spawnAllies(atoi(line.c_str()));
		if (allies->count > 0) {
			for (int i = 0; i < allies->count; i++) {
				//load combatType
				getline(file, line);
				allies[i].loadAllyCombatType(line);
				//load current_health
				getline(file, line);
				//strtof = string to float
				allies[i].current_health = strtof(line.c_str(), NULL); 
				//load current_defense
				getline(file, line);
				allies[i].current_defense = strtof(line.c_str(), NULL);
				//load current_damage
				getline(file, line);
				allies[i].current_damage = strtof(line.c_str(), NULL);
				//load wPos.x
				getline(file, line);
				allies[i].wPos.x = strtof(line.c_str(), NULL);
				//load wPos.y
				getline(file, line);
				allies[i].wPos.y = strtof(line.c_str(), NULL);
				//load wPos.z
				getline(file, line);
				allies[i].wPos.z = strtof(line.c_str(), NULL);
			}
		}
	} else {
#ifdef UNIT
		printf("Could not locate file specified\n");
#endif
	}
}

void NJordGlobal::loadEnemies(ifstream& file)
{
	Log("NJordGlobal::loadEnemies(ifstream& file)\n");
	string line;
	if (file) {
		getline(file, line);
		spawnEnemies(atoi(line.c_str()));
		if (enemyArrayCount > 0) {
			for (int i = 0; i < enemyArrayCount; i++) {
				for (int j = 0; j < enemyCount/enemyArrayCount; j++) {
					//load combatType
					getline(file, line);
					enemies[i][j].loadEnemyCombatType(line);
					//load current_health
					getline(file, line);
					//strtof = string to float
					enemies[i][j].current_health = strtof(line.c_str(), NULL);
					//load current_defense
					getline(file, line);
					enemies[i][j].current_defense = strtof(line.c_str(), NULL);
					//load current_damage
					getline(file, line);
					enemies[i][j].current_damage = strtof(line.c_str(), NULL);
					//load wPos.x
					getline(file, line);
					enemies[i][j].wPos.x = strtof(line.c_str(), NULL);
					//load wPos.y
					getline(file, line);
					enemies[i][j].wPos.y = strtof(line.c_str(), NULL);
					//load wPos.z
					getline(file, line);
					enemies[i][j].wPos.z = strtof(line.c_str(), NULL);
				}
			}
		}
	} else {
#ifdef UNIT
		printf("Could not locate file specified\n");
#endif
	}
}

int NJordGlobal::checkWorldCollision(int x, int z, Tile **tile)
{
	if (x == player->wPos.x && z == player->wPos.z) {
		return 1;
	}
	if (tile[x][z].modelID == 8 || tile[x][z].modelID == 7) {
		return 6;
	}
	for (int i = 0; i < allies->count; i++) {
		if (x == allies[i].wPos.x && z == allies[i].wPos.z)
			return 2;
	}
	for (int i = 0; i < enemyArrayCount; i++) {
		for (int j = 0; j < enemyCount/enemyArrayCount; j++) {
			if (x == enemies[i][j].wPos.x && z == enemies[i][j].wPos.z) {
				if ((abs(player->wPos.x - enemies[i][j].wPos.x) == 1 &&
					abs(player->wPos.z - enemies[i][j].wPos.z) == 0) ||
					(abs(player->wPos.x - enemies[i][j].wPos.x) == 0 &&
					abs(player->wPos.z - enemies[i][j].wPos.z) == 1) ||
					(abs(player->wPos.x - enemies[i][j].wPos.x) == 1 &&
					abs(player->wPos.z - enemies[i][j].wPos.z) == 1)) {
					return i+3;
				}
				return 2;
			}
		}
	}
	return 0;
}

bool NJordGlobal::checkBattleCollision(int x, int z, int position, int arrPos, int type)
{
	switch (type) {
		case 0: //player case
			Log("NJordGlobal::checkBattleCollision(..., type), type = %i\n", type);
			for (int i = 0; i < allies->count; i++) {
				if (x == allies[i].bPos.x && z == allies[i].bPos.z)
					return false;
			}
			for (int i = 0; i < enemyArrayCount; i++) {
				for (int j = 0; j < enemyCount/enemyArrayCount; j++) {
					if (x == enemies[arrPos][j].bPos.x && z == enemies[arrPos][j].bPos.z)
						return false;
				}
			}
			return true;
			break;
		case 1: //ally case
			Log("NJordGlobal::checkBattleCollision(..., type), type = %i\n", type);
			if (x == player->bPos.x && z == player->bPos.z) {
				return false;
			}
			for (int i = 0; i < enemyCount/enemyArrayCount; i++) {
				if (x == enemies[arrPos][i].bPos.x && z == enemies[arrPos][i].bPos.z)
					return false;
			}
			for (int i = 0; i < allies->count; i++) {
				if (i != position) {
					if (x == allies[i].bPos.x && z == allies[i].bPos.z)
						return false;
				}
			}
			return true;
			break;
		case 2: //enemy case
			Log("NJordGlobal::checkBattleCollision(..., type), type = %i\n", type);
			if (x == player->bPos.x && z == player->bPos.z)
				return false;
			for (int i = 0; i < allies->count; i++) {
				if (x == allies[i].bPos.x && z == allies[i].bPos.z)
					return false;
			}
			for (int i = 0; i < njG.enemyCount/njG.enemyArrayCount; i++) {
				if (i != position) {
					if (x == enemies[arrPos][i].bPos.x && z == enemies[arrPos][i].bPos.z)
						return false;
				}
			}
			return true;
			break;
	}
	return false;
}

void NJordGlobal::controlTurns(Entity *target, int dest_x, int dest_z,
								int amount, Tile** tile)
{
	if (target->current_health > 0 && amount > 0) {
		if (target->moveRange <= 0) {
			return;
		}
		if ((abs(target->bPos.x - dest_x) == 1 &&
			abs(target->bPos.z - dest_z) == 1) ||
			(abs(target->bPos.x - dest_x) == 1 &&
			abs(target->bPos.z - dest_z) == 0) ||
			(abs(target->bPos.x - dest_x) == 0 &&
			abs(target->bPos.z - dest_z) == 1)) {
			target->moveRange--;
		} else {
			target->moveRange -= amount-1;
		}
		tile[(int)target->bPos.x][(int)target->bPos.z].occ = false;
		target->bPos.x = dest_x;
		target->bPos.z = dest_z;
		tile[(int)target->bPos.x][(int)target->bPos.z].occ = true;
	}
}

void NJordGlobal::saveEnemies(ofstream& file)
{
	Log("Enemy::saveEnemies(ostream& file)\n");
	if (file) {
		file << Enemy::arrayCount << endl;
		for (int i = 0; i < enemyArrayCount; i++) {
			for (int j = 0; j < enemyCount/enemyArrayCount; j++) {
				file << enemies[i][j].combatType << endl;
				file << enemies[i][j].current_health << endl;
				file << enemies[i][j].current_defense << endl;
				file << enemies[i][j].current_damage << endl;
				file << enemies[i][j].wPos.x << endl;
				file << enemies[i][j].wPos.y << endl;
				file << enemies[i][j].wPos.z << endl;
			}
		}
	} else {
#ifdef UNIT
		printf("Could not locate file specified\n");
#endif
	}
}

bool NJordGlobal::enemiesAreDead(Enemy *target)
{
	int count = 0;
	for (int i = 0; i < enemyCount/enemyArrayCount; i++) {
		if (target[i].current_health <= 0)
			count++;
	}
	if (count == enemyCount/enemyArrayCount)
		return true;
	return false;
}

bool NJordGlobal::allEnemiesAreDead()
{
	int count = 0;
	for (int i = 0; i < enemyArrayCount; i++) {
		for (int j = 0; j < enemyCount/enemyArrayCount; j++) {
			if (enemies[i][j].current_health <= 0)
				count++;
		}
	}
	if (count == enemyCount)
		return true;
	return false;
}

void NJordGlobal::resetTurns(Enemy *target)
{
	player->moveRange = player->getMaxTurns();
	for (int i = 0; i < allies->count; i++)
		allies[i].moveRange = allies[i].getMaxTurns();
	if (target != 0) {
		for (int i = 0; i < enemyCount/enemyArrayCount; i++)
			target[i].moveRange = target[i].getMaxTurns();
	}
	return;
}

void NJordGlobal::resetBPos()
{
	player->bPos.x = player->defaultBPos.x;
	player->bPos.z = player->defaultBPos.z;
	for (int i = 0; i < allies->count; i++) {
		allies[i].bPos.x = allies[i].defaultBPos.x;
		allies[i].bPos.z = allies[i].defaultBPos.z;
	}
	for (int i = 0; i < enemyArrayCount; i++) {
		for (int j = 0; j < enemyCount/enemyArrayCount; j++) {
			enemies[i][j].bPos.x = enemies[i][j].defaultBPos.x;
			enemies[i][j].bPos.z = enemies[i][j].defaultBPos.z;
		}
	}
}

//==========================[SOUND CLASS]===============================
#ifdef SOUND

Sound::Sound()
{	
}

void Sound::clearSounds()
{
	Log("Sound::clearSounds()\n");
	alDeleteSources(1, &menuSound);
	alDeleteSources(1, &moveSound);
	alDeleteSources(1, &ambientSound);
	alDeleteSources(1, &battleSound);
	alDeleteSources(1, &hitSound);
	alDeleteSources(1, &gruntSound[0]);
	alDeleteSources(1, &gruntSound[1]);
	alDeleteSources(1, &gruntSound[2]);
	alDeleteSources(1, &gruntSound[3]);
	alDeleteSources(1, &gruntSound[4]);
	alDeleteSources(1, &gruntSound[5]);
	alDeleteSources(1, &gruntSound[6]);
	alDeleteSources(1, &gruntSound[7]);
	alDeleteSources(1, &gruntSound[8]);
	alDeleteSources(1, &gruntSound[9]);
	alDeleteBuffers(1, &alBuffer[0]);
	alDeleteBuffers(1, &alBuffer[1]);
	alDeleteBuffers(1, &alBuffer[2]);
	alDeleteBuffers(1, &alBuffer[3]);
	alDeleteBuffers(1, &alBuffer[4]);
	alDeleteBuffers(1, &alBuffer[5]);
	alDeleteBuffers(1, &alBuffer[6]);
	alDeleteBuffers(1, &alBuffer[7]);
	alDeleteBuffers(1, &alBuffer[8]);
	alDeleteBuffers(1, &alBuffer[9]);
	alDeleteBuffers(1, &alBuffer[10]);
	alDeleteBuffers(1, &alBuffer[11]);
	alDeleteBuffers(1, &alBuffer[12]);
	alDeleteBuffers(1, &alBuffer[13]);
	alDeleteBuffers(1, &alBuffer[14]);
	alDeleteBuffers(1, &alBuffer[15]);

	ALCcontext *Context = alcGetCurrentContext();
	ALCdevice *Device = alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

void Sound::initializeSounds()
{
	Log("Sound::initializeSounds()\n");
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
#ifdef UNIT
		printf("ERROR: alutInit()\n");
#endif
	}
	alGetError();
	float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
}

void Sound::loadOGG(char *filename, vector<char> &buffer,
					ALenum &format, ALsizei &freq)
{
	Log("Sound::loadOGG(char *filename, ...), filename = %s\n", filename);
	int endian = 0;
	int bitStream;
	long bytes;
	char array[32768]; //32 KB buffer
	FILE *f;

	f = fopen(filename, "rb");

	if (f == NULL) {
		cerr << "Cannot open " << filename << " for reading\n";
		return;
	}
	vorbis_info *pInfo;
	OggVorbis_File oggFile;

	if (ov_open(f, &oggFile, NULL, 0) != 0) {
		cerr << "Cannot open " << filename << " for decoding\n";
		return;
	}

	pInfo = ov_info(&oggFile, -1);
	if (pInfo->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;
	freq = pInfo->rate;

	do {
		bytes = ov_read(&oggFile, array, 32768, endian, 2, 1, &bitStream);
		if (bytes < 0) {
			ov_clear(&oggFile);
			cerr << "Error decoding " << filename << endl;
			return;
		}
		buffer.insert(buffer.end(), array, array + bytes);
	} while (bytes > 0);
	ov_clear(&oggFile);
}

void Sound::loadSounds()
{
#ifdef PROFILE
	struct timespec tstart, tend;
	timeGet(&tstart);
#endif
	Log("Sound::loadSounds()\n");
	alGenBuffers(1, &alBuffer[0]);
	char filename[] = "./sounds/click.ogg";
	vector<char> bufferData;
	ALenum format;
	ALsizei freq;
	loadOGG(filename, bufferData, format, freq);
	alBufferData(alBuffer[0], format, &bufferData[0],
			static_cast<ALsizei>(bufferData.size()), freq);

	alGenBuffers(1, &alBuffer[1]);
	char filename2[] = "./sounds/grass_step.ogg";
	vector<char> bufferData2;
	ALenum format2;
	ALsizei freq2;
	loadOGG(filename2, bufferData2, format2, freq2);
	alBufferData(alBuffer[1], format2, &bufferData2[0],
			static_cast<ALsizei>(bufferData2.size()), freq2);

	alGenBuffers(1, &alBuffer[2]);
	char filename3[] = "./sounds/ambient_nature.ogg";
	vector<char> bufferData3;
	ALenum format3;
	ALsizei freq3;
	loadOGG(filename3, bufferData3, format3, freq3);
	alBufferData(alBuffer[2], format3, &bufferData3[0],
			static_cast<ALsizei>(bufferData3.size()), freq3);

	alGenBuffers(1, &alBuffer[3]);
	char filename4[] = "./sounds/battle_drums.ogg";
	vector<char> bufferData4;
	ALenum format4;
	ALsizei freq4;
	loadOGG(filename4, bufferData4, format4, freq4);
	alBufferData(alBuffer[3], format4, &bufferData4[0],
			static_cast<ALsizei>(bufferData4.size()), freq4);

	alGenBuffers(1, &alBuffer[4]);
	char filename5[] = "./sounds/hit.ogg";
	vector<char> bufferData5;
	ALenum format5;
	ALsizei freq5;
	loadOGG(filename5, bufferData5, format5, freq5);
	alBufferData(alBuffer[4], format5, &bufferData5[0],
			static_cast<ALsizei>(bufferData5.size()), freq5);

	alGenBuffers(1, &alBuffer[5]);
	char filename6[] = "./sounds/grunt1.ogg";
	vector<char> bufferData6;
	ALenum format6;
	ALsizei freq6;
	loadOGG(filename6, bufferData6, format6, freq6);
	alBufferData(alBuffer[5], format6, &bufferData6[0],
			static_cast<ALsizei>(bufferData6.size()), freq6);

	alGenBuffers(1, &alBuffer[6]);
	char filename7[] = "./sounds/grunt2.ogg";
	vector<char> bufferData7;
	ALenum format7;
	ALsizei freq7;
	loadOGG(filename7, bufferData7, format7, freq7);
	alBufferData(alBuffer[6], format7, &bufferData7[0],
			static_cast<ALsizei>(bufferData7.size()), freq7);

	alGenBuffers(1, &alBuffer[7]);
	char filename8[] = "./sounds/grunt3.ogg";
	vector<char> bufferData8;
	ALenum format8;
	ALsizei freq8;
	loadOGG(filename8, bufferData8, format8, freq8);
	alBufferData(alBuffer[7], format8, &bufferData8[0],
			static_cast<ALsizei>(bufferData8.size()), freq8);

	alGenBuffers(1, &alBuffer[8]);
	char filename9[] = "./sounds/grunt4.ogg";
	vector<char> bufferData9;
	ALenum format9;
	ALsizei freq9;
	loadOGG(filename9, bufferData9, format9, freq9);
	alBufferData(alBuffer[8], format9, &bufferData9[0],
			static_cast<ALsizei>(bufferData9.size()), freq9);

	alGenBuffers(1, &alBuffer[9]);
	char filename10[] = "./sounds/grunt5.ogg";
	vector<char> bufferData10;
	ALenum format10;
	ALsizei freq10;
	loadOGG(filename10, bufferData10, format10, freq10);
	alBufferData(alBuffer[9], format10, &bufferData10[0],
			static_cast<ALsizei>(bufferData10.size()), freq10);

	alGenBuffers(1, &alBuffer[10]);
	char filename11[] = "./sounds/grunt6.ogg";
	vector<char> bufferData11;
	ALenum format11;
	ALsizei freq11;
	loadOGG(filename11, bufferData11, format11, freq11);
	alBufferData(alBuffer[10], format11, &bufferData11[0],
			static_cast<ALsizei>(bufferData11.size()), freq11);

	alGenBuffers(1, &alBuffer[11]);
	char filename12[] = "./sounds/grunt7.ogg";
	vector<char> bufferData12;
	ALenum format12;
	ALsizei freq12;
	loadOGG(filename12, bufferData12, format12, freq12);
	alBufferData(alBuffer[11], format12, &bufferData12[0],
			static_cast<ALsizei>(bufferData12.size()), freq12);

	alGenBuffers(1, &alBuffer[12]);
	char filename13[] = "./sounds/grunt8.ogg";
	vector<char> bufferData13;
	ALenum format13;
	ALsizei freq13;
	loadOGG(filename13, bufferData13, format13, freq13);
	alBufferData(alBuffer[12], format13, &bufferData13[0],
			static_cast<ALsizei>(bufferData13.size()), freq13);

	alGenBuffers(1, &alBuffer[13]);
	char filename14[] = "./sounds/grunt9.ogg";
	vector<char> bufferData14;
	ALenum format14;
	ALsizei freq14;
	loadOGG(filename14, bufferData14, format14, freq14);
	alBufferData(alBuffer[13], format14, &bufferData14[0],
			static_cast<ALsizei>(bufferData14.size()), freq14);

	alGenBuffers(1, &alBuffer[14]);
	char filename15[] = "./sounds/grunt10.ogg";
	vector<char> bufferData15;
	ALenum format15;
	ALsizei freq15;
	loadOGG(filename15, bufferData15, format15, freq15);
	alBufferData(alBuffer[14], format15, &bufferData15[0],
			static_cast<ALsizei>(bufferData15.size()), freq15);

	alGenBuffers(1, &alBuffer[15]);
	char filename16[] = "./sounds/menu_music.ogg";
	vector<char> bufferData16;
	ALenum format16;
	ALsizei freq16;
	loadOGG(filename16, bufferData16, format16, freq16);
	alBufferData(alBuffer[15], format16, &bufferData16[0],
			static_cast<ALsizei>(bufferData16.size()), freq16);

	if (alGetError() == AL_INVALID_VALUE) {
#ifdef UNIT
		printf("ERROR3: setting menu sound\n");
#endif
	}

	alGenSources(1, &menuSound);
	alGenSources(1, &moveSound);
	alGenSources(1, &ambientSound);
	alGenSources(1, &battleSound);
	alGenSources(1, &hitSound);
	alGenSources(1, &gruntSound[0]);
	alGenSources(1, &gruntSound[1]);
	alGenSources(1, &gruntSound[2]);
	alGenSources(1, &gruntSound[3]);
	alGenSources(1, &gruntSound[4]);
	alGenSources(1, &gruntSound[5]);
	alGenSources(1, &gruntSound[6]);
	alGenSources(1, &gruntSound[7]);
	alGenSources(1, &gruntSound[8]);
	alGenSources(1, &gruntSound[9]);
	alGenSources(1, &menuMusic);

	alSourcei(menuSound, AL_BUFFER, alBuffer[0]);
	alSourcei(moveSound, AL_BUFFER, alBuffer[1]);
	alSourcei(ambientSound, AL_BUFFER, alBuffer[2]);
	alSourcei(battleSound, AL_BUFFER, alBuffer[3]);
	alSourcei(hitSound, AL_BUFFER, alBuffer[4]);
	alSourcei(gruntSound[0], AL_BUFFER, alBuffer[5]);
	alSourcei(gruntSound[1], AL_BUFFER, alBuffer[6]);
	alSourcei(gruntSound[2], AL_BUFFER, alBuffer[7]);
	alSourcei(gruntSound[3], AL_BUFFER, alBuffer[8]);
	alSourcei(gruntSound[4], AL_BUFFER, alBuffer[9]);
	alSourcei(gruntSound[5], AL_BUFFER, alBuffer[10]);
	alSourcei(gruntSound[6], AL_BUFFER, alBuffer[11]);
	alSourcei(gruntSound[7], AL_BUFFER, alBuffer[12]);
	alSourcei(gruntSound[8], AL_BUFFER, alBuffer[13]);
	alSourcei(gruntSound[9], AL_BUFFER, alBuffer[14]);
	alSourcei(menuMusic, AL_BUFFER, alBuffer[15]);

	alSourcef(menuSound, AL_GAIN, 0.5f);
	alSourcef(moveSound, AL_GAIN, 0.5f);
	alSourcef(ambientSound, AL_GAIN, 1.0f);
	alSourcef(battleSound, AL_GAIN, 0.5f);
	alSourcef(hitSound, AL_GAIN, 1.0f);
	alSourcef(gruntSound[0], AL_GAIN, 1.0f);
	alSourcef(gruntSound[1], AL_GAIN, 1.0f);
	alSourcef(gruntSound[2], AL_GAIN, 1.0f);
	alSourcef(gruntSound[3], AL_GAIN, 1.0f);
	alSourcef(gruntSound[4], AL_GAIN, 1.0f);
	alSourcef(gruntSound[5], AL_GAIN, 1.0f);
	alSourcef(gruntSound[6], AL_GAIN, 1.0f);
	alSourcef(gruntSound[7], AL_GAIN, 1.0f);
	alSourcef(gruntSound[8], AL_GAIN, 1.0f);
	alSourcef(gruntSound[9], AL_GAIN, 1.0f);
	alSourcef(menuMusic, AL_GAIN, 0.5f);

	alSourcef(menuSound, AL_PITCH, 1.0f);
	alSourcef(moveSound, AL_PITCH, 1.0f);
	alSourcef(ambientSound, AL_PITCH, 1.0f);
	alSourcef(battleSound, AL_PITCH, 1.0f);
	alSourcef(hitSound, AL_PITCH, 1.0f);
	alSourcef(gruntSound[0], AL_PITCH, 1.0f);
	alSourcef(gruntSound[1], AL_PITCH, 1.0f);
	alSourcef(gruntSound[2], AL_PITCH, 1.0f);
	alSourcef(gruntSound[3], AL_PITCH, 1.0f);
	alSourcef(gruntSound[4], AL_PITCH, 1.0f);
	alSourcef(gruntSound[5], AL_PITCH, 1.0f);
	alSourcef(gruntSound[6], AL_PITCH, 1.0f);
	alSourcef(gruntSound[7], AL_PITCH, 1.0f);
	alSourcef(gruntSound[8], AL_PITCH, 1.0f);
	alSourcef(gruntSound[9], AL_PITCH, 1.0f);
	alSourcef(menuMusic, AL_PITCH, 1.0f);

	alSourcei(menuSound, AL_LOOPING, AL_FALSE);
	alSourcei(moveSound, AL_LOOPING, AL_FALSE);
	alSourcei(ambientSound, AL_LOOPING, AL_TRUE);
	alSourcei(battleSound, AL_LOOPING, AL_TRUE);
	alSourcei(hitSound, AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[0], AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[1], AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[2], AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[3], AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[4], AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[5], AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[6], AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[7], AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[8], AL_LOOPING, AL_FALSE);
	alSourcei(gruntSound[9], AL_LOOPING, AL_FALSE);
	alSourcei(menuMusic, AL_LOOPING, AL_TRUE);
#ifdef PROFILE
	timeGet(&tend);
	njG.loadSoundTime += timeDiff(&tstart, &tend);
#endif
}

void Sound::playRandomGrunt()
{
	random_device generator;
	// generate a random number between 0 to 9 to play a one of
	// 10 random grunt sounds
	uniform_int_distribution<int> distribution(0, 9);
	switch (distribution(generator)) {
		case 0:
			alSourcePlay(njG.sound.gruntSound[0]);
			break;
		case 1:
			alSourcePlay(njG.sound.gruntSound[1]);
			break;
		case 2:
			alSourcePlay(njG.sound.gruntSound[2]);
			break;
		case 3:
			alSourcePlay(njG.sound.gruntSound[3]);
			break;
		case 4:
			alSourcePlay(njG.sound.gruntSound[4]);
			break;
		case 5:
			alSourcePlay(njG.sound.gruntSound[5]);
			break;
		case 6:
			alSourcePlay(njG.sound.gruntSound[6]);
			break;
		case 7:
			alSourcePlay(njG.sound.gruntSound[7]);
			break;
		case 8:
			alSourcePlay(njG.sound.gruntSound[8]);
			break;
		case 9:
			alSourcePlay(njG.sound.gruntSound[9]);
			break;
	}
}
#endif

//==========================[ENTITY CLASS]===============================

static Model pModel[9] = {
Model("models/characters/Tank.obj","models/characters/ModelTexturePlayer.png"),
Model("models/characters/Tank.obj","models/characters/ModelTextureAlly.png"),
Model("models/characters/Tank.obj","models/characters/ModelTextureEnemy.png"),
Model("models/characters/Archer.obj","models/characters/ModelTexturePlayer.png"),
Model("models/characters/Archer.obj","models/characters/ModelTextureAlly.png"),
Model("models/characters/Archer.obj","models/characters/ModelTextureEnemy.png"),
Model("models/characters/Spearman.obj","models/characters/ModelTexturePlayer.png"),
Model("models/characters/Spearman.obj","models/characters/ModelTextureAlly.png"),
Model("models/characters/Spearman.obj","models/characters/ModelTextureEnemy.png")};

//==========[Health Functions]=========
float Entity::getMaxHealth()
{
	return max_health;
}

float Entity::getCurrentHealth()
{
	if (current_health < 0.0) {
		//negative health is not possible
		current_health = 0.0;
		return current_health;
	}
	if (current_health > max_health) {
		//ensures no overheal
		current_health = max_health;
	}
	return current_health;
}

void Entity::setMaxHealth(float h)
{
	Log("Entity::setMaxHealth(float h), h = %f\n", h);
	max_health = h;
	return;
}
//==========[Defense Functions]=========
float Entity::getDefaultDefense()
{
	return default_defense;
}

float Entity::getCurrentDefense()
{
	return current_defense;
}

void Entity::setDefaultDefense(float d)
{
	Log("Entity::setDefaultDefense(float d), d = %f\n", d);
	default_defense = d;
	return;
}
//==========[Damage Functions]=========
void Entity::setDefaultDamage(float d)
{
	Log("Entity::setDefaultDamage(float d), d = %f\n", d);
	default_damage = d;
	return;
}

float Entity::getCurrentDamage()
{
	return current_damage;
}

float Entity::getDefaultDamage()
{
	return default_damage;
}

void Entity::dealDamage(Entity *target, Tile** tile)
{
	Log("Entity::dealDamage(Entity &target), target->ally = %i\n", target->getAlly());
	if (this->getAlly() != target->getAlly() && target->current_health > 0 &&
			this->current_health > 0 &&
			(this->inWorldRange(target) || this->inBattleRange(target))) {
		target->current_health -= target->current_defense * this->current_damage;
	} else {
#ifdef UNIT
		cout << "Cannot damage an ally!\n";
#endif
	}
	if (target->current_health < 0) {
		target->current_health = 0;
		tile[(int)target->bPos.x][(int)target->bPos.z].occ = false;
		target->bPos.x = -10;
		target->bPos.z = -10;
		//tile[(int)target->bPos.x][(int)target->bPos.z].occ = true;
	}
}
//==========[Ally Functions]=========
void Entity::setAlly(bool a)
{
	Log("Entity::setAlly(bool a), a = %i\n", a);
	ally = a;
	return;
}

bool Entity::getAlly()
{
	Log("Entity::getAlly() returning %i\n", ally);
	return ally;
}
//==========[ETC]=========

void Entity::displayImage(int x, int y, int z)
{
	float wid = 1;
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, image);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(wid, 0.5f, wid);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(wid, 0.5f, -wid);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-wid, 0.5f, -wid);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-wid, 0.5f, wid);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void Entity::resetStats()
{
	Log("Entity::resetStats()\n");
	current_health = max_health;
	current_defense = default_defense;
	current_damage = default_damage;
}

void Entity::drawWorld()
{
	pModel[this->modelID].draw(this->wPos.x, this->wPos.z, 0.3, wYaw);
}

void Entity::drawBattle()
{
	pModel[this->modelID].draw(this->bPos.x, this->bPos.z, 0.3, bYaw);
}

bool Entity::inWorldRange(Entity *target)
{
	if (abs(this->wPos.x - target->wPos.x) <= this->attackRange ||
			abs(this->wPos.z - target->wPos.z) <= this->attackRange)
		return true;
	return false;
}

bool Entity::inBattleRange(Entity *target)
{
	switch (this->attackRange) {
		case 1:
			if ((abs(this->bPos.x - target->bPos.x) <= this->attackRange &&	//(1, 1)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange) ||
				(abs(this->bPos.x - target->bPos.x) <= this->attackRange && //(1, 0)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange-1) ||
				(abs(this->bPos.x - target->bPos.x) <= this->attackRange-1 && //(0, 1)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange)) {
				return true;
			}
			break;
		case 3:
			if ((abs(this->bPos.x - target->bPos.x) <= this->attackRange &&	//(3, 3)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange) ||
				(abs(this->bPos.x - target->bPos.x) <= this->attackRange && //(3, 2)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange-1) ||
				(abs(this->bPos.x - target->bPos.x) <= this->attackRange && //(3, 1)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange-2) ||
				(abs(this->bPos.x - target->bPos.x) <= this->attackRange && //(3, 0)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange-3) ||
				(abs(this->bPos.x - target->bPos.x) <= this->attackRange-1 && //(2, 3)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange) ||
				(abs(this->bPos.x - target->bPos.x) <= this->attackRange-2 && //(1, 3)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange) ||
				(abs(this->bPos.x - target->bPos.x) <= this->attackRange-3 && //(0, 3)
				abs(this->bPos.z - target->bPos.z) <= this->attackRange)) {
				return true;
			}
			break;
	}
	return false;
}

void Entity::setMaxTurns(int t)
{
	turns = t;
}

int Entity::getMaxTurns()
{
	return turns;
}

//==========================[ALLY CLASS]===============================
//Inherits from Entity

int Ally::count = 0;

Ally::Ally()
{
	Log("Ally::Ally()\n");
	setAlly(true);
	setAllyCombatType();
	setAllyImage();
	current_health = getMaxHealth();
	current_defense = getDefaultDefense();
	current_damage = getDefaultDamage();
	switch (count) {
		case 0:
			wPos.x = 6;
			wPos.z = 7;
			defaultBPos.x = 1;
			defaultBPos.z = 0;
			bPos.x = 1;
			bPos.z = 0;
			break;
		case 1:
			wPos.x = 0;
			wPos.z = 0;
			defaultBPos.x = 0;
			defaultBPos.z = 0;
			bPos.x = 0;
			bPos.z = 0;
			break;
		case 2:
			wPos.x = 0;
			wPos.z = 0;
			defaultBPos.x = 0;
			defaultBPos.z = 1;
			bPos.x = 0;
			bPos.z = 1;
			break;
	}
	wYaw = 180.0;
	bYaw = 180.0;
	count++;
}

void Ally::saveAllies(ofstream& file)
{
	Log("Ally::saveAllies(ostream& file)\n");
	if (file) {
		file << Ally::count << endl;
		for (int i = 0; i < Ally::count; i++) {
			file << this[i].combatType << endl;
			file << this[i].current_health << endl;
			file << this[i].current_defense << endl;
			file << this[i].current_damage << endl;
			file << this[i].wPos.x << endl;
			file << this[i].wPos.y << endl;
			file << this[i].wPos.z << endl;
		}
	} else {
#ifdef UNIT
		printf("Could not locate file specified\n");
#endif
	}
}

void Ally::loadAllyCombatType(string c)
{
	Log("Ally::loadAllyCombatType(string c), c = %s\n", c.c_str());
	if (c == "Archer") {
		modelID = 4;
		combatType = c;
		setMaxHealth(50.0);
		setDefaultDefense(0.3);
		setDefaultDamage(10.0);
		setMaxTurns(2);
		moveRange = 2;
		attackRange = 3;
	} else if (c == "Soldier") {
		modelID = 7;
		combatType = c;
		setMaxHealth(75.0);
		setDefaultDefense(0.5);
		setDefaultDamage(15.0);
		setMaxTurns(3);
		moveRange = 3;
		attackRange = 1;
	} else if (c == "Tank") {
		modelID = 1;
		combatType = c;
		setMaxHealth(100.0);
		setDefaultDefense(0.70);
		setDefaultDamage(12.0);
		setMaxTurns(3);
		moveRange = 3;
		attackRange = 1;
	}
	setAllyImage();
	return;
}

void Ally::setAllyCombatType()
{
	Log("Ally::setAllyCombatType()\n");
	//randomly generate a number from 0 to 2 to choose combat type
	random_device generator;
	uniform_int_distribution<int> distribution(0, 2);
	switch (distribution(generator)) {
		case 0:
			modelID = 4;
			combatType = "Archer";
			setMaxHealth(50.0);
			setDefaultDefense(0.3);
			setDefaultDamage(10.0);
			setMaxTurns(2);
			moveRange = 2;
			attackRange = 3;
			break;
		case 1:
			modelID = 7;
			combatType = "Soldier";
			setMaxHealth(75.0);
			setDefaultDefense(0.5);
			setDefaultDamage(15.0);
			setMaxTurns(3);
			moveRange = 3;
			attackRange = 1;
			break;
		case 2:
			modelID = 1;
			combatType = "Tank";
			setMaxHealth(100);
			setDefaultDefense(0.7);
			setDefaultDamage(12.0);
			setMaxTurns(3);
			moveRange = 3;
			attackRange = 1;
			break;
	}
	return;
}

void Ally::setAllyImage()
{
	Log("Ally::setAllyImage()\n");
	if (combatType == "Archer" )
		GenerateGLTexture(image, "./images/aopic.png", false);
	else if (combatType == "Nick")
		GenerateGLTexture(image, "./images/nickLCreditPic.jpg", false);
	else if (combatType == "Soldier")
		GenerateGLTexture(image, "./images/nicholasJo.png", false);
	else if (combatType == "Tank")
		GenerateGLTexture(image, "./images/brandonH.png", false);
}

//==========================[ENEMY CLASS]===============================
//Inherits from Entity

int Enemy::count = 0;
int Enemy::arrayCount = 0;

Enemy::Enemy()
{
	Log("Enemy::Enemy()\n");
	setAlly(false);
	setEnemyCombatType();
	current_health = getMaxHealth();
	current_defense = getDefaultDefense();
	current_damage = getDefaultDamage();
	switch (arrayCount) {
		case 0:
			switch (count) {
				case 0:
					wPos.x = 10;
					wPos.z = 10;
					defaultBPos.x = 8;
					defaultBPos.z = 7;
					bPos.x = 8;
					bPos.z = 7;
					break;
				case 1:
					wPos.x = -1;
					wPos.z = -1;
					defaultBPos.x = 8;
					defaultBPos.z = 8;
					bPos.x = 8;
					bPos.z = 8;
					break;
				case 2:
					wPos.x = -1;
					wPos.z = -1;
					defaultBPos.x = 7;
					defaultBPos.z = 8;
					bPos.x = 7;
					bPos.z = 8;
					break;
			}
			break;
		case 1:
			switch (count) {
				case 3:
					wPos.x = 15;
					wPos.z = 13;
					defaultBPos.x = 8;
					defaultBPos.z = 7;
					bPos.x = 8;
					bPos.z = 7;
					break;
				case 4:
					wPos.x = -1;
					wPos.z = -1;
					defaultBPos.x = 8;
					defaultBPos.z = 8;
					bPos.x = 8;
					bPos.z = 8;
					break;
				case 5:
					wPos.x = -1;
					wPos.z = -1;
					defaultBPos.x = 7;
					defaultBPos.z = 8;
					bPos.x = 7;
					bPos.z = 8;
					break;
			}
			break;
		case 2:
			switch (count) {
				case 6:
					wPos.x = 20;
					wPos.z = 10;
					defaultBPos.x = 8;
					defaultBPos.z = 7;
					bPos.x = 8;
					bPos.z = 7;
					break;
				case 7:
					wPos.x = -1;
					wPos.z = -1;
					defaultBPos.x = 8;
					defaultBPos.z = 8;
					bPos.x = 8;
					bPos.z = 8;
					break;
				case 8:
					wPos.x = -1;
					wPos.z = -1;
					defaultBPos.x = 7;
					defaultBPos.z = 8;
					bPos.x = 7;
					bPos.z = 8;
					break;
			}
			break;
	}
	wYaw = 0.0;
	bYaw = 0.0;
	count++;
}

void Enemy::loadEnemyCombatType(string c)
{
	Log("Enemy::loadEnemyCombatType(string c), c = %s\n", c.c_str());
	if (c == "Archer") {
		modelID = 5;
		combatType = c;
		setMaxHealth(50.0);
		setDefaultDefense(0.3);
		setDefaultDamage(10.0);
		setMaxTurns(2);
		moveRange = 2;
		attackRange = 3;
	} else if (c == "Soldier") {
		modelID = 8;
		combatType = c;
		setMaxHealth(75.0);
		setDefaultDefense(0.5);
		setDefaultDamage(15.0);
		setMaxTurns(3);
		moveRange = 3;
		attackRange = 1;
	} else if (c == "Tank") {
		modelID = 2;
		combatType = c;
		setMaxHealth(100.0);
		setDefaultDefense(0.70);
		setDefaultDamage(12.0);
		setMaxTurns(3);
		moveRange = 3;
		attackRange = 1;
	}
	setEnemyImage();
	return;
}

void Enemy::setEnemyCombatType()
{
	Log("Enemy::setEnemyCombatType()\n");
	//randomly generate a number from 0 to 2 to choose combat type
	random_device generator;
	uniform_int_distribution<int> distribution(0, 2);
	switch (distribution(generator)) {
		case 0 :
			modelID = 5;
			combatType = "Archer";
			setMaxHealth(50.0);
			setDefaultDefense(0.3);
			setDefaultDamage(10.0);
			setMaxTurns(2);
			moveRange = 2;
			attackRange = 3;
			break;
		case 1:
			modelID = 8;
			combatType = "Soldier";
			setMaxHealth(75.0);
			setDefaultDefense(0.5);
			setDefaultDamage(15.0);
			setMaxTurns(3);
			moveRange = 3;
			attackRange = 1;
			break;
		case 2:
			modelID = 2;
			combatType = "Tank";
			setMaxHealth(100);
			setDefaultDefense(0.7);
			setDefaultDamage(12.0);
			setMaxTurns(3);
			moveRange = 3;
			attackRange = 1;
			break;
	}
	return;
}

void Enemy::setEnemyImage()
{
	Log("Enemy::setEnemyImage()\n");
	if (combatType == "Archer")
		GenerateGLTexture(image, "./images/aopic.png", false);
	else if (combatType == "Nick")
		GenerateGLTexture(image, "./images/nickLCreditPic.jpg", false);
	else if (combatType == "Soldier")
		GenerateGLTexture(image, "./images/nicholasJo.png", false);
	else if (combatType == "Tank")
		GenerateGLTexture(image, "./images/brandonH.png", false);
}

//==========================[PLAYER CLASS]===============================
//Inherits from Entity

//Null because instance will be initialized on demand
int Player::count = 0;
Player* Player::instance = 0;

Player::Player(string c)
{
	Log("Player::Player(string c), c = %s\n", c.c_str());
	setPlayerCombatType(c);
	setPlayerImage();
	setAlly(true);
	current_health = getMaxHealth();
	current_defense = getDefaultDefense();
	current_damage = getDefaultDamage();
	wYaw = 180.0;
	bYaw = 180.0;
	wPos.x = 7;
	wPos.z = 7;
	defaultBPos.x = 1;
	defaultBPos.z = 1;
	bPos.x = 1;
	bPos.z = 1;
	score = 0;
	count++;
}

Player* Player::getInstance()
{
	if (count != 0)
		return instance;
	return 0;
}

Player* Player::setInstance(string c)
{
	Log("Player::setInstance(string c), c = %s\n", c.c_str());
	if (count == 0)
		instance = new Player(c);
	return instance;
}

void Player::resetInstance()
{
	Log("Player::resetInstance()\n");
	delete instance;
	instance = NULL;
	count--;
}

void Player::saveInstance(ofstream &file)
{
	Log("Player::saveInstance(ofstream &file)\n");
	if (file) {
		file << instance->combatType << endl;
		file << instance->current_health << endl;
		file << instance->current_defense << endl;
		file << instance->current_damage << endl;
		file << instance->wPos.x << endl;
		file << instance->wPos.y << endl;
		file << instance->wPos.z << endl;
		file << instance->score << endl;
	} else {
		printf("Could not locate file specified\n");
	}
	return;
}

void Player::loadInstance(ifstream &file)
{
	Log("Player::loadInstance()\n");
	string line;
	if (file) {
		//load combatType - line 1
		getline(file, line);
		Player *player = setInstance(line);
		//load current_health - line 2
		getline(file, line);
		//strtof = string to float
		player->current_health = strtof(line.c_str(), NULL);
		//load current_defense - line 3
		getline(file, line);
		player->current_defense = strtof(line.c_str(), NULL);
		//load current_damage - line 4
		getline(file, line);
		player->current_damage = strtof(line.c_str(), NULL);
		//load wPos.x - line 5
		getline(file, line);
		player->wPos.x = strtof(line.c_str(), NULL);
		//load wPos.y - line 6
		getline(file, line);
		player->wPos.y = strtof(line.c_str(), NULL);
		//load wPos.z - line 7
		getline(file, line);
		player->wPos.z = strtof(line.c_str(), NULL);
		//load count - line 8
		getline(file, line);
		player->score = atoi(line.c_str());
		player->count = 1;
	} else {
		printf("Could not locate file specified\n");
	}
	return;
}

void Player::setPlayerCombatType(string c)
{
	Log("Player::setPlayerCombatType(string c), c = %s\n", c.c_str());
	if (c == "Archer") {
		modelID = 3;
		combatType = c;
		setMaxHealth(75.0);
		setDefaultDefense(0.60);
		setDefaultDamage(15.0);
		setMaxTurns(2);
		moveRange = 2;
		attackRange = 3;
	} else if (c == "Soldier") {
		modelID = 6;
		combatType = c;
		setMaxHealth(100.0);
		setDefaultDefense(0.70);
		setDefaultDamage(20.0);
		setMaxTurns(3);
		moveRange = 3;
		attackRange = 1;
	} else if (c == "Tank") {
		modelID = 0;
		combatType = c;
		setMaxHealth(125.0);
		setDefaultDefense(0.80);
		setDefaultDamage(22.0);
		setMaxTurns(3);
		moveRange = 3;
		attackRange = 1;
	} else if (c == "Nick") {
		modelID = 0;
		combatType = c;
		setMaxHealth(999);
		setDefaultDefense(1);
		setDefaultDamage(999);
		setMaxTurns(999);
		moveRange = 999;
		attackRange = 999;
	}
	return;
}

void Player::setPlayerImage()
{
	Log("Player::setPlayerImage()\n");
	if (combatType == "Archer")
		GenerateGLTexture(image, "./images/aopic.png", false);
	else if (combatType == "Nick")
		GenerateGLTexture(image, "./images/nickLCreditPic.jpg", false);
	else if (combatType == "Soldier")
		GenerateGLTexture(image, "./images/nicholasJo.png", false);
	else if (combatType == "Tank")
		GenerateGLTexture(image, "./images/brandonH.png", false);
}

//============================================================================

void Display_NicholasJordan(int x, int y, GLuint textid)
{
	glColor3ub(255, 255, 255);
	//static float angle = 0.0f;
	static int wid = 50;
	//wid += sin(angle) * 10;
	//float fx = (float)x;
	//float fy = (float)y;
	//float a = 0;
	//a += sin(angle) * 10.0f;
	//fx += a;
	//angle += 0.2f;
	glPushMatrix();
	//glTranslatef(fx, fy, 0);
	glTranslatef(x, y, 0);
	//glRotatef(a, 0, 0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
	glEnd();

	Rect r;
	glTranslatef(wid,0,0);
	r.bot = 0;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0xffffffff, "Nicholas Jordan");
	glPopMatrix();
}
#endif
