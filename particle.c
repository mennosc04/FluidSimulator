#include <stdio.h>
#include <math.h>

#include "particle.h"

void draw_particles(HWND hwnd, Particle *particles) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    HBRUSH hbr = CreateSolidBrush(RGB(3, 144, 254));
    SelectObject(hdc,hbr);

    for(int i = 0; i < AMOUNT_PARTICLES; i++) {
        Ellipse(hdc, particles[i].x - PARTICLE_SIZE / 2, particles[i].y - PARTICLE_SIZE / 2, particles[i].x + PARTICLE_SIZE / 2, particles[i].y + PARTICLE_SIZE / 2);
    }

    
    DeleteObject(hbr);
    EndPaint(hwnd, &ps);
}

void particles_create(Particle *particles) {
    int particlesPerRow = (int)(sqrt(AMOUNT_PARTICLES));
    int particlesPerCol = (AMOUNT_PARTICLES - 1) / particlesPerRow + 1;
    float spacing = PARTICLE_SIZE * 2 + PARTICLE_SPACING;

    float initial_x = ((-particlesPerRow / 2) + 0.5f) * spacing + GetSystemMetrics(SM_CXSCREEN) / 2;
    float initial_y = ((-particlesPerCol / 2) + 0.5f) * spacing + GetSystemMetrics(SM_CYSCREEN) / 2; 

    float current_y = initial_y;
    int counter = 0;
    for(int row = 0; row < particlesPerCol; row++) {
        float current_x = initial_x;
        for(int col = 0; col < particlesPerRow && row * particlesPerRow + col < AMOUNT_PARTICLES; col++) {
            Particle particle;
            particle.x = current_x;
            particle.y = current_y;
            particle.bounce_multiplier = 0.1;

            particles[row * particlesPerRow + col] = particle;

            current_x += spacing;
            counter++;
        }
        current_y += spacing;
    }
}

void print_particle(Particle particle) {
    printf("|Particle|: {x: %d, y: %d}\n", particle.x, particle.y);
}