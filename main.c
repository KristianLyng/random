#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct grid {
    int w, h;
    int *ar;
    int *path;
}grid;

static int *list;
static int *marked;
static int *cursor_tail;
static int *cursor_head;

// Pushing information about a cell to be checked to our queue.
static inline void push(int id, int next)
{
    cursor_tail[0] = id;
    cursor_tail[1] = next;
    marked[id]++;
    cursor_tail += 2;
}

void checkfield(int cursor, int nextval)
{
    // Write the amount of steps from the goal to this cell.
    grid.path[cursor] = nextval;

    // Check if the cell is a walkable area, return if it isn't.
    switch(grid.ar[cursor])
    {
        case 0: case 255: break;
        case 1: grid.path[cursor] = -1; return; break;
        default: return;
    }

    // Check the cell to the right.
    if ( (cursor+1) % grid.w != 0 && cursor+1 < grid.w*grid.h && marked[cursor+1] == 0 )
        push(cursor+1,nextval+1);

    // Check the cell to the left.
    if ( (cursor) % grid.w != 0 && cursor-1 >= 0 && marked[cursor-1] == 0 )
        push(cursor-1,nextval+1);

    // Check the cell below.
    if ( (cursor+grid.w) < grid.w*grid.h && marked[cursor+grid.w] == 0 )
        push(cursor+grid.w,nextval+1);

    // Check the cell above.
    if ( (cursor-grid.w) >= 0 && marked[cursor-grid.w] == 0 )
        push(cursor-grid.w,nextval+1);
}

int main(int argc, char *argv[])
{
    int i;

    printf("How large is your grid (WxH)? ");
    scanf("%dx%d",&grid.w, &grid.h);

    // Allocates the buffers we need for pathfinding, and checks if they were.
    // allocated successfully.
    grid.ar = malloc(sizeof(int)*grid.w*grid.h);
    grid.path = malloc(sizeof(int)*grid.w*grid.h);
    list = malloc(sizeof(int)*grid.w*grid.h*2);
    marked = malloc(sizeof(int)*grid.w*grid.h);


    if ( !grid.ar || !grid.path || !list || !marked ) {
        puts("Not enough memory");
        return -1;
    }

    // Reads in the level from stdin.
    for(i=0;i<grid.w*grid.h;i++) {
        if (!scanf("%d",&(grid.ar[i]))) puts("wat!");
    }

// Uncomment this if you want to measure how it performs on several runs
//    int o;
//    for(o=0;o<10000;o++)
//    {
        cursor_tail = cursor_head = list;
        memset(grid.path, '\0', sizeof(int)*grid.w*grid.h);
        memset(list, '\0', sizeof(int)*grid.w*grid.h*2);
        memset(marked, '\0', sizeof(int)*grid.w*grid.h);

        for(i=0;i<grid.w*grid.h;i++)
        {
            switch(grid.ar[i]) {
                case 255:
                    cursor_tail[0] = i;
                    cursor_tail[1] = 1;
                    marked[i]++;
                    cursor_tail += 2;
                break;
            }
        }

        // As long as there are unchecked cells in the queue, check the cells.
        while ( marked[cursor_head[0]] ) {
            checkfield(cursor_head[0],cursor_head[1]);
            cursor_head += 2;
        }
// And this
//    }
    

    // Print the level to stdout.
    for(i=0;i<grid.w*grid.h;i++)
    {
        if ( i % grid.w == 0 ) printf("\n");
        switch(grid.ar[i]) {
            case 0:     printf("%3d",grid.path[i]); break;
            case 1:     printf("###"); break;
            case 254:   printf("SSS"); break;
            case 255:   printf("EEE"); break;

        }
    }
    printf("\n\n");

    // Free the buffers.
    free(grid.ar);
    free(grid.path);
    free(list);
    free(marked);
    return 0;
}
