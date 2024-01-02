#ifndef PARTICLE_H
#define PARTICLE_H

#include <Windows.h>

#define PARTICLE_SIZE 10
#define PARTICLE_SPACING 1
#define AMOUNT_PARTICLES 20

typedef struct {
    int x;
    int y;
    int bounce_multiplier;
} Particle;

void particles_create(Particle *particles);
void draw_particles(HWND hwnd, Particle *particles);
void print_particle(Particle particle);

#endif //PARTICLE_H