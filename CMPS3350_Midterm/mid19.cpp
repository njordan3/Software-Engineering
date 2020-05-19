//https://github.com/njordan3/CMPS3350_Midterm
//
//
//Author: Gordon Griesel
//Modified By: Nicholas Jordan
//Date: April 7, 2019
//Modify Date: April 11, 2019
//Purpose: Framework for a coding exam
//
//See instructions on assignment web page to produce a unit-test
//for the vec_length function.
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "data.h"


//prototype
Flt vec_length(Vec v);

int errors = 0;
//tolerance default value
float tolerance = 0.1;

int main(int argc, char *argv[])
{
    //Program to calculate vector normalization and length.
	//
    if (argc == 2) {
        tolerance = atof(argv[1]);
    }
    printf("                                    \n");
    printf("3350 software engineering           \n");
    #ifdef UNIT_TEST
    printf("                                    \n");
    printf("Unit test of vec_length() function: \n");
    printf("tolerance: %f            \n", tolerance);
    printf("                                    \n");
    printf("     x         y         length      actual \n");
    printf("     --------  --------  ----------  ---------- \n");
    #endif
    #ifndef UNIT_TEST
    printf("                                    \n");
    printf("Calculation of vector lengths:      \n");
    printf("                                    \n");
    printf("     x         y         length  \n");
    printf("     --------  --------  ---------- \n");
    #endif
    //read until sentinel found.
    int i=0;
    Vec v = { input[i*2+0], input[i*2+1] };
    do {
        Flt ret = vec_length(v);
        #ifdef UNIT_TEST
        if (abs(ret - known_output[i]) > tolerance) {
            errors++;
            printf("%3i  %8.5lf  %8.5lf  %10.5lf  %10.5lf <-- error\n", i+1, v[0], v[1], known_output[i], ret);
        } else {
            printf("%3i  %8.5lf  %8.5lf  %10.5lf  %10.5lf\n", i+1, v[0], v[1], known_output[i], ret);
        }
        #endif
        #ifndef UNIT_TEST
        printf("%3i  %8.5lf  %8.5lf  %10.5lf\n", i+1, v[0], v[1], ret);
        #endif
        ++i;
        v[0] = input[i*2+0];
        v[1] = input[i*2+1];
    } while (!(v[0]==0.0 && v[1]==0.0));
    #ifdef UNIT_TEST
    printf("\nUnit test complete\n");
    printf("errors found: %i\n", errors);
    #endif
    #ifndef UNIT_TEST
    printf("\nProgram complete.\n\n");
    #endif
    return 0;
}

Flt vec_length(Vec v)
{
    //Normalize a vector.
    //Return the original length of the vector.
    //
    //input: the 2 components of a vector
    //output: the pre-normalized vector length is returned
    //        the vector argument is scaled to a length of 1.0
    //
    //A degenerate vector causes an error condition.
    //It will return a length of 0.0
    //and an arbitrary vector with length 1.0.
    //
    //calculate the vector dot product with itself...
    Flt dot = v[0]*v[0] + v[1]*v[1];
    //check for degenerate vector...
    if (dot == 0.0) {
        //set an arbitrary vector of length 1.0
        //printf("ERROR vector magnitude is zero.\n");
        v[0] = 1.0;
        v[1] = 0.0;
        return 0.0;
    }
    //vector has a magnitude so continue.
    Flt len = sqrt(dot);
    Flt oolen = 1.0 / len;
    v[0] *= oolen;
    v[1] *= oolen;
    return len;
}

