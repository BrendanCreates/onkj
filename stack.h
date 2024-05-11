#ifndef STACK_H
#define STACK_H
#define MAX 100
// max size is index not length

typedef struct coord {  //not all 4 values will always be used
    int x;              //x coordinate
    int y;              //y coordinate
    char type;
    int deed;
    int dir[4];

} Coord;

typedef struct memory {
    Coord coords[MAX];         //stack with all remembered coords
    int top;                        //top of stack index
} AntMemory;

typedef struct maze {
    int xSize, ySize;               //represents the size of the maze in x and y directions
    Coord **map;              //2d array representing the maze, will work on dynamic size later
    Coord antPos;                   //current position of ant
} Maze;

//i made the move functions return an int, they return the number of good deed points you got with that move

void mark(Maze *maze);
int move_f(Maze *maze);
int move_b(Maze *maze);
int move_l(Maze *maze);
int move_r(Maze *maze);
int cwf(Maze *maze);
int cwb(Maze *maze);
int cwl(Maze *maze);
int cwr(Maze *maze);
void push(Coord coord, AntMemory *coordinates);
Coord pop(AntMemory *coordinates);
Coord peek(AntMemory *coordinates);
void clear(AntMemory *coordinates);
void bjpi(Maze *maze, int x);
void cjpi(Maze *maze);
void backtrack(Maze *maze, AntMemory *memory);

#endif
