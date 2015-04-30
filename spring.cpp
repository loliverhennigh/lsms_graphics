
#include <stdio.h>
#include "spring.h"
#include "force.h"

void spring_init(spring * s, int type, double k, double k_d, double x_0, particle * a, particle *b)
{
	s->type = type;
	s->k = k;
	s->k_d = k_d;
	s->x_0 = x_0;
	s->a = a;
	s->b = b;
}

int spring_get_type(spring * s) { return s->type; }

void spring_calc_force(spring * s, double dt, force * force_store_space)
{
	double distance = 0.0;
	distance = particle_distance_a(s->a, s->b);
	if ((s->type == 1) || ((s->type == 2) && (distance < s->x_0)))
	{
		double damping = 0.0;
		double x_v_diff = 0.0;
		double y_v_diff = 0.0;
		double z_v_diff = 0.0;
		if (distance > .0001)
		{
			x_v_diff = (particle_get_x_a(s->a) - particle_get_x_b(s->a) - particle_get_x_a(s->b) + particle_get_x_b(s->b));
			y_v_diff = (particle_get_y_a(s->a) - particle_get_y_b(s->a) - particle_get_y_a(s->b) + particle_get_y_b(s->b));
			z_v_diff = (particle_get_z_a(s->a) - particle_get_z_b(s->a) - particle_get_z_a(s->b) + particle_get_z_b(s->b));
			damping = x_v_diff * (particle_get_x_a(s->a) - particle_get_x_a(s->b)) + y_v_diff * (particle_get_y_a(s->a) - particle_get_y_a(s->b)) + z_v_diff * (particle_get_z_a(s->a) - particle_get_z_a(s->b));
			damping = (s->k_d * damping) / (dt * distance);
		distance = (((distance - s->x_0) * s->k)+damping) / distance;
		force_init(force_store_space, (particle_get_x_a(s->a) - particle_get_x_a(s->b)) * distance, (particle_get_y_a(s->a) - particle_get_y_a(s->b)) * distance, (particle_get_z_a(s->a) - particle_get_z_a(s->b)) * distance);
		particle_add_force(s->b, force_store_space);	
		force_invert(force_store_space);
		particle_add_force(s->a, force_store_space);
		}
		//printf(" the force is %g \n", force_store_space->x);	
	}

}


double spring_pos_x_a(spring * s) { return particle_get_x_a(s->a); }
double spring_pos_y_a(spring * s) { return particle_get_y_a(s->a); }
double spring_pos_z_a(spring * s) { return particle_get_z_a(s->a); }
double spring_pos_x_b(spring * s) { return particle_get_x_a(s->b); }
double spring_pos_y_b(spring * s) { return particle_get_y_a(s->b); }
double spring_pos_z_b(spring * s) { return particle_get_z_a(s->b); }


