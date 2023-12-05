#pragma once

#include <vector>

class Particle {
    public:
        int x, y, bounce_multiplier;

};

class ParticleManager { 
    public:
        std::vector<Particle> particles;

        void particles_print();
};