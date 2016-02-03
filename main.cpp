#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include "markovmat.h"

using namespace std;

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
        printf("Arquivo matriz.txt não encontrado!");
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
        printf("Falha na alocação do vetor inicial");
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
