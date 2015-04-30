
#include "lsms.h"
#include <malloc.h>
#include <math.h>

lsms * lsms_create(int num_particles, int num_springs, int num_particles_hold, int num_springs_hold)
{
	int i = 0;
	lsms * l = (lsms *)malloc(sizeof(lsms));
	l->p = (particle **)malloc(sizeof(particle *) * num_particles);
	l->s = (spring **)malloc(sizeof(spring *) * num_springs);
	l->p_hold = (particle **)malloc(sizeof(particle *) * num_particles_hold);
	l->s_hold = (spring **)malloc(sizeof(spring *) * num_springs_hold);
	for(i = 0; i < num_particles; i ++)
	{
		l->p[i] = (particle *)malloc(sizeof(particle));
	}
	for(i = 0; i < num_springs; i++)
	{
		l->s[i] = (spring *)malloc(sizeof(spring));
	}
	for(i = 0; i < num_particles_hold; i ++)
	{
		l->p_hold[i] = (particle *)malloc(sizeof(particle));
	}
	for(i = 0; i < num_springs_hold; i++)
	{
		l->s_hold[i] = (spring *)malloc(sizeof(spring));
	}
	
	l->num_particles = num_particles;
	l->num_springs = num_springs;
	l->num_particles_hold = num_particles_hold;
	l->num_springs_hold = num_springs_hold;
	return l;
}

lsms * lsms_create_from_file(char filename[])
{
    FILE * fp;
    int i = 0;
    int num_particles = 0;
    int num_springs = 0;
    int num_particles_hold = 0;
    int num_springs_hold = 0;
    double pos_x = 0;
    double pos_y = 0;
    double pos_z = 0;
    double mass = 0;
    double charge = 0;
    int type = 0;
    double k = 0;
    double k_d = 0;
    double x_0 = 0;
    int pos_a = 0;
    int pos_b = 0;

    fp = fopen(filename, "r");
    fscanf(fp, "%i", &num_particles);
    fscanf(fp, "%i", &num_springs);
    fscanf(fp, "%i", &num_particles_hold);
    fscanf(fp, "%i", &num_springs_hold);

    lsms * l = lsms_create(num_particles, num_springs, num_particles_hold, num_springs_hold);

    for(i = 0; i < num_particles; i++)
    {
        fscanf(fp, "%lf", &pos_x);
        fscanf(fp, "%lf", &pos_y);
        fscanf(fp, "%lf", &pos_z);
        fscanf(fp, "%lf", &mass);
        fscanf(fp, "%lf", &charge);
        lsms_set_particle(l, i, pos_x, pos_y, pos_z, mass, charge);
    }
    for(i = 0; i < num_springs; i++)
    {
        fscanf(fp, "%i", &type);
        fscanf(fp, "%lf", &k);
        fscanf(fp, "%lf", &k_d);
        fscanf(fp, "%lf", &x_0);
        fscanf(fp, "%i", &pos_a);
        fscanf(fp, "%i", &pos_b);
        lsms_set_spring(l, i, type, k, k_d, x_0, pos_a, pos_b);
    }
    for(i = 0; i < num_particles_hold; i++)
    {
        fscanf(fp, "%lf", &pos_x);
        fscanf(fp, "%lf", &pos_y);
        fscanf(fp, "%lf", &pos_z);
        lsms_set_particle_hold(l, i, pos_x, pos_y, pos_z);

    }
    for(i = 0; i < num_springs_hold; i++)
    {
        fscanf(fp, "%i", &type);
        fscanf(fp, "%lf", &k);
        fscanf(fp, "%lf", &k_d);
        fscanf(fp, "%lf", &x_0);
        fscanf(fp, "%i", &pos_a);
        fscanf(fp, "%i", &pos_b);
        lsms_set_spring_hold(l, i, k, pos_a, pos_b);
    }
    fclose(fp);
    return l;
}


void lsms_set_particle(lsms * l, int pos, double x, double y, double z, double mass, double charge)
{
	particle_init(l->p[pos], x, y, z, mass, charge);
}

void lsms_set_particle_hold(lsms * l, int pos, double x, double y, double z)
{
	particle_init(l->p_hold[pos], x, y, z, 1.0, 0.0);
}

void lsms_set_spring(lsms * l, int pos, int type, double k, double k_d, double x_0, int pos_a, int pos_b)
{
	spring_init(l->s[pos], type, k, k_d, x_0, l->p[pos_a], l->p[pos_b]);
}

void lsms_set_spring_hold(lsms * l, int pos, double k, int pos_a, int pos_b)
{
	spring_init(l->s_hold[pos], 1, k, 0.0, 0.0, l->p_hold[pos_a], l->p[pos_b]);
}

lsms * lsms_init_rope(double start_x, double start_y, double start_z, double end_x, double end_y, double end_z, double mass, double k, double k_d, int x_div)
{
	int i = 0;
	lsms * l = lsms_create(x_div, x_div - 1, 2, 2);
	double dx = -(start_x - end_x) / (double)(x_div-1);
	double dy = -(start_y - end_y) / (double)(x_div-1);
	double dz = -(start_z - end_z) / (double)(x_div-1);
	double h = pow(dx,2) + pow(dy,2) + pow(dz,2);
	h = pow(h,.5);
	for (i = 0; i < x_div; i++)
	{ 
		lsms_set_particle(l, i, start_x + (double)(i)*dx, start_y + (double)(i)*dy, start_z + (double)(i)*dz, mass, 0.0);
	}
	for (i = 0; i < x_div-1; i++)
	{
		lsms_set_spring(l, i, 1, k, k_d, h, i, i+1);
	}
	lsms_set_particle_hold(l, 0, start_x, start_y, start_z);
	lsms_set_particle_hold(l, 1, end_x, end_y, end_z);
	lsms_set_spring_hold(l, 0, k, 0, 0);
	lsms_set_spring_hold(l, 1, k, 1, x_div-1);
	return l;
}

void lsms_force_from_springs(lsms * l, double dt)
{
	int i = 0;
	force * f = force_create();
	for(i = 0; i < l->num_springs; i++)
	{
		spring_calc_force(l->s[i], dt, f);
	}
	for(i = 0; i < l->num_springs_hold; i++)
	{
		spring_calc_force(l->s_hold[i], dt, f);
	}
	force_destroy(f);
}

void lsms_force_from_charges(lsms * l)
{
	int i = 0;
	int j = 0;
	force * f = force_create();
	for(i = 0; i < l->num_particles; i++)
	{
		for(j = i+1; j < l->num_particles; j++)
		{
			particle_calc_charge_force(l->p[i], l->p[j], f);
		}

	}
	force_destroy(f);
}

void lsms_first_step(lsms * l, double dt)
{
	int i = 0;
	lsms_force_zero(l);
	lsms_force_from_springs(l, dt);
	lsms_force_from_charges(l);
	for (i = 0; i < l->num_particles; i++)
	{
		particle_first_step(l->p[i], dt);
	}
}

void lsms_update_step(lsms * l, double dt)
{
	int i = 0;
	lsms_force_zero(l);
	lsms_force_from_springs(l, dt);
	lsms_force_from_charges(l);
	for (i = 0; i < l->num_particles; i++)
	{
		particle_update_step(l->p[i], dt);
	}
}

void lsms_force_zero(lsms * l)
{
	int i = 0;
	for(i = 0; i < l->num_particles; i++)
	{
		particle_force_zero(l->p[i]);
	}
}

void lsms_update(lsms * l, double dt, int steps)
{
	int i = 0;
	for(i = 0; i < steps; i++)
	{
		lsms_update_step(l, dt);
	}
}

void lsms_destroy(lsms * l)
{
	int i = 0;
	for(i = 0; i < l->num_particles; i ++)
	{
		free(l->p[i]);
		l->p[i] = NULL;
	}
	for(i = 0; i < l->num_springs; i++)
	{
		free(l->s[i]);
		l->s[i] = NULL;
	}
	for(i = 0; i < l->num_particles_hold; i ++)
	{
		free(l->p_hold[i]);
		l->p_hold[i] = NULL;
	}
	for(i = 0; i < l->num_springs_hold; i++)
	{
		free(l->s_hold[i]);
		l->s_hold[i] = NULL;
	}

	free(l);
	l = NULL;
}

lsms * lsms_add_tensor(lsms * l_a, lsms * l_b, double k, double x_0)
{
	int i = 0;
	int j = 0;
	lsms * l = lsms_create(l_a->num_particles + l_b->num_particles, l_a->num_springs + l_b->num_springs + (l_a->num_particles * l_b->num_particles), l_a->num_particles_hold + l_b->num_particles_hold, l_a->num_springs_hold + l_b->num_springs_hold);
	for (i = 0; i < l_a->num_particles; i++)
	{
		l->p[i] = l_a->p[i];
		l_a->p[i] = NULL;
	}
	for (i = 0; i < l_b->num_particles; i++)
	{
		l->p[i + l_a->num_particles] = l_b->p[i];
		l_b->p[i] = NULL;
	}
	for (i = 0; i < l_a->num_springs; i++)
	{
		l->s[i] = l_a->s[i];
		l_a->s[i] = NULL;
	}
	for (i = 0; i < l_b->num_springs; i++)
	{
		l->s[i + l_a->num_springs] = l_b->s[i];
		l_b->s[i] = NULL;
	}
	for (i = 0; i < l_a->num_particles; i++)
	{
		for (j = 0; j < l_b->num_particles; j++)
		{
			lsms_set_spring(l, j+(i*l_b->num_particles)+l_a->num_springs+l_b->num_springs, 2, k, 0.0, x_0, i, j+l_a->num_particles);
			//printf("num springs is %d \n", j+(i*l_a->num_particles)+l_a->num_springs+l_b->num_springs);
		}
	}
	for (i = 0; i < l_a->num_particles_hold; i++)
	{
		l->p_hold[i] = l_a->p_hold[i];
		l_a->p_hold[i] = NULL;
	}
	for (i = 0; i < l_b->num_particles_hold; i++)
	{
		l->p_hold[i + l_a->num_particles_hold] = l_b->p_hold[i];
		l_b->p_hold[i] = NULL;
	}
	for (i = 0; i < l_a->num_springs_hold; i++)
	{
		l->s_hold[i] = l_a->s_hold[i];
		l_a->s_hold[i] = NULL;
	}
	for (i = 0; i < l_b->num_springs_hold; i++)
	{
		l->s_hold[i + l_a->num_springs_hold] = l_b->s_hold[i];
		l_b->s_hold[i] = NULL;
	}


	l_a = NULL;
	l_b = NULL;
	return l;
}

void lsms_add_charge(lsms * l, double charge)
{
	int i = 0;
	for (i = 0; i < l->num_particles; i++)
	{
		particle_add_charge(l->p[i],charge);
	}
}

