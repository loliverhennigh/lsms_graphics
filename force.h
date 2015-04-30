

#ifndef _point_H_
#define _point_H_


typedef struct {
	double x;
	double y;
	double z;

} force;

force * force_create();
void force_init(force * f, double x, double y, double z);
void force_invert(force * f);
void force_destroy(force * f);

#endif	



