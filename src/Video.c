#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define N 2048
#define nThreads 2
#define nGenerations 50

void renderGrid(SDL_Renderer *renderer, float **grid, int iteration, int numVivos, TTF_Font *font)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < 50; ++i)
    {
        for (int j = 0; j < 50; ++j)
        {
            if (grid[i][j] != 0)
            {
                // Escalar o valor para um intervalo entre 0 e 255
                int grayValue = (int)(grid[i][j] * 255);

                SDL_SetRenderDrawColor(renderer, grayValue, grayValue, grayValue, SDL_ALPHA_OPAQUE);

                SDL_Rect r;
                r.x = i * 10; // 10 pixels por célula
                r.y = j * 10; // 10 pixels por célula
                r.w = 10;
                r.h = 10;
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }

    // Bloco de código para desenhar o texto:
    char buffer[100];
    sprintf(buffer, "Iteracao: %d, Numero de Vivos: %d", iteration, numVivos);

    SDL_Color textColor = {255, 255, 255, 255}; // Branco
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, buffer, textColor);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.x = 0;
    textRect.y = 0;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_RenderCopy(renderer, text, NULL, &textRect);

    SDL_DestroyTexture(text);
    SDL_FreeSurface(textSurface);

    SDL_RenderPresent(renderer);
}

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

    return totalVizinhos * 0.125; // 1/8 = 0.125, assim evitamos a divisão
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

void printarSubMatriz50x50(float **grid)
{
    for (int i = 0; i < 50; ++i)
    {
        for (int j = 0; j < 50; ++j)
        {
            if (grid[i][j] == 0)
            {
                printf(" ");
            }
            else
            {
                printf("#"); // Vamos usar o símbolo "#" para representar uma célula viva
            }
        }
        printf("\n");
    }
    printf("\n");
}

void geracao(float ***grid, float ***newGrid)
{
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
    gettimeofday(&inicioTotal, NULL);

    alocarMatriz(&grid);
    alocarMatriz(&newGrid);

    zerarMatriz(&grid);
    zerarMatriz(&newGrid);

    glider(&grid);
    rPentomino(&grid);

    gettimeofday(&inicioLaco, NULL);

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    TTF_Font *font = TTF_OpenFont("/home/enzo/Downloads/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Bold.ttf", 24);
    if (font == NULL)
    {
        printf("Erro ao abrir a fonte: %s\n", TTF_GetError());
        // Tratar o erro como apropriado para seu programa
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Game of Life",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    for (int i = 0; i < nGenerations; i++)
    {

        geracao(&grid, &newGrid);
        trocarMatriz(&grid, &newGrid);
        zerarMatriz(&newGrid);

        int resultado = somarVivos(&grid);

        if (i < 50)
        { // Atualiza a janela gráfica nas primeiras 50 gerações
            renderGrid(renderer, grid, i + 1, resultado, font);
            SDL_Delay(100); // Espera 100 milissegundos
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    gettimeofday(&finalLaco, NULL);

    double tempoLaco = (finalLaco.tv_sec - inicioLaco.tv_sec);

    desalocarMatriz(&grid);
    desalocarMatriz(&newGrid);

    gettimeofday(&finalTotal, NULL);

    double tempoTotal = (finalTotal.tv_sec - inicioTotal.tv_sec);

    return 0;
}