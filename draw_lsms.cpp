
#include "draw_lsms.h"
#include <malloc.h>
#include <stdio.h>

draw_lsms * draw_lsms_create(int num_draw_vertices)
{
    draw_lsms * dl = (draw_lsms *)malloc(sizeof(draw_lsms));
    dl->draw_vertices_list = (int *)malloc(sizeof(int) * num_draw_vertices);
    dl->num_draw_vertices = num_draw_vertices;
    return dl;
}

draw_lsms * draw_lsms_create_from_file(char filename[])
{
    FILE * fp;
    int i = 0;
    int size = 0;
    int store = 0;
    fp = fopen(filename, "r");
    fscanf(fp, "%i", &size);
    
    draw_lsms *dl = draw_lsms_create(size);

    for(i = 0; i < size; i++)
    {
        fscanf(fp, "%i", &store);
        dl->draw_vertices_list[i] = store;
    }
    fclose(fp);
    return dl;

}

