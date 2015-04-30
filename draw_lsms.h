

#ifndef _draw_lsms_H_
#define _draw_lsms_H_


typedef struct {
	int num_draw_vertices; // multiples of 3
	int * draw_vertices_list;
} draw_lsms;

draw_lsms * draw_lsms_create(int num_draw_vertices);
draw_lsms * draw_lsms_create_from_file(char filename[]);

#endif	



