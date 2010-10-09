#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct grid {
	int w, h;
	int *ar;
	int *path;
}grid;

int checked = 0;
int checkedcomplete = 0;
int wasnt0 = 0;
int *list;
int *marked;
int *cursor_n;
int *cursor_d;


void checkfield(int cursor, int nextval)
{
// This was a pure test of how much time was wasted walking too far. The answer: a lot.
//	if ( nextval > 42 ) return;
	if ( grid.path[cursor] > 0 ) wasnt0++;
	checked++;
    grid.path[cursor] = nextval;
	switch(grid.ar[cursor])
	{
		case 0: case 255: break;
		case 1: grid.path[cursor] = -1; return; break;
		default: return;
	}
	if ( (cursor+1) % grid.w != 0 && cursor+1 < grid.w*grid.h && marked[cursor+1] == 0 )
		if ( grid.path[cursor+1] == 0 || grid.path[cursor+1] > (nextval+1) ) {
            cursor_n[0] = cursor+1;
            cursor_n[1] = nextval+1;
            marked[cursor+1]++;
            cursor_n += 2;
        }
//			checkfield(cursor+1, nextval+1);
    if ( (cursor) % grid.w != 0 && cursor-1 >= 0 && marked[cursor-1] == 0 )
        if ( grid.path[cursor-1] == 0 || grid.path[cursor-1] > (nextval+1) ) {
            cursor_n[0] = cursor-1;
            cursor_n[1] = nextval+1;
            marked[cursor-1]++;
            cursor_n += 2;
        }
//			checkfield(cursor-1, nextval+1);
    if ( (cursor+grid.w) < grid.w*grid.h && marked[cursor+grid.w] == 0 )
        if ( grid.path[cursor+grid.w] == 0 || grid.path[cursor+grid.w] > (nextval+1) ) {
            cursor_n[0] = cursor+grid.w;
            cursor_n[1] = nextval+1;
            marked[cursor+grid.w]++;
            cursor_n += 2;
        }
//			checkfield(cursor+grid.w, nextval+1);
    if ( (cursor-grid.w) >= 0 && marked[cursor-grid.w] == 0 )
        if ( grid.path[cursor-grid.w] == 0 || grid.path[cursor-grid.w] > (nextval+1) ) {
            cursor_n[0] = cursor-grid.w;
            cursor_n[1] = nextval+1;
            marked[cursor-grid.w]++;
            cursor_n += 2;
        }
//			checkfield(cursor-grid.w, nextval+1);
	checkedcomplete++;
}

int main(int argc, char *argv[])
{
	int i, o, cursor = 0;
	printf("How large is your grid (WxH)? ");
	scanf("%dx%d",&grid.w, &grid.h);

	grid.ar = malloc(sizeof(int)*grid.w*grid.h);
	grid.path = malloc(sizeof(int)*grid.w*grid.h);
    list = malloc(sizeof(int)*grid.w*grid.h*2);
    marked = malloc(sizeof(int)*grid.w*grid.h);
    cursor_n = cursor_d = list;
	memset(grid.path, '\0', sizeof(int)*grid.w*grid.h);
    memset(list, '\0', sizeof(int)*grid.w*grid.h*2);
    memset(marked, '\0', sizeof(int)*grid.w*grid.h);


	if ( !grid.ar || !grid.path ) {
		puts("Not enough memory");
		return -1;
	}

	for(i=0;i<grid.w*grid.h;i++) {
		if (!scanf("%d",&(grid.ar[i]))) puts("wat!");
	}

    for(o=0;o<60000;o++)
    {
        cursor_n = cursor_d = list;
        memset(grid.path, '\0', sizeof(int)*grid.w*grid.h);
        memset(list, '\0', sizeof(int)*grid.w*grid.h*2);
        memset(marked, '\0', sizeof(int)*grid.w*grid.h);

    	for(i=0;i<grid.w*grid.h;i++)
    	{
//    		if ( i % grid.w == 0 ) printf("\n");
    		switch(grid.ar[i]) {
/*    			case 0:		printf("   "); break;
    			case 1:		printf("###"); break;
    			case 254:	printf("SSS"); break;*/
    			case 255:	/*printf("EEE"); */cursor = i;
                    cursor_n[0] = i;
                    cursor_n[1] = 1;
                    marked[i]++;
                    cursor_n += 2;
                break;
    		}
    	}
//    	printf("\n");

//	for(i=0;i<1000;i++)
//	{
//		memset(grid.path, '\0', sizeof(int)*grid.w*grid.h);
        while ( marked[cursor_d[0]] ) {
//            printf("checkfield(%d,%d)\n",cursor_d[0],cursor_d[1]);
    		checkfield(cursor_d[0],cursor_d[1]);
            cursor_d += 2;
        }
	}
	
    for(i=0;i<grid.w*grid.h;i++)
    {
        if ( i % grid.w == 0 ) printf("\n");
        switch(grid.ar[i]) {
            case 0:     printf("%3d",grid.path[i]); break;
            case 1:     printf("###"); break;
            case 254:   printf("SSS"); break;
            case 255:   printf("EEE"); cursor = i; break;

        }
    }
	printf("\n\n");

	printf("checked: %d\ncheckedcomplete: %d\nwasnt0: %d\n", checked, checkedcomplete,wasnt0);

	free(grid.ar);
	free(grid.path);
	return 0;
}
