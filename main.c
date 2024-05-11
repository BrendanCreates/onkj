#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define MAX_NUMBER_OF_STEPS 100
#define MAX_SIZE 5 // assuming 5 for now, change according to your maze

Maze* readMaze(FILE *file) {
    Maze *maze = malloc(sizeof(Maze));
    if (!maze) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for map
    maze->map = malloc(MAX_SIZE * sizeof(Coord*));
    if (!maze->map) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_SIZE; i++) {
        maze->map[i] = malloc(MAX_SIZE * sizeof(Coord));
        if (!maze->map[i]) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Initialize maze
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            maze->map[i][j].x = i;
            maze->map[i][j].y = j;
            maze->map[i][j].type = ' ';
            maze->map[i][j].deed = 0;
            for (int k = 0; k < 4; k++) {
                maze->map[i][j].dir[k] = 0;
            }
        }
    }

    // Read maze from file
    char line[MAX_SIZE + 2];
    int row = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0; // remove trailing newline
        for (int i = 0; i < strlen(line); i++) {
            maze->map[row][i].type = line[i];
            if (line[i] >= '0' && line[i] <= '9') {
                maze->map[row][i].deed = line[i] - '0'; // convert char to int
            }
        }
        row++;
    }

    maze->xSize = row;
    maze->ySize = strlen(line);
    maze->antPos.x = 0; // assuming start index at (0,0)
    maze->antPos.y = 0;

    return maze;
}

void printMaze(Maze *maze) {
    for (int i = 0; i < maze->xSize; i++) {
        for (int j = 0; j < maze->ySize; j++) {
            printf("%c", maze->map[i][j].type);
        }
        printf("\n");
    }
}

void processMaze(Maze *maze) {
    int points = 0;
    AntMemory *memory = malloc(sizeof(AntMemory));
    memory->top = -1;

    push(maze->antPos, memory);

    while (points < MAX_NUMBER_OF_STEPS) {
        Coord currentPos = peek(memory);
        points += maze->map[currentPos.x][currentPos.y].deed;
        mark(maze);

        // Check available directions
        int right = cwr(maze);
        int back = cwb(maze);
        int left = cwl(maze);
        int front = cwf(maze);
        int dir[] = {right, back, left, front};

        // Use CJPI to move
        for (int i = 0; i < 4; i++) {
            if (dir[i] > 0) {
                cjpi(maze);
                memory->coords[memory->top].dir[i] = 1; // Mark the direction as used
                break;
            }
        }

        // Check if the ant has no available direction
        int noDirection = 1;
        for (int i = 0; i < 4; i++) {
            if (dir[i] > 0) {
                noDirection = 0;
                break;
            }
        }

        // Backtrack if no available direction
        if (noDirection) {
            for (int i = memory->top; i >= 0; i--) {
                for (int j = 0; j < 4; j++) {
                    if (memory->coords[i].dir[j] == 1) {
                        backtrack(maze, memory);
                        break;
                    }
                }
            }
        }

        // Exit loop if stack is empty
        if (memory->top == -1)
            break;
    }

    printf("Points: %d\n", points);
    clear(memory);
    free(memory);
}

int main() {
    FILE *file = fopen("maze.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    Maze *maze = readMaze(file);
    fclose(file);

    printf("Original Maze:\n");
    printMaze(maze);

    printf("\nProcessed Maze:\n");
    processMaze(maze);

    // Free memory
    for (int i = 0; i < MAX_SIZE; i++) {
        free(maze->map[i]);
    }
    free(maze->map);
    free(maze);

    return 0;
}
