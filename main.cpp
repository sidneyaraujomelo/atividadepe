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

    int option = 9999;
    char respFile = 'n';
    bool fromFile = false;

    while (option != 0)
    {
        option = 9999;
        respFile = 'n';
        fromFile = false;

        printf("Escolha uma das opcoes abaixo:\n");
        printf("1. Método da Matriz\n");
        printf("2. Chapman-Kolmogorov\n");
        printf("3. Probabilidade da primeira visita\n");
        printf("4. Classificacao de estados\n");
        printf("5. Periodicidade de estados\n");
        printf("0. Sair\n");
        printf ("Opcao: ");
        scanf("\n%d", &option);
        if (option != 0 && !feof(arq))
        {
            printf("Deseja ler dados do arquivos? S\\N ");
            scanf("\n%c", &respFile);
            if (respFile=='s' || respFile=='S') fromFile = true;
        }

        switch(option)
        {
        case 0:
            fclose(arq);
            return -1;
        case 1:
            {
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
                        if (fromFile)
                        {
                            fscanf(arq, "%f", &matInicial[0][it]);
                            printf("%f | ", matInicial[0][it]);
                        }
                        else
                        {
                            printf("Posicao %d :", it);
                            scanf("%f",&matInicial[0][it]);
                        }
                        sum+=matInicial[0][it];
                    }
                    printf("\n");
                    if (sum != 1)
                    {
                        printf("Vetor invalido. Soma da linha deve ser igual a 1!");
                        fclose(arq);
                        return -1;
                    }
                }

                if (fromFile)
                {
                    fscanf(arq, "%d", &g);
                    printf("Numero de geracoes: %d\n", g);
                    fscanf(arq, "%d", &i);
                    printf("Estado inicial: %d\n", i);
                    fscanf(arq, "%d", &j);
                    printf("Estado final: %d\n\n", j);
                }
                else
                {
                    printf("Numero de geracoes: ");
                    scanf("\n%d", &g);
                    printf("Estado inicial: ");
                    scanf("\n%d", &i);
                    printf("Estado final: ");
                    scanf("\n%d", &j);
                    printf("\n");
                }

                float **result = metodoMatriz(matInicial, mat, g, n);
                printf ("Matriz final usando Metodo da Matriz \n", g);
                printMatriz(result, 1, n);
                printf("\n");

                break;
            }

        case 2:
            {
                if (fromFile)
                {
                    fscanf(arq, "%d", &g);
                    printf("Numero de geracoes: %d\n", g);
                    fscanf(arq, "%d", &i);
                    printf("Estado inicial: %d\n", i);
                    fscanf(arq, "%d", &j);
                    printf("Estado final: %d\n\n", j);
                }
                else
                {
                    printf("Numero de geracoes: ");
                    scanf("\n%d", &g);
                    printf("Estado inicial: ");
                    scanf("\n%d", &i);
                    printf("Estado final: ");
                    scanf("\n%d", &j);
                    printf("\n");
                }

                float pij = metodoChapman(mat, g, i, j, n);
                printf ("P%d%d usando Chapman-Kolmogorov: %f \n\n", i,j, pij);
                break;
            }

        case 3:
            {
                if (fromFile)
                {
                    fscanf(arq, "%d", &g);
                    printf("Numero de geracoes: %d\n", g);
                    fscanf(arq, "%d", &i);
                    printf("Estado inicial: %d\n", i);
                    fscanf(arq, "%d", &j);
                    printf("Estado final: %d\n\n", j);
                }
                else
                {
                    printf("Numero de geracoes: ");
                    scanf("\n%d", &g);
                    printf("Estado inicial: ");
                    scanf("\n%d", &i);
                    printf("Estado final: ");
                    scanf("\n%d", &j);
                    printf("\n");
                }

                float oij = probPrimeiraVisita(mat, g, i, j, n);
                printf ("O%d%d - probabilidade de primeira visita: %f \n\n", i,j, oij);
                break;
            }

        case 4:
            {
                classifyNode(mat, n);
                break;
            }

        case 5:
            {
                classifyPeriodic(mat, n);
                break;
            }

        }


    }
    return 0;
}
