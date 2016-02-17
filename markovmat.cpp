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

bool arrayContains(int *v, int i, int n)
{
    bool resp = false;
    for(int j = 0; j < n; j++)
    {
        if (v[j] == i)
        {
            resp = true;
            break;
        }
    }
    return resp;
}

bool containsBFS(float **mat, int i, int j, int n)
{
    int v[n];
    int nv=0;
    int lnv = 0;
    v[nv] = i;
    nv++;
    bool contains = false;

    while (nv != lnv && !contains )
    {
        lnv = nv;
        for(int k = 0; k < lnv; k++)
        {
            for (int l = 0; l < n; l++)
            {
                if ((mat[v[k]][l] > 0) && !arrayContains(v, l, nv))
                {
                    if (l==j)
                    {
                        contains = true;
                    }
                    v[nv] = l;
                    nv++;
                }
            }
        }
    }

    return contains;
}

void classifyNode (float **mat, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (mat[i][i] == 1)
        {
            printf("Estado %d: Estado Absorvente\n", i);
        }

        else
        {
            for (int j = 0; j < n; j++)
            {
                if (i==j)   continue;
                if (mat[i][j] > 0){
                    if (containsBFS(mat, j, i, n))
                    {
                        printf("Estado %d em relacao ao estado %d : Estado Recorrente\n", i, j );
                    }
                    else printf("Estado %d em relacao ao estado %d : Estado Transiente\n", i, j );
                }
            }
        }
    }
    printf("\n");
}

int countVizinhos(float **mat, int n, int i)
{
    int countVizinho = 0;
    for (int j = 0; j < n; j++)
    {
        if (mat[i][j]!=0)   countVizinho++;
    }
    return countVizinho;
}

int classifyPeriodicity(float **mat, int n, int targetNode, int startNode, int l, int visited[], int nvisited)
{
    for (int i = 0; i < nvisited; i++)
    {
        if (startNode == visited[i])    return l;
    }
    visited[nvisited] = startNode;
    nvisited++;
    if (targetNode == startNode)    return l+1;
    for (int i = 0; i < n; i++)
    {
        if (mat[startNode][i] != 0)
        {
            if (startNode!=i)
            {
                l = classifyPeriodicity(mat, n, targetNode, i, l, visited, nvisited);
                if (l!= 0)  return l+1;
            }
        }
    }
    return l;
}

void classifyPeriodic(float **mat, int n)
{
    int level;
    int l;
    bool periodic;
    int visited[n];
    for(int i = 0; i < n; i++)
    {
        level = 0;
        l = 1;
        periodic = true;
        for (int j = 0; j < n; j++)
        {
            if (mat[i][j] != 0)
            {
                level = classifyPeriodicity(mat, n, i, j, 0, visited, 0);
                if (level == 0)
                {
                    periodic=false;
                }
                else if (level % l != 0)
                {
                    periodic = false;
                    break;
                }
                else
                {
                    periodic = true;
                    if (l==1)   l = level;
                }
            }
        }
        if (periodic)
        {
            printf("Estado %d: Periodico\n", i);
        }
        else
        {
            printf("Estado %d: Aperiodico\n", i);
        }
    }
}
