#include "defines.h"
#include "platform.h"

#include "particle_demo.h"

#include <stdio.h>

int main() {
    Platform* platform = platform_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "PhysicsEngine");
    Demo* demo = new ParticleDemo();
    demo->start();
    i32 index = 0;
    while (*platform->running) {
        platform_begin_frame(platform);
        i32 stopHere = 0;
        demo->update(platform->current_input, platform->delta_time);
        demo->render();
        platform_end_frame(platform);
        index++;
    }
    demo->destroy();
    delete demo;
    platform_destroy(platform);
    return 0;
}