#include "markovmat.h"

float ** newMatriz(int n, int m)
{
    float** nMat = (float **) malloc(sizeof(float *)*n);
    if (nMat!=NULL)
    {
        for (int it = 0; it < n; it++)
        {
            nMat[it]=(float *)malloc(sizeof(float)*m);
        }
    }
    return nMat;
}

void zeroMatrix(float **mat, int n, int m)
{
    for(int i = 0; i < n; i++)
    {
        for (int j = 0; j< m; j++)
        {
            mat[i][j] = 0;
        }
    }
}

void printMatriz(float **mat, int n, int m)
{
    for (int it = 0; it < n; it++)
    {
        for(int it2 = 0; it2 < m; it2++)
        {
            printf("%f | ", mat[it][it2]);
        }
        printf("\n");
    }
}

float ** multiplyMatriz(float **mat1, float **mat2, int m, int n,int p)
{
    float **result = newMatriz(m,p);
    zeroMatrix(result, m, p);

    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            for (int k = 0; k < p; k++)
            {
                result[i][j]+=mat1[i][k]*mat2[k][j];
            }
        }
    }
    return result;
}

float ** expMatriz(float **mat, int g, int n)
{
    float **result;
    if (g <= 1){
        return mat;
    }
    else{
        result = multiplyMatriz(mat, expMatriz(mat,g-1,n), n, n, n);
        return result;
    }
}

float ** metodoMatriz(float ** P0, float **P, int g, int n)
{
    float **result = newMatriz(n,n);
    zeroMatrix(result, n, n);

    result = expMatriz(P, g, n);
    printf ("Matriz elevada a %d potencia \n", g);
    printMatriz(result, n, n);

    result = multiplyMatriz(P0, result, 1, n, n);

    return result;
}

float metodoChapman(float **P, int g, int i, int j, int n)
{
    float result = 0;

    if (g <= 1)
    {
        return P[i][j];
    }
    else
    {
        for(int k = 0; k < n; k++)
        {
            result+=P[i][k]*metodoChapman(P, g-1, k, j, n);
        }
        return result;
    }

}

float probPrimeiraVisita(float **P, int g, int i, int j, int n)
{
    float result = 0;

    if (g <= 1)
    {
        return P[i][j];
    }
    else
    {
        for (int k = 0; k < n;  k++)
        {
            if (k==j)   continue;
            result+=P[i][k]*probPrimeiraVisita(P, g-1, k, j, n);
        }
        return result;
    }
}
