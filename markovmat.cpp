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


/*
Função: classifyNode, classifica os estados em absorvente, recorrente e transiente
Parâmetros: mat - matriz de transição
            n - numero de estados
Retorna: Vetor de Inteiros de tamanho n, onde cada estado é classificado como 0 (absorvente) ou 1 (recorrente) ou 2 (transiente);
*/
int * classifyNode (float **mat, int n)
{
    int* states = (int *)malloc(sizeof(int)*n);
    for (int i = 0; i < n; i++)
    {
        states[i]=9;
        if (mat[i][i] == 1)
        {
            printf("Estado %d: Estado Absorvente\n", i);
            states[i] = 0;
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
                        states[i]=1;
                    }
                    else
                    {
                        printf("Estado %d em relacao ao estado %d : Estado Transiente\n", i, j );
                        if (states[i]!=1)   states[i]=2;
                    }
                }
            }
        }
    }
    return states;
    printf("\n");
}

bool findNodeInClasses(int **classes, int n, int i, int *currentClass)
{
    for (int k = 0; k < n; k++)
    {
        for (int l = 1; l < classes[k][0]+1; l++)
        {
            if (classes[k][l] == i)
            {
                if (currentClass!=NULL)
                {
                    *currentClass = k;
                }
                return true;
                break;
            }
        }
    }
    return false;
}


int ** classifyNode2 (float **mat, int n)
{
    int **classes;

    classes =(int **) malloc(n*sizeof(int *));
    for (int i =0; i<n; i++)
    {
        classes[i] = (int *)malloc(sizeof(int)*(n+1));
        for (int j = 0; j < n+1; j++)
        {
            classes[i][j]=0;
        }
    }

    int currentClass = 0;
    for (int i = 0; i < n; i++)
    {
        if (mat[i][i] == 1)
        {
            //printf("Estado %d: Estado Absorvente\n", i);
            classes[currentClass][0]++;
            classes[currentClass][classes[currentClass][0]] = i;
            currentClass++;
        }

        else
        {
            for (int j = 0; j < n; j++)
            {
                if (i==j)
                {
                    if (!findNodeInClasses(classes, n, i, &currentClass))
                    {
                        while(classes[currentClass][0]!=0)
                        {
                            currentClass = (currentClass+1)%n;
                        }
                        classes[currentClass][0]++;
                        classes[currentClass][classes[currentClass][0]] = i;
                    }
                }
                else if (mat[i][j] > 0){
                    if (containsBFS(mat, j, i, n))
                    {
                        //printf("Estado %d em relacao ao estado %d : Estado Recorrente\n", i, j );
                        bool hasFoundNodeInClasses = false;

                        if (findNodeInClasses(classes, n, i, &currentClass))
                        {
                            if (!findNodeInClasses(classes,n,j, NULL))
                            {
                                classes[currentClass][0]++;
                                classes[currentClass][classes[currentClass][0]] = j;
                            }
                        }
                        else
                        {
                            while(classes[currentClass][0]!=0)
                            {
                                currentClass = (currentClass+1)%n;
                            }
                            classes[currentClass][0]++;
                            classes[currentClass][classes[currentClass][0]] = i;
                            classes[currentClass][0]++;
                            classes[currentClass][classes[currentClass][0]] = j;
                        }
                    }
                    else
                    {
                        //printf("Estado %d em relacao ao estado %d : Estado Transiente\n", i, j );
                    }
                }
            }
        }
    }

    bool hasClasses = false;
    printf("\n");
    for (int i = 0; i<n; i++)
    {
        if (classes[i][0] == 0) continue;
        printf("Classe Fechada %d \n", i+1);
        for (int j = 1; j < classes[i][0]+1; j++)
        {
            hasClasses = true;
            printf("%d ", classes[i][j]);
        }
        if (hasClasses) printf("\n");
    }
    if (!hasClasses)    printf("Nao ha classes fechadas\n");
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

/*void classifyPeriodic(float **mat, int n)
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
}*/

/*
Função: classifyPeriodic, classifica os estados em periódicos ou aperiódicos
Parâmetros: mat - matriz de transição
            n - numero de estados
Retorna: Vetor de Inteiros de tamanho n, onde cada estado é classificado como 0 (periódico) ou 1 (aperiodico);
*/
int* classifyPeriodic(float **mat, int n)
{
    int l;
    bool periodic;
    int* states = (int *)malloc(sizeof(int)*n);
    for(int i = 0; i < n; i++)
    {
        l = 1;
        periodic = true;
        for(int j = 1; j < n*3; j++)
        {
            float resp = probPrimeiraVisita(mat, j, i, i, n);
            if (resp > 0)
            {
                if (j==1){
                    periodic = false;
                    break;
                }
                else
                {
                    if (l == 1)
                    {
                        l = j;
                    }
                    else if (j % l != 0)
                    {
                        periodic = false;
                        break;
                    }
                }
            }
            //printf("Probabilidade de %d->%d no %d passo: %f\n", i,i,j,resp);
        }
        if (periodic)
        {
            printf("Estado %d: Periodico\n", i);
            states[i]=0;
        }
        else
        {
            printf("Estado %d: Aperiodico\n", i);
            states[i]=1;
        }
    }
    printf("\n");
    return states;
}
