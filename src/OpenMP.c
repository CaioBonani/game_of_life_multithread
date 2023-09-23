#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define N 2048
#define nThreads 8
#define nGenerations 2000

void alocarMatriz(float ***grid)
{
    *grid = (float **)malloc(N * sizeof(float *));

#pragma omp parallel for num_threads(nThreads)
    for (int i = 0; i < N; i++)
    {
        (*grid)[i] = (float *)malloc(N * sizeof(float));
    }
}

void desalocarMatriz(float ***grid)
{

#pragma omp parallel for num_threads(nThreads)
    for (int i = 0; i < N; i++)
    {
        free((*grid)[i]);
    }
    free(*grid);
}

void zerarMatriz(float ***grid)
{

#pragma omp parallel for num_threads(nThreads)
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
#pragma omp parallel for num_threads(nThreads)
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

#pragma omp parallel for reduction(+ : totalVivos)
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

void geracao(float ***grid, float ***newGrid)
{
#pragma omp parallel for num_threads(nThreads)
    for (int i = 0; i < N; i++)
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

            // Agora que temos o total de vizinhos, podemos continuar com o resto da lógica
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
}

int main()
{
    float **grid, **newGrid;
    struct timeval inicioTotal, finalTotal, inicioLaco, finalLaco;
    struct timeval inicioSerial, finalSerial, inicioParalelo, finalParalelo;
    long long tempoSerial = 0, tempoParalelo = 0;
    gettimeofday(&inicioTotal, NULL);

    // Início da parte serial
    gettimeofday(&inicioSerial, NULL);

    alocarMatriz(&grid);
    alocarMatriz(&newGrid);

    zerarMatriz(&grid);
    zerarMatriz(&newGrid);

    glider(&grid);
    rPentomino(&grid);

    // Final da parte serial
    gettimeofday(&finalSerial, NULL);
    tempoSerial += (finalSerial.tv_sec - inicioSerial.tv_sec) * 1000 + (finalSerial.tv_usec - inicioSerial.tv_usec) / 1000;

    gettimeofday(&inicioLaco, NULL);

    // Início da parte paralela
    gettimeofday(&inicioParalelo, NULL);

    for (int i = 0; i < nGenerations; i++)
    {

        geracao(&grid, &newGrid);
        trocarMatriz(&grid, &newGrid);
        zerarMatriz(&newGrid);

        int resultado = somarVivos(&grid);
        // printf("\nRESULTADO DA GERACAO(%i) = %i", i + 1, resultado);
    }

    // Final da parte paralela
    gettimeofday(&finalParalelo, NULL);
    tempoParalelo += (finalParalelo.tv_sec - inicioParalelo.tv_sec) * 1000 + (finalParalelo.tv_usec - inicioParalelo.tv_usec) / 1000;

    gettimeofday(&finalLaco, NULL);

    // Exibir os tempos
    printf("\nTempo gasto na parte serial: %lld milisegundos\n", tempoSerial);
    printf("Tempo gasto na parte paralela: %lld milisegundos\n", tempoParalelo);

    long long tempoLaco = (finalLaco.tv_sec - inicioLaco.tv_sec) * 1000 + (finalLaco.tv_usec - inicioLaco.tv_usec) / 1000;

    printf("\n-----Tempo total do laco -----\n");
    printf("%lld milisegundos \n", tempoLaco);
    printf("------------------------------\n");

    printf("\n-----Num Threads -----\n");
    printf("%d \n", nThreads);
    printf("------------------------------\n");

    desalocarMatriz(&grid);
    desalocarMatriz(&newGrid);

    gettimeofday(&finalTotal, NULL);

    long long tempoTotal = (finalTotal.tv_sec - inicioTotal.tv_sec) * 1000 + (finalTotal.tv_usec - inicioTotal.tv_usec) / 1000;

    printf("---Tempo total do programa ---\n");
    printf("%lld milisegundos \n", tempoTotal);
    printf("------------------------------\n");

    return 0;
}
