#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 2048
#define nThreads 2
#define nGenerations 2000

void alocarMatriz(float ***matriz)
{
    *matriz = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++)
    {
        (*matriz)[i] = (float *)malloc(N * sizeof(float));
    }
}

void desalocarMatriz(float ***matriz)
{
    for (int i = 0; i < N; i++)
    {
        free((*matriz)[i]);
    }
    free(*matriz);
}

void zerarMatriz(float ***matriz)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            (*matriz)[i][j] = 0;
        }
    }
}

void glider(float ***matriz)
{
    int lin = 1, col = 1;
    (*matriz)[lin][col + 1] = 1.0;
    (*matriz)[lin + 1][col + 2] = 1.0;
    (*matriz)[lin + 2][col] = 1.0;
    (*matriz)[lin + 2][col + 1] = 1.0;
    (*matriz)[lin + 2][col + 2] = 1.0;
}

void rPentomino(float ***matriz)
{
    int lin = 10;
    int col = 30;
    (*matriz)[lin][col + 1] = 1.0;
    (*matriz)[lin][col + 2] = 1.0;
    (*matriz)[lin + 1][col] = 1.0;
    (*matriz)[lin + 1][col + 1] = 1.0;
    (*matriz)[lin + 2][col + 1] = 1.0;
}

void printarMatriz(float **matriz)
{

    int i, j;

    for (i = 0; i < 50; i++)
    {
        for (j = 0; j < 50; j++)
        {

            printf("[%2.f]", matriz[i][j]);
        }
    }
}

int main()
{
    float **grid, **newGrid;

    printf("\nTESTE1");

    alocarMatriz(&grid);
    alocarMatriz(&newGrid);

    zerarMatriz(&grid);
    zerarMatriz(&newGrid);

    glider(&grid);
    rPentomino(&grid);

    printf("\nTESTE2");

    printarMatriz(grid);

    desalocarMatriz(&grid);
    desalocarMatriz(&newGrid);

    return 0;
}
