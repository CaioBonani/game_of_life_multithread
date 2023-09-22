#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define N 2048
#define nThreads 1
#define nGenerations 2000

typedef struct
{
    float ***grid;
    float ***newGrid;
    int startRow;
    int endRow;
} ThreadArgs;

void alocarMatriz(float ***grid)
{
    *grid = (float **)malloc(N * sizeof(float *));

    for (int i = 0; i < N; i++)
    {
        (*grid)[i] = (float *)malloc(N * sizeof(float));
    }
}

void desalocarMatriz(float ***grid)
{
    for (int i = 0; i < N; i++)
    {
        free((*grid)[i]);
    }
    free(*grid);
}

void zerarMatriz(float ***grid)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            (*grid)[i][j] = 0;
        }
    }
}

void glider(float ***grid)
{
    int lin = 1, col = 1;
    (*grid)[lin][col + 1] = 1.0;
    (*grid)[lin + 1][col + 2] = 1.0;
    (*grid)[lin + 2][col] = 1.0;
    (*grid)[lin + 2][col + 1] = 1.0;
    (*grid)[lin + 2][col + 2] = 1.0;
}

void rPentomino(float ***grid)
{
    int lin = 10;
    int col = 30;
    (*grid)[lin][col + 1] = 1.0;
    (*grid)[lin][col + 2] = 1.0;
    (*grid)[lin + 1][col] = 1.0;
    (*grid)[lin + 1][col + 1] = 1.0;
    (*grid)[lin + 2][col + 1] = 1.0;
}

void printarMatriz(float **grid)
{

    int i, j;

    for (i = 0; i < 50; i++)
    {
        for (j = 0; j < 50; j++)
        {

            printf("[%2.f]", grid[i][j]);
        }
    }
}

float mediaVivos(float ***grid, int i, int j)
{
    float totalVizinhos = 0.0;
    int offsets[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    for (int k = 0; k < 8; ++k)
    {
        int px = (i + offsets[k][0] + N) % N; // Adicionado +N para garantir que o valor não seja negativo
        int py = (j + offsets[k][1] + N) % N;

        totalVizinhos += (*grid)[px][py];
    }

    return totalVizinhos / 8;
}

void trocarMatriz(float ***grid, float ***newGrid)
{

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {

            (*grid)[i][j] = (*newGrid)[i][j];
        }
    }
}

int somarVivos(float ***grid)
{

    int totalVivos = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {

            if ((*grid)[i][j] != 0.0)
            {
                totalVivos += 1;
            }
        }
    }

    return totalVivos;
}

void *threadWork(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    float ***grid = args->grid;
    float ***newGrid = args->newGrid;
    int startRow = args->startRow;
    int endRow = args->endRow;

    for (int i = startRow; i < endRow; i++)
    {
        for (int j = 0; j < N; j++)
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

                        if ((*grid)[px][py] != 0.0)
                        {
                            totalVizinhos += 1;
                        }
                    }
                }
            }

            // Lógica de atualização do jogo da vida
            if (totalVizinhos < 2)
            {
                (*newGrid)[i][j] = 0;
            }
            if (totalVizinhos > 3)
            {
                (*newGrid)[i][j] = 0;
            }
            if (totalVizinhos == 3)
            {
                if ((*grid)[i][j] == 1)
                {
                    (*newGrid)[i][j] = 1;
                }
                else
                {
                    (*newGrid)[i][j] = mediaVivos(grid, i, j);
                }
            }
            if (totalVizinhos == 2)
            {
                if ((*grid)[i][j] != 0)
                {
                    (*newGrid)[i][j] = 1;
                }
                else
                {
                    (*newGrid)[i][j] = 0;
                }
            }
        }
    }
    return NULL;
}

int main()
{
    float **grid, **newGrid;
    struct timeval inicioTotal, finalTotal, inicioLaco, finalLaco;
    pthread_t threads[nThreads];
    gettimeofday(&inicioTotal, NULL);
    ThreadArgs threadArgs[nThreads];
    int rowsPerThread = N / nThreads;

    alocarMatriz(&grid);
    alocarMatriz(&newGrid);

    zerarMatriz(&grid);
    zerarMatriz(&newGrid);

    glider(&grid);
    rPentomino(&grid);

    gettimeofday(&inicioLaco, NULL);

    for (int gen = 0; gen < nGenerations; gen++)
    {
        for (long i = 0; i < nThreads; i++)
        {
            threadArgs[i].grid = &grid;
            threadArgs[i].newGrid = &newGrid;
            threadArgs[i].startRow = i * rowsPerThread;
            threadArgs[i].endRow = (i + 1) * rowsPerThread;

            pthread_create(&threads[i], NULL, threadWork, (void *)&threadArgs[i]);
        }

        for (long i = 0; i < nThreads; i++)
        {
            pthread_join(threads[i], NULL);
        }

        int resultado = somarVivos(&grid);
        printf("\nRESULTADO DA GERACAO(%i) = %i", gen + 1, resultado);

        trocarMatriz(&grid, &newGrid);
        zerarMatriz(&newGrid);
    }

    gettimeofday(&finalLaco, NULL);

    double tempoLaco = (finalLaco.tv_sec - inicioLaco.tv_sec);

    printf("\n-----Tempo total do laco -----\n");
    printf("%.2f segundos \n", tempoLaco);
    printf("------------------------------\n");

    printf("\n-----Num Threads -----\n");
    printf("%d \n", nThreads);
    printf("------------------------------\n");

    desalocarMatriz(&grid);
    desalocarMatriz(&newGrid);

    gettimeofday(&finalTotal, NULL);

    double tempoTotal = (finalTotal.tv_sec - inicioTotal.tv_sec);

    printf("---Tempo total do programa ---\n");
    printf("%.2f segundos \n", tempoTotal);
    printf("------------------------------\n");

    return 0;
}
