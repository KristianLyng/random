#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

#define RUNS 10000

#define HEAD(q) (q->e[q->head])
#define TAIL(q) (q->e[q->tail])

struct grid_t {
    int w, h;
    int *ar;
    int *path;
    int *marked;
    size_t size;
};

struct queue_element_t {
    int id;
    int next;
};

struct queue_t {
    int head;
    int tail;
    struct queue_element_t *e;
};

// Pushing information about a cell to be checked to our queue.
static inline void
push(struct queue_t *queue, const struct grid_t *grid, int id, int next)
{
    assert(grid->size);
    queue->e[queue->tail].id = id;
    queue->e[queue->tail].next = next;
    grid->marked[id]++;
    queue->tail++;
}

static void
checkfield(struct queue_t *q, const struct grid_t *g)
{
    int cursor = HEAD(q).id;
    int nextval = HEAD(q).next;
    // Write the amount of steps from the goal to this cell.
    g->path[cursor] = nextval;

    // Check if the cell is a walkable area, return if it isn't.
    switch(g->ar[cursor])
    {
        case 0: case 255: break;
        case 1: g->path[cursor] = -1; return; break;
        default: return;
    }

    // Check the cell to the right.
    if ( (cursor+1) % g->w != 0 &&
         (cursor+1) < g->size &&
         g->marked[cursor+1] == 0)
        push(q,g,cursor+1,nextval+1);

    // Check the cell to the left.
    if ( (cursor) % g->w != 0 &&
         (cursor-1) >= 0 &&
         g->marked[cursor-1] == 0)
        push(q,g,cursor-1,nextval+1);

    // Check the cell below.
    if ( (cursor + g->w) < g->size && g->marked[cursor + g->w] == 0 )
        push(q,g,cursor + g->w, nextval+1);

    // Check the cell above.
    if ( (cursor - g->w) >= 0 && g->marked[cursor - g->w] == 0)
        push(q,g,cursor - g->w, nextval+1);
}

/* 
 * Parse the grid input and allocate a matching memory segment
 *
 * XXX: Cheapo-error handling
 */
static struct grid_t *parse_grid(void) {
    struct grid_t *grid;
    int ret,i;
    grid = malloc(sizeof(struct grid_t));
    assert(grid);

    printf("How large is your grid (WxH)? ");
    scanf("%dx%d",&grid->w, &grid->h);

    assert (grid->w > 0 && grid->h > 0);

    grid->size = grid->w * grid->h;

    grid->ar = calloc(grid->size, sizeof(int));
    grid->path = calloc(grid->size, sizeof(int));
    grid->marked = calloc(grid->size, sizeof(int));
    assert(grid->ar && grid->path && grid->marked);

    // Reads in the level from stdin.
    for(i=0; i < grid->size; i++) {
        ret = scanf("%d", &(grid->ar[i]));
        assert (ret != 0);
    }
    return grid;
}

/*
 * Reset grid
 */
static void reset_grid_path(struct grid_t *g) {
    memset(g->path, '\0', g->size * sizeof(int));
    memset(g->marked, '\0', g->size * sizeof(int));
}

/*
 * Free grid
 */
static void free_grid(struct grid_t *g)
{
    free(g->ar);
    free(g->path);
    free(g->marked);
    free(g);
}

/*
 * Allocate and initialize the queue.
 */
static struct queue_t *alloc_queue(const struct grid_t *grid)
{
    struct queue_t *queue;
    queue = calloc(1, sizeof(struct queue_t));

    queue->head = 0;
    queue->tail = 0;

    queue->e = calloc(grid->size, sizeof(struct queue_element_t));

    assert(queue->e);
    return queue;
}

/*
 * Reset the queue, as if it was empty.
 */
static void reset_queue(struct queue_t *q, const struct grid_t *g)
{
    struct queue_element_t *t = q->e;
    memset(q, '\0', sizeof(struct queue_t));
    q->e = t;
    memset(q->e, '\0', g->size);
}

/*
 * Free the queue.
 */
static void free_queue(struct queue_t *q)
{
    free(q->e);
    free(q);
};

int main(void)
{
    int i,o;
    struct grid_t *grid;
    struct queue_t *queue;
    struct timeval starttime, endtime;

    grid = parse_grid();
    queue = alloc_queue(grid);

    gettimeofday(&starttime, NULL);

    for(o=0; o < RUNS; o++)
    {
        reset_grid_path(grid);
        reset_queue(queue, grid);

        for(i=0; i<grid->size; i++)
        {
            if (grid->ar[i] == 255) {
                push(queue, grid, i, 1);
            }
        }

        // As long as there are unchecked cells in the queue, check the cells.
        while (grid->marked[HEAD(queue).id] ) {
            checkfield(queue, grid);
            queue->head++;
        }
    }

    gettimeofday(&endtime, NULL);

    // Print the level to stdout.
    for(i=0;i< grid->size;i++)
    {
        if ( i % grid->w == 0 ) printf("\n");
        switch(grid->ar[i]) {
            case 0:     printf("%3d",grid->path[i]); break;
            case 1:     printf("###"); break;
            case 254:   printf("SSS"); break;
            case 255:   printf("EEE"); break;

        }
    }
    printf("\n\n");

    printf("Spent %d seconds and %d microseconds - avg. %.f usec per run\n", 
           endtime.tv_sec - starttime.tv_sec, 
           endtime.tv_usec - starttime.tv_usec,
           (float)(endtime.tv_usec - starttime.tv_usec) / (float) RUNS );

    free_queue(queue);
    free_grid(grid);
    return 0;
}
