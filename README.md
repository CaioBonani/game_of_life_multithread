# Game of Life - Comparison between Serial Model, OpenMP, and Pthreads

## Authors

- Caio Bonani Carvalho
- Davi Kawai Seiji
- Enzo Reis de Oliveira

## Introduction

This project aims to implement and compare the performance of John H. Conway's Game of Life across different programming models: serial model, parallelized with OpenMP, and parallelized with Pthreads.

### The Game of Life

The Game of Life is a cellular automaton devised by mathematician John H. Conway. The "game" is a mathematical model that simulates the evolution of a set of cells on a grid over time, following a set of simple rules:

- A live cell with fewer than two live neighbors dies.
- A live cell with two or three live neighbors stays alive.
- A live cell with more than three live neighbors dies.
- A dead cell with exactly three live neighbors becomes a live cell.

## Main Proposal

The focus of this work is to compare the performance of the Game of Life in different programming paradigms and with different thread counts (1, 2, 4, 8).

## Results

Times were measured in seconds, and iterations were set at 2000 for each scenario. The obtained results are summarized in the table below:

| Type      | Thread Count          | Total Time (seconds)   | Loop Time (seconds)    | Iterations |
|-----------|-----------------------|------------------------|------------------------|------------|
| pthreads  | 1                     | 332.778                | 332.753                | 2000       |
| pthreads  | 2                     | 200.904                | 200.881                | 2000       |
| pthreads  | 4                     | 137.615                | 137.592                | 2000       |
| pthreads  | 8                     | 126.158                | 126.133                | 2000       |
| openMP    | 1                     | 338.495                | 338.468                | 2000       |
| openMP    | 2                     | 195.868                | 195.849                | 2000       |
| openMP    | 4                     | 127.685                | 127.665                | 2000       |
| openMP    | 8                     | 108.891                | 108.875                | 2000       |
| serial    | 1                     | 340.733                | 340.707                | 2000       |

## Demonstrative Video

A 7-minute video with a detailed explanation and demonstration of the results can be found [here](https://drive.google.com/drive/folders/1xxsGlylHIGf5a_6zun1Uch23Mn53cIlP?usp=sharing).

