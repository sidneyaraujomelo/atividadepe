#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

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

int main()
{
    FILE *arq;
    int n, m = 0;
    float **mat;
    float **matInicial;
    int g, i, j = 0;

    arq = fopen ("matriz.txt","r+");
    if (arq==NULL)
    {
        printf("Arquivo matriz.txt n�o encontrado!");
        fclose (arq);
        return -1;
    }


    fscanf(arq, "%d", &n);
    printf("Grau da matriz: %d\n", n);

    if (n < 1)
    {
        printf("Valores invalidos para matriz");
        fclose(arq);
        return -1;
    }

    mat = newMatriz(n, n);
    if (mat == NULL)
    {
        printf("Falha na alocacao da matriz de probabilidade");
        fclose(arq);
        return -1;
    }

    printf("Matriz de Transicao\n");
    for (int it = 0; it < n; it++)
    {
        float sum = 0;
        for(int it2 = 0; it2 < n; it2++)
        {
            fscanf(arq, "%f", &mat[it][it2]);
            printf("%f | ", mat[it][it2]);
            sum+=mat[it][it2];
        }
        printf("\n");
        if (sum > 1)
        {
            printf("Matriz invalida. Soma da linha deve ser menor ou igual a 1!");
            fclose(arq);
            return -1;
        }
    }

    printf("Vetor Inicial de Transicao\n");
    matInicial = newMatriz(1,n);
    if (matInicial==NULL)
    {
        printf("Falha na aloca��o do vetor inicial");
        fclose(arq);
        return -1;
    }
    else
    {
        float sum = 0;
        for(int it = 0; it < n; it++)
        {
            fscanf(arq, "%f", &matInicial[0][it]);
            printf("%f | ", matInicial[0][it]);
            sum+=matInicial[0][it];
        }
        printf("\n");
        if (sum > 1)
        {
            printf("Vetor invalido. Soma da linha deve ser menor ou igual a 1!");
            fclose(arq);
            return -1;
        }
    }

    fscanf(arq, "%d", &g);
    printf("Numero de geracoes: %d\n", g);
    fscanf(arq, "%d", &i);
    printf("Estado inicial: %d\n", i);
    fscanf(arq, "%d", &j);
    printf("Estado final: %d\n\n", j);

    float **result = metodoMatriz(matInicial, mat, g, n);
    printf ("Matriz final usando Metodo da Matriz \n", g);
    printMatriz(result, 1, n);
    printf("\n");

    float pij = metodoChapman(mat, g, i, j, n);
    printf ("P%d%d usando Chapman-Kolmogorov: %f \n\n", i,j, pij);

    return 0;
}