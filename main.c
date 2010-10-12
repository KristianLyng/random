#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

#define RUNS 100000

#define HEAD(q) (q->e[q->head])
#define TAIL(q) (q->e[q->tail])

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

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

static void push(struct queue_t *queue, const struct grid_t *grid, const
                 int id, const int next);


static void
checkfield(struct queue_t *q, const struct grid_t *g)
{
    int cursor = HEAD(q).id;
    // Write the amount of steps from the goal to this cell.
    g->path[cursor] = HEAD(q).next;

    // Check if the cell is a walkable area, return if it isn't.
    if (unlikely((g->ar[cursor] != 0) && (g->ar[cursor] != 255))) {
        if (g->ar[cursor] == 1)
            g->path[cursor] = -1;
        return;
    }

    // Check the cell to the right.
    if (g->marked[cursor+1] == 0 &&
         (cursor+1) % g->w != 0 &&
         (cursor+1) < g->size )
        push(q, g, cursor+1, HEAD(q).next + 1);

    // Check the cell to the left.
    if (g->marked[cursor-1] == 0 &&
         (cursor) % g->w != 0 &&
         (cursor-1) >= 0)
        push(q, g, cursor-1, HEAD(q).next + 1);

    // Check the cell below.
    if (g->marked[cursor + g->w] == 0 &&
         (cursor + g->w) < g->size)
        push(q, g, cursor + g->w, HEAD(q).next + 1);

    // Check the cell above.
    if (g->marked[cursor - g->w] == 0 &&
        (cursor - g->w) >= 0)
        push(q, g, cursor - g->w, HEAD(q).next + 1);
}

// Pushing information about a cell to be checked to our queue.
static void
push(struct queue_t *queue, 
     const struct grid_t *grid, 
     const int id, 
     const int next)
{
    queue->e[queue->tail].id = id;
    queue->e[queue->tail].next = next;
    grid->marked[id]++;
    queue->tail++;
}

static void
reset_grid_path(const struct grid_t *g)
{
    memset(g->path, '\0', g->size * sizeof(int));
    memset(g->marked, '\0', g->size * sizeof(int));
}

/* 
 * Parse the grid input and allocate a matching memory segment
 *
 * XXX: Cheapo-error handling
 * XXX: Now that reset_grid exists, calloc is redundant.
 */
static struct grid_t *
parse_grid(void)
{
    struct grid_t *grid;
    int ret,i;
    grid = malloc(sizeof(struct grid_t));
    assert(grid);

    printf("How large is your grid (WxH)? ");
    ret = scanf("%dx%d",&grid->w, &grid->h);
    assert(ret == 2);
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
    reset_grid_path(grid);
    return grid;
}

/*
 * Free grid
 */
static void
free_grid(struct grid_t *g)
{
    free(g->ar);
    free(g->path);
    free(g->marked);
    free(g);
}

/*
 * Reset the queue, as if it was empty.
 */
static void
reset_queue(struct queue_t *q)
{
    q->tail = 0;
    q->head = 0;
    q->e[0].id = 0;
    q->e[0].next = 0;
}

/*
 * Allocate and initialize the queue.
 */
static struct queue_t *
alloc_queue(const struct grid_t *grid)
{
    struct queue_t *queue;
    queue = calloc(1, sizeof(struct queue_t));
    queue->e = calloc(grid->size, sizeof(struct queue_element_t));

    assert(queue->e);
    reset_queue(queue);
    return queue;
}

static void
free_queue(struct queue_t *q)
{
    free(q->e);
    free(q);
}

int main(void)
{
    int i,o;
    struct grid_t *grid;
    struct queue_t *queue;
    struct timeval starttime, endtime, comptime;

    grid = parse_grid();
    queue = alloc_queue(grid);

    gettimeofday(&starttime, NULL);

    for(o=0; o < RUNS; o++)
    {
        reset_grid_path(grid);
        reset_queue(queue);

        for(i=0; i<grid->size; i++) {
            if (grid->ar[i] == 255) {
                push(queue, grid, i, 1);
            }
        }

        // As long as there are unchecked cells in the queue, check the cells.
        while (likely(grid->marked[HEAD(queue).id] != 0)) {
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

    timersub(&endtime,&starttime,&comptime); 
    printf("Spent %lu seconds and %lu microseconds - avg. %.f usec per run\n",
           comptime.tv_sec,
           comptime.tv_usec,
           (double)(((comptime.tv_sec*1000000) + comptime.tv_usec) / RUNS));

    free_queue(queue);
    free_grid(grid);
    return 0;
}
