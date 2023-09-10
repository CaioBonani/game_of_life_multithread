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

int somaVizinhos(float **matriz, int i, int j)
{

    int totalVizinhos = 0;
    int aux, aux2;

    for (aux = -1; aux <= 1; aux++)
    {
        for (aux2 = -1; aux2 <= 1; aux2++)
        {

            if (aux != 0 || aux2 != 0)
            {

                int px = i + aux, py = j + aux2;

                if (px <= -1)
                {
                    px = N - 1;
                }

                if (px >= N)
                {
                    px = 0;
                }

                if (py <= -1)
                {
                    py = N - 1;
                }

                if (py >= N)
                {
                    py = 0;
                }

                totalVizinhos += matriz[px][py];

            }
        }
    }

    return totalVizinhos;
}

int main()
{
    float **grid, **newGrid;

    alocarMatriz(&grid);
    alocarMatriz(&newGrid);

    zerarMatriz(&grid);
    zerarMatriz(&newGrid);

    glider(&grid);
    rPentomino(&grid);

    // printarMatriz(grid);

    newGrid[0][0] = 0.0;
    newGrid[1][1] = 1.0;
    newGrid[2][2] = 0.0;
    newGrid[1][0] = 1.0;
    newGrid[0][1] = 1.0;
    newGrid[2][0] = 0.0;
    newGrid[0][2] = 1.0;
    newGrid[2][1] = 0.0;
    newGrid[1][2] = 1.0;

    int teste = somaVizinhos(newGrid, 1, 1);

    printf("\nRESULTADO DO TESTE = %i", teste);

    desalocarMatriz(&grid);
    desalocarMatriz(&newGrid);

    return 0;
}
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

int somaVizinhos(float **matriz, int i, int j)
{

    int totalVizinhos = 0;
    int aux, aux2;

    for (aux = -1; aux <= 1; aux++)
    {
        for (aux2 = -1; aux2 <= 1; aux2++)
        {

            if (aux != 0 || aux2 != 0)
            {

                int px = i + aux, py = j + aux2;

                if (px <= -1)
                {
                    px = N - 1;
                }

                if (px >= N)
                {
                    px = 0;
                }

                if (py <= -1)
                {
                    py = N - 1;
                }

                if (py >= N)
                {
                    py = 0;
                }

                totalVizinhos += matriz[px][py];

            }
        }
    }

    return totalVizinhos;
}

int main()
{
    float **grid, **newGrid;

    alocarMatriz(&grid);
    alocarMatriz(&newGrid);

    zerarMatriz(&grid);
    zerarMatriz(&newGrid);

    glider(&grid);
    rPentomino(&grid);

    // printarMatriz(grid);

    newGrid[0][0] = 0.0;
    newGrid[1][1] = 1.0;
    newGrid[2][2] = 0.0;
    newGrid[1][0] = 1.0;
    newGrid[0][1] = 1.0;
    newGrid[2][0] = 0.0;
    newGrid[0][2] = 1.0;
    newGrid[2][1] = 0.0;
    newGrid[1][2] = 1.0;

    int teste = somaVizinhos(newGrid, 1, 1);

    printf("\nRESULTADO DO TESTE = %i", teste);

    desalocarMatriz(&grid);
    desalocarMatriz(&newGrid);

    return 0;
}