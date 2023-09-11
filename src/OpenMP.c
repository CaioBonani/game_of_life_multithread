#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2048
#define nThreads 2
#define nGenerations 50

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

int somaVizinhos(float ***grid, int i, int j)
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

                if((*grid)[px][py] == 1.0){
                    totalVizinhos += 1;
                }

            }
        }
    }

    return totalVizinhos;
}

void trocarMatriz(float ***grid, float ***newGrid){

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){

            (*grid)[i][j] = (*newGrid)[i][j];
        }
    }
}

int somarVivos(float ***grid){

    int totalVivos = 0;

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){

            if((*grid)[i][j] == 1.0){
                totalVivos += 1;
            }
        }
    }

    return totalVivos;
}


void geracao(float ***grid, float ***newGrid){

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){

            int vizinhos = somaVizinhos(grid, i, j);

            if(vizinhos < 2){
                (*newGrid)[i][j] = 0;
            }

            if(vizinhos > 3){
                (*newGrid)[i][j] = 0;
            }

            if(vizinhos == 3){
                (*newGrid)[i][j] = 1;
            }

            if(vizinhos == 2){
                if((*grid)[i][j] == 1){
                    (*newGrid)[i][j] = 1;
                }

                else{
                    (*newGrid)[i][j] = 0;
                }
            }
        }
    }
}

int main()
{
    float **grid, **newGrid;
    time_t inicioTotal, finalTotal, inicioLaco, finalLaco;
    time(&inicioTotal);

    alocarMatriz(&grid);
    alocarMatriz(&newGrid);

    zerarMatriz(&grid);
    zerarMatriz(&newGrid);

    glider(&grid);
    rPentomino(&grid);

    time(&inicioLaco);

    for(int i = 0; i < nGenerations; i++){

        geracao(&grid, &newGrid);
        trocarMatriz(&grid, &newGrid);
        zerarMatriz(&newGrid);

        int resultado = somarVivos(&grid);
        printf("\nRESULTADO DA GERACAO(%i) = %i", i+1, resultado);
    }

    time(&finalLaco);

    double tempoLaco = difftime(finalLaco, inicioLaco);

    printf("\n-----Tempo total do laco -----\n");
    printf("%.2f segundos \n", tempoLaco);
    printf("------------------------------\n");

    desalocarMatriz(&grid);
    desalocarMatriz(&newGrid);

    time(&finalTotal);

    double tempoTotal = difftime(finalTotal, inicioTotal);

    printf("---Tempo total do programa ---\n");
    printf("%.2f segundos \n", tempoTotal);
    printf("------------------------------\n");

    return 0;
}