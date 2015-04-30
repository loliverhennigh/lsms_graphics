
#include "particle.h"

#ifndef _spring_H_
#define _spring_H_


typedef struct {
	int type;
	double k;
	double k_d;
	double x_0;
	particle * a;
	particle * b;

} spring;

void spring_init(spring * s, int type, double k, double k_d, double x_0, particle * a, particle *b);
int spring_get_type(spring * s);
void spring_calc_force(spring * s, double dt, force * force_store_space);
double spring_pos_x_a(spring * s);
double spring_pos_y_a(spring * s);
double spring_pos_z_a(spring * s);
double spring_pos_x_b(spring * s);
double spring_pos_y_b(spring * s);
double spring_pos_z_b(spring * s);

#endif	



