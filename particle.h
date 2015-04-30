
#include "force.h"

#ifndef _particle_H_
#define _particle_H_


typedef struct {
	double x_a;
	double x_b;
	double x_force;
	double y_a;
	double y_b;
	double y_force;
	double z_a;
	double z_b;
	double z_force;
	double mass;
	double charge;

} particle;

void particle_init(particle * p, double x, double y, double z, double mass, double charge);
double particle_get_x_a(particle * p);
double particle_get_y_a(particle * p);
double particle_get_z_a(particle * p);
double particle_get_x_b(particle * p);
double particle_get_y_b(particle * p);
double particle_get_z_b(particle * p);
double particle_get_mass(particle * p);
double particle_get_charge(particle * p);
void particle_set_x_a(particle * p, double x);
void particle_set_x_b(particle * p, double x);
void particle_set_y_a(particle * p, double y);
void particle_set_y_b(particle * p, double y);
void particle_set_z_a(particle * p, double z);
void particle_set_z_b(particle * p, double z);
void particle_swap_a_b(particle *p);
void particle_add_force(particle * p, force * f);
double particle_distance_a(particle * p_a, particle * p_b);
double particle_distance_b(particle * p_a, particle * p_b);
double particle_distance_squared_a(particle * p_a, particle * p_b);
double particle_distance_squared_b(particle * p_a, particle * p_b);
void particle_calc_charge_force(particle * p_a, particle * p_b, force * force_store_space);
void particle_force_zero(particle * p);
void particle_first_step(particle * p, double dt);
void particle_update_step(particle * p, double dt);
void particle_add_charge(particle * p, double charge);


#endif	



