#ifndef MARKOVMAT_H_INCLUDED
#define MARKOVMAT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

float ** newMatriz(int, int);

void zeroMatrix(float **, int, int);

void printMatriz(float **, int, int);

float ** multiplyMatriz(float **, float **, int, int,int);

float ** expMatriz(float **, int, int);

float ** metodoMatriz(float **, float **, int, int);

float metodoChapman(float **, int, int, int, int);

float probPrimeiraVisita(float **, int, int, int, int);

#endif // MARKOVMAT_H_INCLUDED
