
#include "particle.h"
#include "force.h"
#include <malloc.h>
#include <math.h>

void particle_init(particle * p, double x, double y, double z, double mass, double charge)
{
	// init things
	p->x_a = x;
	p->x_b = x;
	p->x_force = 0.0;
	p->y_a = y;
	p->y_b = y;
	p->y_force = 0.0;
	p->z_a = z;
	p->z_b = z;
	p->z_force = 0.0;
	p->mass = mass;
	p->charge = charge;
}

double particle_get_x_a(particle * p) { return p->x_a; }
double particle_get_y_a(particle * p) { return p->y_a; }
double particle_get_z_a(particle * p) { return p->z_a; }
double particle_get_x_b(particle * p) { return p->x_b; }
double particle_get_y_b(particle * p) { return p->y_b; }
double particle_get_z_b(particle * p) { return p->z_b; }
double particle_get_mass(particle * p) { return p->mass; }
double particle_get_charge(particle * p) { return p->charge; }
void particle_set_x_a(particle * p, double x) { p->x_a = x; } 
void particle_set_x_b(particle * p, double x) { p->x_b = x; } 
void particle_set_y_a(particle * p, double y) { p->y_a = y; } 
void particle_set_y_b(particle * p, double y) { p->y_b = y; } 
void particle_set_z_a(particle * p, double z) { p->z_a = z; } 
void particle_set_z_b(particle * p, double z) { p->z_b = z; } 

void particle_swap_a_b(particle *p)
{
	double bla;
	bla = p->x_a;
	p->x_a = p->x_b;
	p->x_b = bla;
	bla = p->y_a; 
	p->y_a = p->y_b;
	p->y_b = bla;
	bla = p->z_a;
	p->z_a = p->z_b;
	p->z_b = bla;	
}

void particle_add_force(particle * p, force * f)
{
	p->x_force = p->x_force + f->x;
	p->y_force = p->y_force + f->y;
	p->z_force = p->z_force + f->z;
}

double particle_distance_a(particle * p_a, particle * p_b)
{
	double distance;
	distance = pow((p_a->x_a - p_b->x_a), 2);
	distance = distance + pow((p_a->y_a - p_b->y_a), 2);
	distance = distance + pow((p_a->z_a - p_b->z_a), 2);
	distance = sqrt(distance);
	return distance;
}

double particle_distance_b(particle * p_a, particle * p_b)
{
	double distance;
	distance = pow((p_a->x_b - p_b->x_b), 2);
	distance = distance + pow((p_a->y_b - p_b->y_b), 2);
	distance = distance + pow((p_a->z_b - p_b->z_b), 2);
	distance = sqrt(distance);
	return distance;
}

double particle_distance_squared_a(particle * p_a, particle * p_b)
{
	double distance;
	distance = pow((p_a->x_a - p_b->x_a), 2);
	distance = distance + pow((p_a->y_a - p_b->y_a), 2);
	distance = distance + pow((p_a->z_a - p_b->z_a), 2);
	return distance;
}

double particle_distance_squared_b(particle * p_a, particle * p_b)
{
	double distance;
	distance = pow((p_a->x_b - p_b->x_b), 2);
	distance = distance + pow((p_a->y_b - p_b->y_b), 2);
	distance = distance + pow((p_a->z_b - p_b->z_b), 2);
	return distance;
}

void particle_calc_charge_force(particle * p_a, particle * p_b, force * force_store_space)
{
	double distance = 0.0;
	distance = particle_distance_a(p_a, p_b);
	distance = (.1 * p_a->charge * p_b->charge) / pow(distance,3); // need to change charge constant
	force_init(force_store_space, (p_a->x_a - p_b->x_a) * distance, (p_a->y_a - p_b->y_a) * distance, (p_a->z_a - p_b->z_a) * distance);
	particle_add_force(p_a, force_store_space);
	force_invert(force_store_space);
	particle_add_force(p_b, force_store_space);	
}

void particle_first_step(particle * p, double dt)
{
	p->x_a = p->x_b + (.5 * p->x_force * pow(dt, 2)) / p->mass;
	p->y_a = p->y_b + (.5 * p->y_force * pow(dt, 2)) / p->mass;
	p->z_a = p->z_b + (.5 * p->z_force * pow(dt, 2)) / p->mass;
}

void particle_update_step(particle * p, double dt)
{
	p->x_b = 2.0 * p->x_a - p->x_b + (p->x_force * pow(dt, 2)) / p->mass;
	p->y_b = 2.0 * p->y_a - p->y_b + (p->y_force * pow(dt, 2)) / p->mass;
	p->z_b = 2.0 * p->z_a - p->z_b + (p->z_force * pow(dt, 2)) / p->mass;
	particle_swap_a_b(p);
}

void particle_force_zero(particle * p)
{
	p->x_force = 0.0;
	p->y_force = 0.0;
	p->z_force = 0.0;
}

void particle_add_charge(particle * p, double charge)
{
	p->charge = p->charge + charge;
}

