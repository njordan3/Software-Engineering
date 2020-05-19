//
//optimized by: Nicholas Jordan
//date: 3/7/19
//
//
//Lab-7 framework
//This program spell checks the words in a text file.
//
//Original author: Gordon Griesel
//           Date: Spring 2019
//
//Assignment:
//Optimize this program for running time.
//Your time will be compared to other students in the class.
//This is an optimization contest.
//
//Test your program by using the profile times.
//Try to make is spell-check very quickly.
//
//A final test of your program can be done like this:
//   1. turn profiling off
//   2. run program like this:
//      time ./lab7 article
//
//
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <fstream>
#include <unistd.h>
#include <ctime>
#include <iostream>
using namespace std;
/*enum {
	LOWER,
	UPPER
};*/

//=============================================================
//#define PROFILE
#ifdef PROFILE
//This code is activated only for profiling while optimization
//is attempted.
const double oobillion = 1.0 / 1e9; //one over a billion.
struct timespec timeStart, timeCurrent;
double total_0 = 0.0;
double total_1 = 0.0;
double total_2 = 0.0;
double total_3 = 0.0;
double total_4 = 0.0;
double total_5 = 0.0;
double total_6 = 0.0;
double total_7 = 0.0;
void timeGet(struct timespec *t)
{
	clock_gettime(CLOCK_REALTIME, t);
}
double timeDiff(struct timespec *start, struct timespec *end)
{
	//return the difference in two times.
	double ret =
		(double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	return ret;
}
void timeCopy(struct timespec *dest, struct timespec *source)
{
	//copy one time structure to another.
	memcpy(dest, source, sizeof(struct timespec));
}
#endif
//=============================================================
//do not modify the dictionary name.
const char dictname[] = "/usr/share/dict/cracklib-small";
int spell_check(char *text);
int search_for_word(ifstream &din, char *xword);


int main(int argc, char *argv[])
{
    #ifdef PROFILE
	struct timespec tstart, tend;
	timeGet(&tstart);
    #endif
	if (argc < 2) {
		printf("\nUsage: %s <file name>\n\n", argv[0]);
		return 0;
	}
	char fname[256];
	strcpy(fname, argv[1]);
	printf("\n");
	printf("Lab-7 program optimization.\n");
	printf("spell-check this file: **%s**\n", fname);
	ifstream fin(fname);
	if (fin.fail()) {
		printf("ERROR: opening **%s** for input\n", fname);
		return 0;
	}
	int nMisspelled = 0;
	char word[256];
	fin >> word;
	while (!fin.eof()) {
		nMisspelled += spell_check(word);
		fin >> word;
	}
	fin.close();
    #ifdef PROFILE
	timeGet(&tend);
	total_0 += timeDiff(&tstart, &tend);
    printf("\nTotal misspelled words: %i\n", nMisspelled);
	printf("Program complete.\n\n");
	printf("Profiling times:\n");
	printf("              main(): %lf\n", total_0);
	printf("   search_for_word(): %lf\n", total_1);
	printf("   get_word_length(): %lf\n", total_2);
    #endif
	return 0;
}

int spell_check(char *xword) {
    #ifdef PROFILE
	struct timespec tstart, tend;
	timeGet(&tstart);
    #endif
	//Open the dictionary file
	ifstream din(dictname);
	if (din.fail()) {
		printf("Dictionary file failed to open!\n\n");
		exit(1);
	}
	int found = search_for_word(din, xword);
	if (found != 1) {
		printf("  Misspelled: %s\n", xword);
	} else {
		//Nothing to do here yet.
		//no big deal.
	}
	din.close();
	if (found != 0) {
        #ifdef PROFILE
	    timeGet(&tend);
	    total_5 += timeDiff(&tstart, &tend);
        #endif
		return 0;
    } else {
        #ifdef PROFILE
	    timeGet(&tend);
	    total_5 += timeDiff(&tstart, &tend);
        #endif
		return 1;
    }
}

void convert_to_a_case(char *str) {
	//convert a C-string to a case
	//case_flag: 0 = lower
	//           1 = upper
    //
    //Optimization: got rid of some unnecessary looping
    #ifdef PROFILE
	struct timespec tstart, tend;
	timeGet(&tstart);
    #endif
    //Optimization: convert only the first letter to lowercase
    //got rid of toupper loop
    str[0] = (char)tolower(str[0]);
    #ifdef PROFILE
	timeGet(&tend);
	total_6 += timeDiff(&tstart, &tend);
    #endif
    return;
}

void check_last_character(char *str) {
    #ifdef PROFILE
	struct timespec tstart, tend;
	timeGet(&tstart);
    #endif
	//Strip comma, period, colon, semi-colon from end of a string.
	char *p = str;
	//First, go all the way to the end of the string.
	while (*p)
		++p;
	--p;
	//Go backwards looking for a comma, period, colon, and semi-colon
	while (p>=str && ((*p==',') || (*p=='.') || (*p==':') || (*p==';'))) {
		*p = '\0';
		--p;
	}
    #ifdef PROFILE
    timeGet(&tend);
	total_3 += timeDiff(&tstart, &tend);
    #endif
    /*  Optimization: got rid of unnecessary repeated looping
     *
	//Go backwards looking for a period.
	while (p >= str && (*p == '.')) {
		*p = '\0';
		--p;
	}
	//Go backwards looking for a colon.
	while (p >= str && (*p == ':')) {
		*p = '\0';
		--p;
	}
	//Go backwards looking for a semi-colon.
	while (p >= str && (*p == ';')) {
		*p = '\0';
		--p;
	}*/
}

int get_word_length(char *str) {
    #ifdef PROFILE
	struct timespec tstart, tend;
	timeGet(&tstart);
    #endif
	//
	char *p = str;
	int len = 0;
	while (*p != '\0') {
		++len;
		++p;
	}
    #ifdef PROFILE
	timeGet(&tend);
	total_2 += timeDiff(&tstart, &tend);
    #endif 
	return len;
}

int compare_words(char *w1, char *w2) {
    #ifdef PROFILE
	struct timespec tstart, tend;
	timeGet(&tstart);
    #endif
	//return 0  words are equal
	//       1  if not
	char *p1 = w1;
	char *p2 = w2;
    while (!(*p1 == '\0' || *p2 == '\0')) {
		if (*p1 != *p2) {
            #ifdef PROFILE
	        timeGet(&tend);
	        total_4 += timeDiff(&tstart, &tend);
            #endif 
			return 1;
        }
		//if (*p1 > *p2)
		//	return -1;
		++p1;
		++p2;
	}
	if (*p1 == '\0' && *p2 != '\0') {
        #ifdef PROFILE
	    timeGet(&tend);
	    total_4 += timeDiff(&tstart, &tend);
        #endif 
		return 1;
    }
	if (*p1 != '\0' && *p2 == '\0') {
        #ifdef PROFILE
	    timeGet(&tend);
	    total_4 += timeDiff(&tstart, &tend);
        #endif 
		return 1;
    }
    #ifdef PROFILE
	timeGet(&tend);
	total_4 += timeDiff(&tstart, &tend);
    #endif 
	return 0;
}

int search_for_word(ifstream &fin, char *word) {
	//returns: 0 = word not found.
	//         1 = word found.
	//
    #ifdef PROFILE
	struct timespec tstart, tend;
	timeGet(&tstart);
    #endif
	check_last_character(word);
	//
	//Go to the top of the dictionary file.
	fin.clear();
	fin.seekg(0, fin.beg);
	//Now search through the whole file looking for the word.
	//Search top to bottom.
	char line[256];
	int found = 0;
	//priming read of the file.
	fin >> line;
	//Make sure both words are the same case.
    convert_to_a_case(word);
    //Optimization: check if found still equals 0 after each loop
	while (found == 0 && !fin.eof()) {
        //dont compare the words if the first letter don't match
        if (line[0] == word[0]) {
		    //Compare words for spelling match.
			if (!compare_words(line, word))
	            found = 1;
        }
		fin >> line;
	}
    #ifdef PROFILE
	timeGet(&tend);
	total_1 += timeDiff(&tstart, &tend);
	#endif
    return found;
}
