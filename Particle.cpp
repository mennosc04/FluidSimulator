#include <vector>
#include <iostream>

#include "Particle.hpp"

void ParticleManager::particles_print() {
    for(Particle p : this->particles) {
        std::cout << "{x: " << p.x << ", y: " << p.y << ", bounce multiplier: " << p.bounce_multiplier << "}" << std::endl;
    }
}

